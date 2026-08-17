# AURELIUS — Electrical Design

> **Document status:** Draft  
> **Document:** Electrical Architecture and Design  
> **Version:** 1.0
> **Last updated:** 2026-08-16

---

## Table of Contents

- [1. Purpose](#1-purpose)
- [2. Electrical Design Philosophy](#2-electrical-design-philosophy)
- [3. System Overview](#3-system-overview)
- [4. Electrical Domains](#4-electrical-domains)
- [5. Power Architecture](#5-power-architecture)
- [6. Battery System](#6-battery-system)
- [7. Power Distribution](#7-power-distribution)
- [8. Computing Electronics](#8-computing-electronics)
- [9. Sensor Electronics](#9-sensor-electronics)
- [10. Actuator Electronics](#10-actuator-electronics)
- [11. Communication Interfaces](#11-communication-interfaces)
- [12. Grounding](#12-grounding)
- [13. Electrical Protection](#13-electrical-protection)
- [14. Wiring](#14-wiring)
- [15. PCB Architecture](#15-pcb-architecture)
- [16. Electrical Testing](#16-electrical-testing)
- [17. V1 Electrical Scope](#17-v1-electrical-scope)
- [18. Future Expansion](#18-future-expansion)
- [19. Open Electrical Questions](#19-open-electrical-questions)

---

# 1. Purpose

This document defines the electrical architecture of AURELIUS.

It describes how electrical energy, signals, control information, and sensor data move through the robot.

Specific component selections are intentionally deferred until system requirements and power budgets have been established.

---

# 2. Electrical Design Philosophy

The electrical system should prioritize:

1. Safety
2. Reliability
3. Modularity
4. Serviceability
5. Expandability
6. Low electrical noise
7. Ease of debugging

The system should avoid unnecessarily complicated electrical architecture during early prototypes.

---

# 3. System Overview

The conceptual electrical architecture is:

```text
                         BATTERY
                            │
                            ▼
                   ┌────────────────┐
                   │ Main Protection│
                   └───────┬────────┘
                           │
                    Power Distribution
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
      Logic Rail       Motor Rail       Auxiliary
          │                │                │
          ▼                ▼                ▼
        MCU            Motor Drivers      Devices
          │                │
     ┌────┼────┐          ▼
     │    │    │        Motors
     ▼    ▼    ▼
 Sensors  Bus  Safety
```

---

# 4. Electrical Domains

AURELIUS should conceptually separate electrical loads into domains.

## 4.1 Logic Domain

Includes:

- Microcontroller
- Digital logic
- Communication interfaces
- Low-power sensors

Characteristics:

- Sensitive to voltage instability
- Sensitive to electrical noise
- Generally lower current

---

## 4.2 Actuator Domain

Includes:

- Motors
- Motor drivers
- High-current actuators

Characteristics:

- High transient current
- Electrical noise
- Potentially significant heat generation

---

## 4.3 Auxiliary Domain

May include:

- Displays
- Cameras
- Audio
- Lighting
- USB devices
- Future expansion modules

The auxiliary domain should not compromise the stability of the control system.

---

# 5. Power Architecture

The power architecture should follow:

```text
Battery
   │
   ▼
Fuse / Protection
   │
   ▼
Main Power Switch
   │
   ▼
Power Distribution
   │
   ├──────────────► Motor Power
   │
   ├──────────────► Logic Regulation
   │
   └──────────────► Auxiliary Regulation
```

The final voltage rails will be determined after component selection.

Potential rails may include:

- Battery voltage
- Motor supply
- Logic supply
- Sensor supply
- High-power computing supply

No voltage should be assumed until verified against component requirements.

---

# 6. Battery System

The battery system shall be selected according to:

- Required voltage
- Peak current
- Average current
- Desired runtime
- Weight
- Physical volume
- Safety
- Charging requirements

A preliminary energy estimate should follow:

```text
Energy ≈ Voltage × Capacity
```

Estimated runtime can then be approximated using:

```text
Runtime ≈ Available Energy / Average Power
```

These values must account for real-world inefficiencies and operating conditions.

---

# 7. Power Distribution

Power distribution should provide separate paths for major load categories.

```text
                 MAIN POWER
                     │
             ┌───────┴───────┐
             │               │
          LOGIC             MOTOR
             │               │
          Regulator       Driver(s)
             │               │
        ┌────┼────┐          ▼
        │    │    │        Motors
       MCU Sensors Bus
```

Motor current should not pass through traces, connectors, or regulators intended only for logic loads.

---

# 8. Computing Electronics

The computing architecture should consist of:

### Low-level controller

Handles:

- GPIO
- ADC
- PWM
- Timers
- Interrupts
- Sensor interfaces
- Motor control
- Safety monitoring

### High-level computer

Potentially handles:

- Vision
- Audio
- Planning
- User interface
- Logging
- AI
- Networking

The exact hardware is TBD.

---

# 9. Sensor Electronics

Sensors may communicate using:

- Analog signals
- Digital GPIO
- I²C
- SPI
- UART
- CAN
- USB

Sensor interfaces should be selected according to:

- Data rate
- Cable length
- Noise sensitivity
- Number of devices
- Timing requirements
- Power consumption

---

# 10. Actuator Electronics

Actuators require appropriate driver circuitry.

Conceptually:

```text
MCU
 │
 │ Control Signal
 ▼
Motor Driver
 │
 │ Power
 ▼
Motor
 │
 ▼
Mechanical System
 │
 ▼
Encoder / Sensor
 │
 └──────────► MCU
```

The motor driver must be sized for:

- Continuous current
- Peak current
- Supply voltage
- Thermal conditions
- Switching frequency
- Protection requirements

---

# 11. Communication Interfaces

Potential communication technologies include:

| Interface | Potential Use |
|---|---|
| GPIO | Simple digital signals |
| ADC | Analog sensors |
| PWM | Motor/servo control |
| I²C | Short-range peripheral communication |
| SPI | High-speed peripheral communication |
| UART | Point-to-point serial communication |
| CAN | Robust distributed control |
| USB | High-level peripherals |
| Ethernet | High-bandwidth networking |
| Wi-Fi | Wireless communication |
| Bluetooth | Short-range wireless control |

The final interface selection should be based on actual requirements.

---

# 12. Grounding

AURELIUS should use a deliberate grounding strategy.

Important considerations:

- Common reference voltage
- High-current return paths
- Motor noise
- Sensor noise
- Ground loops
- Logic stability

High-current motor returns should not unnecessarily share thin traces with sensitive sensor grounds.

---

# 13. Electrical Protection

Potential protection mechanisms include:

### Overcurrent

- Fuses
- Current-limited regulators
- Protected motor drivers

### Overvoltage

- Voltage regulators
- TVS protection where appropriate
- Correct power supplies

### Undervoltage

- Battery monitoring
- Low-voltage shutdown

### Thermal

- Temperature sensors
- Thermal shutdown
- Heat sinks
- Adequate ventilation

### Reverse polarity

Where appropriate:

- Protection circuits
- Correct connectors
- Mechanical keying

---

# 14. Wiring

Wiring should be designed as part of the robot rather than added as an afterthought.

Requirements include:

- Secure connectors
- Strain relief
- Protected cable paths
- Appropriate wire gauge
- Separation of high-power and sensitive signal wiring
- Labeling
- Service loops around moving joints

Moving wires require special attention to:

- Bend radius
- Repeated flexing
- Abrasion
- Connector stress

---

# 15. PCB Architecture

The first prototype may use development boards and modular breakout boards.

As the design stabilizes, custom PCBs may consolidate:

- Power distribution
- Sensor connections
- MCU
- Communication
- Motor control

Potential PCB hierarchy:

```text
                Main Control PCB
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
      Power PCB     Sensor PCB   Motor PCB
```

Custom PCBs should be introduced only when they provide a meaningful benefit.

---

# 16. Electrical Testing

Electrical testing should proceed incrementally.

### Stage 1 — Power

Verify:

- Voltage
- Current
- Regulation
- Protection

### Stage 2 — Logic

Verify:

- MCU startup
- Communication
- GPIO
- Sensors

### Stage 3 — Actuation

Verify:

- Driver behavior
- Motor direction
- Current
- Temperature

### Stage 4 — Integration

Verify:

- Simultaneous loads
- Communication
- Sensors
- Motors
- Power stability

---

# 17. V1 Electrical Scope

V1 should establish:

- Battery/power source
- Power protection
- Voltage regulation
- MCU
- At least one actuator driver
- At least one actuator
- Basic sensors
- Communication interface
- Emergency-stop capability

Exact component selection remains TBD.

---

# 18. Future Expansion

The electrical architecture should eventually support:

- Additional motor channels
- Higher-performance actuators
- Additional sensor buses
- Custom PCBs
- Battery monitoring
- Power telemetry
- Wireless communication
- Cameras
- Audio hardware
- High-performance computing

Expansion should occur without compromising the core control and safety system.

---

# 19. Open Electrical Questions

- What is the V1 actuator?
- What is its operating voltage?
- What are its continuous and peak current requirements?
- What is the expected total system power?
- What battery technology is appropriate?
- What runtime is required?
- What voltage rails are needed?
- Which MCU should be used?
- Is CAN required?
- Which components require custom PCBs?
- How should the emergency-stop circuit be implemented?
- What thermal management is required?

These questions must be resolved through engineering analysis before final electrical implementation.

---