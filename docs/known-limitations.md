# Dispatcher Known Limitations

## Назначение

Документ описывает актуальные ограничения Dispatcher после завершения Sprint 014.

Это текущий список ограничений, а не исторический журнал.

Текущее состояние проекта:

    docs/CURRENT_STATUS.md

История разработки:

    docs/development-log.md

## Общий статус

Dispatcher находится на стадии post-MVP engineering development.

Архитектурный, transport, frontend integration, testing и CI foundations работают.

Система пока не готова к промышленной эксплуатации.

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
- JSON serialization foundation;
- единый API error envelope;
- correlation ID;
- Blazor WebAssembly frontend;
- русский UI;
- реальная интеграция System page;
- backend Catch2 tests;
- frontend xUnit tests;
- CTest;
- local integration smoke-test;
- Windows CI;
- Linux Debug и Release CI;
- LF validation.

## Backend limitations

### Ограниченный HTTP API

Реально доступны только:

    GET /api/system/health
    GET /api/system/modules

Objects, Devices, Tags, Runtime, History, Events и Alarms пока не предоставляют полноценные HTTP endpoints.

### JSON conventions пока ограничены

`JsonValue` поддерживает базовые JSON types и используется System endpoints.

Пока не зафиксированы полные conventions для:

- timestamps во всех API areas;
- pagination;
- filtering;
- sorting;
- nullable domain fields;
- polymorphic values;
- large numeric values во frontend;
- API contract versioning.

### Error catalog пока минимален

Единый error envelope реализован.

Пока отсутствуют:

- централизованный production error catalog;
- localized frontend mapping для всех backend error codes;
- field-level validation errors;
- batch operation errors;
- retry metadata;
- rate-limit metadata.

### Correlation ID не связан с logging

Correlation ID передается через HTTP и error body.

Пока отсутствуют:

- structured request logs;
- correlation ID в log context;
- distributed tracing;
- span identifiers;
- trace export.

### CORS предназначен только для разработки

Development origins задаются backend options.

Нет:

- external production configuration;
- production origin policy;
- reverse-proxy-aware policy;
- deployment-specific credential policy.

### Нет HTTPS configuration

Backend development listener использует HTTP.

Production TLS termination и HTTPS deployment пока не настроены.

### Завершение процесса development-only

`dispatcher_server` останавливается через ввод в консоль.

Не реализованы:

- SIGINT handling;
- SIGTERM handling;
- Windows Service lifecycle;
- systemd lifecycle;
- coordinated shutdown timeout;
- background worker shutdown;
- production service supervision.

### Module status является startup snapshot

System modules endpoint получает список модулей при создании route dispatcher.

Динамическое изменение runtime status пока не предоставляется через отдельный status provider.

### Нет authentication и authorization

Не реализованы:

- users;
- sessions;
- access tokens;
- refresh tokens;
- roles;
- permissions;
- object responsibility enforcement;
- external identity providers;
- operator identity context.

### Нет production persistence

Database migrations и repository interfaces существуют.

Нет production implementations для:

- objects;
- devices;
- tags;
- polling configuration;
- current values;
- history;
- events;
- alarms.

### Нет production configuration subsystem

Пока отсутствуют:

- backend configuration files;
- environment override model;
- secret references;
- configuration validation report;
- configuration reload;
- deployment profiles.

### Нет реальных protocol drivers

Существуют protocol contracts и simulator foundation.

Не реализованы:

- Modbus TCP;
- SNMP;
- OPC UA;
- MQTT;
- BACnet;
- S7;
- IEC 60870-5-104;
- DNP3;
- retry/backoff;
- connection pooling;
- protocol diagnostics workers.

### Polling ограничен foundation-уровнем

Нет:

- background polling loop;
- scheduler service lifecycle;
- concurrency control;
- device load limiting;
- retry scheduling;
- connection-aware polling;
- production diagnostics.

### Runtime pipeline не завершен

`TagValueStore` и value applier существуют.

Нет полной production-цепочки:

    Polling
        -> TagValueStore
        -> EventBus
        -> Historian
        -> Events
        -> Alarms
        -> Realtime

### Historian ограничен foundation-уровнем

Не реализованы:

- PostgreSQL writer;
- TimescaleDB hypertables;
- batch persistence service;
- retention jobs;
- compression;
- downsampling;
- archive query API;
- disk spool;
- restart recovery.

### Events ограничены foundation-уровнем

Не реализованы:

- Event Manager;
- automatic event creation;
- persistence;
- filtering API;
- operator journal API;
- realtime delivery.

### Alarm processing ограничен foundation-уровнем

Не реализованы:

- Alarm Manager;
- persistent state;
- automatic rule loop;
- acknowledgement API;
- shelving;
- suppression;
- escalation;
- notification delivery;
- operator audit trail.

### Нет realtime transport

WebSocket и SSE пока не работают как transport.

Нет:

- client registry;
- subscription lifecycle;
- topic routing;
- delivery backpressure;
- reconnect protocol;
- frontend realtime client.

## Frontend limitations

### Реальная интеграция ограничена System page

Реальный backend используется только страницей:

    /system

Другие страницы используют placeholder или demo-data.

### Legacy demo-client остается

`IDispatcherApiClient` и `DispatcherApiClient` временно обслуживают demo-data для:

- Runtime;
- Events;
- Alarms.

Они должны заменяться отдельными typed API clients.

### Objects, Devices и Tags не подключены к backend

Пока отсутствуют:

- typed clients;
- loading real data;
- empty real state;
- backend validation errors;
- selection state;
- pagination;
- editing operations.

### Нет authentication UI

Не реализованы:

- login;
- logout;
- session state;
- role-aware navigation;
- permission-aware widgets;
- token refresh.

### Нет realtime client

Frontend не получает live values, events или alarms через WebSocket/SSE.

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

UI написан на русском языке.

Полноценная localization infrastructure пока не добавлена.

### Frontend configuration публична

`wwwroot/appsettings.json` загружается браузером.

В нем нельзя хранить секреты.

## Testing limitations

### Backend coverage пока сосредоточен на transport foundation

Автоматически покрыты:

- `scada_common` version behavior;
- `scada_http`;
- JSON;
- routing;
- CORS;
- API errors;
- correlation ID;
- System contracts.

Пока нет систематических unit tests для:

- objects;
- devices;
- tags;
- protocols;
- polling;
- runtime;
- historian;
- events;
- alarms;
- application composition.

### Нет настоящих network integration tests в CTest

Unit tests не запускают реальный Drogon listener.

Реальный listener проверяет local PowerShell smoke-test.

CI пока не запускает автоматический HTTP smoke-test.

### Frontend coverage ограничен service layer

Покрыты API options, DTO mappings и `SystemHttpApiClient`.

Не добавлены:

- Razor component tests;
- MudBlazor component tests;
- browser end-to-end tests;
- accessibility tests;
- visual regression tests.

### Нет performance и endurance tests

Не выполняются:

- HTTP load tests;
- high-frequency tag update tests;
- historian throughput tests;
- long-running polling tests;
- memory growth tests;
- reconnect storm tests.

## Build and delivery limitations

### CI не публикует artifacts

CI собирает и тестирует проект, но не публикует:

- backend executable;
- frontend static assets;
- symbols;
- checksums;
- test reports as artifacts;
- coverage reports.

### Linux CI использует один основной compiler path

Linux CI проверяет GCC на Ubuntu.

Clang matrix пока не добавлена.

### Нет release packaging

Не реализованы:

- versioned release artifacts;
- Linux package;
- Windows installer;
- release archive;
- migration runner package;
- deployment manifest;
- checksum publishing.

### Docker отложен

Docker не используется на текущем этапе.

Domain architecture не должна зависеть от контейнеризации.

## Observability limitations

Не реализованы:

- structured logging;
- log rotation policy;
- request logging policy;
- correlation-aware logs;
- metrics;
- dependency health checks;
- performance counters;
- centralized diagnostics;
- audit persistence;
- tracing backend.

## Security limitations

Не выполнены:

- threat model;
- secrets management;
- TLS policy;
- secure headers policy;
- rate limiting;
- dependency vulnerability workflow;
- static security analysis;
- security audit;
- penetration testing;
- authorization tests.

## Следующий инженерный этап

Sprint 015 — Object, Device and Tag Read API Foundation.

Приоритет:

- read-only transport contracts;
- typed DTO mapping;
- object tree read API;
- device list read API;
- tag list read API;
- typed frontend clients;
- real Objects, Devices и Tags page states;
- tests для новых contracts и endpoints.
