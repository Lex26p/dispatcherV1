# scada_alarms

## Назначение

`scada_alarms` — backend-модуль аварий Dispatcher.

Модуль отвечает за базовую модель аварий и будущий Alarm Manager.

На текущем шаге реализована только фундаментальная модель аварии.

Полный lifecycle аварий и transitions будут добавлены следующим шагом.

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

## Границы ответственности

`scada_alarms` отвечает за:

- `AlarmId`;
- `AlarmTimestamp`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- создание базовой аварии;
- связь severity аварии с severity события;
- подготовку к alarm lifecycle;
- подготовку к alarm transitions;
- подготовку к alarm rules;
- подготовку к future Alarm Manager;
- подготовку к future Event Manager integration.

## Что модуль не должен делать сейчас

На текущем шаге `scada_alarms` не должен выполнять:

- вычисление alarm rules;
- lifecycle transitions;
- квитирование;
- нормализацию;
- закрытие аварий;
- хранение аварий в БД;
- публикацию в EventBus;
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

Например:

- авария может быть технически `Critical`, но временно иметь priority `Medium`;
- предупреждение может иметь priority `High`, если связано с важным объектом.

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

На текущем шаге state только хранится.

Правила переходов будут добавлены следующим шагом.

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

## Почему аварии отделены от событий

Событие — это любой факт, который произошел в системе.

Авария — это особый тип события с состоянием и жизненным циклом.

Событие может быть одноразовым.

Авария может жить во времени.

Пример события:

    Пользователь вошел в систему.

Пример аварии:

    Температура выше порога и требует реакции оператора.

Авария требует:

- activation;
- acknowledgement;
- clearing;
- closing;
- shelving;
- suppression;
- transition history.

Поэтому аварии вынесены в отдельный модуль.

## Почему AlarmState добавлен сейчас, а transitions позже

State нужен уже в базовой модели аварии.

Но правила переходов требуют отдельной модели:

- transition type;
- actor;
- timestamp;
- reason;
- previous state;
- new state;
- validation.

Это будет добавлено следующим шагом.

## Почему source ids пока строки

AlarmRecord должен уметь ссылаться на разные сущности:

- объект;
- устройство;
- тег;
- скрипт;
- интеграцию;
- внешний источник.

Чтобы не связывать `scada_alarms` со всеми доменными модулями сразу, source ids пока представлены строками.

Позже mapper/repository/API слой сможет валидировать и связывать эти идентификаторы с конкретными доменными сущностями.

## Почему `event_id` есть в AlarmRecord

Авария должна быть связана с событийным журналом.

В будущем при создании аварии можно будет создавать связанный `EventRecord`.

`event_id` хранит эту связь.

На текущем шаге event еще не создается автоматически.

## Почему severity и priority разделены

Severity описывает серьезность технического состояния.

Priority описывает срочность реакции.

Это разные понятия.

Пример:

    Severity = Critical
    Priority = Medium

Такое возможно, если объект в резерве или находится в maintenance.

Пример:

    Severity = Warning
    Priority = High

Такое возможно, если предупреждение относится к критически важному объекту.

## Почему пока нет Alarm Manager

Сейчас мы создаем только доменную модель аварии.

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
- `include/scada_alarms/alarm_module.h`
- `src/alarm_record.cpp`
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
- Добавлен `get_alarm_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Alarms.
- Готовим базовую модель аварии.
- Готовим основу для Alarm Manager.
- Готовим связь аварий с Events.

### Нужно доделать

- Добавить alarm lifecycle transitions.
- Добавить alarm transition history.
- Добавить alarm rules foundation.
- Добавить event/alarm DTO.
- Добавить repository-интерфейсы.
- Добавить миграции.

### Расширить позже

- Alarm Manager.
- Event Manager integration.
- Alarm storage.
- Active alarm index.
- Alarm shelving.
- Alarm suppression.
- Alarm escalation.
- Alarm notifications.
- Alarm API.
- WebSocket delivery.
- UI alarm journal.
- UI active alarm panel.

### Не входит в этот шаг

- Lifecycle transitions.
- Alarm rules.
- EventBus.
- Alarm storage.
- Alarm API.
- WebSocket.
- Frontend.