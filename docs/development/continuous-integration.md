# Dispatcher Continuous Integration

## Назначение

Dispatcher использует GitHub Actions для автоматической проверки Windows и Linux.

Workflow:

    .github/workflows/ci.yml

Название:

    Dispatcher CI

## Триггеры

Workflow запускается:

- при push в `master`;
- при pull request в `master`;
- вручную через `workflow_dispatch`.

## Jobs

### Repository quality

Проверяет:

- LF line endings;
- валидность `CMakePresets.json`;
- наличие обязательных build и test файлов.

### Windows VS 2026

Runner:

    windows-2025-vs2026

Проверяет:

- pinned vcpkg;
- Windows x64 Debug configure;
- backend build;
- CTest;
- frontend restore;
- frontend Release build;
- frontend xUnit tests;
- LF line endings.

### Ubuntu 24.04

Runner:

    ubuntu-24.04

Проверяет:

- pinned vcpkg;
- Linux x64 Debug configure;
- Linux Debug backend build;
- Linux Debug CTest;
- Linux x64 Release configure;
- Linux Release backend build;
- Linux Release CTest;
- frontend Release build;
- frontend xUnit tests;
- case-sensitive paths;
- LF line endings.

## vcpkg

CI клонирует vcpkg и переключает его на commit:

    0308db5e395e701b5dff6e2f141bf7f42f58e60c

Этот commit совпадает с `builtin-baseline` в `vcpkg.json`.

Binary packages кешируются отдельно для Windows и Linux.

Cache key включает:

- operating system;
- architecture;
- hash `vcpkg.json`.

## .NET

CI явно устанавливает:

    .NET SDK 10.0.x

Product и test projects собираются через:

    frontend/Dispatcher.Frontend.slnx

## Локальные эквиваленты

Windows backend:

    cmake --preset x64-debug
    cmake --build --preset x64-debug
    ctest --preset x64-debug

Linux backend:

    bash scripts/test-linux-build.sh linux-x64-debug
    bash scripts/test-linux-build.sh linux-x64-release

Frontend:

    dotnet restore frontend/Dispatcher.Frontend.slnx
    dotnet build frontend/Dispatcher.Frontend.slnx
    dotnet test frontend/Dispatcher.Frontend.slnx

LF:

    pwsh scripts/check-line-endings.ps1

## Ограничения

CI пока не запускает:

- browser end-to-end tests;
- PostgreSQL;
- реальное оборудование;
- Docker;
- production deployment;
- release packaging;
- interactive local integration smoke-test.

Local HTTP/CORS smoke-test остается отдельной ручной проверкой, поскольку текущий `dispatcher_server` использует интерактивное завершение.
