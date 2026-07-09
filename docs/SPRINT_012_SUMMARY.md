# Sprint 012 Summary

## Название спринта

Backend HTTP API Transport Foundation

## Статус

Closed

## Этап

Post-MVP development

## Диапазон шагов

Шаги 81–90.

---

# Цель спринта

Цель Sprint 012 — добавить foundation реального backend HTTP transport для Dispatcher.

После Sprint 011 frontend стал русскоязычным, но продолжал использовать demo-data.

Sprint 012 должен был подготовить первый реальный мост:

    dispatcher_server
        -> scada_http
        -> HTTP server
        -> /api/system/health
        -> /api/system/modules
        -> frontend real integration in next sprint

---

# Итоговое решение

В Sprint 012 был создан backend HTTP transport foundation на базе:

    vcpkg + Drogon

Drogon выбран как постоянный transport framework для HTTP/WebSocket gateway foundation.

Drogon изолирован внутри модуля:

    backend/libs/scada_http

Доменные backend-модули не зависят от Drogon.

---

# Почему решение было изменено

В начале Sprint 012 предварительно рассматривался вариант:

    Boost.Asio + Boost.Beast

После анализа было принято решение перейти на:

    vcpkg + Drogon

Причины:

- Boost.Beast требует много низкоуровневого infrastructure-кода;
- для Dispatcher важнее быстрее получить устойчивый backend gateway foundation;
- Drogon дает HTTP server, routing и будущий WebSocket path;
- vcpkg дает воспроизводимое dependency management;
- Drogon можно спрятать внутри `scada_http`.

---

# Выполненные шаги

## Шаг 81 — Sprint 012 plan and HTTP transport decision

Созданы и обновлены:

- `docs/SPRINT_012_PLAN.md`;
- `docs/adr/0001-backend-http-transport.md`.

Результат:

- Sprint 012 был спланирован;
- принято первоначальное transport decision;
- позже решение было пересмотрено в пользу vcpkg + Drogon.

## Шаг 82 — Create scada_http module foundation

Создан модуль:

    backend/libs/scada_http

Добавлены:

- `CMakeLists.txt`;
- `SERVICE.md`;
- `include/scada_http/http_module.h`;
- `src/http_module.cpp`.

Результат:

- `scada_http` подключен к backend build;
- `scada_http` подключен к `dispatcher_server`;
- module info отображается при запуске backend.

## Шаг 83 — Add HTTP server options and endpoint models

Добавлены transport-level модели:

- `HttpMethod`;
- `HttpStatusCode`;
- `HttpServerOptions`;
- `HttpHeader`;
- `HttpRequest`;
- `HttpResponse`;
- `HttpEndpoint`.

Результат:

- подготовлены базовые HTTP-типы Dispatcher;
- добавлены helper-функции для method/status formatting и response creation.

## Шаг 84 — Add HTTP route dispatcher foundation

Добавлены:

- `HttpRouteHandler`;
- `HttpRoute`;
- `HttpRouteDispatcher`.

Результат:

- добавлена регистрация routes;
- добавлен exact path matching;
- добавлен method matching;
- добавлена защита от duplicate routes;
- добавлены foundation responses для 400, 404 и 405.

## Шаг 85 — Add /api/system/health endpoint

Добавлен foundation endpoint:

    GET /api/system/health

Результат:

- endpoint возвращает JSON health response;
- health response использует данные из `scada_common::VersionInfo`.

## Шаг 86 — Add /api/system/modules endpoint

Добавлен foundation endpoint:

    GET /api/system/modules

Результат:

- endpoint возвращает JSON со списком backend-модулей;
- endpoint использует `dispatcher::core::ModuleInfo`.

## Шаг 87 — Switch HTTP transport to vcpkg + Drogon

Добавлены и обновлены:

- `vcpkg.json`;
- `CMakePresets.json`;
- `docs/adr/0001-backend-http-transport.md`;
- `docs/SPRINT_012_PLAN.md`.

Результат:

- HTTP transport decision переключен на vcpkg + Drogon;
- CMake configure использует vcpkg toolchain;
- dependency management переведен в manifest mode.

## Шаг 88 — Add Drogon HTTP server wrapper in scada_http

Добавлен Drogon wrapper:

- `DrogonHttpServer`;
- bridge Drogon request -> Dispatcher `HttpRequest`;
- bridge Dispatcher `HttpResponse` -> Drogon response;
- registration of Dispatcher routes in Drogon.

Результат:

- `scada_http` линкуется с Drogon;
- Drogon остается implementation detail модуля `scada_http`;
- backend собирается с vcpkg + Drogon.

## Шаг 89 — Wire Drogon HTTP server into dispatcher_server

Обновлен:

- `backend/apps/dispatcher_server/src/main.cpp`.

Результат:

- `dispatcher_server` запускает Drogon HTTP server;
- backend остается запущенным и ждет Enter;
- в console output отображаются HTTP URLs;
- `/api/system/health` проверен через PowerShell;
- `/api/system/modules` проверен через PowerShell;
- warning C4834 исправлен;
- backend сборка и запуск прошли.

## Шаг 90 — Sprint 012 docs update and closure

Создан:

- `docs/SPRINT_012_SUMMARY.md`.

Обновлены:

- `docs/SPRINT_012_PLAN.md`;
- `docs/development-log.md`;
- `docs/known-limitations.md`.

Результат:

- Sprint 012 закрыт документально.

---

# Итоговая backend-цепочка

После Sprint 012 backend имеет цепочку:

    dispatcher_server
        -> ApplicationContext
        -> scada_http
        -> DrogonHttpServer
        -> HttpRouteDispatcher
        -> /api/system/health
        -> /api/system/modules

---

# Реализованные endpoints

## GET /api/system/health

Назначение:

- smoke-test backend HTTP transport;
- проверка доступности API;
- базовая информация о Dispatcher backend.

Пример проверки:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/health

## GET /api/system/modules

Назначение:

- вернуть список backend-модулей;
- подготовить основу для frontend real API integration.

Пример проверки:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/modules

---

# vcpkg и Drogon

Добавлен файл:

    vcpkg.json

Используемая dependency:

    drogon

CMake presets используют vcpkg toolchain через:

    $env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

Важно:

- рекомендуется использовать standalone vcpkg;
- локально использовался `C:\vcpkg`;
- `VCPKG_ROOT` должен указывать на корректную установку vcpkg;
- `builtin-baseline` должен быть зафиксирован в `vcpkg.json`.

---

# Проверки

Выполнены проверки:

- backend configure;
- backend build;
- backend run;
- `/api/system/health` через PowerShell;
- `/api/system/modules` через PowerShell;
- frontend project build;
- frontend solution build;
- git commit and push.

---

# Что осталось вне Sprint 012

Не реализовано в Sprint 012:

- HTTPS/TLS;
- authentication;
- authorization;
- CORS policy;
- OpenAPI;
- Swagger UI;
- request logging;
- structured JSON error fallback for unknown routes;
- WebSocket/SSE;
- object/device/tag API endpoints;
- runtime/events/alarms API endpoints;
- frontend real API integration;
- production deployment;
- Linux build verification.

---

# Известное ограничение

Запрос неизвестного маршрута, например:

    http://127.0.0.1:8080/api/unknown

пока может возвращать стандартный Drogon 404 response.

Позже нужно добавить unified JSON error response для неизвестных API routes.

---

# Результат Sprint 012

Sprint 012 успешно добавил реальный backend HTTP transport foundation.

Ключевой результат:

    Dispatcher backend теперь запускает Drogon HTTP server
    и отвечает на первые системные API endpoints.

---

# Следующий спринт

Рекомендуемый следующий спринт:

    Sprint 013 — Frontend Real API Client Integration

Цели Sprint 013:

- добавить frontend API base URL settings;
- подключить frontend к `/api/system/health`;
- подключить frontend к `/api/system/modules`;
- заменить demo-data на System page реальными backend данными;
- добавить loading/error/no connection states;
- подготовить frontend к runtime/events/alarms API integration.
