# Sprint 012 Plan

## Название спринта

Backend HTTP API Transport Foundation

## Статус

Planned

## Этап

Post-MVP development

## Диапазон шагов

Шаги 81–88.

---

# Цель спринта

Цель Sprint 012 — добавить foundation реального backend HTTP transport для Dispatcher.

После Sprint 011 frontend стал русскоязычным, но все еще использует demo-data.

Sprint 012 должен подготовить первый реальный мост:

    dispatcher_server
        -> HTTP transport
        -> API route handling
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

В Sprint 012 добавляем отдельный transport foundation.

Разделение ответственности:

    scada_api
        отвечает за API-модель, маршруты, response contracts и catalog

    scada_http
        отвечает за TCP/HTTP transport, request parsing и response writing

    dispatcher_server
        запускает приложение и подключает HTTP transport

---

# Решение по transport

Для backend HTTP foundation выбираем:

    Boost.Asio + Boost.Beast

Причины:

- Boost.Asio уже подходит под ранее принятую архитектуру async networking;
- Boost.Beast дает HTTP/WebSocket foundation поверх Boost.Asio;
- подход переносим между Windows и Linux;
- можно начать с HTTP/1.1;
- позже можно расширить до WebSocket gateway;
- не нужно привязываться к тяжелому web framework;
- контроль над lifecycle, потоками и shutdown остается у Dispatcher.

---

# Почему не framework-first подход

В Sprint 012 не выбираем тяжелый C++ web framework.

Не берем на старте:

- Crow;
- Pistache;
- Drogon;
- oatpp;
- cpp-httplib as main server foundation.

Причина:

- Dispatcher — не обычное web-приложение;
- нужен контроль над gateway lifecycle;
- нужен будущий bridge к polling/runtime/realtime;
- Boost.Asio уже подходит и для protocol drivers, и для HTTP transport;
- меньше архитектурной зависимости от внешнего framework.

---

# Новый backend module

Планируемый модуль:

    scada_http

Назначение:

- HTTP server options;
- HTTP request model;
- HTTP response writer foundation;
- route dispatch bridge;
- server lifecycle foundation;
- first system endpoints.

Почему не `scada_gateway`:

- gateway шире, чем HTTP;
- в gateway позже войдут HTTP, WebSocket/SSE, auth middleware, CORS, diagnostics;
- на Sprint 012 нужен узкий transport module;
- `scada_app` уже хранит application composition/gateway startup plan.

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

Пример будущего ответа:

    {
      "status": "ok",
      "product": "Dispatcher",
      "mode": "development",
      "api": "available"
    }

## /api/system/modules

Назначение:

- вернуть список backend modules;
- дать frontend возможность заменить demo module list на реальные данные в Sprint 013.

Пример будущего ответа:

    {
      "modules": [
        {
          "code": "scada_core",
          "name": "Core",
          "status": "Running"
        }
      ]
    }

---

# In scope

В Sprint 012 входит:

- зафиксировать HTTP transport decision;
- создать `scada_http`;
- добавить HTTP server options;
- добавить HTTP request/response models;
- добавить простой route dispatch;
- добавить JSON response helper foundation;
- добавить `/api/system/health`;
- добавить `/api/system/modules`;
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
- request body parsing;
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

# JSON policy foundation

На Sprint 012 JSON можно формировать вручную для первых endpoints.

Причина:

- endpoints маленькие;
- DTO уже простые;
- не нужно сразу выбирать полноценную JSON-библиотеку;
- позже можно добавить dedicated JSON serialization module.

После Sprint 012 нужно будет принять отдельное решение:

    manual JSON
        -> dedicated JSON writer
        -> nlohmann/json or Boost.JSON

---

# План шагов

## Шаг 81 — Sprint 012 plan and HTTP transport decision

Создать:

- `docs/SPRINT_012_PLAN.md`;
- `docs/adr/0001-backend-http-transport.md`.

Результат:

- план спринта зафиксирован;
- transport decision зафиксирован.

## Шаг 82 — Create scada_http module foundation

Создать backend module:

- `backend/libs/scada_http`.

Добавить:

- `CMakeLists.txt`;
- `include/scada/http/http_module.hpp`;
- `src/http_module.cpp`;
- `SERVICE.md`.

Подключить к root CMake и `dispatcher_server`.

Результат:

- модуль собирается;
- module info отображается в server output.

## Шаг 83 — Add HTTP server options and endpoint models

Добавить:

- `HttpServerOptions`;
- `HttpEndpoint`;
- `HttpRequest`;
- `HttpResponse`;
- `HttpMethod`;
- `HttpStatusCode`.

Результат:

- transport-модель подготовлена.

## Шаг 84 — Add HTTP route dispatcher foundation

Добавить:

- route registration;
- exact path matching;
- method matching;
- not found response;
- method not allowed response.

Результат:

- можно связать route path с handler.

## Шаг 85 — Add /api/system/health endpoint

Добавить первый endpoint:

    GET /api/system/health

Результат:

- endpoint возвращает JSON health response.

## Шаг 86 — Add /api/system/modules endpoint

Добавить endpoint:

    GET /api/system/modules

Результат:

- endpoint возвращает JSON list backend modules.

## Шаг 87 — Wire HTTP server into dispatcher_server

Подключить HTTP server startup к `dispatcher_server`.

Результат:

- при запуске backend стартует HTTP server;
- в console output отображается адрес;
- endpoints можно проверить через browser или PowerShell.

## Шаг 88 — Sprint 012 docs update and closure

Создать:

- `docs/SPRINT_012_SUMMARY.md`.

Обновить:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- service docs.

Результат:

- Sprint 012 закрыт.

---

# Acceptance criteria

Sprint 012 считается выполненным, если:

- создан `scada_http`;
- backend собирается;
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

После Sprint 012 backend получит первый настоящий HTTP API foundation.

Ожидаемый результат:

    dispatcher_server
        -> starts HTTP server
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
