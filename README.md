![andro](docs/ico/AndroL.png)

# AndromacheOS

AndromacheOS is a hybrid modular operating system written in C, designed with a focus on modularity and resilience. It separates the kernel core from modular subsystems, allowing for a flexible and robust architecture.

## Features

- **Hybrid Modular Architecture**: Separation of core kernel logic and subsystems.
- **Freestanding Environment**: Built as a freestanding kernel using the C11 standard.
- **Modular Subsystems**: Includes support for dynamically handled modules.
- **IPC Support**: Inter-Process Communication capabilities.

## Architecture

The project is structured to maintain a clean separation between the core kernel and its modules:

- **Core**: `src/kernel.c` contains the main kernel entry point and core logic.
- **Modules**: `src/modules/` houses the modular subsystems.
- **Include**: `include/` contains the necessary header files.

## Modules

Currently implemented modules include:
- `child_handler`: Manages child processes/tasks.
- `ipc`: Handles Inter-Process Communication.

## Building

To build the project, ensure you have CMake installed.

```bash
mkdir -p build
cd build
cmake ..
make
```

## Dependencies

- **CMake**: Build system.
- **Linux Headers**: Relies on Linux kernel headers located in `extern/linux` for compatibility and definitions.
