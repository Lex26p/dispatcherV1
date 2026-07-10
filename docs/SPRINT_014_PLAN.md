# Sprint 014 Plan

## Название

Engineering Baseline

## Статус

Completed

## Этап

Post-MVP engineering stabilization.

## Диапазон шагов

Шаги 99–107.

## Цель

Защитить сформированный foundation Dispatcher автоматическими тестами, Windows/Linux CI и стабильными transport contracts до активного расширения Object, Device и Tag API.

## Причина

После Sprint 013 в проекте уже работали:

- модульный backend;
- Drogon HTTP transport;
- System API;
- development CORS;
- Blazor WebAssembly frontend;
- real System page integration;
- local integration smoke-test.

При этом отсутствовали:

- backend unit tests;
- frontend automated tests;
- CTest;
- Windows/Linux CI;
- регулярная Linux-сборка;
- контролируемый JSON serializer;
- единый API error envelope;
- transport correlation ID;
- канонический current-status document.

Расширение прикладного API без этих основ повысило бы стоимость последующих изменений.

## Главный результат

Sprint 014 завершен.

В проекте теперь есть:

- повторяемые backend unit tests;
- frontend service tests;
- CTest;
- `dotnet test`;
- Windows CI;
- Linux CI;
- Linux CMake presets;
- стабильный JSON serialization foundation;
- единый API error envelope;
- correlation ID foundation;
- `docs/CURRENT_STATUS.md`.

## Testing strategy

### Backend

Framework:

    Catch2 v3

Runner:

    CTest

Test registration:

    catch_discover_tests

Backend tests:

- собираются только при `BUILD_TESTING=ON`;
- не запускают настоящий network listener;
- не требуют database;
- не требуют реальных устройств;
- являются быстрыми и детерминированными;
- тестируют публичное поведение модулей.

### Frontend

Framework:

    xUnit

Runner:

    dotnet test

Первый frontend test scope:

- DTO mapping;
- API options;
- `SystemHttpApiClient`;
- successful responses;
- HTTP errors;
- invalid JSON;
- cancellation;
- connection failure;
- timeout;
- configuration errors;
- correlation ID.

### Test layers

Unit tests:

- отдельные типы и компоненты;
- без сети;
- без production infrastructure.

Local integration smoke-test:

    scripts/test-local-integration.ps1

Проверяет реальный development HTTP transport и CORS.

## Dependency rules

Backend test dependency:

    Catch2

Frontend test dependencies находятся только в test project.

JsonCpp является private implementation dependency `scada_http`.

Domain modules не зависят от Drogon или JsonCpp.

## CI strategy

### Windows

Проверяются:

- CMake configure;
- backend build;
- CTest;
- frontend build;
- frontend tests;
- LF validation.

### Linux

Проверяются:

- Debug configure;
- Debug build;
- Debug CTest;
- Release configure;
- Release build;
- Release CTest;
- frontend build;
- frontend tests;
- case-sensitive paths;
- LF validation.

CI не требует:

- PostgreSQL;
- оборудования;
- Docker;
- секретов;
- production certificates.

## Linux strategy

Добавлены presets:

    linux-x64-debug
    linux-x64-release

Используются:

- Ninja;
- GCC;
- vcpkg triplet `x64-linux`;
- Debug;
- Release.

Windows Visual Studio presets сохранены.

## JSON strategy

Ручная сборка JSON заменена на:

    dispatcher::http::JsonValue

JsonCpp скрыт внутри implementation.

System endpoints используют структурированную JSON serialization.

## API error contract

Формат:

    error.code
    error.message
    error.correlationId
    error.details

Покрыты основные transport statuses, включая:

- 400;
- 401;
- 403;
- 404;
- 405;
- 409;
- 413;
- 500;
- 501;
- 503.

## Correlation ID

Каждый API request получает correlation ID.

Правила:

- валидный входящий `X-Correlation-Id` сохраняется;
- отсутствующий или некорректный ID заменяется;
- ID возвращается в response header;
- ID включается в JSON error envelope.

## Выполненные шаги

### Шаг 99

Engineering baseline plan and test strategy.

### Шаг 100

Backend Catch2 and CTest foundation.

### Шаг 101

Backend HTTP and core behavior tests.

### Шаг 102

Frontend xUnit test foundation.

### Шаг 103

JSON serialization foundation.

### Шаг 104

API error envelope and correlation ID.

### Шаг 105

Linux CMake presets and portability cleanup.

### Шаг 106

Windows and Linux CI.

### Шаг 107

Current status and Sprint 014 closure.

## Acceptance criteria

Выполнены:

- Windows backend build проходит;
- Linux backend Debug build проходит;
- Linux backend Release build проходит;
- CTest запускает backend tests;
- все 36 backend tests проходят;
- frontend tests проходят;
- CI проходит на Windows и Linux;
- System endpoints используют serializer foundation;
- API errors имеют единый JSON envelope;
- responses содержат correlation ID;
- LF validation проходит;
- local integration smoke-test проходит;
- frontend и backend не имеют новых warnings;
- документация отражает текущее состояние.

## Out of scope

В Sprint 014 не входили:

- Object API;
- Device API;
- Tag API;
- Runtime API;
- Events API;
- Alarms API;
- PostgreSQL integration;
- real Modbus TCP;
- real SNMP;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- Docker;
- production deployment.

## Итог

Sprint 014 закрыт.

Engineering baseline готов к расширению прикладного API.

## Следующий спринт

Sprint 015 — Object, Device and Tag Read API Foundation.
