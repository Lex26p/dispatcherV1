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

не должны напрямую становиться API-моделями.

Для внешних слоев используются отдельные DTO:

    ObjectNodeDto
    DeviceDto
    TagDto
    PollingGroupDto
    PollingTaskDto

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
- `scada_contracts` не должен зависеть от `scada_protocols`;
- `scada_contracts` не должен зависеть от `scada_polling`;
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

### PollingAddressDto

Контрактное представление адреса polling task.

Поля:

- `protocol`;
- `address`;
- `protocol_address_json`;
- `bit_index`;
- `byte_offset`;
- `mask`;
- `shift`;
- `bit_length`.

### PollingTaskDto

Контрактное представление polling task.

Поля:

- `id`;
- `group_id`;
- `device_id`;
- `tag_id`;
- `protocol`;
- `address`;
- `state`;
- `interval_ms`;
- `timeout_ms`;
- `priority`;
- `is_enabled`;
- `description`.

### PollingGroupDto

Контрактное представление polling group.

Поля:

- `id`;
- `device_id`;
- `protocol`;
- `mode`;
- `code`;
- `name`;
- `description`;
- `interval_ms`;
- `timeout_ms`;
- `max_batch_size`;
- `priority`;
- `is_enabled`;
- `tasks`.

### PollingTaskExecutionResultDto

Контрактное представление результата выполнения одной polling task.

Поля:

- `task_id`;
- `tag_id`;
- `resulting_state`;
- `quality`;
- `success`;
- `latency_ms`;
- `message`.

### PollingGroupExecutionResultDto

Контрактное представление результата выполнения polling group.

Поля:

- `group_id`;
- `status`;
- `task_results`;
- `message`.

Методы:

- `is_success()`.

### PollingValidationIssueDto

Контрактное представление ошибки валидации polling configuration.

Поля:

- `code`;
- `entity_id`;
- `message`.

### PollingTaskValidationResultDto

Контрактное представление результата валидации polling task.

Поля:

- `issues`.

Методы:

- `is_valid()`.

### PollingGroupValidationResultDto

Контрактное представление результата валидации polling group.

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

В DTO значения и timestamps будут представляться строками или простыми типами.

Фактическое форматирование будет добавлено позже в mapper/API-слое.

## Основные файлы

- `include/scada_contracts/api_envelope.h`
- `include/scada_contracts/objects/object_dto.h`
- `include/scada_contracts/devices/device_dto.h`
- `include/scada_contracts/tags/tag_dto.h`
- `include/scada_contracts/polling/polling_dto.h`
- `src/api_envelope.cpp`
- `src/object_dto.cpp`
- `src/device_dto.cpp`
- `src/tag_dto.cpp`
- `src/polling_dto.cpp`

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
- Модуль подключен к CMake.
- Модуль используется `dispatcher_server`.

### Делаем сейчас

- Формируем слой контрактов для объектной модели.
- Формируем слой контрактов для модели устройств.
- Формируем слой контрактов для модели тегов.
- Формируем слой контрактов для polling.
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