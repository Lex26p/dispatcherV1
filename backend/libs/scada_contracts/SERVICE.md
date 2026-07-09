# scada_contracts

## Назначение

`scada_contracts` — backend-модуль контрактов Dispatcher.

Модуль содержит структуры, которые используются на границах между слоями системы:

- API;
- frontend;
- backend-модули;
- репозитории;
- импорт и экспорт;
- интеграции.

Главная задача модуля — не отдавать наружу внутренние доменные структуры напрямую.

Например, внутренний доменный объект:

    ObjectNode

не должен напрямую становиться API-моделью.

Для внешних слоев используется отдельная DTO-модель:

    ObjectNodeDto

То же правило действует для устройств и тегов.

Внутренние доменные модели:

    Device
    Tag

не должны напрямую становиться API-моделями.

Для внешних слоев используются:

    DeviceDto
    TagDto

## Границы ответственности

`scada_contracts` отвечает за:

- общие API-обертки;
- DTO;
- структуры запросов и ответов;
- простые контрактные модели;
- стабильные структуры для внешних слоев.

## Что модуль не должен делать

`scada_contracts` не должен содержать:

- бизнес-логику;
- работу с базой данных;
- опрос оборудования;
- протокольные драйверы;
- доменную валидацию;
- управление жизненным циклом модулей;
- UI-логику.

## Основные принципы

Контракты должны быть простыми.

DTO должны содержать данные, а не бизнес-логику.

Контракты не должны зависеть от доменных модулей.

Например:

- `scada_contracts` может зависеть от `scada_common`;
- `scada_contracts` не должен зависеть от `scada_objects`;
- `scada_contracts` не должен зависеть от `scada_devices`;
- `scada_contracts` не должен зависеть от `scada_tags`;
- `scada_objects` не должен зависеть от DTO;
- `scada_devices` не должен зависеть от DTO;
- `scada_tags` не должен зависеть от DTO;
- преобразование domain <-> DTO будет вынесено в отдельный слой позже.

## Уже существующие контракты

### ApiEnvelope

`ApiEnvelope` — базовая обертка API-ответа.

Используется для единообразного ответа системы.

Поля:

- `success`;
- `correlation_id`;
- `message`.

## Контракты объектной модели

DTO объектной модели находятся в:

- `include/scada_contracts/objects/object_dto.h`

### ObjectNodeDto

Контрактное представление объекта дерева.

Поля:

- `id`;
- `parent_id`;
- `type`;
- `code`;
- `name`;
- `description`;
- `path`.

### ObjectTreeDto

Контрактное представление дерева объектов.

Поля:

- `objects`.

### ObjectTreeValidationIssueDto

Контрактное представление ошибки валидации дерева объектов.

Поля:

- `code`;
- `object_id`;
- `message`.

### ObjectTreeValidationResultDto

Контрактное представление результата валидации дерева объектов.

Поля:

- `issues`.

Методы:

- `is_valid()`.

### ResponsibilityZoneDto

Контрактное представление зоны ответственности.

Поля:

- `id`;
- `code`;
- `name`;
- `description`;
- `object_ids`;
- `user_ids`;
- `role_codes`.

### ResponsibilityZoneValidationIssueDto

Контрактное представление ошибки валидации зоны ответственности.

Поля:

- `code`;
- `zone_id`;
- `message`.

### ResponsibilityZoneValidationResultDto

Контрактное представление результата валидации зоны ответственности.

Поля:

- `issues`.

Методы:

- `is_valid()`.

## Контракты устройств

DTO устройств находятся в:

- `include/scada_contracts/devices/device_dto.h`

### DeviceConnectionDto

Контрактное представление подключения устройства.

Поля:

- `protocol`;
- `host`;
- `port`;
- `ip_mode`;
- `network_interface`.

### DeviceDto

Контрактное представление устройства.

Поля:

- `id`;
- `object_id`;
- `code`;
- `name`;
- `description`;
- `protocol`;
- `state`;
- `connection`.

### DeviceListDto

Контрактное представление списка устройств.

Поля:

- `devices`.

### DeviceValidationIssueDto

Контрактное представление ошибки валидации устройства.

Поля:

- `code`;
- `device_id`;
- `message`.

### DeviceValidationResultDto

Контрактное представление результата валидации устройства.

Поля:

- `issues`.

Методы:

- `is_valid()`.

### DeviceDiagnosticsDto

Контрактное представление диагностики устройства.

Поля:

- `device_id`;
- `communication_status`;
- `health_status`;
- `successful_requests`;
- `failed_requests`;
- `timeout_count`;
- `reconnect_count`;
- `last_latency_ms`;
- `last_error`.

## Контракты тегов

DTO тегов находятся в:

- `include/scada_contracts/tags/tag_dto.h`

### TagAddressDto

Контрактное представление адреса тега.

Поля:

- `protocol`;
- `address`;
- `protocol_address_json`;
- `bit_index`;
- `byte_offset`;
- `mask`;
- `shift`;
- `bit_length`.

### TagDto

Контрактное представление тега.

Поля:

- `id`;
- `object_id`;
- `device_id`;
- `type`;
- `value_type`;
- `archive_policy`;
- `code`;
- `name`;
- `description`;
- `engineering_unit`;
- `scale`;
- `offset`;
- `address`;
- `is_enabled`.

### TagListDto

Контрактное представление списка тегов.

Поля:

- `tags`.

### TagCurrentValueDto

Контрактное представление текущего значения тега.

Поля:

- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `last_good_value`;
- `last_good_timestamp`;
- `change_counter`.

### TagValidationIssueDto

Контрактное представление ошибки валидации тега.

Поля:

- `code`;
- `tag_id`;
- `message`.

### TagValidationResultDto

Контрактное представление результата валидации тега.

Поля:

- `issues`.

Методы:

- `is_valid()`.

## Почему идентификаторы в DTO являются строками

В доменной модели используются типизированные идентификаторы:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `DeviceId`;
- `TagId`.

Но в DTO идентификаторы представлены как строки.

Причины:

- DTO должны быть удобны для API;
- JSON обычно передает идентификаторы строками;
- frontend не должен знать C++-типы идентификаторов;
- внешние интеграции не должны зависеть от внутренних domain-классов.

## Почему enum в DTO являются строками

В доменной модели используются enum:

- `ObjectType`;
- `DeviceProtocol`;
- `DeviceState`;
- `IpMode`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagValueSource`.

Но в DTO эти значения представлены строками.

Причины:

- API должен быть читаемым;
- frontend проще отображать строковые значения;
- внешние интеграции не должны зависеть от C++ enum;
- текстовые значения удобнее логировать и диагностировать.

## Почему значения тегов в DTO являются строками

В доменной модели текущее значение тега хранится в `TagValuePayload`.

Это variant-тип, который может содержать:

- пустое значение;
- `bool`;
- `int32`;
- `int64`;
- `float`;
- `double`;
- `string`.

В DTO значения представлены как строки.

Причины:

- это упрощает первый контрактный слой;
- JSON-сериализация будет добавлена позже;
- API сможет дополнительно передавать `value_type`;
- frontend сможет сам форматировать значение;
- сложная типизация значений будет уточняться в runtime/API-слое.

## Почему timestamps в DTO являются строками

В доменной модели время хранится как `std::chrono::system_clock::time_point`.

В DTO timestamps представлены строками.

Ожидаемый будущий формат:

    ISO 8601 / UTC

Например:

    2026-07-09T12:30:00Z

Фактическое форматирование будет добавлено позже в mapper/API-слое.

## Основные файлы

- `include/scada_contracts/api_envelope.h`
- `include/scada_contracts/objects/object_dto.h`
- `include/scada_contracts/devices/device_dto.h`
- `include/scada_contracts/tags/tag_dto.h`
- `src/api_envelope.cpp`
- `src/object_dto.cpp`
- `src/device_dto.cpp`
- `src/tag_dto.cpp`

## Зависимости

- `scada_common`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_contracts`.
- Добавлен `ApiEnvelope`.
- Добавлен `ObjectNodeDto`.
- Добавлен `ObjectTreeDto`.
- Добавлен `ObjectTreeValidationIssueDto`.
- Добавлен `ObjectTreeValidationResultDto`.
- Добавлен `ResponsibilityZoneDto`.
- Добавлен `ResponsibilityZoneValidationIssueDto`.
- Добавлен `ResponsibilityZoneValidationResultDto`.
- Добавлен `DeviceConnectionDto`.
- Добавлен `DeviceDto`.
- Добавлен `DeviceListDto`.
- Добавлен `DeviceValidationIssueDto`.
- Добавлен `DeviceValidationResultDto`.
- Добавлен `DeviceDiagnosticsDto`.
- Добавлен `TagAddressDto`.
- Добавлен `TagDto`.
- Добавлен `TagListDto`.
- Добавлен `TagCurrentValueDto`.
- Добавлен `TagValidationIssueDto`.
- Добавлен `TagValidationResultDto`.
- Модуль подключен к CMake.
- Модуль используется `dispatcher_server`.

### Делаем сейчас

- Формируем слой контрактов для объектной модели.
- Формируем слой контрактов для модели устройств.
- Формируем слой контрактов для модели тегов.
- Разделяем внутреннюю доменную модель и будущую API-модель.

### Нужно доделать

- Добавить request/response DTO для API объектной модели.
- Добавить request/response DTO для API устройств.
- Добавить request/response DTO для API тегов.
- Добавить DTO для создания объекта.
- Добавить DTO для обновления объекта.
- Добавить DTO для создания зоны ответственности.
- Добавить DTO для обновления зоны ответственности.
- Добавить DTO для создания устройства.
- Добавить DTO для обновления устройства.
- Добавить DTO для создания тега.
- Добавить DTO для обновления тега.
- Добавить mapper-слой domain <-> DTO.
- Добавить тесты DTO и мапперов позже.

### Расширить позже

- Контракты аварий.
- Контракты событий.
- Контракты истории.
- Контракты дашбордов.
- Контракты команд.
- Контракты пользователей и прав доступа.
- Контракты polling.
- Контракты диагностики системы.

### Не входит в MVP этого шага

- HTTP API.
- JSON-сериализация.
- REST-контроллеры.
- WebSocket-контракты.
- OpenAPI-генерация.
- Mapper-слой.