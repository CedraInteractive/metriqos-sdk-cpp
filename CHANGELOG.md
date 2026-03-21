# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/), and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.1.0] - 2026-03-19

### Added
- Core SDK with `init`, `shutdown`, `identify`, `track`, `flush`, `setOptOut`, `getStatus` API
- Automatic session management (session_start, session_end, heartbeat)
- Event batching with configurable interval and batch size
- libcurl HTTP transport with `X-API-Key` authentication
- Jittered exponential backoff with `Retry-After` header support
- SQLite offline queue for unreliable networks
- Platform-specific device info collection (Windows, macOS, Linux)
- User properties (`setUserProperty`) attached to every event
- GDPR opt-out support
- CMake build system with FetchContent for dependencies
- Unit tests for UUID, event serialization, and retry logic
- Raylib integration example
- CI workflow (Windows, macOS, Linux)
- Release workflow with prebuilt static libraries

[Unreleased]: https://github.com/CedraInteractive/teliqos-sdk-cpp/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/CedraInteractive/teliqos-sdk-cpp/releases/tag/v0.1.0
