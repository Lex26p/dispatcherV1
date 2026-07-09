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

На текущем шаге реализовано только in-memory хранилище текущих значений тегов.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Runtime-слой находится здесь:

    Polling -> ProtocolReadResult -> Runtime -> TagCurrentValue -> future EventBus / Historian / Alarms

## Границы ответственности

`scada_runtime` отвечает за:

- `TagValueStore`;
- хранение текущих значений тегов в памяти;
- поиск текущего значения по `TagId`;
- получение всех текущих значений;
- обновление текущего значения;
- удаление текущего значения;
- подготовку к будущему применению protocol read results;
- подготовку к change detection;
- подготовку к last good value handling;
- подготовку к engineering transform;
- подготовку к runtime events.

## Что модуль не должен делать сейчас

На текущем шаге `scada_runtime` не должен выполнять:

- polling;
- сетевой обмен;
- protocol driver calls;
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

## Почему пока нет change detection

Change detection должен учитывать:

- предыдущее значение;
- новое значение;
- качество;
- deadband;
- archive policy;
- event policy;
- alarm rules.

Это будет добавлено отдельным шагом.

На текущем шаге `upsert()` просто записывает новое текущее значение.

## Почему пока нет last good value update

`TagCurrentValue` уже содержит:

- `last_good_value`;
- `last_good_timestamp`.

Но автоматическое обновление `last_good_value` пока не выполняется.

Это будет добавлено в runtime data engine позже.

## Почему пока нет engineering transform

`Tag` уже содержит:

- `scale`;
- `offset`;
- `engineering_unit`.

Но `TagValueStore` не должен сам знать конфигурацию тега.

Engineering transform будет выполнен отдельным runtime/service слоем, который получит:

- конфигурацию тега;
- raw value;
- правила преобразования.

## Зависимости

`scada_runtime` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_tags`.

Причины:

- нужны общие типы;
- нужен `ModuleInfo`;
- нужен `TagId`;
- нужен `TagCurrentValue`.

## Основные файлы

- `include/scada_runtime/tag_value_store.h`
- `include/scada_runtime/runtime_module.h`
- `src/tag_value_store.cpp`
- `src/runtime_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_runtime`.
- Добавлен `TagValueStore`.
- Добавлен `get_runtime_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем runtime-слой текущих значений.
- Готовим основу для применения результатов polling.
- Готовим основу для runtime value processing.

### Нужно доделать

- Добавить применение `ProtocolReadResult` к `TagCurrentValue`.
- Добавить quality handling.
- Добавить last good value update.
- Добавить change detection.
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
- Protocol result applying.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.