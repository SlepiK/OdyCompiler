> [!IMPORTANT]\
> **This repository is archived**\
> OdyCompiler is no longer developed here. Active development has moved to:
>
> **https://gitlab.example.com/sierra/odycompiler**
>
> Please open issues, merge requests, and questions there. This repository is kept read-only for reference.
> The last release published here is [**v0.1.0**](https://github.com/SlepiK/OdyPlan/releases/tag/v0.6.3).

---

# OdyCompiler

![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform: Linux x64](https://img.shields.io/badge/platform-Linux%20x64-lightgrey.svg)
![Status: Archived](https://img.shields.io/badge/status-archived-red.svg)

**OdyCompiler** is a C++ based on OdyPlan (https://gitlab.example.com/sierra/odyplan) created Compiler to compile given logical plans to a native executable.

> ⚠️ **Research prototype.**\
> OdyCompiler is intended for experimentation and exploration, not for production use.

## Versions & Releases

The last release published on this repository is [**v0.1.0**](https://github.com/SlepiK/OdyPlan/releases/tag/v0.6.3). For anything newer, see the GitLab repository linked above.

## Platform Support

OdyCompiler has so far only been built and tested on **Linux (x86-64)**. Other operating systems and architectures (Windows, macOS, ARM) are untested and may require changes to build or run.

## Compilation Modes

OdyCompiler only supports **AOT**. See [`examples/plan`] for working demonstrations.

## Architecture

The project is split into clearly separated modules. Each module has its own `include/` (public API) and `src/` (implementation):

```
odycompiler/          compiler, interfaces
└── parser/           parsers
```

> ⚠️ The Parser for Odysseus (OdysseusPlanParser) is only a POC - it should be reimplemented by using a more cleaner approach!

## Build

```bash
cmake -S . -B build
cmake --build build
```

**Dependencies:** `nlohmann_json`, `spdlog`, `llvm`, `zstd`, `odyplan`

## Namespaces

```cpp
sierra::odycompiler::...
```

## License

This project is licensed under the [MIT License](LICENSE).