# Dispatcher MVP Scope

## Назначение документа

Этот документ фиксирует границы MVP Dispatcher.

Он нужен, чтобы явно разделить:

- MVP scope;
- foundation scope;
- mock scope;
- deferred scope;
- production scope.

---

# MVP definition

MVP Dispatcher в текущей версии — это минимальный демонстрационный и архитектурный контур.

Он показывает:

- модульную backend-архитектуру;
- основные доменные модели SCADA/диспетчеризации;
- API/realtime gateway foundation;
- frontend operator UI direction;
- навигацию будущего рабочего места оператора;
- mock-визуализацию runtime/events/alarms.

MVP не означает готовность к промышленной эксплуатации.

---

# MVP goals

Цели MVP:

- собрать backend foundation;
- собрать frontend foundation;
- показать основные product boundaries;
- показать operator UI direction;
- зафиксировать архитектурную основу;
- подготовить дальнейший переход к real API, persistence и polling.

---

# In scope

## Backend foundation

В MVP входит backend foundation:

- `scada_common`;
- `scada_contracts`;
- `scada_core`;
- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`;
- `scada_polling`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`;
- `scada_api`;
- `scada_realtime`;
- `scada_app`.

## Domain models

В MVP входят foundation-модели:

- objects;
- responsibility zones;
- devices;
- device connections;
- device diagnostics;
- tags;
- tag addresses;
- current values;
- polling tasks;
- polling groups;
- protocol read requests/results;
- runtime value apply result;
- runtime events;
- history samples;
- archive decisions;
- event records;
- alarm records;
- alarm transitions;
- alarm rules.

## SQL migration drafts

В MVP входят SQL drafts:

- object model migration;
- device model migration;
- tag model migration;
- polling model migration;
- runtime values migration;
- tag history migration;
- events and alarms migration.

## API foundation

В MVP входит API foundation:

- endpoint model;
- route registry;
- response model;
- mapper foundation;
- read endpoint catalog.

## Realtime foundation

В MVP входит realtime foundation:

- realtime messages;
- realtime subscriptions;
- channels;
- delivery modes;
- startup preparation.

## Application composition foundation

В MVP входит application composition foundation:

- gateway mode;
- startup status;
- startup options;
- startup plan;
- application composition snapshot.

## Frontend foundation

В MVP входит frontend foundation:

- Blazor WebAssembly project;
- MudBlazor foundation;
- operator shell;
- AppBar;
- Drawer/sidebar;
- navigation;
- overview page;
- system/modules page;
- runtime placeholder page;
- events placeholder page;
- alarms placeholder page;
- object placeholder page;
- devices placeholder page;
- tags placeholder page.

## Mock frontend data

В MVP входит mock data для:

- backend modules;
- gateway summary;
- API route summary;
- runtime values;
- events;
- active alarms.

---

# Out of scope

## Production runtime

Не входит в MVP:

- production deployment;
- Docker deployment;
- Linux service setup;
- Windows service setup;
- high availability;
- clustering;
- failover.

## Real HTTP API

Не входит в MVP:

- HTTP listener;
- controllers;
- middleware;
- request parsing;
- response serialization;
- OpenAPI;
- versioned REST API.

## Real realtime gateway

Не входит в MVP:

- WebSocket server;
- SSE server;
- client registry;
- topic routing;
- message dispatch;
- backpressure;
- reconnect;
- ping/pong.

## Real database layer

Не входит в MVP:

- PostgreSQL connection;
- migration runner;
- repository implementations;
- transaction management;
- TimescaleDB hypertables;
- query optimization.

## Real protocols

Не входит в MVP:

- Modbus TCP implementation;
- SNMP implementation;
- OPC UA;
- MQTT;
- BACnet;
- IEC 60870-5-104;
- Siemens S7;
- DNP3.

## Real polling

Не входит в MVP:

- background polling loop;
- scheduling thread;
- async polling;
- timeout/retry/backoff;
- device reconnect logic.

## Real historian

Не входит в MVP:

- writing history to database;
- reading history from database;
- historian worker;
- disk spool;
- retention policy.

## Real event manager

Не входит в MVP:

- Event Manager service;
- automatic event publishing;
- event persistence;
- event filtering API.

## Real alarm manager

Не входит в MVP:

- Alarm Manager service;
- automatic alarm lifecycle;
- alarm acknowledgement action;
- shelving;
- suppression;
- escalation;
- notification.

## Security

Не входит в MVP:

- users;
- roles;
- authentication;
- authorization;
- sessions;
- tokens;
- command permissions;
- audit trail.

## Operator actions

Не входит в MVP:

- acknowledgement action;
- command execution;
- write tag;
- control mode;
- dry run command;
- real execution command.

## Advanced UI

Не входит в MVP:

- dashboards;
- widgets;
- mimic diagrams;
- SVG editor;
- trend charts;
- history charts;
- alarm sound;
- notifications;
- workstation mode;
- panel mode.

---

# Mock scope

В MVP mock означает:

- данные созданы во frontend client;
- данные не приходят из backend HTTP API;
- данные не сохраняются;
- данные меняются только при изменении кода;
- данные нужны для проверки UI direction.

Mock не означает:

- настоящую диагностику;
- настоящие runtime values;
- настоящие аварии;
- настоящие события;
- настоящую связь с устройствами.

---

# Foundation scope

Foundation означает:

- типы и модели уже созданы;
- границы ответственности уже намечены;
- код компилируется;
- структуры можно расширять;
- интеграция запланирована;
- production behavior еще не реализован.

Foundation не означает:

- feature complete;
- production ready;
- tested at scale;
- secure by default;
- fault tolerant.

---

# MVP acceptance

MVP foundation можно считать достигнутым, если:

- backend собирается;
- backend запускается;
- все foundation modules регистрируются;
- frontend собирается;
- frontend запускается;
- operator layout отображается;
- navigation работает;
- System page показывает mock modules;
- Runtime page показывает mock current values;
- Events page показывает mock journal;
- Alarms page показывает mock active alarms;
- ограничения явно документированы;
- дальнейшие deferred features не скрыты.

---

# Post-MVP direction

После MVP основной фокус должен перейти к real integration:

- backend HTTP transport;
- JSON serialization;
- real API handlers;
- in-memory repositories or PostgreSQL repositories;
- runtime API;
- events API;
- alarms API;
- frontend real API client;
- realtime gateway;
- polling loop;
- Modbus TCP driver;
- MVP hardening tests.

---

# Product note

Dispatcher остается универсальной SCADA/диспетчерской платформой-конструктором.

MVP foundation должен сохранять универсальную модель:

    object -> device -> tag -> value -> quality -> history -> alarm -> widget

Текущий MVP не должен превращаться в решение под один конкретный объект или отрасль.
