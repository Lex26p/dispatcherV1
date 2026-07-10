# Dispatcher Roadmap

## Статус

Актуально после завершения Sprint 014.

Канонический технический статус:

    docs/CURRENT_STATUS.md

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
- Sprint 014 — Engineering Baseline.

## Текущее состояние

Сформированы:

- modular backend;
- domain foundations;
- operator UI foundation;
- real System HTTP API;
- real System frontend integration;
- stable JSON serializer boundary;
- API error envelope;
- correlation ID;
- backend tests;
- frontend tests;
- Windows/Linux CI.

## Следующий спринт

### Sprint 015 — Object, Device and Tag Read API Foundation

Основная цель:

    domain foundations
        -> read application services
        -> HTTP endpoints
        -> JSON contracts
        -> typed frontend clients
        -> real operator pages

Предполагаемый scope:

- Object read models;
- object tree endpoint;
- object list endpoint;
- Device read models;
- device list endpoint;
- Tag read models;
- tag list endpoint;
- query validation;
- API DTO mapping;
- unit tests;
- endpoint contract tests;
- frontend typed clients;
- loading, empty и error states;
- real Objects page;
- real Devices page;
- real Tags page;
- documentation.

Не входят в первый read API scope:

- create;
- update;
- delete;
- commands;
- authentication;
- authorization;
- database persistence;
- realtime updates.

## Последующие направления

Точный порядок следующих спринтов фиксируется отдельными sprint plans.

Основные направления:

### Persistence

- PostgreSQL connection;
- migration runner;
- repository implementations;
- transaction boundaries;
- configuration persistence.

### Protocols and polling

- Modbus TCP;
- SNMP;
- connection lifecycle;
- background polling;
- retry/backoff;
- diagnostics.

### Runtime data pipeline

- TagValueStore lifecycle;
- EventBus integration;
- historian delivery;
- events generation;
- alarm evaluation;
- realtime publication.

### Historian

- batch writer;
- TimescaleDB;
- query API;
- retention;
- compression;
- downsampling.

### Events and alarms

- Event Manager;
- Alarm Manager;
- acknowledgement;
- shelving;
- suppression;
- notifications;
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
- audit identity;
- secrets management.

### Operator functions

- dashboards;
- SVG mimic diagrams;
- commands;
- trends;
- reports;
- maintenance workflows.

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
