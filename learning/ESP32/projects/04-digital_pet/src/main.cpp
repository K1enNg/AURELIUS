#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//pin map
constexpr uint8_t PIN_SDA = 21;  
constexpr uint8_t PIN_SCL = 22;

constexpr uint8_t PIN_BTN_FEED  = 32;
constexpr uint8_t PIN_BTN_PLAY  = 33;
constexpr uint8_t PIN_BTN_CLEAN = 25;

constexpr uint8_t PIN_BUZZER = 26;

// (single-color LEDs: long leg/anode -> resistor -> GPIO, short leg/cathode -> GND)
constexpr uint8_t PIN_LED_HUNGER = 27;
constexpr uint8_t PIN_LED_HAPPY  = 14;

// display
constexpr uint8_t SCREEN_WIDTH  = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// debounced button
class Button {
public:
    explicit Button(uint8_t pin) : pin_(pin) {}

    void begin() { pinMode(pin_, INPUT_PULLUP); }

    // Returns true exactly once per physical press (rising edge after debounce)
    bool pressed() {
        bool raw = digitalRead(pin_) == LOW; // active-low (pulled up, button -> GND)
        unsigned long now = millis();

        if (raw != lastRaw_) {
            lastChangeMs_ = now;
            lastRaw_ = raw;
        }

        if ((now - lastChangeMs_) > kDebounceMs_ && raw != stable_) {
            stable_ = raw;
            if (stable_) return true; // just became pressed
        }
        return false;
    }

private:
    uint8_t pin_;
    bool lastRaw_ = false;
    bool stable_ = false;
    unsigned long lastChangeMs_ = 0;
    static constexpr unsigned long kDebounceMs_ = 30;
};

// buzzer
struct Note { uint16_t freq; uint16_t durationMs; };

class SoundPlayer {
public:
    void begin(uint8_t pin) { pin_ = pin; }

    void play(const Note* seq, uint8_t len) {
        seq_ = seq;
        len_ = len;
        idx_ = 0;
        noteStartMs_ = millis();
        active_ = true;
        startNote(seq_[0]);
    }

    void update() {
        if (!active_) return;
        if (millis() - noteStartMs_ >= seq_[idx_].durationMs) {
            idx_++;
            if (idx_ >= len_) {
                noTone(pin_);
                active_ = false;
                return;
            }
            noteStartMs_ = millis();
            startNote(seq_[idx_]);
        }
    }

    bool busy() const { return active_; }

private:
    void startNote(const Note& n) {
        if (n.freq == 0) noTone(pin_);
        else tone(pin_, n.freq);
    }

    uint8_t pin_ = 0;
    const Note* seq_ = nullptr;
    uint8_t len_ = 0, idx_ = 0;
    unsigned long noteStartMs_ = 0;
    bool active_ = false;
};

// jingles for each event
const Note kFeedTune[]    = {{800, 80}, {1000, 80}, {1200, 120}};
const Note kPlayTune[]    = {{1200, 60}, {1600, 60}, {1200, 60}, {1600, 100}};
const Note kCleanTune[]   = {{600, 150}};
const Note kWarningTune[] = {{300, 120}, {0, 80}, {300, 120}};

// stats and mood
enum class Mood { HAPPY, NEUTRAL, SAD, SLEEPY, EXCITED, EATING, KAWAII, CRYING, TEARS };

class Pet {
public:
    void update(unsigned long nowMs) {
        // decay stats once per tick interval
        if (nowMs - lastDecayMs_ >= kDecayIntervalMs_) {
            lastDecayMs_ = nowMs;
            hunger_     = clamp(hunger_ - 2);
            happiness_  = clamp(happiness_ - 1);
            cleanliness_ = clamp(cleanliness_ - 1);
        }

        // timed reactions (feed/play/eat animations) expire on their own
        if (reactionUntilMs_ && nowMs > reactionUntilMs_) {
            reactionUntilMs_ = 0;
        }
    }

    void feed()  { hunger_ = clamp(hunger_ + 25);      react(Mood::KAWAII, 1500); touch(); }
    void play()  { happiness_ = clamp(happiness_ + 20); energy_ = clamp(energy_ - 10); react(Mood::CRYING, 1500); touch(); }
    void clean() { cleanliness_ = clamp(cleanliness_ + 30); react(Mood::TEARS, 1500); touch(); }

    Mood mood() const {
        if (reactionUntilMs_) return reaction_;
        if (millis() - lastInteractionMs_ > kIdleSleepMs_ && averageStat() > 40) return Mood::SLEEPY;
        if (averageStat() < 30) return Mood::SAD;
        if (averageStat() > 75) return Mood::HAPPY;
        return Mood::NEUTRAL;
    }

    uint8_t hunger() const { return hunger_; }
    uint8_t happiness() const { return happiness_; }
    uint8_t cleanliness() const { return cleanliness_; }

private:
    static uint8_t clamp(int v) { return static_cast<uint8_t>(constrain(v, 0, 100)); }
    uint8_t averageStat() const { return (hunger_ + happiness_ + cleanliness_) / 3; }

    void react(Mood m, unsigned long durationMs) {
        reaction_ = m;
        reactionUntilMs_ = millis() + durationMs;
    }

    void touch() { lastInteractionMs_ = millis(); } // any feed/play/clean counts as "attention"

    uint8_t hunger_ = 80, happiness_ = 80, cleanliness_ = 80, energy_ = 80;

    Mood reaction_ = Mood::NEUTRAL;
    unsigned long reactionUntilMs_ = 0;

    unsigned long lastDecayMs_ = 0;
    static constexpr unsigned long kDecayIntervalMs_ = 15000; // stat tick every 15s

    unsigned long lastInteractionMs_ = 0;
    static constexpr unsigned long kIdleSleepMs_ = 60000; // no interaction for 60s -> sleepy
};

//globals
Pet pet;
Button btnFeed(PIN_BTN_FEED), btnPlay(PIN_BTN_PLAY), btnClean(PIN_BTN_CLEAN);
SoundPlayer buzzer;

unsigned long lastWarningMs = 0;
constexpr unsigned long kWarningCooldownMs = 10000; // don't nag more than every 10s

unsigned long lastDrawMs = 0;
constexpr unsigned long kDrawIntervalMs = 100; // ~10fps is plenty for a face

// interface
void drawFace(Mood m) {
    display.clearDisplay();

    const int eyeY = 22;
    const int leftX = 40, rightX = 88;
    const int eyeR = 8;

    switch (m) {
        case Mood::HAPPY:
            display.drawLine(leftX - 6, eyeY - 4, leftX - 6, eyeY + 2, SSD1306_WHITE);
            display.drawLine(leftX - 6, eyeY + 2, leftX + 6, eyeY + 2, SSD1306_WHITE);
            display.drawLine(leftX + 6, eyeY + 2, leftX + 6, eyeY - 4, SSD1306_WHITE);

            display.drawLine(rightX - 6, eyeY - 4, rightX - 6, eyeY + 2, SSD1306_WHITE);
            display.drawLine(rightX - 6, eyeY + 2, rightX + 6, eyeY + 2, SSD1306_WHITE);
            display.drawLine(rightX + 6, eyeY + 2, rightX + 6, eyeY - 4, SSD1306_WHITE);

            display.drawCircle(60, 40, 4, SSD1306_WHITE); 
            display.drawCircle(68, 40, 4, SSD1306_WHITE); 
            display.fillRect(56, 34, 16, 6, SSD1306_BLACK); 
            break;
        case Mood::EXCITED:
            display.fillCircle(leftX, eyeY, eyeR, SSD1306_WHITE);
            display.fillCircle(rightX, eyeY, eyeR, SSD1306_WHITE);
            display.drawCircle(64, 42, 10, SSD1306_WHITE);
            break;
        case Mood::EATING:
            // .·°՞(˃ ᗝ ˂)՞°·. -- excited chevron eyes, round "o" mouth, sparkles framing the face
            display.drawLine(leftX - 6, eyeY - 6, leftX + 6, eyeY, SSD1306_WHITE);
            display.drawLine(leftX + 6, eyeY, leftX - 6, eyeY + 6, SSD1306_WHITE);
            display.drawLine(rightX + 6, eyeY - 6, rightX - 6, eyeY, SSD1306_WHITE);
            display.drawLine(rightX - 6, eyeY, rightX + 6, eyeY + 6, SSD1306_WHITE);
            display.drawCircle(64, 45, 5, SSD1306_WHITE); // "ᗝ" round open mouth
            // sparkle marks framing the corners
            display.drawLine(14, 4, 18, 4, SSD1306_WHITE);
            display.drawLine(16, 2, 16, 6, SSD1306_WHITE);
            display.drawLine(110, 4, 114, 4, SSD1306_WHITE);
            display.drawLine(112, 2, 112, 6, SSD1306_WHITE);
            display.drawLine(18, 48, 22, 48, SSD1306_WHITE);
            display.drawLine(20, 46, 20, 50, SSD1306_WHITE);
            display.drawLine(106, 48, 110, 48, SSD1306_WHITE);
            display.drawLine(108, 46, 108, 50, SSD1306_WHITE);
            break;
        case Mood::SLEEPY:
            display.drawLine(leftX - 8, eyeY, leftX + 8, eyeY, SSD1306_WHITE); // closed eyes
            display.drawLine(rightX - 8, eyeY, rightX + 8, eyeY, SSD1306_WHITE);
            display.setCursor(96, 8);
            display.print("z");
            break;
        case Mood::KAWAII:
            display.drawLine(leftX - 6, eyeY - 6, leftX + 6, eyeY, SSD1306_WHITE);
            display.drawLine(leftX + 6, eyeY, leftX - 6, eyeY + 6, SSD1306_WHITE);
            display.drawLine(rightX + 6, eyeY - 6, rightX - 6, eyeY, SSD1306_WHITE);
            display.drawLine(rightX - 6, eyeY, rightX + 6, eyeY + 6, SSD1306_WHITE);
            display.drawLine(54, 44, 58, 48, SSD1306_WHITE);
            display.drawLine(58, 48, 62, 44, SSD1306_WHITE);
            display.drawLine(62, 44, 66, 48, SSD1306_WHITE);
            display.drawLine(66, 48, 70, 44, SSD1306_WHITE);
            display.drawLine(70, 44, 74, 48, SSD1306_WHITE);

            display.drawLine(leftX - 14, eyeY - 2, leftX - 10, eyeY + 2, SSD1306_WHITE);
            display.drawLine(rightX + 10, eyeY - 2, rightX + 14, eyeY + 2, SSD1306_WHITE);
            break;
        case Mood::TEARS:
            display.drawLine(leftX - 6, eyeY, leftX + 6, eyeY, SSD1306_WHITE);
            display.drawLine(rightX - 6, eyeY, rightX + 6, eyeY, SSD1306_WHITE);
            display.drawLine(leftX, eyeY + 2, leftX, eyeY + 16, SSD1306_WHITE);  
            display.drawLine(rightX, eyeY + 2, rightX, eyeY + 16, SSD1306_WHITE); 
            display.fillRoundRect(56, 40, 16, 10, 3, SSD1306_WHITE);
            break;
        case Mood::NEUTRAL:
        default:
            display.fillCircle(leftX, eyeY, eyeR, SSD1306_WHITE);
            display.fillCircle(rightX, eyeY, eyeR, SSD1306_WHITE);
            display.drawLine(56, 42, 72, 42, SSD1306_WHITE);
            break;
    }

    // stat bar at bottom
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.printf("H:%3d P:%3d C:%3d", pet.hunger(), pet.happiness(), pet.cleanliness());

    display.display();
}

void setLedsForStats() {
    constexpr uint8_t kLowThreshold = 30; // below this, the stat "needs attention"
    digitalWrite(PIN_LED_HUNGER, pet.hunger()    < kLowThreshold ? HIGH : LOW);
    digitalWrite(PIN_LED_HAPPY,  pet.happiness() < kLowThreshold ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);

    Wire.begin(PIN_SDA, PIN_SCL); 
    btnFeed.begin();
    btnPlay.begin();
    btnClean.begin();
    pinMode(PIN_LED_HUNGER, OUTPUT);
    pinMode(PIN_LED_HAPPY, OUTPUT);
    buzzer.begin(PIN_BUZZER);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 init failed -- check wiring/address");
        while (true) delay(1000);
    }
    display.clearDisplay();
    display.display();
}

void loop() {
    unsigned long now = millis();

    // buttons
    // Sound-triggering lives here in main.cpp, not inside Pet -- Pet just
    // tracks stats and doesn't need to know hardware (buzzer/LED) exists.
    if (btnFeed.pressed())  { pet.feed();  buzzer.play(kFeedTune, 3); }
    if (btnPlay.pressed())  { pet.play();  buzzer.play(kPlayTune, 4); }
    if (btnClean.pressed()) { pet.clean(); buzzer.play(kCleanTune, 1); }

    pet.update(now);

    // low-stat warning beep
    bool needsAttention = pet.hunger() < 20 || pet.happiness() < 20 || pet.cleanliness() < 20;
    if (needsAttention && !buzzer.busy() && (now - lastWarningMs) >= kWarningCooldownMs) {
        lastWarningMs = now;
        buzzer.play(kWarningTune, 3);
    }

    // sound tick
    buzzer.update();

    // render
    if (now - lastDrawMs >= kDrawIntervalMs) {
        lastDrawMs = now;
        drawFace(pet.mood());
        setLedsForStats();
    }
}