# scada_polling

## Назначение

`scada_polling` — backend-модуль модели и будущего выполнения опроса Dispatcher.

Модуль отвечает за описание polling task, polling group, repository-интерфейсы polling configuration и основу polling scheduler.

На текущем этапе реализована минимальная синхронная основа scheduler.

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
- `IPollingTaskRepository`;
- `IPollingGroupRepository`;
- базовую модель задачи опроса;
- базовую модель группы опроса;
- repository-интерфейсы polling configuration;
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
- SQL-запросы;
- PostgreSQL-реализацию;
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

## Repository-интерфейсы

На текущем этапе добавлены repository-интерфейсы polling configuration.

### IPollingTaskRepository

Интерфейс хранения polling task.

Отвечает за:

- получение всех polling task;
- поиск task по `PollingTaskId`;
- получение task по `PollingGroupId`;
- получение task по `DeviceId`;
- получение task по `TagId`;
- получение task по `PollingTaskState`;
- сохранение task;
- удаление task.

### IPollingGroupRepository

Интерфейс хранения polling group.

Отвечает за:

- получение всех polling group;
- поиск group по `PollingGroupId`;
- поиск group по `code`;
- получение group по `DeviceId`;
- получение group по `DeviceProtocol`;
- получение group по `PollingGroupMode`;
- сохранение group;
- удаление group.

Repository-интерфейсы не содержат SQL и не знают, где хранятся данные.

Будущие реализации могут быть разными:

- in-memory repository для тестов;
- PostgreSQL repository для основной системы;
- mock repository для unit-тестов;
- file-based repository для отладки.

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

## Почему polling task хранит TagAddress

Polling task должна иметь snapshot адреса тега для будущего опроса.

Это позволит scheduler не зависеть напрямую от UI или DTO.

В будущем task может формироваться из конфигурации тега.

## Batch execution

`PollingScheduler` может использовать batch read, если:

- включен `allow_batch_read`;
- группа находится в `Batch` mode;
- группа может использовать batch mode;
- драйвер поддерживает batch read;
- размер группы не превышает `group.max_batch_size`;
- размер группы не превышает `driver.capabilities().max_batch_size`.

Иначе scheduler выполняет sequential read.

## База данных

На текущем этапе добавлена черновая миграция:

- `database/migrations/0004_polling_model.sql`.

Она описывает будущие таблицы:

- `polling_groups`;
- `polling_tasks`.

Миграция не выполняется приложением автоматически.

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
- `include/scada_polling/repository/IPollingTaskRepository.h`
- `include/scada_polling/repository/IPollingGroupRepository.h`
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
- Добавлен `IPollingTaskRepository`.
- Добавлен `IPollingGroupRepository`.
- Добавлен `get_polling_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем модель задач и групп опроса.
- Готовим основу для polling scheduler.
- Готовим polling configuration к будущему хранению.
- Проверяем связку polling -> protocol registry -> driver.

### Нужно доделать

- Добавить conversion из Tag configuration в PollingTask.
- Добавить execution result mapping в future TagCurrentValue.
- Добавить runtime TagValueStore integration.
- Добавить API polling позже.

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
- PostgreSQL implementation.
- HTTP API.
- Frontend.
