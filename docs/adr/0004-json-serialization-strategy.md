# ADR 0004 — JSON Serialization Strategy

## Статус

Accepted

## Контекст

System HTTP endpoints первоначально формировали JSON вручную через:

    std::ostringstream
    json_escape

Одинаковая escaping implementation существовала отдельно в health и modules endpoints.

Такой подход создает риски:

- дублирование;
- неполная обработка JSON escaping;
- ошибки при создании вложенных объектов и массивов;
- сложность nullable values;
- нестабильные числовые типы;
- трудное расширение API contracts;
- низкая тестируемость.

Drogon использует JsonCpp, однако Dispatcher не должен зависеть от транзитивной зависимости framework неявно.

Domain и application modules также не должны включать JsonCpp или Drogon headers.

## Решение

Использовать JsonCpp как implementation dependency модуля:

    scada_http

JsonCpp объявляется прямой зависимостью в:

    vcpkg.json

В `scada_http` вводится transport abstraction:

    JsonValue

Публичный header `JsonValue` использует только стандартные C++ types.

JsonCpp скрыт в `.cpp` через PIMPL.

Поддерживаемые JSON types:

- null;
- object;
- array;
- string;
- bool;
- signed 64-bit integer;
- unsigned 64-bit integer;
- double.

## Ownership model

`JsonValue` является move-only type.

Вложенные объекты и массивы передаются через move.

Это исключает неявное дорогое копирование JSON trees и делает ownership явным.

## Serialization format

Production responses используют compact JSON без форматирующих отступов.

Порядок object properties не является частью API contract.

Consumers должны читать свойства по имени.

## Module boundaries

Допустимо:

    scada_http -> JsonCpp

Недопустимо:

    scada_core -> JsonCpp
    scada_objects -> JsonCpp
    scada_devices -> JsonCpp
    scada_tags -> JsonCpp
    scada_api -> Drogon
    domain model -> JsonCpp

Domain values преобразуются в JSON только на transport boundary.

## Endpoint migration

На первом этапе переводятся:

    GET /api/system/health
    GET /api/system/modules

Ручные `json_escape` implementations удаляются.

Форма response contracts не меняется.

## Error contract

Следующий шаг использует тот же `JsonValue` для единого API error envelope.

## Тестирование

Тесты должны проверять:

- strings и escaping;
- bool;
- signed integer;
- unsigned integer;
- double;
- null;
- nested objects;
- arrays;
- неверные object/array operations;
- валидность health JSON;
- валидность modules JSON;
- специальные символы в module fields.

Тесты анализируют JSON parser-ом и не зависят от порядка properties или пробелов.

## Отклоненные варианты

### Ручная конкатенация JSON

Отклонена из-за дублирования и риска некорректного escaping.

### Публичное использование Json::Value

Отклонено, потому что JsonCpp стал бы частью public API `scada_http`.

### Использование Drogon JSON types во всех endpoint headers

Отклонено из-за привязки Dispatcher transport contracts к framework.

### Новый отдельный scada_json module

Отложен.

На текущем этапе JSON нужен только HTTP transport. Отдельный модуль будет рассмотрен, если serializer понадобится realtime, integrations или persistence layers.

### Добавление второй JSON library

Отклонено.

Drogon уже использует JsonCpp, поэтому дополнительная библиотека увеличила бы dependency footprint без достаточной пользы.

## Последствия

Положительные:

- удаляется duplicated escaping;
- формирование nested JSON становится безопаснее;
- JSON behavior покрывается unit tests;
- domain modules остаются framework-independent;
- error envelope сможет использовать тот же foundation.

Отрицательные:

- появляется небольшая wrapper abstraction;
- `JsonValue` нужно расширять при появлении новых transport requirements;
- JsonCpp становится явно закрепленной dependency HTTP transport.
