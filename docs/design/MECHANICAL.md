# AURELIUS — Mechanical Design

> **Document status:** Draft  
> **Document:** Mechanical Architecture and Design  
> **Version:** 1.0
> **Last updated:** 2026-08-16

---

## Table of Contents

- [1. Purpose](#1-purpose)
- [2. Mechanical Design Philosophy](#2-mechanical-design-philosophy)
- [3. Mechanical System Overview](#3-mechanical-system-overview)
- [4. Structural Architecture](#4-structural-architecture)
- [5. Actuation Architecture](#5-actuation-architecture)
- [6. Degrees of Freedom](#6-degrees-of-freedom)
- [7. Joint Architecture](#7-joint-architecture)
- [8. Sensors and Mechanical Integration](#8-sensors-and-mechanical-integration)
- [9. Electronics Integration](#9-electronics-integration)
- [10. Power System Integration](#10-power-system-integration)
- [11. Cable Management](#11-cable-management)
- [12. Materials](#12-materials)
- [13. Manufacturing](#13-manufacturing)
- [14. Mechanical Loads](#14-mechanical-loads)
- [15. Center of Mass](#15-center-of-mass)
- [16. Thermal Considerations](#16-thermal-considerations)
- [17. Serviceability](#17-serviceability)
- [18. Mechanical Safety](#18-mechanical-safety)
- [19. V1 Mechanical Scope](#19-v1-mechanical-scope)
- [20. Future Expansion](#20-future-expansion)
- [21. Open Mechanical Questions](#21-open-mechanical-questions)

---

# 1. Purpose

This document defines the mechanical architecture of AURELIUS.

It describes the physical structure, actuator integration, electronics mounting, cable routing, manufacturing approach, and mechanical safety requirements.

The design should remain adaptable until the first prototype requirements have been validated.

---

# 2. Mechanical Design Philosophy

The mechanical design should prioritize:

1. Structural integrity
2. Simplicity
3. Modularity
4. Low cost
5. Manufacturability
6. Serviceability
7. Weight efficiency
8. Future expansion

AURELIUS should not be mechanically complicated merely to resemble its inspiration.

Functional engineering decisions take priority over visual imitation.

---

# 3. Mechanical System Overview

The robot can be conceptually divided into:

```text
                         AURELIUS
                            │
             ┌──────────────┼──────────────┐
             │              │              │
             ▼              ▼              ▼
        STRUCTURE       ACTUATION      ENCLOSURE
             │              │              │
             ▼              ▼              ▼
          Frame          Motors        Electronics
             │              │
             └───────┬──────┘
                     ▼
              Physical Robot
```

The final geometry will be determined through iterative design.

---

# 4. Structural Architecture

The structural system should provide:

- Support for actuators
- Mounting points for electronics
- Sensor mounting
- Battery mounting
- Protection for internal components
- Defined mechanical interfaces

The structure should be divided into replaceable modules wherever practical.

Example:

```text
┌──────────────────────────────┐
│          Upper Module        │
├──────────────────────────────┤
│      Electronics Module      │
├──────────────────────────────┤
│        Actuation Module      │
├──────────────────────────────┤
│          Base Module         │
└──────────────────────────────┘
```

The exact geometry is TBD.

---

# 5. Actuation Architecture

Actuators are responsible for generating mechanical motion.

Potential actuator technologies include:

- DC gear motors
- Servo motors
- Brushless motors
- Linear actuators

Selection should be based on:

- Required torque
- Speed
- Position accuracy
- Weight
- Power consumption
- Cost
- Availability
- Control complexity

---

# 6. Degrees of Freedom

A degree of freedom (DOF) represents an independently controllable movement.

The V1 number of DOFs should be determined from the minimum physical behavior required.

Rather than designing a large number of joints immediately:

```text
Complex Robot
     ↑
     │
  Future
     │
     │
Simple V1
```

V1 should use the minimum number of DOFs necessary to validate the control architecture.

---

# 7. Joint Architecture

Each actuated joint should define:

- Rotation/translation axis
- Range of motion
- Maximum speed
- Maximum torque/force
- Mechanical stops
- Sensor position
- Motor mounting
- Cable routing
- Maintenance access

Conceptually:

```text
          Structural Link
                │
                ▼
        ┌──────────────┐
        │     Joint    │
        │              │
Motor ──┤   Bearing    ├── Link
        │              │
        └──────────────┘
                │
             Encoder
```

The final bearing and transmission architecture will depend on the selected actuator.

---

# 8. Sensors and Mechanical Integration

Sensors should be mechanically mounted so that measurements remain meaningful.

Examples:

### Encoder

Must maintain appropriate alignment with the actuator/joint.

### IMU

Should be rigidly attached to the structure to minimize unwanted vibration.

### Distance sensor

Should have a clear field of view.

### Camera

Should have:

- Stable mounting
- Appropriate orientation
- Protection
- Service access

Sensor mounting should be considered during CAD design rather than added after fabrication.

---

# 9. Electronics Integration

Electronics should be physically separated from unnecessary mechanical hazards.

Mounting requirements:

- Secure fastening
- Vibration resistance where required
- Airflow where required
- Connector access
- Cable access
- Replaceability

Conceptual enclosure:

```text
┌──────────────────────────────┐
│        Protective Shell      │
│                              │
│   ┌──────────────────────┐   │
│   │      Electronics     │   │
│   │                      │   │
│   │ MCU / Power / Comms  │   │
│   └──────────────────────┘   │
│                              │
└──────────────────────────────┘
```

---

# 10. Power System Integration

The battery should be mounted securely and positioned with system balance in mind.

Battery mounting must account for:

- Mass
- Mechanical shock
- Heat
- Cable routing
- Service access
- Charging/removal

The battery should not be located where normal mechanical movement can crush, puncture, or excessively heat it.

---

# 11. Cable Management

Cable routing is a mechanical design problem.

Cables should:

- Avoid moving mechanisms
- Avoid sharp edges
- Avoid high-temperature components
- Have appropriate strain relief
- Maintain bend radius
- Remain serviceable

For moving joints:

```text
Fixed Structure
      │
      │ cable
      ▼
   [strain relief]
      │
      │ flexible loop
      ▼
   Moving Joint
```

The cable path should be tested through the complete range of motion.

---

# 12. Materials

Potential materials include:

### 3D-printed plastics

Useful for:

- Brackets
- Covers
- Sensor mounts
- Prototypes
- Cable guides

### Aluminum

Useful for:

- Structural plates
- Motor mounts
- Load-bearing components

### Steel

Useful where higher strength or wear resistance is required.

### Fasteners

Standardized fasteners should be preferred where practical.

Material selection should consider:

- Strength
- Weight
- Cost
- Availability
- Manufacturability
- Heat resistance

---

# 13. Manufacturing

The project may use:

- 3D printing
- Hand tools
- Laser-cut components
- CNC machining where available
- Off-the-shelf mechanical hardware

The design should match available manufacturing capabilities.

A part that cannot realistically be manufactured is not a successful design.

---

# 14. Mechanical Loads

Every load-bearing component should eventually be evaluated for:

- Static load
- Dynamic load
- Impact
- Motor torque
- Joint reaction forces
- Vibrations

For rotational systems:

```text
Torque = Force × Lever Arm
```

For an actuator supporting a load:

```text
Required Torque
    ≈
Load × Distance
```

A suitable safety factor should be applied.

Exact values will depend on the final geometry.

---

# 15. Center of Mass

Center of mass should be considered when positioning:

- Battery
- Motors
- Electronics
- Structural components

A poor mass distribution can significantly increase actuator requirements.

The design should aim to keep heavy components positioned strategically relative to the robot's support structure.

---

# 16. Thermal Considerations

Mechanical design must provide adequate thermal paths for heat-generating components.

Potential heat sources:

- Motors
- Motor drivers
- Voltage regulators
- CPUs
- Batteries

Possible solutions:

- Ventilation
- Heat sinks
- Thermal interfaces
- Aluminum structures
- Active cooling where necessary

Thermal design should be validated experimentally.

---

# 17. Serviceability

AURELIUS should be designed so that common maintenance operations do not require complete disassembly.

Examples:

- Battery replacement
- Motor replacement
- Sensor replacement
- Firmware access
- Cable inspection
- Fuse replacement

A useful design principle is:

> **The component most likely to fail should not be the component hardest to reach.**

---

# 18. Mechanical Safety

The mechanical system should minimize:

- Pinch points
- Sharp edges
- Exposed rotating parts
- Uncontrolled movement
- Structural collapse
- Loose components

During early testing, exposed mechanisms should be treated as hazardous.

Testing should begin at reduced speed and reduced power where practical.

---

# 19. V1 Mechanical Scope

V1 should establish:

- A stable physical frame
- At least one controllable mechanical axis
- Secure actuator mounting
- Electronics mounting
- Battery mounting
- Basic cable management
- Mechanical limits
- Access for maintenance

V1 does not need to reproduce the final AURELIUS body.

Its purpose is to validate the mechanical and electrical architecture.

---

# 20. Future Expansion

Future mechanical development may introduce:

- Additional joints
- More sophisticated transmissions
- Improved structural materials
- Custom machined components
- Advanced locomotion
- Protective outer shell
- Modular limbs
- Sensor arrays
- Display mechanisms
- Improved cable routing

Future structures should reuse validated interfaces wherever possible.

---

# 21. Open Mechanical Questions

- What physical form should V1 use?
- What is the minimum useful number of DOFs?
- What are the target dimensions?
- What is the target mass?
- What actuator technology should be used?
- What torque is required?
- What transmission is required?
- What materials are practical?
- What manufacturing tools are available?
- Where should the battery be located?
- Where should the electronics be located?
- What mechanical interfaces should be standardized?
- What CAD workflow will be used?
- What tolerances are required?
- How will structural testing be performed?

These questions should be answered progressively through requirements analysis, calculations, CAD, prototyping, and physical testing.

---