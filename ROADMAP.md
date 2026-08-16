AURELIUS Roadmap
================

This roadmap describes the intended development path of AURELIUS.

The roadmap is deliberately flexible.

Engineering discoveries, failures, hardware availability, academic workload, and new ideas may change the sequence or scope of milestones.

The goal is not to obey a calendar blindly.

The goal is to progressively increase the robot's capabilities while developing the engineering foundation necessary to understand them. 

* * *

Phase 1 — Programming Foundation
================================

**Target:** Month 1

### Objective

Develop a strong foundation in C and C++ suitable for embedded and robotics development.

### C

* Variables and types

* Control flow

* Functions

* Arrays

* Pointers

* Structs

* Memory management

* Bitwise operations

* Compilation and linking

* Debugging

### C++

* Classes

* Encapsulation

* References

* RAII

* STL

* Smart pointers

* Basic templates

* Interfaces

* Modular architecture

### Deliverable

Build a small software simulation of a robot containing:
    Robot
     ├── Motor
     ├── Sensor
     └── Joint

* * *

Phase 2 — Electronics Foundation
================================

**Target:** Months 1–2

### Objective

Understand the electrical systems that will eventually power AURELIUS.

### Topics

* Voltage

* Current

* Resistance

* Power

* Ohm's law

* Kirchhoff's laws

* Series/parallel circuits

* Capacitors

* Diodes

* Transistors

* MOSFETs

* Voltage regulation

* Batteries

* Motor fundamentals

### Laboratory

* LED circuit

* Button input

* Potentiometer

* Voltage measurement

* Basic transistor circuit

* Motor experiment

### Deliverable

A functioning electronics test bench.

* * *

Phase 3 — Embedded Systems
==========================

**Target:** Months 2–3

### Objective

Develop the ability to control physical hardware using a microcontroller.

### Topics

* GPIO

* PWM

* ADC

* Timers

* Interrupts

* UART

* I²C

* SPI

* Memory

* Real-time considerations

* Hardware abstraction

### Experiments

* GPIO control

* PWM

* Servo control

* Sensor reading

* UART communication

* I²C sensor

* SPI device

* Encoder reading

### Deliverable

A microcontroller capable of controlling and reading basic robotic hardware.

* * *

Phase 4 — Mechanical Engineering
================================

**Target:** Months 2–4

### Objective

Develop the ability to design and prototype mechanical structures.

### Topics

* Force

* Torque

* Center of mass

* Friction

* Gear ratios

* Mechanical advantage

* Bearings

* Shafts

* Fasteners

* Structural rigidity

* Tolerances

### CAD

* Sketches

* Constraints

* Parts

* Assemblies

* Joints

* Technical drawings

* Manufacturing considerations

### Deliverable

Design and manufacture a prototype robotic joint.

* * *

Phase 5 — AURELIUS Joint
========================

**Target:** Months 3–5

### Objective

Build the fundamental actuator unit of AURELIUS.

### Requirements

The joint should:

* Have a motor/actuator

* Have position feedback

* Have mechanical limits

* Receive commands from software

* Move to a specified position

* Maintain a target position

* Detect abnormal conditions

### Control

* Open-loop control

* Closed-loop control

* PID fundamentals

* Position control

* Velocity control

### Deliverable

> **AURELIUS Joint V1**

A single controllable robotic joint.

* * *

Phase 6 — Kinematics and Robotics
=================================

**Target:** Months 4–6

### Objective

Move from individual joints to coordinated robotic motion.

### Mathematics

* Vectors

* Matrices

* Coordinate systems

* Rotations

* Transformation matrices

### Robotics

* Forward kinematics

* Inverse kinematics

* Joint limits

* Motion interpolation

* Trajectory generation

### Deliverable

A simulated and physical multi-joint limb.

* * *

Phase 7 — AURELIUS Prototype 0
==============================

**Target:** Months 5–7

### Objective

Build the first integrated physical prototype.

The prototype does not need to resemble TARS perfectly.

Priority:

> **Function before appearance.**

### Systems

* Multiple actuators

* Motor controllers

* Encoders

* IMU

* Power system

* Embedded controller

* Communication system

* Mechanical frame

* Emergency stop/safety mechanism

### Deliverable

> **AURELIUS-0**

A physically integrated robotic prototype capable of controlled movement.

* * *

Phase 8 — Perception
====================

**Target:** Months 6–8

### Sensors

* IMU

* Distance sensors

* Encoders

* Camera

* Additional environmental sensors as required

### Software

* Sensor abstraction

* Sensor fusion

* Camera pipeline

* Basic computer vision

### Deliverable

AURELIUS can obtain and interpret basic information about its environment.

* * *

Phase 9 — Robotics Software Architecture
========================================

**Target:** Months 7–9

### Objective

Develop a scalable software architecture for the complete robot.

Potential technologies:

* C++

* Linux

* ROS 2

* Python where appropriate

* Networking protocols

* Simulation tools

### Architecture

    Perception
         ↓
    State Estimation
         ↓
    Planning
         ↓
    Motion Control
         ↓
    Hardware

### Deliverable

A modular software architecture capable of coordinating multiple robotic subsystems.

* * *

Phase 10 — Autonomy
===================

**Target:** Months 8–10

### Capabilities

* State machines

* Localization

* Obstacle detection

* Path planning

* Obstacle avoidance

* Basic navigation

* Autonomous behavior

### Deliverable

AURELIUS can perform a predefined task without continuous human control.

* * *

Phase 11 — Human Interaction
============================

**Target:** Months 9–11

### Objective

Allow humans to communicate naturally with AURELIUS.

### Systems

* Audio input

* Speech recognition

* Command interpretation

* Voice output

* Behavioral responses

### Example

    Human
      ↓
    Speech
      ↓
    Recognition
      ↓
    Intent
      ↓
    Behavior
      ↓
    Robot Action

### Deliverable

Basic voice-controlled interaction.

* * *

Phase 12 — AURELIUS V1
======================

**Target:** Months 10–12+

### Objective

Integrate the major subsystems into the first complete prototype.

### V1 Goals

* Functional mechanical structure

* Reliable actuator control

* Sensor integration

* Embedded firmware

* High-level software

* Basic autonomy

* Human interaction

* Safety systems

* Documentation

* Demonstration

### Deliverable

> **AURELIUS V1**

The first complete AURELIUS prototype.

It will almost certainly be imperfect.

That is expected.

* * *

Roadmap Principle
=================

The roadmap is not a promise that every milestone will happen on schedule.

It is a direction.

The project should prioritize:
    Understanding
        ↓
    Experimentation
        ↓
    Reliability
        ↓
    Integration
        ↓
    Capability

rather than rushing toward a visually impressive prototype.


