# Dispatcher Linux Build

## Назначение

Документ описывает локальную сборку backend Dispatcher на Linux.

Основной production target:

    Linux x64

Поддерживаемый build generator:

    Ninja

Управление C++ dependencies:

    vcpkg manifest mode

## CMake presets

Debug:

    linux-x64-debug

Release:

    linux-x64-release

Presets используют:

    VCPKG_TARGET_TRIPLET = x64-linux
    BUILD_TESTING = ON
    CMAKE_EXPORT_COMPILE_COMMANDS = ON

## Требования

Нужны:

- CMake 3.21 или новее;
- Ninja;
- GCC или Clang с поддержкой C++20;
- Git;
- Linux vcpkg installation;
- стандартные build tools.

Переменная окружения:

    VCPKG_ROOT

должна указывать на Linux installation vcpkg.

Windows executable `vcpkg.exe` нельзя использовать как Linux vcpkg host tool.

## Ubuntu packages

Рекомендуемый набор:

    sudo apt-get update
    sudo apt-get install -y build-essential cmake ninja-build git curl zip unzip tar pkg-config autoconf autoconf-archive automake libtool perl python3 rsync bison flex

## vcpkg

Пример установки:

    git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
    "$HOME/vcpkg/bootstrap-vcpkg.sh" -disableMetrics
    export VCPKG_ROOT="$HOME/vcpkg"

## Debug build

    bash scripts/test-linux-build.sh linux-x64-debug

## Release build

    bash scripts/test-linux-build.sh linux-x64-release

Скрипт выполняет:

- CMake configure;
- backend build;
- CTest;
- проверку наличия Linux vcpkg toolchain.

## Выбор компилятора

По умолчанию используется системный C++ compiler.

Для GCC перед первой конфигурацией:

    export CXX=g++

Для Clang перед первой конфигурацией:

    export CXX=clang++

При смене компилятора нужно удалить соответствующий build directory.

## Build outputs

Debug:

    out/build/linux-x64-debug

Release:

    out/build/linux-x64-release

Linux executable:

    out/build/linux-x64-debug/backend/apps/dispatcher_server/dispatcher_server

## WSL

Для корректной проверки case-sensitive paths рекомендуется собирать копию исходников в Linux filesystem, а не непосредственно в `/mnt/c`.

Windows и Linux используют разные vcpkg installations и разные build directories.

## Ограничения

Текущий `dispatcher_server` использует интерактивное завершение через стандартный ввод.

SIGINT, SIGTERM и systemd lifecycle будут реализованы отдельным этапом.
