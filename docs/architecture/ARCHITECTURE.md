# AURELIUS — System Architecture

> **Document status:** Onboarding
> **Document:** System Architecture  
> **Version:** 1.0
> **Last updated:** 2026-08-16

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. Architectural Philosophy](#2-architectural-philosophy)
- [3. System Goals](#3-system-goals)
- [4. System Constraints](#4-system-constraints)
- [5. High-Level Architecture](#5-high-level-architecture)
- [6. Subsystem Architecture](#6-subsystem-architecture)
  - [6.1 Mechanical Subsystem](#61-mechanical-subsystem)
  - [6.2 Power Subsystem](#62-power-subsystem)
  - [6.3 Compute Subsystem](#63-compute-subsystem)
  - [6.4 Firmware Subsystem](#64-firmware-subsystem)
  - [6.5 Sensor Subsystem](#65-sensor-subsystem)
  - [6.6 Actuation Subsystem](#66-actuation-subsystem)
  - [6.7 Communication Subsystem](#67-communication-subsystem)
  - [6.8 Perception Subsystem](#68-perception-subsystem)
  - [6.9 Human Interface Subsystem](#69-human-interface-subsystem)
- [7. Software Architecture](#7-software-architecture)
- [8. Hardware Architecture](#8-hardware-architecture)
- [9. Communication and Interfaces](#9-communication-and-interfaces)
- [10. Data Flow](#10-data-flow)
- [11. Operating Modes](#11-operating-modes)
- [12. Control Architecture](#12-control-architecture)
- [13. Power Architecture](#13-power-architecture)
- [14. Safety Architecture](#14-safety-architecture)
- [15. Fault Handling](#15-fault-handling)
- [16. AURELIUS V1 Scope](#16-aurelius-v1-scope)
- [17. V1 Architecture](#17-v1-architecture)
- [18. Future Expansion](#18-future-expansion)
- [19. Design Decisions](#19-design-decisions)
- [20. Open Engineering Questions](#20-open-engineering-questions)
- [21. Architectural Principles](#21-architectural-principles)

---

# 1. Overview

The project emphasizes:

- Embedded systems
- Robotics
- Electronics
- Mechanical design
- Real-time control
- Software engineering
- Computer vision
- Autonomous behavior
- Human-robot interaction
- Modular system design

AURELIUS is designed as a **distributed robotic system** rather than a single computer running everything.

The architecture separates:

1. High-level computation
2. Real-time control
3. Sensors
4. Actuators
5. Power management
6. Communication
7. Human interaction

This separation allows individual components to evolve without requiring the entire system to be redesigned.

---

# 2. Architectural Philosophy

AURELIUS follows several fundamental architectural principles.

## 2.1 Modularity

Every major subsystem should have a clearly defined responsibility and interface.

For example:

```text
Camera
   │
   ▼
Perception
   │
   ▼
Decision
   │
   ▼
Motion Planning
   │
   ▼
Motor Controller
   │
   ▼
Actuator
```

The camera should not directly control the motors.

Likewise, the motor controller should not need to understand computer vision.

---

## 2.2 Separation of Concerns

AURELIUS separates high-level intelligence from low-level hardware control.

### High-level system

Responsible for:

- Perception
- Planning
- Decision making
- User interaction
- Logging
- Configuration
- Long-running processes

### Low-level system

Responsible for:

- Motor control
- Sensor acquisition
- Timing-critical operations
- Safety monitoring
- Hardware abstraction
- Communication with actuators

This prevents high-level software failures from directly compromising low-level control.

---

## 2.3 Hardware Abstraction

Application-level software should not depend directly on specific hardware whenever practical.

Instead:

```text
Application
     │
     ▼
Hardware Abstraction Layer
     │
     ▼
Device Driver
     │
     ▼
Physical Hardware
```

This allows hardware to be replaced without rewriting the entire software stack.

---

## 2.4 Fail Safely

AURELIUS must prioritize predictable behavior over maximum capability.

When a subsystem fails, the robot should move toward a known safe state rather than continuing uncontrolled operation.

Examples:

```text
Communication lost
       ↓
Controller timeout
       ↓
Stop active motion
       ↓
Enter SAFE state
```

---

## 2.5 Incremental Development

The architecture must support gradual development.

AURELIUS V1 should not attempt to implement:

- Full autonomy
- Advanced AI
- Human-level conversation
- Complex locomotion
- Full computer vision
- Sophisticated manipulation

Instead, V1 establishes the hardware and software foundation required for those capabilities later.

---

# 3. System Goals

## 3.1 Primary Goals

AURELIUS should eventually provide:

- Controlled physical movement
- Sensor-based environmental awareness
- Local computation
- Human interaction
- Modular software and hardware
- Autonomous behaviors
- Expandable electronics
- Reproducible development and testing

---

## 3.2 Engineering Goals

The project should provide practical experience with:

- Microcontrollers
- Embedded programming
- Electronics
- Motor control
- Communication protocols
- Real-time systems
- Linux-based computing
- Robotics middleware
- Sensors
- Control systems
- Computer vision
- System integration

---

## 3.3 Non-Goals

AURELIUS is not initially intended to:

- Recreate TARS exactly
- Become a humanoid robot
- Solve general artificial intelligence
- Perform dangerous physical tasks
- Operate indefinitely without supervision
- Replace a human operator

---

# 4. System Constraints

The initial development environment is assumed to be:

- Solo development
- Limited budget
- Limited workshop equipment
- Limited manufacturing capability
- University-level engineering resources
- Incremental hardware acquisition

Therefore, architectural decisions should favor:

1. Affordable components
2. Easily available hardware
3. Open documentation
4. Replaceable modules
5. Simple manufacturing
6. Debuggability
7. Safety

---

# 5. High-Level Architecture

The complete AURELIUS architecture is divided into several major layers.

```text
┌───────────────────────────────────────────────────────────┐
│                     HUMAN INTERFACE                       │
│              Operator / Developer / User                 │
└────────────────────────────┬──────────────────────────────┘
                             │
                             ▼
┌───────────────────────────────────────────────────────────┐
│                 HIGH-LEVEL SOFTWARE                      │
│                                                           │
│  Perception │ Decision │ Planning │ Interaction │ Logging │
└────────────────────────────┬──────────────────────────────┘
                             │
                       Commands / Data
                             │
                             ▼
┌───────────────────────────────────────────────────────────┐
│                 ROBOT COMMUNICATION                       │
│                                                           │
│            Command / Telemetry / Events                   │
└────────────────────────────┬──────────────────────────────┘
                             │
                             ▼
┌───────────────────────────────────────────────────────────┐
│                  LOW-LEVEL CONTROL                        │
│                                                           │
│       Firmware │ Control │ Safety │ Hardware Abstraction │
└───────────────┬──────────────┬──────────────┬──────────────┘
                │              │              │
                ▼              ▼              ▼
          ┌──────────┐   ┌──────────┐   ┌──────────┐
          │ Sensors  │   │ Actuators│   │  Power   │
          └──────────┘   └──────────┘   └──────────┘
                │              │
                └──────┬───────┘
                       ▼
              ┌─────────────────┐
              │    MECHANICAL   │
              │     SYSTEM      │
              └─────────────────┘
```

---

# 6. Subsystem Architecture

## 6.1 Mechanical Subsystem

The mechanical subsystem provides the physical structure of AURELIUS.

Responsibilities:

- Structural frame
- Enclosures
- Moving joints
- Motor mounting
- Sensor mounting
- Electronics mounting
- Cable routing
- Mechanical protection

The mechanical system should be designed around modular assemblies.

```text
AURELIUS
│
├── Structural Frame
│
├── Actuated Modules
│   ├── Joint A
│   ├── Joint B
│   └── Joint C
│
├── Electronics Housing
│
├── Sensor Mounts
│
└── Power Housing
```

### Design requirements

The mechanical system should:

- Protect electronics
- Minimize unnecessary weight
- Allow component replacement
- Provide sufficient structural rigidity
- Provide access to electronics
- Support future actuator upgrades

---

# 6.2 Power Subsystem

The power subsystem supplies electrical energy to all components.

Responsibilities:

- Battery storage
- Voltage regulation
- Power distribution
- Current monitoring
- Battery monitoring
- Emergency power shutdown

Conceptual architecture:

```text
                 BATTERY
                    │
                    ▼
            ┌───────────────┐
            │ Power Control │
            └───────┬───────┘
                    │
          ┌─────────┼─────────┐
          ▼         ▼         ▼
       Logic      Motors     Sensors
        Rail       Rail       Rail
          │         │         │
          ▼         ▼         ▼
        MCU      Drivers    Devices
```

The exact voltage rails and battery chemistry are intentionally left undecided until hardware requirements are established.

---

# 6.3 Compute Subsystem

AURELIUS should eventually use a hierarchical computing architecture.

### Low-level processor

Responsible for:

- Real-time control
- Sensor acquisition
- Actuator control
- Safety monitoring
- Timing-critical tasks

### High-level computer

Responsible for:

- Perception
- Planning
- User interface
- Data processing
- Logging
- Advanced algorithms
- AI workloads

Conceptually:

```text
┌──────────────────────────┐
│     High-Level Computer  │
│                          │
│ AI / Vision / Planning   │
└────────────┬─────────────┘
             │
        Robot Bus
             │
┌────────────▼─────────────┐
│       Microcontroller    │
│                          │
│ Control / Sensors / I/O  │
└──────────────────────────┘
```

This architecture prevents computationally expensive tasks from interfering with real-time control.

---

# 6.4 Firmware Subsystem

Firmware operates directly on the microcontroller.

Responsibilities:

- Hardware initialization
- Device drivers
- Sensor acquisition
- Motor control
- Communication
- Timing
- Fault detection
- Safety enforcement

Conceptual structure:

```text
Firmware
│
├── Boot / Initialization
│
├── Hardware Abstraction Layer
│
├── Device Drivers
│   ├── Sensors
│   ├── Motors
│   └── Communication
│
├── Control Layer
│
├── Safety Manager
│
├── Communication Manager
│
└── Diagnostics
```

The firmware should remain deterministic wherever possible.

---

# 6.5 Sensor Subsystem

Sensors provide information about the robot and its environment.

Potential sensor categories include:

### Internal sensors

- Motor position
- Motor velocity
- Motor current
- Battery voltage
- Temperature
- Joint position

### Environmental sensors

- Distance
- Light
- Temperature
- Inertial measurement
- Audio
- Camera data

Future sensor selection should be driven by specific system requirements rather than adding sensors without purpose.

---

# 6.6 Actuation Subsystem

The actuation subsystem converts electrical commands into physical movement.

Potential actuators include:

- DC motors
- Geared motors
- Servo motors
- Brushless motors
- Linear actuators

The architecture separates:

```text
Motion Command
      ↓
Motion Controller
      ↓
Motor Driver
      ↓
Actuator
      ↓
Mechanical System
```

Feedback should flow in the opposite direction:

```text
Mechanical System
      ↓
Sensor
      ↓
Motor Controller
      ↓
Telemetry
```

This creates a closed-loop control system.

---

# 6.7 Communication Subsystem

Communication connects AURELIUS's computing layers.

Potential interfaces include:

- UART
- I²C
- SPI
- CAN
- USB
- Ethernet
- Wi-Fi
- Bluetooth

The final protocols will be selected according to subsystem requirements.

A conceptual hierarchy is:

```text
High-Level Computer
        │
   Ethernet / USB
        │
Microcontroller
        │
   ┌────┼─────┐
   │    │     │
  I²C  SPI   UART
   │    │     │
Sensors Drivers Devices
```

---

# 6.8 Perception Subsystem

Perception transforms raw sensor data into useful information.

```text
Raw Sensors
     │
     ▼
Data Acquisition
     │
     ▼
Filtering
     │
     ▼
Sensor Fusion
     │
     ▼
Environmental Model
     │
     ▼
Decision / Planning
```

Potential future capabilities:

- Object detection
- Distance estimation
- Visual tracking
- Audio processing
- Localization
- Obstacle detection

Perception is explicitly **out of scope for the minimum V1 prototype** unless required by the selected V1 functionality.

---

# 6.9 Human Interface Subsystem

The human interface allows an operator to interact with AURELIUS.

Potential interfaces:

- Command-line interface
- Web interface
- Mobile interface
- Gamepad/controller
- Physical buttons
- Display
- Audio
- Voice interface

The initial interface should prioritize reliability and simplicity.

---

# 7. Software Architecture

The high-level software stack should be divided into layers.

```text
┌──────────────────────────────┐
│       User Interface         │
├──────────────────────────────┤
│      Behavior / Logic        │
├──────────────────────────────┤
│   Planning / Decision Layer  │
├──────────────────────────────┤
│       Perception             │
├──────────────────────────────┤
│     Robot Abstraction        │
├──────────────────────────────┤
│ Communication Middleware      │
├──────────────────────────────┤
│          Hardware            │
└──────────────────────────────┘
```

Each layer should depend primarily on the layer immediately below it.

---

# 8. Hardware Architecture

The hardware architecture is divided into functional domains.

```text
                       AURELIUS
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
       ▼                   ▼                   ▼
     POWER              COMPUTE            ACTUATION
       │                   │                   │
       │           ┌───────┴───────┐           │
       │           │               │           │
       │           ▼               ▼           ▼
       │          SBC             MCU       Drivers
       │                           │           │
       │                           │           ▼
       │                           │        Motors
       │                           │
       └──────────────┬────────────┘
                      │
                    Sensors
```

The architecture deliberately avoids specifying exact components at this stage.

Component selection should follow requirements rather than drive them.

---

# 9. Communication and Interfaces

Every subsystem should expose explicit interfaces.

## 9.1 Command Interface

High-level software sends commands such as:

```text
MOVE
STOP
SET_SPEED
SET_POSITION
SET_MODE
REQUEST_STATUS
```

---

## 9.2 Telemetry Interface

The low-level controller reports:

```text
POSITION
VELOCITY
CURRENT
TEMPERATURE
BATTERY
STATE
ERROR
```

---

## 9.3 Event Interface

Important asynchronous events may include:

```text
LOW_BATTERY
OVER_TEMPERATURE
COMMUNICATION_TIMEOUT
MOTOR_FAULT
EMERGENCY_STOP
SENSOR_FAILURE
```

---

## 9.4 Interface Principle

Interfaces should describe **what a subsystem provides**, rather than exposing unnecessary implementation details.

For example:

```text
High-Level Software
        │
        │ "Set joint position to X"
        ▼
Motion Interface
        │
        ▼
Motor Controller
        │
        ▼
Hardware
```

The high-level software should not need to know which GPIO pins or PWM registers are involved.

---

# 10. Data Flow

## 10.1 Sensor Data Flow

```text
Physical Environment
        │
        ▼
     Sensor
        │
        ▼
   MCU / Driver
        │
        ▼
 Data Filtering
        │
        ▼
   Telemetry
        │
        ▼
High-Level Computer
        │
        ▼
Perception
```

---

## 10.2 Motion Command Flow

```text
User / Behavior
       │
       ▼
Motion Planner
       │
       ▼
Motion Command
       │
       ▼
Communication Layer
       │
       ▼
Microcontroller
       │
       ▼
Control Algorithm
       │
       ▼
Motor Driver
       │
       ▼
Actuator
```

---

## 10.3 Feedback Flow

```text
Actuator
   │
   ▼
Position / Current Sensor
   │
   ▼
Controller
   │
   ▼
Telemetry
   │
   ▼
High-Level Computer
```

---

# 11. Operating Modes

AURELIUS should operate as a finite-state system.

```text
                 ┌───────────┐
                 │   BOOT    │
                 └─────┬─────┘
                       ▼
                 ┌───────────┐
                 │   INIT    │
                 └─────┬─────┘
                       ▼
                 ┌───────────┐
                 │   IDLE    │
                 └─────┬─────┘
                       ▼
                 ┌───────────┐
                 │   READY   │
                 └─────┬─────┘
                       ▼
                 ┌───────────┐
                 │ OPERATING │
                 └─────┬─────┘
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
       ┌───────────┐       ┌───────────┐
       │  FAULT    │       │   SAFE    │
       └───────────┘       └───────────┘
```

## 11.1 BOOT

Initial hardware startup.

## 11.2 INIT

Verify:

- Sensors
- Actuators
- Communication
- Power
- Configuration

## 11.3 IDLE

System powered but not actively moving.

## 11.4 READY

System has passed initialization and is ready for commands.

## 11.5 OPERATING

Normal operation.

## 11.6 FAULT

A recoverable or serious fault has been detected.

## 11.7 SAFE

All non-essential motion is disabled.

---

# 12. Control Architecture

AURELIUS should eventually use closed-loop control.

```text
             Desired State
                  │
                  ▼
           ┌──────────────┐
           │   Controller │
           └──────┬───────┘
                  │
                  ▼
               Actuator
                  │
                  ▼
              Mechanics
                  │
                  ▼
               Sensor
                  │
                  └──────────────┐
                                 │
                                 ▼
                              Feedback
                                 │
                                 └──────► Controller
```

The exact control algorithms will depend on the actuator and mechanical architecture.

Potential future approaches include:

- PID control
- Velocity control
- Position control
- Trajectory control
- State-space control

V1 should favor the simplest controller capable of producing stable and repeatable motion.

---

# 13. Power Architecture

Power distribution should isolate high-current loads from sensitive logic circuitry where practical.

```text
                    BATTERY
                       │
                       ▼
              Main Power Protection
                       │
              ┌────────┴────────┐
              │                 │
              ▼                 ▼
         Logic Power        Motor Power
              │                 │
              ▼                 ▼
            MCU              Drivers
              │                 │
        ┌─────┴─────┐          ▼
        ▼           ▼        Motors
     Sensors     Comms
```

Important power considerations:

- Voltage regulation
- Current limits
- Battery protection
- Motor startup current
- Electrical noise
- Grounding
- Thermal management
- Emergency shutdown

The power architecture must be validated before autonomous operation.

---

# 14. Safety Architecture

Safety is a cross-cutting subsystem rather than a single component.

## 14.1 Hardware Safety

Potential mechanisms:

- Physical emergency-stop
- Fuses
- Current protection
- Battery protection
- Thermal protection
- Motor driver protection

---

## 14.2 Software Safety

Software should enforce:

- Maximum velocity
- Maximum acceleration
- Maximum actuator limits
- Communication watchdogs
- Sensor validity checks
- Battery limits
- Temperature limits

---

## 14.3 Communication Watchdog

If the high-level computer stops communicating:

```text
No command received
       │
       ▼
Watchdog timeout
       │
       ▼
Stop commanded motion
       │
       ▼
SAFE state
```

The low-level controller must not depend on the high-level computer continuously issuing commands to remain safe.

---

# 15. Fault Handling

Faults should be classified by severity.

### Level 1 — Warning

Example:

```text
Temperature approaching limit
```

System continues operation while reporting the condition.

### Level 2 — Recoverable Fault

Example:

```text
Sensor temporarily unavailable
```

Subsystem attempts recovery.

### Level 3 — Critical Fault

Example:

```text
Motor controller failure
```

Motion is disabled.

### Level 4 — Emergency

Example:

```text
Electrical emergency
```

Power is immediately disabled where appropriate.

---

# 16. AURELIUS V1 Scope

AURELIUS V1 should be treated as a **proof-of-concept robotic platform**, not the final robot.

The primary objective is to demonstrate that the architecture works end-to-end.

## V1 should demonstrate

- Functional mechanical structure
- Embedded controller
- Basic actuator control
- Basic sensor acquisition
- Reliable communication
- Basic telemetry
- Manual control
- Emergency stop
- Basic fault handling
- Repeatable operation

---

## V1 should NOT attempt

- Full autonomous navigation
- Advanced AI
- Complex computer vision
- Human-level conversation
- Sophisticated manipulation
- Large-scale mapping
- Fully autonomous decision-making

Those capabilities belong to later iterations.

---

# 17. V1 Architecture

The minimum viable architecture is:

```text
                    OPERATOR
                       │
                       ▼
                Control Interface
                       │
                       ▼
              ┌─────────────────┐
              │ High-Level Host │
              └────────┬────────┘
                       │
                 Communication
                       │
                       ▼
              ┌─────────────────┐
              │ Microcontroller │
              └───────┬─────────┘
                      │
             ┌────────┼────────┐
             ▼        ▼        ▼
          Sensors  Drivers   Safety
                      │
                      ▼
                  Actuators
                      │
                      ▼
                 AURELIUS V1
```

The exact hardware implementation remains an engineering decision to be made during the design phase.

---

# 18. Future Expansion

The architecture is designed to support progressive capability increases.

## V2 — Improved Mobility

Potential additions:

- More actuators
- Better motor control
- Encoder feedback
- Improved mechanical structure
- Trajectory control

---

## V3 — Environmental Awareness

Potential additions:

- Cameras
- LiDAR / distance sensing
- IMU
- Sensor fusion
- Obstacle detection

---

## V4 — Autonomous Behavior

Potential additions:

- Localization
- Mapping
- Navigation
- Path planning
- Autonomous movement

---

## V5 — Human-Robot Interaction

Potential additions:

- Voice recognition
- Speech synthesis
- Natural-language commands
- Interactive display
- Personality/behavior system

---

## V6 — Advanced Intelligence

Potential additions:

- Machine learning
- Computer vision
- Local AI inference
- Multimodal perception
- Adaptive behavior

The architecture should allow these features to be added without fundamentally redesigning the low-level control system.

---

# 19. Design Decisions

This section records major architectural decisions and their reasoning.

| Decision | Status | Reason |
|---|---|---|
| Separate high-level and low-level computing | Accepted | Protect real-time control from high-level workload |
| Modular subsystem architecture | Accepted | Simplifies development and future upgrades |
| Hardware abstraction | Accepted | Prevent hardware lock-in |
| Closed-loop control | Planned | Required for reliable motion |
| Explicit safety subsystem | Accepted | Physical robotics requires predictable failure behavior |
| Exact MCU selection | TBD | Requires hardware requirements |
| Exact SBC selection | TBD | Depends on computational requirements |
| Communication protocol | TBD | Depends on topology and bandwidth |
| Motor technology | TBD | Depends on mechanical design |
| Battery architecture | TBD | Depends on power budget |

Architectural decisions should be updated as the project progresses.

---

# 20. Open Engineering Questions

The following questions must be resolved during the design phase.

## Mechanical

- What physical form will V1 take?
- What are the target dimensions?
- What materials will be used?
- What degrees of freedom are required?
- What is the maximum acceptable mass?
- What manufacturing methods are available?

## Electrical

- What voltage rails are required?
- What is the expected peak current?
- What motor drivers are appropriate?
- How will power be distributed?
- What protection circuits are required?

## Embedded

- Which microcontroller is appropriate?
- Is an RTOS required?
- What peripherals are required?
- What timing constraints exist?
- What communication protocol should be used?

## Computing

- Is a single-board computer sufficient?
- What operating system will be used?
- Is GPU acceleration required?
- What workloads need real-time processing?

## Communication

- What information must cross the high-level/low-level boundary?
- What latency is acceptable?
- What bandwidth is required?
- What happens when communication fails?

## Software

- What language should each layer use?
- Should a robotics middleware framework be adopted?
- How will modules communicate?
- How will configuration be managed?
- How will software be tested?

## Safety

- What conditions trigger emergency shutdown?
- How will physical emergency stop be implemented?
- What happens if the MCU crashes?
- What happens if a motor becomes uncontrollable?
- What happens if battery voltage falls below safe limits?

---

# 21. Architectural Principles

The following principles govern future AURELIUS development.

### 1. Requirements before components

Do not choose hardware simply because it is interesting.

Define the requirement first.

```text
Requirement
     ↓
Specification
     ↓
Candidate Components
     ↓
Evaluation
     ↓
Selection
```

---

### 2. Keep real-time control local

The system responsible for controlling physical movement must remain capable of operating safely even if high-level software becomes unavailable.

---

### 3. Prefer simple solutions

Complexity should be introduced only when it solves a real engineering problem.

---

### 4. Design for replacement

Every major component should be replaceable whenever practical.

---

### 5. Measure before optimizing

Engineering decisions should be supported by:

- Measurements
- Experiments
- Datasheets
- Tests
- Logs

rather than assumptions.

---

### 6. Document decisions

Important decisions should be recorded with:

- Problem
- Constraints
- Alternatives
- Decision
- Reasoning
- Consequences

---

### 7. Build vertically

Whenever possible, implement complete end-to-end functionality rather than developing every subsystem independently for months.

For example:

```text
Command
  ↓
Communication
  ↓
MCU
  ↓
Motor Driver
  ↓
Motor
  ↓
Physical Movement
```

A small working system is more valuable than a large collection of unfinished subsystems.

---

### 8. Safety before autonomy

AURELIUS should become autonomous only after its underlying control systems are predictable and safe.

---

### 9. V1 is a foundation, not a destination

The purpose of V1 is not to create the final AURELIUS.

Its purpose is to prove that the architecture, engineering workflow, and development process work.

---

## Architecture Evolution

This document is intentionally a living document.

As AURELIUS develops, architectural assumptions should be challenged by:

- Prototype results
- Hardware testing
- Simulation
- Measurements
- Software experiments
- Failure analysis

When an architectural decision changes, the reason for the change should be documented rather than silently overwritten.
