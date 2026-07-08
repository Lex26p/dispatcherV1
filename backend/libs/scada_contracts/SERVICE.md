# scada_contracts

## Назначение

`scada_contracts` — библиотека контрактов обмена Dispatcher.

В этом модуле должны находиться DTO, API envelopes, WebSocket envelopes и другие структуры, которые используются на границах системы.

## Границы ответственности

Модуль отвечает за:

- REST DTO;
- WebSocket DTO;
- API envelope;
- error response;
- paged response;
- operation response;
- контракты между frontend и backend;
- будущие контракты между сервисами.

## Что модуль не должен делать

`scada_contracts` не должен содержать:

- бизнес-логику;
- работу с БД;
- опрос оборудования;
- вычисление аварий;
- проверку прав;
- хранение текущих значений.

## Основные файлы

- `include/scada_contracts/api_envelope.h`
- `src/api_envelope.cpp`

## Зависимости

- `scada_common`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_contracts`.
- Добавлен минимальный `ApiEnvelope`.
- Добавлена функция `make_success_envelope()`.

### Делаем сейчас

- Подключаем модуль к `dispatcher_server`.

### Нужно доделать

- Добавить единый `ErrorResponse`.
- Добавить `PagedResponse`.
- Добавить `OperationResponse`.
- Добавить `WebSocketEnvelope`.
- Добавить DTO для текущих значений тегов.
- Добавить DTO для health endpoints.

### Расширить позже

- Добавить OpenAPI-совместимые DTO.
- Добавить DTO для Integration API.
- Добавить DTO для Panel API.
- Добавить DTO для Workstation API.
- Добавить контракты для будущих gRPC/Protobuf сервисов.

### Не входит в MVP

- SDK generation.
- Полный внешний OpenAPI portal.
- GraphQL.