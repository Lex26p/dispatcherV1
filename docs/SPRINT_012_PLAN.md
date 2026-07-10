# Sprint 012 Plan

## Название спринта

Backend HTTP API Transport Foundation

## Статус

In Progress

## Этап

Post-MVP development

## Диапазон шагов

Шаги 81–90.

---

# Цель спринта

Цель Sprint 012 — добавить foundation реального backend HTTP transport для Dispatcher.

После Sprint 011 frontend стал русскоязычным, но все еще использует demo-data.

Sprint 012 должен подготовить первый реальный мост:

    dispatcher_server
        -> scada_http
        -> Drogon HTTP server
        -> /api/system/health
        -> /api/system/modules
        -> frontend real integration in next sprint

---

# Главная идея

В проекте уже есть модуль `scada_api`, но он пока содержит только API foundation:

- endpoint model;
- route registry;
- response model;
- mapper foundation;
- read endpoint catalog.

В Sprint 012 добавляем отдельный transport foundation:

    scada_http

`scada_http` изолирует HTTP framework от остальных backend-модулей.

---

# Решение по transport

Для backend HTTP/WebSocket foundation выбираем:

    vcpkg + Drogon

Drogon является постоянным transport framework для Dispatcher gateway foundation.

---

# Почему Drogon

Drogon выбран, потому что:

- это C++17/20 HTTP application framework;
- поддерживает Windows и Linux;
- подходит для HTTP API;
- подходит для будущего WebSocket gateway;
- доступен через vcpkg;
- позволяет не писать собственный HTTP server с нуля;
- может быть спрятан внутри `scada_http`.

---

# Почему vcpkg

vcpkg выбран как dependency manager, потому что:

- поддерживает manifest mode через `vcpkg.json`;
- интегрируется с CMake через toolchain file;
- подходит для Windows/Linux development;
- позволяет явно хранить зависимости проекта.

---

# Framework isolation rule

Drogon можно использовать только внутри:

    backend/libs/scada_http

Нельзя протаскивать Drogon types в:

- `scada_core`;
- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_runtime`;
- `scada_events`;
- `scada_alarms`;
- `scada_app`.

---

# Первый API scope

В Sprint 012 реализуем только системные endpoints:

    GET /api/system/health
    GET /api/system/modules

## /api/system/health

Назначение:

- проверить, что backend HTTP transport работает;
- вернуть базовый статус backend;
- использовать endpoint как smoke-test.

## /api/system/modules

Назначение:

- вернуть список backend modules;
- дать frontend возможность заменить demo module list на реальные данные в Sprint 013.

---

# In scope

В Sprint 012 входит:

- зафиксировать HTTP transport decision;
- создать `scada_http`;
- добавить HTTP server options;
- добавить HTTP request/response models;
- добавить простой route dispatch foundation;
- добавить `/api/system/health`;
- добавить `/api/system/modules`;
- добавить vcpkg manifest;
- подключить Drogon через vcpkg;
- добавить Drogon HTTP server wrapper;
- подключить HTTP server к `dispatcher_server`;
- добавить startup output с HTTP address/port;
- обновить документацию.

---

# Out of scope

В Sprint 012 не входит:

- HTTPS/TLS;
- authentication;
- authorization;
- CORS policy;
- OpenAPI;
- Swagger UI;
- middleware pipeline;
- POST/PUT/DELETE commands;
- WebSocket/SSE;
- realtime dispatch;
- database access;
- object/device/tag endpoints;
- runtime/events/alarms endpoints;
- frontend real API integration.

---

# Security note

Sprint 012 не делает систему production-ready.

HTTP server будет development foundation.

Ограничения:

- слушать только localhost по умолчанию;
- не открывать наружу без отдельной настройки;
- не считать endpoint безопасным;
- не использовать для production deployment.

Рекомендуемый default bind address:

    127.0.0.1

Рекомендуемый default port:

    8080

---

# План шагов

## Шаг 81 — Sprint 012 plan and HTTP transport decision

Результат:

- план спринта зафиксирован;
- initial transport decision зафиксирован.

## Шаг 82 — Create scada_http module foundation

Результат:

- модуль `scada_http` создан;
- модуль собирается;
- module info отображается в server output.

## Шаг 83 — Add HTTP server options and endpoint models

Результат:

- transport-модель подготовлена.

## Шаг 84 — Add HTTP route dispatcher foundation

Результат:

- можно связать route path с handler.

## Шаг 85 — Add /api/system/health endpoint

Результат:

- endpoint возвращает JSON health response foundation.

## Шаг 86 — Add /api/system/modules endpoint

Результат:

- endpoint возвращает JSON list backend modules foundation.

## Шаг 87 — Switch HTTP transport to vcpkg + Drogon

Результат:

- ADR обновлен;
- Sprint plan обновлен;
- добавлен `vcpkg.json`;
- `CMakePresets.json` подключает vcpkg toolchain.

## Шаг 88 — Add Drogon HTTP server wrapper in scada_http

Результат:

- `scada_http` зависит от Drogon;
- добавлен wrapper для запуска HTTP server;
- Dispatcher endpoints регистрируются через Drogon.

## Шаг 89 — Wire Drogon HTTP server into dispatcher_server

Результат:

- при запуске backend стартует HTTP server;
- в console output отображается адрес;
- endpoints можно проверить через browser или PowerShell.

## Шаг 90 — Sprint 012 docs update and closure

Результат:

- создан `docs/SPRINT_012_SUMMARY.md`;
- обновлены `development-log` и `known-limitations`;
- Sprint 012 закрыт.

---

# Acceptance criteria

Sprint 012 считается выполненным, если:

- создан `scada_http`;
- добавлен `vcpkg.json`;
- CMake configure использует vcpkg toolchain;
- backend собирается с Drogon;
- backend запускается;
- HTTP server стартует на localhost;
- `GET /api/system/health` возвращает JSON;
- `GET /api/system/modules` возвращает JSON;
- frontend пока остается на demo-data;
- документация обновлена;
- изменения отправлены в `master`.

---

# Проверка будущих endpoints

Через browser:

    http://127.0.0.1:8080/api/system/health
    http://127.0.0.1:8080/api/system/modules

Через PowerShell:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/health
    Invoke-RestMethod http://127.0.0.1:8080/api/system/modules

---

# Expected result after Sprint 012

После Sprint 012 backend получит первый настоящий HTTP API foundation на Drogon.

Ожидаемый результат:

    dispatcher_server
        -> starts Drogon HTTP server
        -> responds to /api/system/health
        -> responds to /api/system/modules

Frontend пока не меняется на real API.

Это будет сделано в Sprint 013.

---

# Следующий спринт после Sprint 012

Рекомендуемый следующий спринт:

    Sprint 013 — Frontend Real API Client Integration

Цель:

- добавить frontend API settings;
- подключить `/api/system/health`;
- подключить `/api/system/modules`;
- заменить demo System page на реальные backend данные;
- добавить loading/error/no connection states.
