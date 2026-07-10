# Dispatcher MVP Status

## Назначение документа

Этот документ фиксирует текущий статус MVP Dispatcher.

MVP в данном проекте означает не production-ready SCADA-систему, а минимально собранный архитектурный и пользовательский контур, который показывает направление продукта и готовит основу для дальнейшей разработки.

---

# Текущий статус

## Общий статус

Dispatcher находится на стадии MVP foundation.

Реализованы основные backend foundation-модули и минимальный frontend operator UI foundation.

Система пока не является промышленной SCADA-платформой.

## Что уже есть

В проекте уже есть:

- backend modular foundation;
- object model foundation;
- device model foundation;
- tag model foundation;
- protocol foundation;
- polling foundation;
- runtime values foundation;
- historian foundation;
- events foundation;
- alarms foundation;
- API foundation;
- realtime foundation;
- application composition foundation;
- Blazor WebAssembly frontend foundation;
- MudBlazor operator layout;
- frontend API client placeholder;
- mock System page;
- mock Runtime page;
- mock Events page;
- mock Alarms page.

---

# Backend status

## scada_common

Статус:

    Foundation ready

Реализовано:

- common error model;
- result model;
- typed ids foundation;
- correlation id;
- timestamp/version primitives.

Ограничения:

- нет production error catalog;
- нет полного logging layer.

## scada_contracts

Статус:

    Foundation ready

Реализовано:

- DTO foundation для основных доменных областей;
- contracts для object/device/tag/runtime/historian/events/alarms.

Ограничения:

- DTO есть не для всех будущих областей;
- нет versioned API contracts;
- нет serialization policy.

## scada_core

Статус:

    Foundation ready

Реализовано:

- application context;
- module info;
- module status;
- module registration;
- core interfaces;
- basic event bus contracts.

Ограничения:

- нет полноценного lifecycle manager;
- нет DI container;
- нет background worker management.

## scada_objects

Статус:

    Foundation ready

Реализовано:

- object tree foundation;
- object node;
- object type;
- object path;
- hierarchy validation;
- responsibility zone foundation;
- repository interfaces;
- migration draft.

Ограничения:

- нет repository implementation;
- нет API handlers;
- нет UI object tree;
- нет authorization integration.

## scada_devices

Статус:

    Foundation ready

Реализовано:

- device model;
- device connection model;
- diagnostics foundation;
- device state;
- protocol enum;
- repository interfaces;
- migration draft.

Ограничения:

- нет реального network communication;
- нет Modbus TCP driver;
- нет SNMP driver;
- нет device diagnostics worker.

## scada_tags

Статус:

    Foundation ready

Реализовано:

- tag model;
- tag address;
- tag current value;
- tag quality;
- archive policy;
- value source;
- repository interfaces;
- migration draft.

Ограничения:

- нет command execution;
- нет API handlers;
- нет database persistence.

## scada_protocols

Статус:

    Foundation ready

Реализовано:

- protocol driver interface;
- request/result models;
- protocol capabilities;
- simulator driver;
- protocol driver registry.

Ограничения:

- нет реального Modbus TCP;
- нет SNMP;
- нет async TCP implementation;
- нет retry/backoff policy.

## scada_polling

Статус:

    Foundation ready

Реализовано:

- polling task;
- polling group;
- polling scheduler foundation;
- execution result models;
- repository interfaces;
- migration draft.

Ограничения:

- нет background polling loop;
- нет scheduler thread/timer;
- нет integration with runtime applier.

## scada_runtime

Статус:

    Foundation ready

Реализовано:

- TagValueStore;
- RuntimeValueApplier;
- protocol result to current value flow;
- quality handling;
- last good value handling;
- value conversion;
- engineering transform;
- runtime event foundation;
- repository interfaces;
- migration draft.

Ограничения:

- store in-memory only;
- нет EventBus publish;
- нет historian integration;
- нет alarm integration;
- нет realtime integration.

## scada_historian

Статус:

    Foundation ready

Реализовано:

- HistorySample;
- archive decision;
- HistorianBuffer;
- batch write contract;
- history query model;
- repository interface;
- migration draft.

Ограничения:

- нет PostgreSQL writer;
- нет TimescaleDB hypertable;
- нет real history query;
- нет disk spool.

## scada_events

Статус:

    Foundation ready

Реализовано:

- EventRecord;
- event category;
- event severity;
- event source type;
- repository interface;
- DTO;
- migration draft.

Ограничения:

- нет Event Manager;
- нет автоматического создания событий;
- нет persistence;
- нет realtime delivery.

## scada_alarms

Статус:

    Foundation ready

Реализовано:

- AlarmRecord;
- alarm state;
- alarm severity;
- alarm priority;
- lifecycle transitions;
- alarm rule foundation;
- repository interfaces;
- DTO;
- migration draft.

Ограничения:

- нет Alarm Manager;
- нет automatic rule evaluation loop;
- нет persistence;
- нет acknowledgement API;
- нет realtime delivery.

## scada_api

Статус:

    Foundation ready

Реализовано:

- API endpoint model;
- route registry;
- API response model;
- mapper foundation;
- read endpoint catalog.

Ограничения:

- нет HTTP listener;
- нет controllers;
- нет request parsing;
- нет response serialization;
- нет OpenAPI.

## scada_realtime

Статус:

    Foundation ready

Реализовано:

- realtime message model;
- realtime subscription model;
- channel types;
- message types;
- delivery modes.

Ограничения:

- нет WebSocket server;
- нет SSE server;
- нет client registry;
- нет topic router;
- нет message dispatch.

## scada_app

Статус:

    Foundation ready

Реализовано:

- application composition model;
- gateway startup options;
- gateway startup plan.

Ограничения:

- нет настоящего composition root;
- нет transport startup;
- нет repository wiring;
- нет configuration loading.

---

# Frontend status

## Dispatcher.Frontend

Статус:

    Foundation ready

Реализовано:

- Blazor WebAssembly project;
- MudBlazor integration;
- operator layout;
- AppBar;
- sidebar Drawer;
- navigation;
- overview page;
- System page;
- Runtime page;
- Events page;
- Alarms page;
- Objects placeholder;
- Devices placeholder;
- Tags placeholder;
- API client placeholder;
- mock data models.

Ограничения:

- нет real backend API calls;
- нет realtime client;
- нет authentication;
- нет authorization;
- нет dashboards;
- нет mimic diagrams;
- нет command execution;
- нет alarm acknowledgement actions.

---

# Current MVP user-visible behavior

Пользователь может:

- запустить backend `dispatcher_server`;
- увидеть список backend modules в console output;
- запустить frontend Blazor WebAssembly;
- открыть operator UI;
- использовать navigation;
- открыть System page;
- увидеть mock backend modules;
- открыть Runtime page;
- увидеть mock current values;
- открыть Events page;
- увидеть mock event journal;
- открыть Alarms page;
- увидеть mock active alarms;
- открыть Objects/Devices/Tags placeholders.

---

# Что является mock/foundation

Следующие части являются mock/foundation:

- frontend API client;
- frontend backend module list;
- frontend runtime values;
- frontend events;
- frontend alarms;
- gateway status;
- API route counts;
- realtime channel counts;
- System page data;
- Runtime page data;
- Events page data;
- Alarms page data.

---

# Что пока не работает как настоящая система

В текущем MVP еще нет:

- real HTTP API;
- real WebSocket/SSE gateway;
- database connection;
- repository implementations;
- real device polling;
- Modbus TCP;
- SNMP;
- runtime-to-historian pipeline;
- runtime-to-alarms pipeline;
- event manager;
- alarm manager;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- command execution.

---

# MVP conclusion

Текущий MVP показывает:

- модульную backend-архитектуру;
- основные доменные boundaries;
- planned API/realtime gateway layer;
- frontend operator UI direction;
- базовую визуальную структуру будущей диспетчерской системы.

Текущий MVP не является:

- production-ready SCADA;
- real-time monitoring system;
- historian system;
- alarm management system;
- secured operator console.

Следующий большой этап после MVP:

    переход от foundation/mock к real transport, persistence и service integration
