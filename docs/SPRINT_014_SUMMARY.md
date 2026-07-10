# Sprint 014 Summary

## Название

Engineering Baseline

## Статус

Completed

## Шаги

Шаги 99–107.

## Этап

Post-MVP engineering stabilization.

## Цель

Защитить сформированный foundation Dispatcher автоматическими тестами, Windows/Linux CI и стабильными transport contracts до расширения Object, Device и Tag API.

## Главный результат

В Dispatcher сформирован повторяемый engineering baseline:

- backend unit tests;
- frontend service tests;
- CTest;
- `dotnet test`;
- Windows CI;
- Linux CI;
- Linux Debug и Release presets;
- JSON serialization foundation;
- единый API error envelope;
- correlation ID;
- LF validation;
- актуальный current-status document.

## Шаг 99 — Engineering baseline plan and test strategy

Созданы:

    docs/SPRINT_014_PLAN.md
    docs/adr/0003-testing-and-ci-strategy.md

Приняты решения:

- Catch2 v3 для backend tests;
- CTest для discovery и orchestration;
- xUnit для frontend service tests;
- отдельные unit, integration и smoke-test layers;
- обязательная Windows/Linux CI;
- отсутствие реальной сети и production infrastructure в unit tests.

## Шаг 100 — Backend Catch2 and CTest foundation

Добавлены:

- Catch2 dependency;
- `BUILD_TESTING`;
- CTest integration;
- `tests/CMakeLists.txt`;
- `scada_common_tests`;
- первые version tests.

## Шаг 101 — Backend HTTP and core behavior tests

Добавлен:

    scada_http_tests

Покрыты:

- HTTP method parsing;
- enum names;
- status names;
- response factories;
- CORS policy;
- server options;
- request validation;
- endpoint validation;
- route registration;
- duplicate routes;
- handler dispatch;
- 400;
- 404;
- 405.

## Шаг 102 — Frontend xUnit test foundation

Создан:

    frontend/Dispatcher.Frontend.Tests

Проект добавлен в frontend solution.

Покрыты:

- `DispatcherApiClientOptions`;
- URL и API path normalization;
- timeout fallback;
- DTO mapping;
- successful HTTP responses;
- HTTP status errors;
- invalid JSON;
- null response;
- connection failure;
- timeout;
- caller cancellation;
- configuration errors.

## Шаг 103 — JSON serialization foundation

Принято решение использовать JsonCpp как private implementation dependency `scada_http`.

Создан:

    dispatcher::http::JsonValue

Публичные headers не раскрывают JsonCpp или Drogon types.

System endpoints переведены с ручной конкатенации строк на структурированный serializer.

Добавлены tests для:

- scalar values;
- escaping;
- objects;
- arrays;
- null;
- nested values;
- System response contracts.

ADR:

    docs/adr/0004-json-serialization-strategy.md

## Шаг 104 — API error envelope and correlation ID

Добавлены:

- correlation ID validation;
- correlation ID generation;
- `X-Correlation-Id`;
- единый API error envelope;
- structured error details;
- framework-level Drogon error conversion;
- безопасное преобразование handler exceptions;
- frontend error DTO;
- frontend error code и correlation ID handling.

ADR:

    docs/adr/0005-api-errors-and-correlation-id.md

## Шаг 105 — Linux presets and portability cleanup

Добавлены:

- `linux-x64-debug`;
- `linux-x64-release`;
- Linux build presets;
- Linux test presets;
- `x64-linux`;
- Ninja;
- `Threads::Threads`;
- Linux build script;
- Linux build documentation.

MSVC-specific configuration ограничена MSVC compiler.

Исправлен поврежденный `.gitignore`.

## Шаг 106 — Windows and Linux CI

Создан:

    .github/workflows/ci.yml

Jobs:

- Repository quality;
- Windows VS 2026;
- Ubuntu 24.04.

Проверяются:

- LF;
- CMake presets;
- pinned vcpkg;
- Windows backend;
- Windows CTest;
- Linux Debug backend;
- Linux Debug CTest;
- Linux Release backend;
- Linux Release CTest;
- frontend build;
- frontend xUnit tests.

Все jobs успешно проходят.

## Шаг 107 — Current status and closure

Созданы:

    docs/CURRENT_STATUS.md
    docs/SPRINT_014_SUMMARY.md

Актуализированы:

    docs/SPRINT_014_PLAN.md
    docs/known-limitations.md
    docs/MVP_STATUS.md
    docs/roadmap.md
    docs/development-log.md

Sprint 014 закрыт.

## Test baseline

Backend:

    36 CTest tests

Frontend:

    xUnit service tests

Local integration:

    scripts/test-local-integration.ps1

Все текущие tests проходят.

## CI baseline

Подтверждены:

- Windows VS 2026 backend build;
- Windows CTest;
- Ubuntu Linux Debug build;
- Ubuntu Linux Debug CTest;
- Ubuntu Linux Release build;
- Ubuntu Linux Release CTest;
- frontend Release build;
- frontend xUnit tests;
- LF validation.

## Transport baseline

Рабочая цепочка:

    HTTP request
        -> Drogon adapter
        -> HttpRouteDispatcher
        -> endpoint
        -> JsonValue
        -> HttpResponse

Error chain:

    transport/framework error
        -> Dispatcher API error envelope
        -> X-Correlation-Id
        -> frontend DispatcherApiException

## Принятые ADR

- ADR 0003 — Testing and CI Strategy;
- ADR 0004 — JSON Serialization Strategy;
- ADR 0005 — API Errors and Correlation ID.

## Acceptance criteria

Выполнены:

- Windows backend build;
- Linux backend build;
- backend unit tests;
- CTest;
- frontend tests;
- Windows CI;
- Linux CI;
- JSON serializer foundation;
- API error envelope;
- correlation ID;
- LF validation;
- local integration smoke-test;
- актуальная документация;
- отсутствие новых build warnings.

## Что сознательно не входило

В Sprint 014 не реализовывались:

- Object API;
- Device API;
- Tag API;
- Runtime API;
- Events API;
- Alarms API;
- persistence;
- authentication;
- authorization;
- realtime transport;
- protocol drivers;
- dashboards;
- mimic diagrams;
- deployment.

## Итог

Sprint 014 завершил переход от ручного post-MVP development process к повторяемому engineering process.

Теперь изменения backend и frontend автоматически проверяются на Windows и Linux.

Transport contracts защищены tests и готовы к расширению прикладного API.

## Следующий спринт

Sprint 015 — Object, Device and Tag Read API Foundation.
