# scada_http

## Назначение

`scada_http` — backend-модуль transport foundation для HTTP API Dispatcher.

Модуль отвечает за transport-level часть backend API gateway.

## Статус

Drogon HTTP server wrapper foundation.

На текущем этапе модуль содержит:

- module info;
- HTTP server options;
- HTTP method model;
- HTTP status code model;
- HTTP header model;
- HTTP request model;
- HTTP response model;
- HTTP endpoint model;
- HTTP route dispatcher foundation;
- Drogon HTTP server wrapper;
- Drogon request to Dispatcher request bridge;
- Dispatcher response to Drogon response bridge;
- `/api/system/health` endpoint foundation;
- `/api/system/modules` endpoint foundation;
- helper-функции для response и method/status formatting.

HTTP server wrapper добавлен, но `dispatcher_server` еще не запускает его.

Запуск из `dispatcher_server` будет добавлен следующим шагом.

## Зона ответственности

`scada_http` отвечает за:

- HTTP server options;
- Drogon integration;
- HTTP transport;
- request mapping;
- response mapping;
- route dispatch bridge;
- development HTTP server lifecycle;
- future WebSocket/SSE gateway foundation.

## Не отвечает за

`scada_http` не отвечает за:

- доменную бизнес-логику;
- object/device/tag/runtime models;
- API contracts;
- repository implementations;
- authorization;
- authentication;
- frontend behavior.

## Связь с другими модулями

`scada_api`:

- описывает API endpoints, response model и read catalog.

`scada_http`:

- доставляет HTTP-запросы до route handlers и возвращает HTTP-ответы.

`scada_app`:

- хранит application composition и startup plan.

`dispatcher_server`:

- запускает application и на следующем шаге будет запускать embedded Drogon HTTP server.

## Framework isolation rule

Drogon является implementation detail модуля `scada_http`.

Разрешено:

- включать Drogon headers внутри `scada_http/src`;
- линковать Drogon с `scada_http`;
- использовать Drogon внутри wrapper classes.

Запрещено:

- использовать Drogon types в доменных модулях;
- использовать Drogon types в `scada_core`;
- использовать Drogon types в `scada_app`;
- использовать Drogon types в public contracts за пределами `scada_http`;
- писать доменную бизнес-логику напрямую в Drogon handlers.

## Default policy

Development HTTP server должен слушать только localhost:

    127.0.0.1

Development port:

    8080

## Текущие endpoints

Подготовлены:

    GET /api/system/health
    GET /api/system/modules

После подключения в `dispatcher_server` они будут доступны через browser:

    http://127.0.0.1:8080/api/system/health
    http://127.0.0.1:8080/api/system/modules

И через PowerShell:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/health
    Invoke-RestMethod http://127.0.0.1:8080/api/system/modules

## Route dispatcher foundation

`HttpRouteDispatcher` поддерживает:

- регистрацию маршрута;
- exact path matching;
- method matching;
- защиту от duplicate method/path route;
- `400 Bad Request` для невалидного request;
- `404 Not Found` для неизвестного path;
- `405 Method Not Allowed` для известного path с неподдерживаемым method.

## Drogon wrapper

`DrogonHttpServer` выполняет:

- настройку bind address;
- настройку port;
- настройку thread count;
- регистрацию Dispatcher routes в Drogon;
- запуск Drogon event loop в отдельном thread;
- остановку через `drogon::app().quit()`.

## Ограничения

Пока нет:

- подключения wrapper к `dispatcher_server`;
- проверки endpoints через browser/PowerShell;
- production-ready HTTP settings;
- HTTPS;
- CORS;
- authentication;
- authorization;
- WebSocket/SSE;
- request body parsing policy;
- graceful full application shutdown;
- structured request logging.
