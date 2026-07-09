# scada_polling

## Назначение

`scada_polling` — backend-модуль модели и будущего выполнения опроса Dispatcher.

Модуль отвечает за описание polling task, polling group и основу polling scheduler.

На текущем шаге реализована минимальная синхронная основа scheduler.

Реальное фоновое выполнение опроса будет добавлено позже.

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
- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`;
- базовую модель задачи опроса;
- базовую модель группы опроса;
- связь polling task с `DeviceId`;
- связь polling task с `TagId`;
- связь polling task с `TagAddress`;
- минимальное синхронное выполнение polling group;
- выбор драйвера через `ProtocolDriverRegistry`;
- подготовку к будущему polling scheduler.

## Что модуль не должен делать сейчас

На текущем шаге `scada_polling` не должен выполнять:

- бесконечный polling loop;
- фоновые потоки;
- запуск таймеров;
- Boost.Asio scheduling;
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

Scheduler пока не меняет состояние задач в хранилище.

### PollingGroupMode

Режим выполнения группы опроса.

Поддерживаются значения:

- `Sequential`;
- `Batch`.

`Sequential` означает, что задачи группы могут выполняться последовательно.

`Batch` означает, что scheduler может попытаться объединить задачи в batch read.

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

### PollingExecutionStatus

Статус выполнения polling group.

Поддерживаются значения:

- `NotStarted`;
- `Skipped`;
- `Succeeded`;
- `Failed`;
- `PartialSuccess`.

### PollingTaskExecutionResult

Результат выполнения одной polling task.

Поля:

- `task_id`;
- `tag_id`;
- `resulting_state`;
- `quality`;
- `success`;
- `latency_ms`;
- `message`.

### PollingGroupExecutionResult

Результат выполнения polling group.

Поля:

- `group_id`;
- `status`;
- `task_results`;
- `message`.

Методы:

- `is_success()`;
- `has_failures()`;
- `has_message()`;
- `task_count()`;
- `success_count()`;
- `failure_count()`.

### PollingSchedulerOptions

Настройки одного запуска scheduler.

Поля:

- `allow_batch_read`;
- `check_connection_before_polling`;
- `correlation_id_prefix`.

На текущем этапе настройки передаются в один синхронный запуск.

### PollingScheduler

Минимальная основа scheduler.

Метод:

- `execute_group_once()`.

Метод выполняет одну polling group один раз.

Логика:

- проверяет, включена ли группа;
- проверяет готовность группы к polling;
- проверяет совпадение протокола группы и подключения;
- получает драйвер из `ProtocolDriverRegistry`;
- опционально проверяет соединение;
- выбирает enabled tasks;
- если возможно, выполняет batch read;
- иначе выполняет sequential read;
- возвращает `PollingGroupExecutionResult`.

Важно: `PollingScheduler` пока не пишет значения в `TagValueStore`.

Он только возвращает результат выполнения.

## Почему scheduler пока синхронный

На текущем этапе важно закрепить архитектурную границу:

    PollingGroup -> ProtocolDriverRegistry -> IProtocolDriver -> ProtocolReadResult

Фоновые потоки, таймеры и async I/O будут добавлены позже.

Так проще проверить модель без сложной инфраструктуры.

## Почему scheduler не пишет TagCurrentValue

Запись текущих значений должна быть в будущем runtime values layer.

Polling не должен напрямую владеть всей логикой:

- last good value;
- change detection;
- deadband;
- EventBus;
- Historian;
- Alarm evaluation.

Поэтому текущий шаг возвращает результат, а не пишет его в runtime storage.

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

## Batch execution

`PollingScheduler` может использовать batch read, если:

- включен `allow_batch_read`;
- группа находится в `Batch` mode;
- группа может использовать batch mode;
- драйвер поддерживает batch read;
- размер группы не превышает `group.max_batch_size`;
- размер группы не превышает `driver.capabilities().max_batch_size`.

Иначе scheduler выполняет sequential read.

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
- нужен `DeviceConnection`;
- нужен `TagId`;
- нужен `TagAddress`;
- нужен `IProtocolDriver`;
- нужен `ProtocolDriverRegistry`;
- нужны request/result протокольного слоя.

## Основные файлы

- `include/scada_polling/polling_ids.h`
- `include/scada_polling/polling_task_state.h`
- `include/scada_polling/polling_group_mode.h`
- `include/scada_polling/polling_task.h`
- `include/scada_polling/polling_group.h`
- `include/scada_polling/polling_execution.h`
- `include/scada_polling/polling_scheduler.h`
- `include/scada_polling/polling_module.h`
- `src/polling_task_state.cpp`
- `src/polling_group_mode.cpp`
- `src/polling_task.cpp`
- `src/polling_group.cpp`
- `src/polling_execution.cpp`
- `src/polling_scheduler.cpp`
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
- Добавлен `PollingExecutionStatus`.
- Добавлен `PollingTaskExecutionResult`.
- Добавлен `PollingGroupExecutionResult`.
- Добавлен `PollingSchedulerOptions`.
- Добавлен `PollingScheduler`.
- Добавлен один синхронный запуск polling group.
- Добавлен batch/sequential выбор выполнения.
- Добавлен `get_polling_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем модель задач и групп опроса.
- Готовим основу для polling scheduler.
- Проверяем связку polling -> protocol registry -> driver.

### Нужно доделать

- Добавить conversion из Tag configuration в PollingTask.
- Добавить execution result mapping в future TagCurrentValue.
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

- Бесконечный polling loop.
- Фоновые потоки.
- Таймеры.
- Runtime values.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.