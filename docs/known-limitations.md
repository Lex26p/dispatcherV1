# Dispatcher Known Limitations

## Назначение

Документ описывает актуальные ограничения Dispatcher после завершения Sprint 013.

Это текущий список ограничений, а не исторический журнал разработки.

История изменений хранится в:

    docs/development-log.md

## Общий статус

Dispatcher находится на стадии post-MVP foundation development.

Архитектурный и интеграционный foundation работает, но система пока не готова к промышленной эксплуатации.

## Что уже работает

Реализованы:

- модульный C++20 backend;
- application composition;
- object, device и tag domain foundations;
- protocol driver interfaces;
- polling foundation;
- runtime value foundation;
- historian foundation;
- event foundation;
- alarm foundation;
- API route foundation;
- Drogon HTTP transport;
- development CORS;
- System health endpoint;
- System modules endpoint;
- Blazor WebAssembly frontend;
- русский UI;
- реальная интеграция System page;
- local integration smoke-test.

## Backend limitations

### Ограниченный HTTP API

Реально доступны только:

    GET /api/system/health
    GET /api/system/modules

Objects, Devices, Tags, Runtime, Events и Alarms пока не предоставляют полноценные HTTP endpoints.

### Нет единого API error envelope

Backend пока не возвращает единый JSON-формат ошибок.

Не унифицированы ответы:

- 400;
- 401;
- 403;
- 404;
- 405;
- 409;
- 500;
- 503.

Не реализован correlation ID.

Неизвестные маршруты могут возвращать стандартный ответ Drogon вместо Dispatcher JSON response.

### JSON serialization foundation ограничен

System endpoints пока формируют JSON вручную.

Нет единого transport-независимого serializer foundation.

Перед расширением API нужно определить стабильные правила для:

- строк;
- массивов;
- nullable values;
- enum values;
- timestamps;
- errors;
- pagination.

### CORS предназначен только для разработки

Development origins задаются backend options.

Нет:

- внешней production configuration;
- production origin policy;
- credential policy;
- reverse-proxy integration.

### Нет HTTPS

Backend development listener использует HTTP.

TLS termination и production HTTPS пока не настроены.

### Завершение процесса остается development-only

`dispatcher_server` останавливается через ввод в консоль.

Не реализованы:

- SIGINT handling;
- SIGTERM handling;
- systemd service lifecycle;
- coordinated shutdown timeout;
- production service supervision.

### Module status является startup snapshot

System modules endpoint получает список модулей при построении route dispatcher.

Динамическое изменение runtime status модулей пока не отражается через отдельный status provider.

### Нет authentication и authorization

Не реализованы:

- users;
- sessions;
- tokens;
- roles;
- permissions;
- external identity providers;
- audit identity context.

### Нет production persistence

PostgreSQL schema foundation существует, но backend не подключен к production database.

Runtime, historian, events и alarms не образуют полноценный persistent production flow.

### Polling не подключен к реальному объекту

Protocol и polling foundations существуют, но нет завершенного production pipeline для реального Modbus TCP или SNMP оборудования.

### Historian ограничен foundation-уровнем

Не реализованы:

- production storage;
- buffered database writer;
- retention jobs;
- compression;
- downsampling;
- archive query API;
- recovery after restart.

### Alarm processing ограничен foundation-уровнем

Не реализованы:

- persistent alarm state;
- acknowledgement API;
- shelving;
- suppression;
- escalation;
- notification delivery;
- operator audit trail.

### Нет realtime transport

WebSocket/SSE gateway пока не реализован как рабочий transport.

## Frontend limitations

### Реальная интеграция ограничена System page

Реальный backend используется только страницей:

    /system

Другие страницы пока используют placeholder или demo-data.

### Legacy mock client еще существует

`IDispatcherApiClient` и `DispatcherApiClient` временно содержат demo-data для Runtime, Events и Alarms.

Они должны постепенно заменяться отдельными typed API clients.

### Нет authentication UI

Не реализованы:

- login;
- logout;
- session state;
- role-aware navigation;
- permission-aware widgets;
- token refresh.

### Нет realtime client

Frontend пока не получает live tag values, events или alarms через WebSocket/SSE.

### Нет полноценных операторских функций

Не реализованы:

- command execution;
- alarm acknowledgement;
- alarm shelving;
- dashboard editor;
- mimic editor;
- reports;
- maintenance workflows;
- notification settings.

### Нет localization engine

UI написан на русском языке, но полноценная infrastructure локализации еще не добавлена.

### Frontend configuration публична

`wwwroot/appsettings.json` загружается браузером.

В нем нельзя хранить секреты.

## Testing limitations

### Нет backend unit-test foundation

Каталог `tests` пока не содержит полноценного набора C++ unit tests.

Не покрыты автоматическими unit tests:

- route dispatcher;
- CORS options;
- tag transitions;
- archive policy;
- polling scheduling;
- event transitions;
- alarm transitions.

### Нет frontend automated tests

Не добавлены:

- API client unit tests;
- DTO mapping tests;
- component tests;
- browser end-to-end tests.

### Smoke-test ограничен System integration

`scripts/test-local-integration.ps1` проверяет только System API и CORS.

Он не заменяет полноценные unit, integration и browser tests.

## Build and delivery limitations

### Нет CI

Не настроены автоматические GitHub Actions для:

- Windows build;
- Linux build;
- frontend build;
- tests;
- LF validation;
- artifact publishing.

### Linux build еще не закреплен в CI

Production target — Linux, но постоянная автоматическая Linux-сборка пока отсутствует.

Не проверены на регулярной основе:

- GCC;
- Clang;
- Linux vcpkg triplet;
- case-sensitive paths;
- POSIX signals;
- systemd lifecycle.

### Нет release packaging

Не реализованы:

- versioned artifacts;
- Linux package;
- Windows installer;
- release archive;
- migration runner package;
- deployment manifest;
- checksum publishing.

### Docker отложен

Docker не используется на текущем этапе.

Архитектура должна позволять добавить контейнеризацию позже без изменения domain modules.

## Observability limitations

Не реализованы:

- structured logging;
- correlation ID;
- request tracing;
- metrics;
- health dependency checks;
- performance counters;
- centralized diagnostics;
- audit persistence.

## Security limitations

Не выполнены:

- threat model;
- secrets management;
- TLS policy;
- secure headers policy;
- rate limiting;
- request size enforcement на всех уровнях;
- dependency vulnerability scanning;
- security audit;
- penetration testing.

## Следующий инженерный этап

Следующий спринт:

    Sprint 014 — Engineering Baseline

Приоритеты:

- backend unit tests;
- frontend client tests;
- CTest;
- Windows/Linux CI;
- Linux preset;
- JSON serialization foundation;
- API error envelope;
- correlation ID;
- current-status document.

После Engineering Baseline планируется переход к:

    Sprint 015 — Object, Device and Tag Read API Foundation
