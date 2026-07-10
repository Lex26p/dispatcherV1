# Sprint 014 Plan

## Название

Engineering Baseline

## Статус

In Progress

## Этап

Post-MVP engineering stabilization

## Диапазон шагов

Шаги 99–107.

## Цель

Защитить сформированный foundation Dispatcher автоматическими тестами, Windows/Linux CI и стабильными transport contracts до активного расширения Object, Device и Tag API.

## Причина

После Sprint 013 в проекте уже работают:

- модульный backend;
- Drogon HTTP transport;
- System API;
- development CORS;
- Blazor WebAssembly frontend;
- real System page integration;
- local integration smoke-test.

При этом пока отсутствуют:

- backend unit tests;
- frontend automated tests;
- CTest integration;
- Windows/Linux CI;
- регулярная Linux-сборка;
- единый JSON serializer foundation;
- единый API error envelope;
- correlation ID;
- единый документ текущего состояния проекта.

Расширение прикладного API без этих основ повысит стоимость последующих переделок.

## Главный результат

После Sprint 014 проект должен иметь:

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
- актуальный `CURRENT_STATUS.md`.

## Testing strategy

### Backend

Framework:

    Catch2 v3

Runner and orchestration:

    CTest

Test registration:

    catch_discover_tests

Backend tests должны:

- собираться только при `BUILD_TESTING=ON`;
- не запускать реальные сетевые listeners для unit tests;
- не зависеть от внешней базы данных;
- не зависеть от реальных устройств;
- быть быстрыми и детерминированными;
- тестировать публичное поведение модулей.

### Frontend

Framework:

    xUnit

Runner:

    dotnet test

Первый frontend test scope:

- DTO mapping;
- API options normalization;
- `SystemHttpApiClient`;
- connection failure;
- HTTP error;
- invalid JSON;
- cancellation;
- timeout-related behavior, где это возможно без нестабильных задержек.

Component tests и browser tests не входят в минимальный baseline этого спринта.

### Test layers

Unit tests:

- отдельные классы и чистые компоненты;
- без сети;
- без filesystem side effects;
- без database.

Integration tests:

- взаимодействие нескольких внутренних модулей;
- controlled dependencies;
- без production infrastructure.

Local smoke-test:

    scripts/test-local-integration.ps1

Проверяет работающий backend HTTP transport и CORS.

CI smoke-test может иметь отдельный режим, если интерактивный lifecycle `dispatcher_server` нельзя надежно использовать в pipeline.

## Test naming

Имена C++ и C# тестов пишутся на английском языке.

Пользовательские UI-тексты остаются на русском языке.

Рекомендуемый стиль:

    component_action_expected_result

или читаемое поведение:

    dispatch_returns_not_found_for_unknown_path

## Test directory structure

Рекомендуемая backend-структура:

    tests/
        CMakeLists.txt
        backend/
            scada_http/
            scada_tags/
            scada_historian/
            scada_events/
            scada_alarms/

Рекомендуемая frontend-структура:

    frontend/
        Dispatcher.Frontend.Tests/

Тесты группируются по тестируемому модулю, а не по типу assertion.

## Dependency rules

Тестовые зависимости должны быть объявлены явно.

Backend:

- Catch2 добавляется в `vcpkg.json`;
- тестовые targets не должны становиться зависимостями product targets.

Frontend:

- test packages добавляются только в test project;
- frontend product project не должен зависеть от xUnit.

## CI strategy

GitHub Actions должен проверять:

### Windows

- CMake configure;
- backend build;
- CTest;
- frontend build;
- frontend tests;
- LF validation.

### Linux

- CMake configure;
- backend build;
- CTest;
- frontend build;
- frontend tests;
- case-sensitive paths.

CI не должен требовать:

- PostgreSQL;
- реальное оборудование;
- Docker;
- секреты;
- production certificates.

## Linux strategy

Нужно добавить Linux presets, совместимые с:

- Ninja;
- GCC или Clang;
- vcpkg triplet `x64-linux`;
- Debug;
- Release.

Windows Visual Studio presets сохраняются.

Product code не должен предполагать:

- Windows paths;
- console-only shutdown;
- case-insensitive filesystem;
- MSVC-specific behavior без защитных условий.

## JSON strategy

Ручная сборка JSON через конкатенацию строк должна быть заменена контролируемым serializer foundation.

Требования:

- JSON library остается implementation detail transport layer;
- domain models не должны зависеть от Drogon;
- serializers должны тестироваться;
- enum, timestamp, nullable и array conventions должны быть единообразными;
- ошибки сериализации должны обрабатываться явно.

Точный serializer design будет зафиксирован отдельным решением перед реализацией шага 103.

## API error contract

Нужно определить единый envelope:

    error
        code
        message
        correlationId
        details

Минимальные HTTP statuses:

- 400;
- 401;
- 403;
- 404;
- 405;
- 409;
- 500;
- 503.

Frontend не должен анализировать plain-text ошибки Drogon.

## Correlation ID

Каждый HTTP request должен получать correlation ID.

Правила:

- принять входящий `X-Correlation-Id`, если он валиден;
- иначе создать новый ID;
- вернуть ID в response header;
- включить ID в JSON error envelope;
- позже включить ID в structured logs.

## In scope

В Sprint 014 входит:

- план и ADR engineering baseline;
- Catch2 dependency;
- CTest foundation;
- первые backend unit tests;
- frontend xUnit project;
- frontend API client tests;
- JSON serializer foundation;
- API error envelope;
- correlation ID;
- Linux presets;
- Windows/Linux GitHub Actions;
- LF validation;
- `CURRENT_STATUS.md`;
- закрытие документации Sprint 014.

## Out of scope

В Sprint 014 не входит:

- Object API;
- Device API;
- Tag API;
- Runtime API;
- Events API;
- Alarms API;
- PostgreSQL integration;
- real Modbus TCP polling;
- real SNMP polling;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- Docker;
- production deployment.

## План шагов

### Шаг 99 — Engineering baseline plan and test strategy

Результат:

- создан Sprint 014 plan;
- принято решение по Catch2, CTest и xUnit;
- определены test layers и CI scope.

### Шаг 100 — Backend Catch2 and CTest foundation

Результат:

- Catch2 добавлен в vcpkg;
- корневой CMake подключает CTest;
- создан `tests/CMakeLists.txt`;
- первый backend test target собирается и запускается.

### Шаг 101 — Backend HTTP and core behavior tests

Результат:

- протестирован `HttpRouteDispatcher`;
- протестированы HTTP method/status helpers;
- протестирован `HttpCorsOptions`;
- зафиксированы 400, 404, 405 и duplicate route rules.

### Шаг 102 — Frontend xUnit test foundation

Результат:

- создан `Dispatcher.Frontend.Tests`;
- добавлены tests для API options;
- добавлены DTO mapping tests;
- добавлены `SystemHttpApiClient` tests;
- `dotnet test` проходит.

### Шаг 103 — JSON serialization foundation

Результат:

- выбран и зафиксирован serializer design;
- ручная сборка JSON в System endpoints заменена;
- serialization покрыта tests;
- domain modules не зависят от JSON framework.

### Шаг 104 — API error envelope and correlation ID

Результат:

- добавлен единый error response contract;
- добавлен correlation ID;
- базовые transport errors возвращаются как Dispatcher JSON;
- frontend foundation готов к чтению correlation ID.

### Шаг 105 — Linux CMake presets and portability cleanup

Результат:

- добавлены Linux Debug/Release presets;
- проект собирается с GCC или Clang;
- исправлены найденные portability warnings;
- Windows presets продолжают работать.

### Шаг 106 — Windows and Linux CI

Результат:

- добавлен GitHub Actions workflow;
- проверяется backend build;
- запускается CTest;
- проверяется frontend build;
- запускается `dotnet test`;
- проверяются LF endings.

### Шаг 107 — Current status and Sprint 014 closure

Результат:

- создан `docs/CURRENT_STATUS.md`;
- создан `docs/SPRINT_014_SUMMARY.md`;
- обновлены limitations и development log;
- Sprint 014 закрыт.

## Acceptance criteria

Sprint 014 считается выполненным, если:

- Windows backend build проходит;
- Linux backend build проходит;
- CTest запускает backend tests;
- backend unit tests проходят;
- frontend tests проходят;
- CI проходит на Windows и Linux;
- System endpoints используют serializer foundation;
- базовые API errors имеют единый JSON envelope;
- responses содержат correlation ID;
- LF validation проходит;
- local integration smoke-test продолжает работать;
- frontend и backend не имеют новых warnings;
- documentation отражает текущее состояние.

## Следующий спринт

    Sprint 015 — Object, Device and Tag Read API Foundation

Sprint 015 начинается только после стабильного прохождения test и CI baseline.
