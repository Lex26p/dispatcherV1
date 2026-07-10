# Dispatcher Roadmap

## Статус

Актуально после завершения Sprint 015.

Дата фиксации:

    2026-07-10

Канонический статус:

    docs/CURRENT_STATUS.md

Текущие ограничения:

    docs/known-limitations.md

## Завершенные этапы

### MVP foundation

Завершены:

- Sprint 001 — Project Foundation;
- Sprint 002 — Object Model Foundation;
- Sprint 003 — Device Model Foundation;
- Sprint 004 — Tag Model Foundation;
- Sprint 005 — Communication and Polling Foundation;
- Sprint 006 — Runtime Values and Data Engine;
- Sprint 007 — Historian Foundation;
- Sprint 008 — Events and Alarms Foundation;
- Sprint 009 — API and Realtime Gateway;
- Sprint 010 — Minimal Operator UI and MVP Stabilization.

### Post-MVP foundation

Завершены:

- Sprint 011 — Russian UI Cleanup and Frontend Terminology;
- Sprint 012 — Backend HTTP API Transport Foundation;
- Sprint 013 — Frontend Real API Client Integration;
- Sprint 014 — Engineering Baseline;
- Sprint 015 — Object, Device and Tag Read API Foundation.

## Текущее состояние

Сформированы:

- modular C++20 backend;
- Object, Device и Tag domain models;
- protocol and polling foundations;
- runtime, historian, event и alarm foundations;
- Drogon HTTP transport;
- JSON serializer boundary;
- API error envelope;
- correlation ID;
- strict HTTP query parser;
- configuration read application service;
- deterministic development configuration provider;
- Object list API;
- Object tree API;
- Device list API;
- Tag list API;
- typed frontend configuration client;
- real System page;
- real Objects page;
- real Devices page;
- real Tags page;
- backend Catch2 tests;
- frontend xUnit tests;
- local integration smoke-test;
- Windows CI;
- Linux Debug and Release CI.

## Рабочий configuration vertical slice

    Object
        ->
    Device
        ->
    Tag

Техническая цепочка:

    domain
        ->
    IConfigurationReadService
        ->
    HTTP
        ->
    JSON
        ->
    ConfigurationHttpApiClient
        ->
    Blazor pages

## Следующий planning checkpoint

Sprint 016 scope должен быть утвержден отдельным plan.

Рекомендуемый приоритет:

    Configuration Persistence Foundation

Предполагаемая цель:

    development snapshot
        ->
    repository-backed configuration reads

Возможный scope для отдельного обсуждения:

- PostgreSQL connection foundation;
- backend configuration options;
- migration runner boundary;
- Object repository implementation;
- Device repository implementation;
- Tag repository implementation;
- transaction boundary;
- seed/development data strategy;
- repository-backed `IConfigurationReadService`;
- development snapshot fallback policy;
- persistence integration tests;
- startup validation;
- failure reporting.

Этот scope не считается утвержденным до создания Sprint 016 plan и архитектурного решения.

## Последующие направления

Точный порядок фиксируется отдельными sprint plans.

### Configuration management

- CRUD;
- validation reports;
- versioning;
- import and export;
- audit;
- conflict handling;
- responsibility zones;
- engineering workflows.

### Protocols and polling

- Modbus TCP;
- SNMP;
- connection lifecycle;
- background polling;
- retry/backoff;
- diagnostics;
- load limiting.

### Runtime data pipeline

- TagValueStore lifecycle;
- EventBus integration;
- historian delivery;
- event generation;
- alarm evaluation;
- realtime publication.

### Historian

- PostgreSQL or TimescaleDB writer;
- batch worker;
- query API;
- retention;
- compression;
- downsampling;
- trend data.

### Events and alarms

- Event Manager;
- Alarm Manager;
- persistent state;
- acknowledgement;
- shelving;
- suppression;
- notification delivery;
- audit trail.

### Realtime

- WebSocket or SSE;
- subscriptions;
- reconnect;
- backpressure;
- frontend live updates.

### Security

- authentication;
- authorization;
- roles;
- responsibility zones;
- operator identity;
- audit identity;
- secrets management.

### Operator functions

- dashboards;
- SVG mimic diagrams;
- commands;
- trends;
- reports;
- maintenance workflows;
- commissioning tools.

### Production engineering

- structured logging;
- metrics;
- service lifecycle;
- deployment;
- packaging;
- backups;
- recovery;
- performance testing;
- security testing.
