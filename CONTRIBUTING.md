Contributing to AURELIUS
========================

Thank you for your interest in AURELIUS.

AURELIUS begins as a solo robotics project, but its architecture is intentionally designed to support future collaboration.

The project spans multiple engineering disciplines, including:

* Software engineering

* C/C++

* Embedded systems

* Electronics

* Mechanical engineering

* Robotics

* Control systems

* Computer vision

* Artificial intelligence

Contributions are welcome when they improve the project's engineering quality, functionality, documentation, or understanding.

* * *

Before Contributing
-------------------

Please read:

* `README.md`

* `ROADMAP.md`

* Relevant documentation under `docs/`

* Relevant Architecture Decision Records under `docs/decisions/`

For major changes, understand the existing architecture before proposing modifications.

* * *

Areas of Contribution
---------------------

Future contributors may work in areas such as:

### Embedded Systems

* Microcontroller firmware

* Drivers

* Communication protocols

* Sensor interfaces

* Motor control

* Real-time systems

### Electronics

* Circuit design

* PCB design

* Power systems

* Wiring

* Sensor integration

### Mechanical Systems

* CAD

* Structural design

* Actuator mounting

* Joint mechanisms

* Manufacturing

### Robotics

* Kinematics

* Motion planning

* Control systems

* Localization

* Navigation

### Software

* Robot control

* Simulation

* Computer vision

* Networking

* User interfaces

* Testing

### Artificial Intelligence

* Speech interfaces

* Perception

* Behavior systems

* AI-assisted interaction

### Documentation

Documentation is a first-class contribution.

Clear explanations, diagrams, experimental reports, and troubleshooting guides are valuable contributions.

* * *

Repository Structure
--------------------

    docs/          Engineering knowledge and decisions
    firmware/      Embedded software
    software/      High-level software
    hardware/      Electronics and mechanical designs
    experiments/   Engineering experiments
    tests/         Tests
    simulation/    Simulation
    tools/         Development utilities
    assets/        Supporting media

Keep new files in the subsystem to which they belong.

Avoid creating new top-level directories without a clear architectural reason.

* * *

Development Principles
----------------------

### Small changes

Prefer focused changes over large unrelated commits.

### Explain engineering decisions

When a change affects architecture, hardware, interfaces, safety, or performance, explain why the change was made.

### Test before merging

Software changes should include appropriate tests where practical.

Hardware changes should include:

* Test procedure

* Measurements

* Expected behavior

* Observed behavior

* Known limitations

* * *

Branches
--------

The `main` branch should remain stable.

For larger changes, use feature branches.

Small documentation changes may be made directly when appropriate.

* * *

Architecture Decisions
----------------------

Major technical decisions should be documented using an Architecture Decision Record (ADR).

An ADR should explain:

* Context

* Alternatives

* Decision

* Reasoning

* Consequences

A decision may later be replaced.

Do not erase the history of why the original decision was made.

* * *

Hardware Contributions
----------------------

Hardware changes should include appropriate documentation.

Depending on the change, this may include:

* CAD files

* Schematics

* PCB files

* Wiring diagrams

* Component specifications

* BOM updates

* Mechanical drawings

* Test results

Never commit sensitive credentials, private keys, passwords, or personal information.

* * *

Safety
------

Safety takes priority over experimentation speed.

Do not introduce changes that could reasonably cause:

* Electrical damage

* Battery hazards

* Unexpected motor movement

* Mechanical injury

* Thermal damage

* Loss of control

Hardware experiments involving significant power or moving mechanisms should be performed with appropriate precautions.

* * *

Code Quality
------------

Prefer:

* Clear names

* Small functions

* Modular architecture

* Explicit interfaces

* Comments explaining _why_, rather than merely _what_

* Deterministic behavior where required

* Appropriate error handling

* Testable components

Avoid unnecessary complexity.

AURELIUS is a learning project as well as an engineering project. Code should therefore favor clarity over cleverness.

* * *

Experimental Code
-----------------

Experimental code is allowed.

However, experimental work should be clearly separated from production components.

Use:
    experiments/

for exploratory work.

Once an experiment produces a reliable implementation suitable for AURELIUS, it may be promoted into the appropriate subsystem.

* * *

Future Contributors
-------------------

As the project grows, contribution guidelines may evolve.

The architecture, development process, coding standards, and review requirements should become more formal only when the project's complexity requires it.
