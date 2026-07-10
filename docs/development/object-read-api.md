# Dispatcher Object Read API

## Статус

Foundation создан в Sprint 015, шаг 110.

## Endpoints

Object collection:

    GET /api/objects

Object tree:

    GET /api/objects/tree

## Object collection query

Поддерживаются:

    id
    parentId
    includeChildren
    limit
    offset

Пример:

    GET /api/objects?id=site-main&includeChildren=true

## Object tree query

Поддерживаются:

    id
    parentId

Пример subtree:

    GET /api/objects/tree?id=site-main

Пример children of parent:

    GET /api/objects/tree?parentId=site-main

## Collection response

Формат:

    {
      "objects": [],
      "count": 0,
      "total": 0,
      "offset": 0,
      "limit": 1000,
      "hasMore": false
    }

`count` — количество возвращенных элементов.

`total` — количество элементов после filters и до pagination.

## Object item

Формат:

    {
      "id": "site-main",
      "parentId": null,
      "type": "Site",
      "code": "SITE_MAIN",
      "name": "Main site",
      "description": "",
      "isRoot": true
    }

Отсутствующий parent сериализуется как JSON null.

## Tree response

Формат:

    {
      "roots": [],
      "count": 0
    }

Каждый tree node содержит:

    children

Tree строится application read service.

HTTP layer не реконструирует hierarchy.

## Ordering

Flat objects и tree siblings имеют детерминированную сортировку:

    code
    id

Pagination применяется после сортировки.

## Query validation

Query parameter names чувствительны к регистру.

Неизвестные параметры отклоняются.

Дублирующиеся параметры отклоняются.

Поддерживаются percent encoding и `+` как space.

Boolean values:

    true
    false
    1
    0

Limit:

- должен быть unsigned integer;
- должен быть больше нуля;
- не должен превышать `max_limit`.

Offset должен быть unsigned integer.

## Query error

Status:

    400 Bad Request

Envelope:

    {
      "error": {
        "code": "invalid_query",
        "message": "The query string is invalid.",
        "correlationId": "request-id",
        "details": {
          "field": "limit",
          "reason": "must_be_positive"
        }
      }
    }

## Empty result

Валидный filter без совпадений возвращает:

    HTTP 200

с пустым массивом или пустым tree.

## Composition

Development provider создается в:

    dispatcher_server

HTTP endpoint зависит только от:

    IConfigurationReadService

`scada_http` не зависит от concrete `DevelopmentConfigurationReadService`.

## Ограничения

На шаге 110 отсутствуют:

- create;
- update;
- delete;
- single-resource path routes;
- persistence;
- authorization;
- frontend Objects integration.
