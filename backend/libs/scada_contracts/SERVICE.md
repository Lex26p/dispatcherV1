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

Внутренние доменные модели:

    ObjectNode
    Device
    Tag
    PollingGroup
    PollingTask
    TagCurrentValue
    RuntimeValueEvent

не должны напрямую становиться API-моделями.

Для внешних слоев используются отдельные DTO:

    ObjectNodeDto
    DeviceDto
    TagDto
    PollingGroupDto
    PollingTaskDto
    RuntimeValueDto
    RuntimeValueEventDto

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
- runtime processing;
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
- `scada_contracts` не должен зависеть от `scada_protocols`;
- `scada_contracts` не должен зависеть от `scada_polling`;
- `scada_contracts` не должен зависеть от `scada_runtime`;
- преобразование domain <-> DTO будет вынесено в отдельный слой позже.

## ApiEnvelope

`ApiEnvelope` — базовая обертка API-ответа.

Поля:

- `success`;
- `correlation_id`;
- `message`.

## Контракты объектной модели

DTO объектной модели находятся в:

- `include/scada_contracts/objects/object_dto.h`

Сейчас есть DTO для:

- объектов;
- дерева объектов;
- валидации дерева объектов;
- зон ответственности;
- валидации зон ответственности.

## Контракты устройств

DTO устройств находятся в:

- `include/scada_contracts/devices/device_dto.h`

Сейчас есть DTO для:

- подключения устройства;
- устройства;
- списка устройств;
- валидации устройств;
- диагностики устройств.

## Контракты тегов

DTO тегов находятся в:

- `include/scada_contracts/tags/tag_dto.h`

Сейчас есть DTO для:

- адреса тега;
- тега;
- списка тегов;
- текущего значения тега;
- валидации тегов.

## Контракты polling

DTO polling находятся в:

- `include/scada_contracts/polling/polling_dto.h`

Сейчас есть DTO для:

- polling address;
- polling task;
- polling group;
- polling task execution result;
- polling group execution result;
- polling validation issue;
- polling task validation result;
- polling group validation result.

## Контракты runtime

DTO runtime находятся в:

- `include/scada_contracts/runtime/runtime_dto.h`

### RuntimeValueDto

Контрактное представление текущего runtime-значения тега.

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

Методы:

- `has_value()`;
- `has_last_good_value()`.

### RuntimeValueEventDto

Контрактное представление runtime-события изменения значения.

Поля:

- `tag_id`;
- `event_type`;
- `change_kind`;
- `previous_quality`;
- `current_quality`;
- `timestamp`;
- `change_counter`;
- `message`.

Методы:

- `has_message()`.

### RuntimeValueApplyResultDto

Контрактное представление результата применения значения к runtime store.

Поля:

- `tag_id`;
- `success`;
- `inserted`;
- `updated`;
- `changed`;
- `good_quality`;
- `last_good_updated`;
- `value_converted`;
- `engineering_transform_applied`;
- `change_kind`;
- `event`;
- `previous_change_counter`;
- `new_change_counter`;
- `quality`;
- `message`.

Методы:

- `is_success()`;
- `has_changed()`;
- `has_event()`;
- `has_message()`.

### RuntimeValueSnapshotDto

Контрактное представление сохраненного runtime snapshot.

Поля:

- `value`;
- `saved_at`;
- `updated_at`.

### RuntimeValueValidationIssueDto

Контрактное представление ошибки валидации runtime value.

Поля:

- `code`;
- `tag_id`;
- `message`.

### RuntimeValueValidationResultDto

Контрактное представление результата валидации runtime value.

Поля:

- `issues`.

Методы:

- `is_valid()`.

## Почему идентификаторы в DTO являются строками

В доменной модели используются типизированные идентификаторы:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `DeviceId`;
- `TagId`;
- `PollingTaskId`;
- `PollingGroupId`.

Но в DTO идентификаторы представлены как строки.

Причины:

- DTO должны быть удобны для API;
- JSON обычно передает идентификаторы строками;
- frontend не должен знать C++-типы идентификаторов;
- внешние интеграции не должны зависеть от внутренних domain-классов.

## Почему enum в DTO являются строками

В доменной модели используются enum.

Но в DTO эти значения представлены строками.

Причины:

- API должен быть читаемым;
- frontend проще отображать строковые значения;
- внешние интеграции не должны зависеть от C++ enum;
- текстовые значения удобнее логировать и диагностировать.

## Почему timestamps и значения в DTO являются строками

В доменной модели значения и время могут иметь C++-специфичные типы.

В DTO значения и timestamps представлены строками или простыми типами.

Фактическое форматирование будет добавлено позже в mapper/API-слое.

## Основные файлы

- `include/scada_contracts/api_envelope.h`
- `include/scada_contracts/objects/object_dto.h`
- `include/scada_contracts/devices/device_dto.h`
- `include/scada_contracts/tags/tag_dto.h`
- `include/scada_contracts/polling/polling_dto.h`
- `include/scada_contracts/runtime/runtime_dto.h`
- `src/api_envelope.cpp`
- `src/object_dto.cpp`
- `src/device_dto.cpp`
- `src/tag_dto.cpp`
- `src/polling_dto.cpp`
- `src/runtime_dto.cpp`

## Зависимости

- `scada_common`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_contracts`.
- Добавлен `ApiEnvelope`.
- Добавлены DTO объектной модели.
- Добавлены DTO устройств.
- Добавлены DTO тегов.
- Добавлены DTO polling.
- Добавлены DTO runtime.
- Модуль подключен к CMake.
- Модуль используется `dispatcher_server`.

### Делаем сейчас

- Формируем слой контрактов для объектной модели.
- Формируем слой контрактов для модели устройств.
- Формируем слой контрактов для модели тегов.
- Формируем слой контрактов для polling.
- Формируем слой контрактов для runtime values.
- Разделяем внутреннюю доменную модель и будущую API-модель.

### Нужно доделать

- Добавить request/response DTO для API.
- Добавить mapper-слой domain <-> DTO.
- Добавить тесты DTO и мапперов позже.

### Расширить позже

- Контракты аварий.
- Контракты событий.
- Контракты истории.
- Контракты дашбордов.
- Контракты команд.
- Контракты пользователей и прав доступа.
- Контракты protocol diagnostics.

### Не входит в этот шаг

- HTTP API.
- JSON-сериализация.
- REST-контроллеры.
- WebSocket-контракты.
- OpenAPI-генерация.
- Mapper-слой.