# Dispatcher MVP Status

## Статус документа

Historical.

Этот документ фиксирует завершение MVP foundation после Sprint 010.

Он больше не является источником текущего технического состояния проекта.

Актуальный статус:

    docs/CURRENT_STATUS.md

Актуальные ограничения:

    docs/known-limitations.md

История разработки:

    docs/development-log.md

## Что означает MVP foundation

MVP в Dispatcher означал минимальный архитектурный и пользовательский контур, который:

- определил основные domain boundaries;
- создал modular backend foundation;
- создал operator UI foundation;
- зафиксировал направление API и realtime layers;
- подготовил проект к post-MVP development.

MVP foundation не означал production-ready SCADA.

## Результат MVP foundation

К завершению Sprint 010 были созданы:

- C++20 backend project;
- common и core foundations;
- object model foundation;
- device model foundation;
- tag model foundation;
- protocol foundation;
- polling foundation;
- runtime foundation;
- historian foundation;
- event foundation;
- alarm foundation;
- API model foundation;
- realtime model foundation;
- application composition foundation;
- Blazor WebAssembly frontend;
- MudBlazor operator layout;
- System page;
- Runtime page;
- Events page;
- Alarms page;
- Objects placeholder;
- Devices placeholder;
- Tags placeholder;
- MVP scope и stabilization documentation.

## Закрытие этапа

MVP foundation stage закрыт в Sprint 010.

Завершенные MVP foundation спринты:

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

## Развитие после MVP

После закрытия MVP foundation были выполнены:

- Sprint 011 — Russian UI Cleanup and Frontend Terminology;
- Sprint 012 — Backend HTTP API Transport Foundation;
- Sprint 013 — Frontend Real API Client Integration;
- Sprint 014 — Engineering Baseline.

В результате после MVP появились:

- реальный HTTP transport;
- реальные System endpoints;
- real System frontend integration;
- development CORS;
- local integration smoke-test;
- JSON serialization foundation;
- API error envelope;
- correlation ID;
- backend tests;
- frontend tests;
- Windows/Linux CI.

Эти результаты описаны в:

    docs/CURRENT_STATUS.md
