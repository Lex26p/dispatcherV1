# scada_runtime

## Назначение

`scada_runtime` — backend-модуль runtime-слоя текущих значений Dispatcher.

Модуль отвечает за хранение и будущую обработку актуальных значений тегов во время работы системы.

Runtime-слой находится между polling и следующими подсистемами:

- EventBus;
- Historian;
- Alarm Engine;
- WebSocket Gateway;
- API;
- UI.

На текущем этапе реализовано in-memory хранилище текущих значений тегов и базовое применение результата чтения протокольного драйвера.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Runtime-слой находится здесь:

    Polling -> ProtocolReadResult -> Runtime -> TagCurrentValue -> future EventBus / Historian / Alarms

## Границы ответственности

`scada_runtime` отвечает за:

- `TagValueStore`;
- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`;
- хранение текущих значений тегов в памяти;
- поиск текущего значения по `TagId`;
- получение всех текущих значений;
- обновление текущего значения;
- удаление текущего значения;
- применение `ProtocolReadResult` к `TagCurrentValue`;
- подготовку к future quality handling;
- подготовку к future last good value handling;
- подготовку к future change detection;
- подготовку к future engineering transform;
- подготовку к future runtime events.

## Что модуль не должен делать сейчас

На текущем шаге `scada_runtime` не должен выполнять:

- polling scheduling;
- сетевой обмен;
- прямой вызов protocol drivers;
- SQL-запросы;
- запись истории;
- обработку аварий;
- HTTP API;
- UI-логику.

## Основные сущности

### TagValueStore

`TagValueStore` — in-memory хранилище текущих значений тегов.

Хранит:

- `TagId`;
- `TagCurrentValue`.

Основные методы:

- `empty()`;
- `size()`;
- `contains()`;
- `find_by_tag_id()`;
- `get_all()`;
- `upsert()`;
- `remove_by_tag_id()`;
- `clear()`.

На текущем этапе store хранит значения в памяти процесса.

Он не пишет данные в PostgreSQL.

Он не является Historian.

Он не является permanent storage.

### RuntimeValueApplyResult

Результат применения значения к runtime store.

Поля:

- `tag_id`;
- `success`;
- `inserted`;
- `updated`;
- `previous_change_counter`;
- `new_change_counter`;
- `quality`;
- `message`.

Методы:

- `is_success()`;
- `was_inserted()`;
- `was_updated()`;
- `has_message()`.

### RuntimeValueApplier

`RuntimeValueApplier` применяет результат чтения протокольного драйвера к `TagValueStore`.

Основной метод:

- `apply_protocol_read_result()`.

Метод принимает:

- `ProtocolReadResult`.

Метод выполняет:

- проверку `tag_id`;
- поиск предыдущего значения в `TagValueStore`;
- создание нового `TagCurrentValue`;
- перенос `raw_value`;
- перенос `quality`;
- перенос `source_timestamp`;
- перенос `server_timestamp`;
- установку source в `Device`;
- временную установку `engineering_value = raw_value`;
- увеличение `change_counter`;
- запись значения в `TagValueStore`.

## Почему engineering_value пока равен raw_value

Engineering transform будет добавлен отдельным шагом.

Сейчас важно соединить цепочку:

    ProtocolReadResult -> TagCurrentValue -> TagValueStore

Преобразование через `scale`, `offset` и `engineering_unit` будет добавлено позже.

## Почему change_counter пока увеличивается всегда

На текущем шаге нет полноценного change detection.

Поэтому каждое применение `ProtocolReadResult` считается изменением runtime state.

Позже change detection будет учитывать:

- предыдущее значение;
- новое значение;
- качество;
- deadband;
- archive policy;
- alarm rules.

## Почему last_good_value пока не обновляется

`TagCurrentValue` уже содержит:

- `last_good_value`;
- `last_good_timestamp`.

Но автоматическое обновление `last_good_value` будет добавлено следующим отдельным шагом.

На текущем шаге previous `last_good_value` сохраняется, если он уже был в предыдущем `TagCurrentValue`.

## Почему RuntimeValueApplier отдельный от TagValueStore

`TagValueStore` отвечает только за хранение.

`RuntimeValueApplier` отвечает за применение результата чтения к runtime state.

Это разделение нужно, чтобы `TagValueStore` оставался простым storage-классом, а вся runtime-логика развивалась отдельно.

## Почему TagValueStore отдельный от polling

Polling отвечает за получение значений из источников.

Runtime отвечает за хранение и обработку текущего значения.

Это разделение важно, потому что значения могут приходить не только из polling.

Будущие источники значений:

- Polling Engine;
- ручной ввод;
- расчетные теги;
- системные теги;
- simulator;
- внешние интеграции;
- import;
- scripting.

## Почему TagValueStore не является repository

Repository отвечает за сохранение и загрузку данных из внешнего хранилища.

`TagValueStore` отвечает за runtime-состояние в памяти.

Это разные ответственности.

В будущем возможны оба слоя:

    TagValueStore -> current runtime state
    Repository -> persistent configuration / snapshots

## Почему TagValueStore не является Historian

Historian хранит временные ряды.

`TagValueStore` хранит только последнее текущее значение тега.

Пример:

    TagValueStore:
        Pump01.Current = 12.7 A

    Historian:
        10:00:00 Pump01.Current = 12.1 A
        10:00:01 Pump01.Current = 12.3 A
        10:00:02 Pump01.Current = 12.7 A

История будет отдельным модулем в будущем спринте.

## Почему пока нет потокобезопасности

На текущем этапе нет фонового polling loop и нет параллельной записи значений.

Поэтому `TagValueStore` пока реализован без mutex.

Когда появятся фоновые потоки, async polling или WebSocket consumers, нужно будет добавить потокобезопасность или отдельную модель владения данными.

## Зависимости

`scada_runtime` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_tags`;
- `scada_protocols`.

Причины:

- нужны общие типы;
- нужен `ModuleInfo`;
- нужен `TagId`;
- нужен `TagCurrentValue`;
- нужен `TagValuePayload`;
- нужен `ProtocolReadResult`.

## Основные файлы

- `include/scada_runtime/tag_value_store.h`
- `include/scada_runtime/runtime_value_applier.h`
- `include/scada_runtime/runtime_module.h`
- `src/tag_value_store.cpp`
- `src/runtime_value_applier.cpp`
- `src/runtime_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_runtime`.
- Добавлен `TagValueStore`.
- Добавлен `RuntimeValueApplyResult`.
- Добавлен `RuntimeValueApplier`.
- Добавлено применение `ProtocolReadResult` к `TagCurrentValue`.
- Добавлен `get_runtime_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем runtime-слой текущих значений.
- Готовим основу для применения результатов polling.
- Готовим основу для runtime value processing.

### Нужно доделать

- Добавить quality handling.
- Добавить last good value update.
- Добавить полноценный change detection.
- Добавить engineering transform.
- Добавить runtime event model.
- Добавить интеграцию polling -> runtime.

### Расширить позже

- Потокобезопасность.
- Snapshot persistence.
- EventBus integration.
- Historian integration.
- Alarm integration.
- WebSocket integration.
- API integration.
- Diagnostics.
- Metrics.

### Не входит в этот шаг

- Polling integration.
- Engineering transform.
- Last good value update.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.