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
- event processing;
- alarm processing;
- управление жизненным циклом модулей;
- UI-логику.

## Основные принципы

Контракты должны быть простыми.

DTO должны содержать данные, а не бизнес-логику.

Контракты не должны зависеть от доменных модулей.

`scada_contracts` может зависеть от:

- `scada_common`.

`scada_contracts` не должен зависеть от:

- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`;
- `scada_polling`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`.

Преобразование domain <-> DTO будет вынесено в отдельный слой позже.

## ApiEnvelope

`ApiEnvelope` — базовая обертка API-ответа.

Поля:

- `success`;
- `correlation_id`;
- `message`.

## Контракты объектной модели

DTO объектной модели находятся в:

- `include/scada_contracts/objects/object_dto.h`.

## Контракты устройств

DTO устройств находятся в:

- `include/scada_contracts/devices/device_dto.h`.

## Контракты тегов

DTO тегов находятся в:

- `include/scada_contracts/tags/tag_dto.h`.

## Контракты polling

DTO polling находятся в:

- `include/scada_contracts/polling/polling_dto.h`.

## Контракты runtime

DTO runtime находятся в:

- `include/scada_contracts/runtime/runtime_dto.h`.

## Контракты historian

DTO historian находятся в:

- `include/scada_contracts/historian/historian_dto.h`.

## Контракты events

DTO events находятся в:

- `include/scada_contracts/events/event_dto.h`.

### EventRecordDto

Контрактное представление системного события.

Поля:

- `id`;
- `category`;
- `severity`;
- `source_type`;
- `code`;
- `title`;
- `message`;
- `source_id`;
- `object_id`;
- `device_id`;
- `tag_id`;
- `correlation_id`;
- `timestamp`;
- `sequence_number`.

### EventListDto

Контрактное представление списка событий.

Поля:

- `events`;
- `total_count`;
- `truncated`;
- `message`.

### EventValidationIssueDto

Контрактное представление ошибки валидации события.

Поля:

- `code`;
- `event_id`;
- `message`.

### EventValidationResultDto

Контрактное представление результата валидации событий.

Поля:

- `issues`.

## Контракты alarms

DTO alarms находятся в:

- `include/scada_contracts/alarms/alarm_dto.h`.

### AlarmRecordDto

Контрактное представление аварии.

Поля:

- `id`;
- `event_id`;
- `source_type`;
- `severity`;
- `priority`;
- `state`;
- `code`;
- `title`;
- `message`;
- `source_id`;
- `object_id`;
- `device_id`;
- `tag_id`;
- `correlation_id`;
- `created_at`;
- `activated_at`;
- `acknowledged_at`;
- `cleared_at`;
- `closed_at`;
- `acknowledged_by`;
- `closed_by`;
- `occurrence_count`;
- `requires_acknowledgement`.

### AlarmTransitionDto

Контрактное представление transition аварии.

Поля:

- `alarm_id`;
- `type`;
- `previous_state`;
- `new_state`;
- `timestamp`;
- `actor`;
- `reason`.

### AlarmRuleDto

Контрактное представление alarm rule.

Поля:

- `id`;
- `type`;
- `state`;
- `source_type`;
- `severity`;
- `priority`;
- `comparison`;
- `code`;
- `title`;
- `message`;
- `tag_id`;
- `object_id`;
- `device_id`;
- `threshold`;
- `expected_quality`;
- `require_good_quality`;
- `requires_acknowledgement`.

### AlarmRuleEvaluationResultDto

Контрактное представление результата проверки alarm rule.

Поля:

- `status`;
- `rule`;
- `alarm`;
- `evaluated`;
- `matched`;
- `has_alarm_record`;
- `message`.

### AlarmListDto

Контрактное представление списка аварий.

Поля:

- `alarms`;
- `total_count`;
- `truncated`;
- `message`.

### AlarmValidationIssueDto

Контрактное представление ошибки валидации alarms.

Поля:

- `code`;
- `alarm_id`;
- `rule_id`;
- `message`.

### AlarmValidationResultDto

Контрактное представление результата валидации alarms.

Поля:

- `issues`.

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

- `include/scada_contracts/api_envelope.h`;
- `include/scada_contracts/objects/object_dto.h`;
- `include/scada_contracts/devices/device_dto.h`;
- `include/scada_contracts/tags/tag_dto.h`;
- `include/scada_contracts/polling/polling_dto.h`;
- `include/scada_contracts/runtime/runtime_dto.h`;
- `include/scada_contracts/historian/historian_dto.h`;
- `include/scada_contracts/events/event_dto.h`;
- `include/scada_contracts/alarms/alarm_dto.h`;
- `src/api_envelope.cpp`;
- `src/object_dto.cpp`;
- `src/device_dto.cpp`;
- `src/tag_dto.cpp`;
- `src/polling_dto.cpp`;
- `src/runtime_dto.cpp`;
- `src/historian_dto.cpp`;
- `src/event_dto.cpp`;
- `src/alarm_dto.cpp`.

## Зависимости

- `scada_common`.

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
- Добавлены DTO events.
- Добавлены DTO alarms.
- Модуль подключен к CMake.
- Модуль используется `dispatcher_server`.

### Нужно доделать

- Добавить request/response DTO для API.
- Добавить mapper-слой domain <-> DTO.
- Добавить JSON serialization позже.
- Добавить тесты DTO и мапперов позже.

### Расширить позже

- Контракты команд.
- Контракты пользователей и прав доступа.
- Контракты дашбордов.
- Контракты protocol diagnostics.
- Контракты уведомлений.

### Не входит в этот шаг

- HTTP API.
- JSON-сериализация.
- REST-контроллеры.
- WebSocket-контракты.
- OpenAPI-генерация.
- Mapper-слой.
