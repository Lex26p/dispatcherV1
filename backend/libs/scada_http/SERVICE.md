# scada_http

## Назначение

`scada_http` — backend-модуль transport foundation для HTTP API Dispatcher.

Модуль отвечает за transport-level часть будущего API gateway.

## Статус

Foundation.

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
- `/api/system/health` endpoint foundation;
- `/api/system/modules` endpoint foundation;
- helper-функции для response и method/status formatting.

HTTP server еще не реализован.

## Зона ответственности

`scada_http` отвечает за:

- HTTP server options;
- TCP/HTTP transport;
- request parsing;
- response writing;
- route dispatch bridge;
- development HTTP server lifecycle.

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

- будет доставлять HTTP-запросы до API handlers и возвращать HTTP-ответы.

`scada_app`:

- хранит application composition и startup plan.

`dispatcher_server`:

- запускает application и позже будет запускать HTTP server.

## Default policy

На этапе foundation будущий HTTP server должен слушать только localhost:

    127.0.0.1

Рекомендуемый development port:

    8080

## Текущие модели

Добавлены:

- `HttpMethod`;
- `HttpStatusCode`;
- `HttpServerOptions`;
- `HttpHeader`;
- `HttpRequest`;
- `HttpResponse`;
- `HttpEndpoint`;
- `HttpRoute`;
- `HttpRouteDispatcher`.

## Route dispatcher foundation

`HttpRouteDispatcher` поддерживает:

- регистрацию маршрута;
- exact path matching;
- method matching;
- защиту от duplicate method/path route;
- `400 Bad Request` для невалидного request;
- `404 Not Found` для неизвестного path;
- `405 Method Not Allowed` для известного path с неподдерживаемым method.

## System health endpoint

Добавлен foundation endpoint:

    GET /api/system/health

Endpoint возвращает JSON с базовым состоянием backend:

    {
      "status": "ok",
      "product": "Dispatcher",
      "executable": "dispatcher_server",
      "version": "0.1.0-dev",
      "mode": "Development",
      "api": "available",
      "transport": "http",
      "endpoint": "/api/system/health"
    }

## System modules endpoint

Добавлен foundation endpoint:

    GET /api/system/modules

Endpoint возвращает JSON со списком backend-модулей:

    {
      "modules": [
        {
          "code": "scada_core",
          "name": "Core",
          "description": "Core runtime services",
          "status": "Running"
        }
      ],
      "count": 1
    }

На текущем этапе endpoints можно зарегистрировать в `HttpRouteDispatcher`, но нельзя открыть из браузера, потому что HTTP server еще не подключен.

## Ограничения

Пока нет:

- HTTP server;
- TCP acceptor;
- request parser на основе Boost.Beast;
- response writer на основе Boost.Beast;
- HTTPS;
- CORS;
- authentication;
- authorization;
- WebSocket/SSE.
