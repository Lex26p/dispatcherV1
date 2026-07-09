# ADR 0001 — Backend HTTP Transport

## Статус

Accepted

## Дата

2026-07-09

## Контекст

Dispatcher backend написан на C++.

На MVP foundation этапе уже были созданы:

- `scada_api`;
- `scada_realtime`;
- `scada_app`.

Но реального HTTP transport еще нет.

Frontend после Sprint 011 стал русскоязычным, но продолжает использовать demo-data.

Для перехода к real integration нужен backend HTTP transport foundation.

---

# Решение

Для backend HTTP transport foundation используем:

    Boost.Asio + Boost.Beast

Создаем отдельный backend module:

    scada_http

---

# Разделение ответственности

## scada_api

`scada_api` отвечает за API-level concepts:

- API endpoint model;
- route registry;
- API response model;
- mapper foundation;
- read endpoint catalog;
- future API contracts.

`scada_api` не должен напрямую владеть TCP socket lifecycle.

## scada_http

`scada_http` отвечает за transport-level concepts:

- HTTP server options;
- TCP acceptor;
- HTTP session lifecycle;
- request parsing;
- response writing;
- route dispatch bridge;
- development HTTP server.

## scada_app

`scada_app` отвечает за application composition и startup plan.

`scada_app` может знать, включен ли API gateway, но не должен реализовывать HTTP parsing.

## dispatcher_server

`dispatcher_server` является executable entry point.

На Sprint 012 он будет запускать HTTP server в development mode.

---

# Почему Boost.Asio + Boost.Beast

Причины выбора:

- подходит под C++ backend;
- переносим между Windows и Linux;
- использует async I/O model;
- согласуется с будущими protocol drivers;
- дает HTTP foundation;
- дает future path к WebSocket;
- не навязывает heavy web framework;
- позволяет контролировать lifecycle и shutdown;
- подходит для modular monolith stage.

---

# Альтернативы

## cpp-httplib

Плюсы:

- простой старт;
- header-only;
- быстро сделать endpoints.

Минусы:

- хуже ложится на общий async networking подход Dispatcher;
- может стать временным решением, которое придется заменить;
- меньше полезен для будущего WebSocket/realtime direction.

## Crow

Плюсы:

- удобный routing;
- быстрее писать web endpoints.

Минусы:

- framework-first подход;
- сложнее контролировать архитектурные boundaries;
- может смешать API, transport и application layer.

## Drogon

Плюсы:

- мощный framework;
- много готовых возможностей.

Минусы:

- слишком тяжелый для Sprint 012;
- может навязать структуру приложения;
- не нужен для minimal foundation.

## Pistache

Плюсы:

- HTTP framework;
- подходит для REST-like server.

Минусы:

- дополнительная архитектурная зависимость;
- не так хорошо согласуется с уже выбранным async networking direction.

## Boost.JSON сразу

Это не HTTP transport, а JSON serialization option.

В Sprint 012 JSON можно формировать вручную для первых двух endpoints.

Отдельное решение по JSON serialization нужно принять позже.

---

# Последствия

## Положительные последствия

- единый async networking direction;
- контроль над server lifecycle;
- путь к WebSocket/SSE;
- переносимость Windows/Linux;
- меньше зависимости от web framework;
- хорошо ложится на modular monolith.

## Отрицательные последствия

- больше low-level кода;
- routing нужно реализовать самостоятельно;
- error handling нужно проектировать самостоятельно;
- JSON serialization нужно решить отдельно;
- нужен аккуратный shutdown model.

---

# Ограничения решения

Sprint 012 HTTP server не является production-ready.

Ограничения:

- только development foundation;
- только localhost по умолчанию;
- без HTTPS;
- без authentication;
- без authorization;
- без CORS;
- без request body parsing;
- без OpenAPI;
- без WebSocket/SSE;
- без rate limiting;
- без production hardening.

---

# Первые endpoints

На Sprint 012 реализуются только:

    GET /api/system/health
    GET /api/system/modules

Остальные endpoints будут добавляться позже.

---

# Default network policy

Default bind address:

    127.0.0.1

Default HTTP port:

    8080

Причина:

- безопаснее для development;
- не открывает API наружу по умолчанию;
- подходит для локальной проверки frontend/backend integration.

---

# Future work

После Sprint 012 нужно сделать:

- frontend real API client integration;
- configurable HTTP settings;
- JSON serialization policy;
- CORS policy;
- error response policy;
- logging;
- graceful shutdown;
- WebSocket/SSE transport;
- authentication;
- authorization.
