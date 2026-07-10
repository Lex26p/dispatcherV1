# ADR 0001 — Backend HTTP Transport

## Статус

Accepted

## Дата

2026-07-09

## Обновлено

2026-07-09

---

# Контекст

Dispatcher backend написан на C++.

На MVP foundation этапе уже были созданы:

- `scada_api`;
- `scada_realtime`;
- `scada_app`;
- `scada_http`.

Frontend после Sprint 011 стал русскоязычным, но продолжает использовать demo-data.

Для перехода к real integration нужен постоянный backend HTTP/WebSocket transport foundation.

---

# Первоначальное решение

На шаге 81 было принято предварительное решение использовать:

    Boost.Asio + Boost.Beast

Причина:

- низкоуровневый контроль;
- хорошая переносимость;
- async networking;
- будущая поддержка WebSocket.

---

# Пересмотр решения

После шагов 82–86 стало понятно, что Boost.Beast приведет к большому объему низкоуровневого infrastructure-кода:

- accept loop;
- request parsing;
- response writing;
- routing bridge;
- error handling;
- shutdown;
- WebSocket foundation.

Для Dispatcher важнее быстрее получить устойчивый постоянный gateway foundation, не превращая проект в разработку собственного web framework.

---

# Новое решение

Для backend HTTP/WebSocket transport foundation используем:

    vcpkg + Drogon

Drogon используется только внутри backend-модуля:

    scada_http

---

# Почему Drogon

Drogon выбран как постоянное решение, потому что:

- это C++17/20 HTTP application framework;
- поддерживает Windows и Linux;
- имеет asynchronous foundation;
- подходит для HTTP API;
- подходит для будущего WebSocket gateway;
- доступен через vcpkg;
- позволяет не писать собственный HTTP server с нуля;
- может быть изолирован внутри `scada_http`.

---

# Почему vcpkg

vcpkg выбран как dependency manager, потому что:

- поддерживает manifest mode через `vcpkg.json`;
- интегрируется с CMake через toolchain file;
- подходит для Windows/Linux development;
- позволяет явно хранить зависимости проекта;
- хорошо подходит для Visual Studio/CMake workflow.

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

`scada_api` не должен зависеть от Drogon.

## scada_http

`scada_http` отвечает за transport-level concepts:

- HTTP server options;
- Drogon integration;
- route registration;
- request-to-endpoint bridge;
- response writing;
- future WebSocket/SSE gateway.

Drogon types не должны попадать в доменные модули.

## scada_app

`scada_app` отвечает за application composition и startup plan.

`scada_app` может знать, включен ли API gateway, но не должен реализовывать HTTP handlers.

## dispatcher_server

`dispatcher_server` является executable entry point.

Он запускает application context и embedded HTTP server.

---

# Правило изоляции framework

Drogon является implementation detail модуля `scada_http`.

Запрещено:

- использовать Drogon types в `scada_core`;
- использовать Drogon types в `scada_objects`;
- использовать Drogon types в `scada_devices`;
- использовать Drogon types в `scada_tags`;
- использовать Drogon types в `scada_runtime`;
- использовать Drogon types в `scada_events`;
- использовать Drogon types в `scada_alarms`;
- писать доменную бизнес-логику напрямую в Drogon handlers.

Разрешено:

- использовать Drogon внутри `scada_http`;
- создавать wrapper classes в `scada_http`;
- маппить Drogon request/response к Dispatcher HTTP models;
- регистрировать Dispatcher endpoints через `scada_http`.

---

# Альтернативы

## Boost.Asio + Boost.Beast

Плюсы:

- максимальный контроль;
- низкоуровневый async networking;
- хорошо подходит для custom protocols.

Минусы:

- слишком много инфраструктурного кода для HTTP gateway;
- нужно самим писать routing, parsing bridge, response writer, shutdown;
- замедляет переход к реальному API.

Решение:

    Не используем как основной HTTP gateway foundation сейчас.

## Crow

Плюсы:

- проще Drogon;
- быстрый routing;
- HTTP/WebSocket support.

Минусы:

- более легкий microframework;
- хуже подходит как долгосрочная gateway foundation для большой SCADA-платформы.

Решение:

    Не выбираем, потому что хотим более постоянный framework-level foundation.

## cpp-httplib

Плюсы:

- очень простой старт;
- удобно для маленьких tools.

Минусы:

- blocking socket I/O;
- слабее как постоянная foundation для realtime gateway;
- не лучший выбор для будущего WebSocket/API gateway.

Решение:

    Не выбираем как основной HTTP transport.

## Собственный socket server

Плюсы:

- полный контроль;
- нет внешней зависимости.

Минусы:

- большой риск ошибок;
- много низкоуровневого кода;
- нецелесообразно для постоянной платформы.

Решение:

    Не делаем.

---

# Первые endpoints

На Sprint 012 реализуются:

    GET /api/system/health
    GET /api/system/modules

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

# Последствия

## Положительные последствия

- быстрее получаем рабочий HTTP API;
- меньше собственного infrastructure-кода;
- появляется путь к WebSocket gateway;
- vcpkg фиксирует dependency management;
- `scada_http` становится реальным transport layer;
- доменные модули остаются независимыми от framework.

## Отрицательные последствия

- появляется внешняя зависимость;
- сборка через vcpkg может стать дольше;
- нужно настроить `VCPKG_ROOT`;
- нужно следить, чтобы Drogon не проникал в доменные слои;
- Linux deployment позже потребует отдельной проверки triplet/dependency setup.

---

# Ограничения решения в Sprint 012

Sprint 012 не делает HTTP gateway production-ready.

Пока нет:

- HTTPS;
- authentication;
- authorization;
- CORS policy;
- OpenAPI;
- Swagger UI;
- request body parsing policy;
- WebSocket/SSE;
- rate limiting;
- production hardening;
- structured request logging.

---

# Future work

После Sprint 012 нужно сделать:

- frontend real API client integration;
- configurable HTTP settings;
- CORS policy;
- error response policy;
- structured logging;
- graceful shutdown;
- WebSocket/SSE transport;
- authentication;
- authorization.
