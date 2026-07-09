# scada_alarms

## Назначение

`scada_alarms` — backend-модуль аварий Dispatcher.

Модуль отвечает за базовую модель аварий и будущий Alarm Manager.

На текущем этапе реализованы:

- базовая модель аварии;
- базовые состояния аварии;
- lifecycle transition foundation.

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

## Границы ответственности

`scada_alarms` отвечает за:

- `AlarmId`;
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
- создание базовой аварии;
- проверку допустимости transition;
- применение transition к аварии;
- связь severity аварии с severity события;
- подготовку к alarm transition history;
- подготовку к alarm rules;
- подготовку к future Alarm Manager;
- подготовку к future Event Manager integration.

## Что модуль не должен делать сейчас

На текущем шаге `scada_alarms` не должен выполнять:

- вычисление alarm rules;
- хранение transition history в БД;
- хранение аварий в БД;
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

На текущем этапе alarm id может быть пустым до момента сохранения.

Будущая repository-реализация сможет назначать id при записи.

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

Severity отвечает на вопрос:

    насколько серьезна авария?

### AlarmPriority

Приоритет аварии.

Поддерживаются значения:

- `Low`;
- `Medium`;
- `High`;
- `Critical`.

Priority отвечает на вопрос:

    насколько срочно оператор должен отреагировать?

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

Методы:

- `has_alarm_id()`;
- `has_event_id()`;
- `has_code()`;
- `has_title()`;
- `has_message()`;
- `has_source_id()`;
- `has_object_id()`;
- `has_device_id()`;
- `has_tag_id()`;
- `has_correlation_id()`;
- `has_acknowledged_by()`;
- `has_closed_by()`;
- `is_active()`;
- `is_closed()`;
- `has_valid_identity()`.

### make_alarm_record

Функция создает базовую аварийную запись.

Принимает:

- source type;
- severity;
- priority;
- code;
- title;
- message.

Автоматически выставляет:

- `state = New`;
- `created_at`;
- `activated_at`;
- `occurrence_count = 1`.

### to_event_severity

Функция преобразует `AlarmSeverity` в `EventSeverity`.

Это нужно для будущей связи:

    AlarmRecord -> EventRecord

## Alarm lifecycle

Lifecycle описывает изменение состояния аварии.

На текущем этапе lifecycle представлен через:

- `AlarmTransitionType`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `can_apply_alarm_transition()`;
- `apply_alarm_transition()`.

### AlarmTransitionType

Тип перехода аварии.

Поддерживаются значения:

- `Activate`;
- `Acknowledge`;
- `Clear`;
- `Close`;
- `Shelve`;
- `Unshelve`;
- `Suppress`;
- `Unsuppress`;
- `Reactivate`.

### AlarmTransitionValidationCode

Код результата проверки transition.

Поддерживаются значения:

- `None`;
- `InvalidTransition`;
- `MissingActor`;
- `AcknowledgementNotRequired`;
- `AlarmAlreadyClosed`.

### AlarmTransitionRequest

Запрос на transition.

Поля:

- `type`;
- `actor`;
- `reason`;
- `timestamp`;
- `require_actor`.

Методы:

- `has_actor()`;
- `has_reason()`;
- `has_timestamp()`.

### AlarmTransitionRecord

Запись о выполненном transition.

Поля:

- `type`;
- `previous_state`;
- `new_state`;
- `timestamp`;
- `actor`;
- `reason`.

Методы:

- `has_actor()`;
- `has_reason()`;
- `has_timestamp()`.

На текущем этапе transition record возвращается в результате, но еще не сохраняется в БД.

### AlarmTransitionResult

Результат transition.

Поля:

- `success`;
- `code`;
- `alarm`;
- `transition`;
- `message`.

Методы:

- `is_success()`;
- `has_transition()`;
- `has_message()`.

## Поддержанные переходы

### Activate

Разрешено:

    New -> Active

### Acknowledge

Разрешено:

    New -> Acknowledged
    Active -> Acknowledged

Если `requires_acknowledgement = false`, transition отклоняется.

### Clear

Разрешено:

    New -> Cleared
    Active -> Cleared
    Acknowledged -> Cleared

### Close

Разрешено:

    Cleared -> Closed

### Shelve

Разрешено:

    New -> Shelved
    Active -> Shelved
    Acknowledged -> Shelved

### Unshelve

Разрешено:

    Shelved -> Active

### Suppress

Разрешено:

    New -> Suppressed
    Active -> Suppressed
    Acknowledged -> Suppressed

### Unsuppress

Разрешено:

    Suppressed -> Active

### Reactivate

Разрешено:

    Cleared -> Active

При reactivation увеличивается `occurrence_count`.

## Почему lifecycle отделен от AlarmRecord

`AlarmRecord` хранит состояние.

Lifecycle logic управляет переходами между состояниями.

Это разделение нужно, чтобы:

- не перегружать модель данными и правилами;
- отдельно тестировать transition rules;
- позже добавить Alarm Manager;
- позже добавить transition history repository;
- позже добавить audit и Event Manager integration.

## Почему transition применяется к копии

`apply_alarm_transition()` принимает `const AlarmRecord&` и возвращает обновленную копию в `AlarmTransitionResult`.

Это безопаснее на раннем этапе.

Будущий Alarm Manager сможет:

- загрузить аварийную запись;
- применить transition;
- сохранить обновленную запись;
- сохранить transition history;
- создать EventRecord;
- опубликовать событие.

## Почему actor пока строка

Actor может быть:

- пользователем;
- сервисным аккаунтом;
- системой;
- интеграцией;
- скриптом.

Пока auth/user model не реализована, actor хранится строкой.

Позже он будет связан с пользователями, ролями и аудитом.

## Почему close разрешен только после clear

Закрытие означает, что аварийное состояние завершено.

Поэтому базовое правило:

    сначала clear, потом close

Более сложные политики можно будет добавить позже через `AlarmLifecyclePolicy`.

## Почему shelve и suppress не закрывают аварию

Shelve и suppress — это временные режимы отображения или обработки.

Они не означают, что аварийное условие исчезло.

Поэтому после `Unshelve` или `Unsuppress` авария возвращается в `Active`.

## Почему пока нет Alarm Manager

Сейчас мы создаем только доменную модель lifecycle.

Alarm Manager будет отвечать за:

- создание аварий;
- поиск активной аварии;
- lifecycle transitions;
- квитирование;
- закрытие;
- suppression;
- shelving;
- связь с Event Manager;
- хранение в БД;
- доставку в UI.

Это будет развиваться постепенно.

## Зависимости

`scada_alarms` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_events`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`;
- нужна связь с `EventId`;
- нужно сопоставление `AlarmSeverity` с `EventSeverity`.

На текущем шаге `scada_alarms` не зависит от:

- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_runtime`;
- `scada_historian`.

Это сделано намеренно, чтобы alarms layer оставался универсальным.

## Основные файлы

- `include/scada_alarms/alarm_ids.h`
- `include/scada_alarms/alarm_record.h`
- `include/scada_alarms/alarm_lifecycle.h`
- `include/scada_alarms/alarm_module.h`
- `src/alarm_record.cpp`
- `src/alarm_lifecycle.cpp`
- `src/alarm_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_alarms`.
- Добавлен `AlarmId`.
- Добавлен `AlarmTimestamp`.
- Добавлен `AlarmSourceType`.
- Добавлен `AlarmSeverity`.
- Добавлен `AlarmPriority`.
- Добавлен `AlarmState`.
- Добавлен `AlarmRecord`.
- Добавлена функция `make_alarm_record()`.
- Добавлена функция `to_event_severity()`.
- Добавлены функции `to_string()` для enum.
- Добавлен alarm lifecycle foundation.
- Добавлены alarm transitions.
- Добавлен `get_alarm_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Alarms.
- Готовим базовую модель аварии.
- Готовим lifecycle transitions.
- Готовим основу для Alarm Manager.
- Готовим связь аварий с Events.

### Нужно доделать

- Добавить alarm rules foundation.
- Добавить event/alarm DTO.
- Добавить repository-интерфейсы.
- Добавить миграции.

### Расширить позже

- Alarm Manager.
- Event Manager integration.
- Alarm storage.
- Active alarm index.
- Alarm shelving policy.
- Alarm suppression policy.
- Alarm escalation.
- Alarm notifications.
- Alarm API.
- WebSocket delivery.
- UI alarm journal.
- UI active alarm panel.

### Не входит в этот шаг

- Alarm rules.
- EventBus.
- Alarm storage.
- Alarm API.
- WebSocket.
- Frontend.# scada_alarms

## Назначение

`scada_alarms` — backend-модуль аварий Dispatcher.

Модуль отвечает за базовую модель аварий и будущий Alarm Manager.

На текущем этапе реализованы:

- базовая модель аварии;
- базовые состояния аварии;
- lifecycle transition foundation;
- foundation alarm rules.

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

- хранение alarm rules в БД;
- хранение transition history в БД;
- хранение аварий в БД;
- поиск существующих активных аварий;
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

На текущем этапе alarm id может быть пустым до момента сохранения.

Будущая repository-реализация сможет назначать id при записи.

### AlarmRuleId

Типизированный идентификатор alarm rule.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

На текущем этапе rule id может быть пустым до момента сохранения.

Будущая repository-реализация сможет назначать id при записи.

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

Severity отвечает на вопрос:

    насколько серьезна авария?

### AlarmPriority

Приоритет аварии.

Поддерживаются значения:

- `Low`;
- `Medium`;
- `High`;
- `Critical`.

Priority отвечает на вопрос:

    насколько срочно оператор должен отреагировать?

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

Методы:

- `has_alarm_id()`;
- `has_event_id()`;
- `has_code()`;
- `has_title()`;
- `has_message()`;
- `has_source_id()`;
- `has_object_id()`;
- `has_device_id()`;
- `has_tag_id()`;
- `has_correlation_id()`;
- `has_acknowledged_by()`;
- `has_closed_by()`;
- `is_active()`;
- `is_closed()`;
- `has_valid_identity()`.

### make_alarm_record

Функция создает базовую аварийную запись.

Принимает:

- source type;
- severity;
- priority;
- code;
- title;
- message.

Автоматически выставляет:

- `state = New`;
- `created_at`;
- `activated_at`;
- `occurrence_count = 1`.

### to_event_severity

Функция преобразует `AlarmSeverity` в `EventSeverity`.

Это нужно для будущей связи:

    AlarmRecord -> EventRecord

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

### AlarmRuleType

Тип правила аварии.

Поддерживаются значения:

- `NumericThreshold`;
- `QualityEquals`;
- `QualityNotGood`;
- `QualityBad`.

### AlarmRuleState

Состояние правила.

Поддерживаются значения:

- `Draft`;
- `Enabled`;
- `Disabled`.

Только `Enabled` rule участвует в evaluation.

### AlarmRuleComparison

Сравнение для numeric threshold.

Поддерживаются значения:

- `GreaterThan`;
- `GreaterOrEqual`;
- `LessThan`;
- `LessOrEqual`;
- `Equal`;
- `NotEqual`.

### AlarmRuleEvaluationStatus

Статус evaluation.

Поддерживаются значения:

- `NotEvaluated`;
- `Matched`;
- `NotMatched`;
- `DisabledRule`;
- `InvalidRule`;
- `InvalidValue`.

### AlarmRule

Базовая модель alarm rule.

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

Методы:

- `has_rule_id()`;
- `has_code()`;
- `has_title()`;
- `has_message()`;
- `has_tag_id()`;
- `has_object_id()`;
- `has_device_id()`;
- `is_enabled()`;
- `has_expected_quality()`;
- `has_valid_identity()`.

### AlarmRuleEvaluationResult

Результат проверки alarm rule.

Поля:

- `status`;
- `rule`;
- `alarm`;
- `evaluated`;
- `matched`;
- `message`.

Методы:

- `is_evaluated()`;
- `is_matched()`;
- `has_alarm()`;
- `has_message()`.

### evaluate_alarm_rule

Функция проверяет `AlarmRule` по текущему `TagCurrentValue`.

Поддерживает:

- numeric threshold по `engineering_value`;
- comparison по threshold;
- quality equals;
- quality not good;
- quality bad.

Если rule сработал, создается `AlarmRecord`.

## Почему rules foundation зависит от scada_tags

Alarm rules должны уметь проверять текущее значение тега.

Для этого нужен:

- `TagCurrentValue`;
- `TagValuePayload`;
- `TagQuality`.

Поэтому с этого шага `scada_alarms` зависит от `scada_tags`.

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

Этого пока нет.

Дедупликация будет добавлена позже в Alarm Manager.

## Почему threshold использует engineering_value

Операторские аварии должны строиться по инженерному значению, а не по raw value.

Пример:

    raw_value = 12400
    engineering_value = 12.4 A

Правило аварии должно сравнивать именно `12.4 A`.

## Почему есть require_good_quality

Иногда аварийное правило должно срабатывать только при хорошем качестве значения.

Например:

    Temperature > 80

не должно срабатывать, если значение имеет качество `Timeout`.

Для таких случаев есть `require_good_quality`.

Отдельные аварии качества задаются через:

- `QualityEquals`;
- `QualityNotGood`;
- `QualityBad`.

## Почему source ids пока строки

AlarmRecord и AlarmRule должны уметь ссылаться на разные сущности:

- объект;
- устройство;
- тег;
- скрипт;
- интеграцию;
- внешний источник.

Чтобы не связывать `scada_alarms` со всеми доменными модулями сразу, source ids пока представлены строками.

Позже mapper/repository/API слой сможет валидировать и связывать эти идентификаторы с конкретными доменными сущностями.

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

На текущем шаге `scada_alarms` не зависит от:

- `scada_objects`;
- `scada_devices`;
- `scada_runtime`;
- `scada_historian`.

## Основные файлы

- `include/scada_alarms/alarm_ids.h`
- `include/scada_alarms/alarm_record.h`
- `include/scada_alarms/alarm_lifecycle.h`
- `include/scada_alarms/alarm_rule.h`
- `include/scada_alarms/alarm_module.h`
- `src/alarm_record.cpp`
- `src/alarm_lifecycle.cpp`
- `src/alarm_rule.cpp`
- `src/alarm_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_alarms`.
- Добавлен `AlarmId`.
- Добавлен `AlarmRuleId`.
- Добавлен `AlarmTimestamp`.
- Добавлен `AlarmSourceType`.
- Добавлен `AlarmSeverity`.
- Добавлен `AlarmPriority`.
- Добавлен `AlarmState`.
- Добавлен `AlarmRecord`.
- Добавлена функция `make_alarm_record()`.
- Добавлена функция `to_event_severity()`.
- Добавлены функции `to_string()` для enum.
- Добавлен alarm lifecycle foundation.
- Добавлены alarm transitions.
- Добавлен alarm rules foundation.
- Добавлена функция `evaluate_alarm_rule()`.
- Добавлен `get_alarm_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Alarms.
- Готовим базовую модель аварии.
- Готовим lifecycle transitions.
- Готовим alarm rules foundation.
- Готовим основу для Alarm Manager.
- Готовим связь аварий с Events.

### Нужно доделать

- Добавить event/alarm DTO.
- Добавить repository-интерфейсы.
- Добавить миграции.

### Расширить позже

- Alarm Manager.
- Event Manager integration.
- Alarm storage.
- Active alarm index.
- Alarm deduplication.
- Alarm shelving policy.
- Alarm suppression policy.
- Alarm escalation.
- Alarm notifications.
- Alarm API.
- WebSocket delivery.
- UI alarm journal.
- UI active alarm panel.

### Не входит в этот шаг

- Alarm Manager.
- EventBus.
- Alarm storage.
- Alarm API.
- WebSocket.
- Frontend.