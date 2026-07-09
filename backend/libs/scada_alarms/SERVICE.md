# scada_alarms

## Назначение

`scada_alarms` — backend-модуль аварий Dispatcher.

Модуль отвечает за базовую модель аварий и будущий Alarm Manager.

На текущем этапе реализованы:

- базовая модель аварии;
- базовые состояния аварии;
- lifecycle transition foundation;
- foundation alarm rules;
- repository-интерфейсы alarms;
- DTO alarms в `scada_contracts`;
- черновая SQL-миграция events/alarms.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Alarms layer будет принимать причины аварий из разных подсистем:

    Runtime -> Alarms
    Polling -> Alarms
    Devices -> Alarms
    Protocols -> Alarms
    Scripts -> Alarms
    Integrations -> Alarms
    Manual actions -> Alarms

Связь с events layer:

    AlarmRecord -> EventRecord -> future Event Manager

Текущая lifecycle-цепочка:

    AlarmRecord -> AlarmTransitionRequest -> AlarmTransitionResult -> AlarmTransitionRecord

Текущая rules-цепочка:

    TagCurrentValue -> AlarmRule -> AlarmRuleEvaluationResult -> AlarmRecord

## Границы ответственности

`scada_alarms` отвечает за:

- `AlarmId`;
- `AlarmRuleId`;
- `AlarmTimestamp`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- `AlarmTransitionType`;
- `AlarmTransitionValidationCode`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `AlarmRuleType`;
- `AlarmRuleState`;
- `AlarmRuleComparison`;
- `AlarmRuleEvaluationStatus`;
- `AlarmRule`;
- `AlarmRuleEvaluationResult`;
- `IAlarmRepository`;
- `IAlarmTransitionRepository`;
- `IAlarmRuleRepository`;
- создание базовой аварии;
- проверку допустимости transition;
- применение transition к аварии;
- базовую оценку alarm rules;
- создание AlarmRecord при срабатывании rule;
- связь severity аварии с severity события;
- подготовку к alarm transition history;
- подготовку к future Alarm Manager;
- подготовку к future Event Manager integration.

## Что модуль не должен делать сейчас

На текущем шаге `scada_alarms` не должен выполнять:

- реализацию repository;
- хранение alarm rules в БД через приложение;
- хранение transition history в БД через приложение;
- хранение аварий в БД через приложение;
- поиск существующих активных аварий через БД;
- дедупликацию аварий;
- автоматическую подписку на runtime events;
- публикацию в EventBus;
- автоматическое создание EventRecord;
- доставку аварий в UI;
- HTTP API;
- WebSocket API;
- UI-логику.

## Основные сущности

### AlarmId

Типизированный идентификатор аварии.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### AlarmRuleId

Типизированный идентификатор alarm rule.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### AlarmTimestamp

Временная метка аварии.

На текущем этапе используется timestamp из `scada_events`:

    dispatcher::events::EventTimestamp

### AlarmSourceType

Тип источника аварии.

Поддерживаются значения:

- `Unknown`;
- `System`;
- `Object`;
- `Device`;
- `Tag`;
- `Communication`;
- `Runtime`;
- `Historian`;
- `Polling`;
- `Protocol`;
- `Script`;
- `Integration`;
- `Manual`.

### AlarmSeverity

Серьезность аварии.

Поддерживаются значения:

- `Information`;
- `Warning`;
- `Minor`;
- `Major`;
- `Critical`.

### AlarmPriority

Приоритет аварии.

Поддерживаются значения:

- `Low`;
- `Medium`;
- `High`;
- `Critical`.

Severity и priority разделены намеренно.

### AlarmState

Базовое состояние аварии.

Поддерживаются значения:

- `New`;
- `Active`;
- `Acknowledged`;
- `Cleared`;
- `Closed`;
- `Shelved`;
- `Suppressed`.

### AlarmRecord

Базовая модель аварии.

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

## Alarm lifecycle

Lifecycle описывает изменение состояния аварии.

На текущем этапе lifecycle представлен через:

- `AlarmTransitionType`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `can_apply_alarm_transition()`;
- `apply_alarm_transition()`.

## Alarm rules foundation

Alarm rules foundation отвечает за проверку текущего значения тега на аварийное условие.

Текущая цепочка:

    TagCurrentValue -> AlarmRule -> AlarmRuleEvaluationResult

Если rule matched, результат содержит `AlarmRecord`.

## Repository-интерфейсы

### IAlarmRepository

Интерфейс будущего хранения аварий.

Методы:

- `find_by_id()`;
- `find_active_by_source_id()`;
- `find_active_by_tag_id()`;
- `find_active()`;
- `find_recent()`;
- `find_by_state()`;
- `save()`;
- `remove_by_id()`;
- `remove_closed_older_than()`;
- `count_active()`.

### IAlarmTransitionRepository

Интерфейс будущего хранения transition history.

Методы:

- `find_by_alarm_id()`;
- `save()`;
- `remove_by_alarm_id()`.

### IAlarmRuleRepository

Интерфейс будущего хранения alarm rules.

Методы:

- `find_by_id()`;
- `find_enabled()`;
- `find_by_tag_id()`;
- `find_by_state()`;
- `save()`;
- `remove_by_id()`;
- `set_state()`;
- `count_enabled()`.

На текущем этапе реализаций repository нет.

Будущие реализации:

- PostgreSQL;
- in-memory для тестов;
- mock для unit-тестов.

## База данных

Добавлена черновая миграция:

- `database/migrations/0007_events_alarms.sql`.

Миграция описывает таблицы:

- `event_records`;
- `alarm_records`;
- `alarm_transitions`;
- `alarm_rules`.

## Почему alarms связаны с events

Авария является особым типом события.

В будущем при создании аварии можно будет создавать связанный `EventRecord`.

`AlarmRecord.event_id` хранит эту связь.

## Почему пока нет Alarm Manager

`evaluate_alarm_rule()` проверяет только одно правило и одно значение.

Alarm Manager позже будет отвечать за:

- загрузку правил;
- проверку всех правил;
- поиск существующих активных аварий;
- дедупликацию;
- reactivation;
- transition history;
- Event Manager integration;
- хранение аварий;
- доставку в UI.

## Почему пока нет дедупликации аварий

Если rule matched, функция создает новый `AlarmRecord`.

Проверка существующей активной аварии требует repository или active alarm index.

Дедупликация будет добавлена позже в Alarm Manager.

## Почему source ids пока строки

AlarmRecord и AlarmRule должны уметь ссылаться на разные сущности:

- объект;
- устройство;
- тег;
- скрипт;
- интеграцию;
- внешний источник.

Чтобы не связывать `scada_alarms` со всеми доменными модулями сразу, source ids пока представлены строками.

## Зависимости

`scada_alarms` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_events`;
- `scada_tags`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`;
- нужна связь с `EventId`;
- нужно сопоставление `AlarmSeverity` с `EventSeverity`;
- нужен `TagCurrentValue`;
- нужен `TagValuePayload`;
- нужен `TagQuality`.

## Основные файлы

- `include/scada_alarms/alarm_ids.h`;
- `include/scada_alarms/alarm_record.h`;
- `include/scada_alarms/alarm_lifecycle.h`;
- `include/scada_alarms/alarm_rule.h`;
- `include/scada_alarms/alarm_module.h`;
- `include/scada_alarms/repository/IAlarmRepository.h`;
- `include/scada_alarms/repository/IAlarmTransitionRepository.h`;
- `include/scada_alarms/repository/IAlarmRuleRepository.h`;
- `src/alarm_record.cpp`;
- `src/alarm_lifecycle.cpp`;
- `src/alarm_rule.cpp`;
- `src/alarm_module.cpp`.

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_alarms`.
- Добавлен `AlarmId`.
- Добавлен `AlarmRuleId`.
- Добавлен `AlarmRecord`.
- Добавлен alarm lifecycle foundation.
- Добавлены alarm transitions.
- Добавлен alarm rules foundation.
- Добавлена функция `evaluate_alarm_rule()`.
- Добавлены repository-интерфейсы alarms.
- Добавлены DTO alarms.
- Добавлена миграция events/alarms.
- Добавлен `get_alarm_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Нужно доделать

- Добавить Alarm Manager.
- Добавить Event Manager integration.
- Добавить PostgreSQL repository implementation.
- Добавить Alarm API.
- Добавить WebSocket delivery.
- Добавить UI alarm journal.

### Не входит в этот шаг

- Alarm Manager.
- EventBus.
- Alarm storage implementation.
- Alarm API.
- WebSocket.
- Frontend.