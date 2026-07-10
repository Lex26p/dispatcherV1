# Dispatcher Known Limitations

## Статус документа

Актуально после завершения:

    Sprint 015 — Object, Device and Tag Read API Foundation

Дата фиксации:

    2026-07-10

Текущий статус:

    docs/CURRENT_STATUS.md

История разработки:

    docs/development-log.md

## Общий статус

Dispatcher находится на стадии post-MVP application development.

Работают:

- modular backend;
- domain foundations;
- Drogon HTTP transport;
- System API;
- Object API;
- Device API;
- Tag API;
- typed frontend clients;
- real System page;
- real Objects page;
- real Devices page;
- real Tags page;
- backend tests;
- frontend tests;
- Windows CI;
- Linux CI.

Dispatcher пока не готов к промышленной эксплуатации.

## Configuration data source

Object, Device и Tag read API используют:

    DevelopmentConfigurationReadService

Источник данных:

    deterministic in-process snapshot

Ограничения:

- данные не читаются из PostgreSQL;
- данные не сохраняются;
- изменения после перезапуска невозможны;
- snapshot предназначен только для development и tests;
- production configuration files отсутствуют;
- configuration reload отсутствует;
- configuration versioning отсутствует;
- audit trail отсутствует.

## Read-only API

Configuration API поддерживает только чтение.

Отсутствуют:

- create;
- update;
- delete;
- bulk edit;
- import;
- export;
- validation preview;
- conflict handling;
- optimistic concurrency;
- audit history.

## Exact-path routing

Текущий HTTP router использует exact paths.

Поддерживаются query filters:

    /api/devices?id=device-1

Не поддерживаются path templates:

    /api/devices/{id}

Path-template engine отложен до появления реальной необходимости.

## Pagination UI

Backend предоставляет:

    limit
    offset
    count
    total
    hasMore

Frontend пока загружает development collections одним большим page request.

Отсутствуют:

- pagination controls;
- server-side grid virtualization;
- page-size selection;
- sort UI;
- filter UI;
- search UI.

## Object page

Работают:

- Object tree;
- flat Object list;
- summary metrics;
- refresh;
- loading and error states.

Отсутствуют:

- selection state;
- Object details panel;
- create and edit;
- drag-and-drop hierarchy;
- responsibility zones;
- search;
- saved expansion state;
- commands;
- audit history.

## Device page

Работают:

- Device collection;
- state;
- protocol;
- connection fields;
- runtimeEnabled;
- disabled configuration toggle.

Отсутствуют:

- live communication status;
- live diagnostics;
- last poll time;
- test connection;
- reconnect command;
- protocol-specific settings editor;
- credentials;
- Device CRUD;
- maintenance workflows.

`runtimeEnabled` является configuration-derived read field, а не доказательством активного network connection.

## Tag page

Работают:

- Tag configuration;
- relationships;
- type;
- value type;
- archive policy;
- engineering metadata;
- enabled state.

Отсутствуют:

- current value;
- quality;
- timestamp;
- source;
- history;
- alarm state;
- command execution;
- live updates;
- Tag CRUD;
- address validation against real driver;
- engineering conversion preview.

## Runtime

Runtime domain foundation существует.

Не завершена production chain:

    Polling
        ->
    TagValueStore
        ->
    EventBus
        ->
    Historian
        ->
    Events
        ->
    Alarms
        ->
    Realtime

`TagValueStore` остается in-memory.

## Protocols and polling

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
- connection pooling;
- retry/backoff;
- connection lifecycle;
- background polling loop;
- protocol diagnostics workers;
- load limiting.

## Persistence

Существуют repository interfaces и SQL migration drafts.

Не реализованы:

- PostgreSQL connection;
- migration runner;
- schema version check;
- repository implementations;
- transaction boundaries;
- connection pool;
- TimescaleDB integration;
- restart recovery;
- backup;
- restore.

## Historian

Historian foundation существует.

Не реализованы:

- persistent writer;
- TimescaleDB hypertable;
- batch worker;
- retention;
- compression;
- downsampling;
- query API;
- trend API;
- disk spool;
- backpressure;
- recovery.

## Events

Event domain foundation существует.

Не реализованы:

- Event Manager;
- automatic event generation;
- persistent journal;
- event query API;
- filtering;
- realtime delivery;
- real frontend journal.

Страница `/events` пока использует demo-data.

## Alarms

Alarm domain foundation существует.

Не реализованы:

- Alarm Manager;
- persistent active state;
- automatic evaluation loop;
- deduplication;
- acknowledgement API;
- shelving;
- suppression;
- escalation;
- notifications;
- audit trail;
- realtime delivery.

Страница `/alarms` пока использует demo-data.

## Realtime

WebSocket и SSE transport отсутствуют.

Не реализованы:

- client registry;
- subscriptions;
- topic routing;
- ping/pong;
- reconnect;
- backpressure;
- delivery guarantees;
- frontend realtime client.

## Security

Не реализованы:

- authentication;
- authorization;
- users;
- roles;
- sessions;
- access tokens;
- refresh tokens;
- responsibility enforcement;
- secure command policy;
- audit identity;
- secrets management;
- rate limiting;
- TLS policy;
- secure headers;
- threat model.

Device read API сознательно не возвращает secrets.

## CORS and HTTPS

CORS policy предназначена для development origins.

Отсутствуют:

- production origin configuration;
- reverse-proxy awareness;
- credentialed-origin policy;
- HTTPS listener configuration;
- certificate management;
- TLS hardening.

## Process lifecycle

`dispatcher_server` development mode останавливается через ввод в консоль.

Не реализованы:

- SIGINT lifecycle;
- SIGTERM lifecycle;
- Windows Service;
- systemd service;
- coordinated shutdown timeout;
- background worker shutdown;
- restart policy;
- service supervision.

## Frontend

Реальные backend data используют:

    /system
    /objects
    /devices
    /tags

Demo-data пока используют:

    /runtime
    /events
    /alarms

Не реализованы:

- authentication UI;
- authorization-aware navigation;
- global retry policy;
- global notification policy;
- offline mode;
- localization engine;
- browser persistence;
- accessibility audit;
- responsive large-data virtualization.

## Testing

Backend имеет 78 CTest tests.

Frontend имеет xUnit tests.

Local smoke-test проверяет реальный listener.

Ограничения:

- CTest не запускает network listener;
- local smoke-test не запускается в CI;
- нет browser E2E tests;
- нет component tests;
- нет accessibility tests;
- нет visual regression;
- нет contract schema generation;
- нет coverage reports;
- нет mutation testing;
- нет load tests;
- нет endurance tests.

## Observability

Не реализованы:

- structured logging;
- correlation-aware log context;
- request logs;
- metrics;
- tracing;
- health checks dependencies;
- performance counters;
- centralized diagnostics;
- audit persistence.

## Build and delivery

CI собирает и тестирует Windows и Linux.

Не реализованы:

- published build artifacts;
- signed artifacts;
- checksums;
- Windows installer;
- Linux package;
- release archive;
- Docker image;
- deployment manifest;
- systemd unit;
- release promotion;
- rollback procedure.

## Следующий приоритет

Рекомендуемое направление следующего sprint:

    repository-backed configuration persistence

Оно должно быть подтверждено отдельным Sprint plan до изменения architecture или dependencies.
