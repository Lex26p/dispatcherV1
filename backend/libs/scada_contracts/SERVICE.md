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
    HistorySample
    HistoryQuery

не должны напрямую становиться API-моделями.

Для внешних слоев используются отдельные DTO:

    ObjectNodeDto
    DeviceDto
    TagDto
    PollingGroupDto
    PollingTaskDto
    RuntimeValueDto
    RuntimeValueEventDto
    HistorySampleDto
    HistoryQueryDto

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
- historian processing;
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
- `scada_contracts` не должен зависеть от `scada_historian`;
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

Сейчас есть DTO для:

- runtime value;
- runtime value event;
- runtime value apply result;
- runtime value snapshot;
- runtime validation issue;
- runtime validation result.

## Контракты historian

DTO historian находятся в:

- `include/scada_contracts/historian/historian_dto.h`

### HistorySampleDto

Контрактное представление исторической записи значения тега.

Поля:

- `id`;
- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `change_counter`;
- `engineering_unit`;
- `message`.

Методы:

- `has_sample_id()`;
- `has_tag_id()`;
- `has_value()`;
- `has_message()`.

### ArchiveDecisionDto

Контрактное представление решения об архивировании.

Поля:

- `tag_id`;
- `policy`;
- `reason`;
- `should_archive`;
- `first_sample`;
- `value_changed`;
- `quality_changed`;
- `throttled`;
- `message`.

Методы:

- `should_archive_sample()`;
- `has_message()`.

### HistoryBatchDto

Контрактное представление batch history samples.

Поля:

- `samples`.

Методы:

- `empty()`;
- `size()`.

### HistoryBatchWriteResultDto

Контрактное представление результата batch-записи.

Поля:

- `status`;
- `attempted_count`;
- `written_count`;
- `failed_count`;
- `message`.

Методы:

- `is_success()`;
- `has_failures()`;
- `has_message()`.

### HistoryQueryDto

Контрактное представление запроса истории.

Состоит из:

- `HistoryQueryTimeRangeDto`;
- `HistoryQueryFilterDto`;
- `HistoryQueryOptionsDto`.

### HistoryQueryResultDto

Контрактное представление результата запроса истории.

Поля:

- `samples`;
- `total_count`;
- `truncated`;
- `message`.

Методы:

- `empty()`;
- `size()`;
- `has_message()`.

### HistoryValidationIssueDto

Контрактное представление ошибки historian validation.

Поля:

- `code`;
- `tag_id`;
- `message`.

### HistoryValidationResultDto

Контрактное представление результата historian validation.

Поля:

- `issues`.

Методы:

- `is_valid()`;
- `has_issues()`.

## Почему идентификаторы в DTO являются строками

В доменной модели используются типизированные идентификаторы.

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
- `include/scada_contracts/historian/historian_dto.h`
- `src/api_envelope.cpp`
- `src/object_dto.cpp`
- `src/device_dto.cpp`
- `src/tag_dto.cpp`
- `src/polling_dto.cpp`
- `src/runtime_dto.cpp`
- `src/historian_dto.cpp`

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
- Добавлены DTO historian.
- Модуль подключен к CMake.
- Модуль используется `dispatcher_server`.

### Делаем сейчас

- Формируем слой контрактов для объектной модели.
- Формируем слой контрактов для модели устройств.
- Формируем слой контрактов для модели тегов.
- Формируем слой контрактов для polling.
- Формируем слой контрактов для runtime values.
- Формируем слой контрактов для historian.
- Разделяем внутреннюю доменную модель и будущую API-модель.

### Нужно доделать

- Добавить request/response DTO для API.
- Добавить mapper-слой domain <-> DTO.
- Добавить тесты DTO и мапперов позже.

### Расширить позже

- Контракты аварий.
- Контракты событий Event Manager.
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