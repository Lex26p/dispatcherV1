# scada_polling

## Назначение

`scada_polling` — backend-модуль модели и будущего выполнения опроса Dispatcher.

Модуль отвечает за описание polling task, polling group и будущую основу scheduler.

На текущем шаге реализована только модель задач и групп опроса.

Реальное выполнение опроса будет добавлено позже.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Polling находится между протокольным слоем и runtime values:

    PollingGroup -> PollingTask -> IProtocolDriver -> ProtocolReadResult -> future TagValueStore

## Границы ответственности

`scada_polling` отвечает за:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`;
- базовую модель задачи опроса;
- базовую модель группы опроса;
- связь polling task с `DeviceId`;
- связь polling task с `TagId`;
- связь polling task с `TagAddress`;
- подготовку к будущему polling scheduler.

## Что модуль не должен делать сейчас

На текущем шаге `scada_polling` не должен выполнять:

- реальный polling;
- сетевой обмен;
- создание потоков;
- запуск таймеров;
- запись текущих значений;
- запись истории;
- обработку аварий;
- HTTP API;
- UI-логику.

## Основные сущности

### PollingTaskId

Типизированный идентификатор задачи опроса.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### PollingGroupId

Типизированный идентификатор группы опроса.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### PollingTaskState

Состояние задачи опроса.

Поддерживаются значения:

- `Disabled`;
- `Waiting`;
- `Due`;
- `Running`;
- `Succeeded`;
- `Failed`;
- `Skipped`.

На текущем этапе это только модель состояния.

Scheduler пока не меняет эти состояния автоматически.

### PollingGroupMode

Режим выполнения группы опроса.

Поддерживаются значения:

- `Sequential`;
- `Batch`.

`Sequential` означает, что задачи группы могут выполняться последовательно.

`Batch` означает, что scheduler сможет попытаться объединить задачи в batch read.

Фактический batch optimizer будет добавлен позже.

### PollingTask

Задача опроса одного тега.

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

Методы:

- `has_valid_identity()`;
- `has_valid_timing()`;
- `has_address()`;
- `is_enabled_for_polling()`;
- `is_batch_compatible_with()`.

### PollingGroup

Группа задач опроса.

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

Методы:

- `has_valid_identity()`;
- `has_valid_timing()`;
- `has_tasks()`;
- `task_count()`;
- `enabled_task_count()`;
- `is_enabled_for_polling()`;
- `can_use_batch_mode()`.

## Почему polling group привязана к device

В большинстве SCADA-сценариев polling group читается с одного устройства.

Это нужно для:

- batch read;
- оптимизации Modbus TCP requests;
- оптимизации SNMP requests;
- диагностики связи;
- timeout/retry политики;
- группировки ошибок.

Позже можно будет добавить более сложные сценарии.

## Почему polling task хранит TagAddress

Polling task должна иметь snapshot адреса тега для будущего опроса.

Это позволит scheduler не зависеть напрямую от UI или DTO.

В будущем task может формироваться из конфигурации тега.

## Почему есть interval и в group, и в task

`PollingGroup::interval_ms` задает базовый интервал группы.

`PollingTask::interval_ms` позволяет в будущем поддержать индивидуальные интервалы тегов внутри группы.

На первом этапе scheduler может использовать interval группы.

Позже можно будет добавить оптимизацию по минимальному ближайшему времени опроса.

## Batch compatibility

`PollingTask::is_batch_compatible_with()` проверяет, можно ли задачи потенциально объединить.

На текущем этапе проверяется:

- одинаковое устройство;
- одинаковый протокол;
- одинаковый interval;
- одинаковый timeout;
- обе задачи включены.

Позже batch compatibility будет учитывать:

- contiguous Modbus registers;
- SNMP OID grouping;
- protocol capabilities;
- max batch size;
- driver-specific optimizer.

## Зависимости

`scada_polling` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`;
- нужен `DeviceId`;
- нужен `DeviceProtocol`;
- нужен `TagId`;
- нужен `TagAddress`;
- позже будет нужен `IProtocolDriver`.

## Основные файлы

- `include/scada_polling/polling_ids.h`
- `include/scada_polling/polling_task_state.h`
- `include/scada_polling/polling_group_mode.h`
- `include/scada_polling/polling_task.h`
- `include/scada_polling/polling_group.h`
- `include/scada_polling/polling_module.h`
- `src/polling_task_state.cpp`
- `src/polling_group_mode.cpp`
- `src/polling_task.cpp`
- `src/polling_group.cpp`
- `src/polling_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_polling`.
- Добавлен `PollingTaskId`.
- Добавлен `PollingGroupId`.
- Добавлен `PollingTaskState`.
- Добавлен `PollingGroupMode`.
- Добавлен `PollingTask`.
- Добавлен `PollingGroup`.
- Добавлен `get_polling_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем модель задач и групп опроса.
- Готовим основу для polling scheduler.

### Нужно доделать

- Добавить polling scheduler foundation.
- Добавить conversion из PollingTask в ProtocolReadRequest.
- Добавить batch request builder.
- Добавить execution result model.
- Добавить DTO polling.
- Добавить repository-интерфейсы polling.
- Добавить черновую миграцию polling configuration.

### Расширить позже

- Таймеры.
- Потоки.
- Async polling.
- Retry policy.
- Timeout policy.
- Backoff policy.
- Batch optimizer.
- Device diagnostics update.
- TagValueStore integration.
- EventBus integration.
- Historian integration.
- Alarm integration.

### Не входит в этот шаг

- Реальный polling.
- Scheduler.
- Protocol execution.
- Runtime values.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.