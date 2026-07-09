# scada_runtime

## Назначение

`scada_runtime` — backend-модуль runtime-слоя текущих значений Dispatcher.

Модуль отвечает за хранение и обработку актуальных значений тегов во время работы системы.

Runtime-слой находится между polling и следующими подсистемами:

- EventBus;
- Historian;
- Alarm Engine;
- WebSocket Gateway;
- API;
- UI.

На текущем этапе реализовано in-memory хранилище текущих значений тегов, применение результата чтения протокольного драйвера, обработка last good value, value conversion, engineering transform, change detection, базовая модель runtime event и repository-интерфейсы runtime snapshot/event.

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
- `RuntimeValueChangeKind`;
- `RuntimeValueChangeResult`;
- `RuntimeEventType`;
- `RuntimeValueEvent`;
- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`;
- runtime quality helpers;
- runtime value conversion;
- engineering transform;
- runtime change detection;
- runtime event foundation;
- repository-интерфейсы runtime snapshots;
- repository-интерфейсы runtime events;
- хранение текущих значений тегов в памяти;
- поиск текущего значения по `TagId`;
- получение всех текущих значений;
- обновление текущего значения;
- удаление текущего значения;
- применение `ProtocolReadResult` к `TagCurrentValue`;
- преобразование raw value к целевому типу тега;
- применение `scale` и `offset`;
- обновление `last_good_value`;
- обновление `last_good_timestamp`;
- подготовку к future EventBus integration;
- подготовку к future Historian integration;
- подготовку к future Alarm evaluation.

## Что модуль не должен делать сейчас

На текущем шаге `scada_runtime` не должен выполнять:

- polling scheduling;
- сетевой обмен;
- прямой вызов protocol drivers;
- SQL-запросы;
- PostgreSQL-реализацию repository;
- отправку событий в EventBus;
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

### Runtime quality helpers

Runtime quality helpers определяют базовые правила качества для runtime-слоя.

Хорошими quality считаются:

- `Good`;
- `Manual`;
- `Simulation`.

Эти качества могут обновлять `last_good_value`.

Остальные качества считаются недостаточными для обновления `last_good_value`.

### Runtime value conversion

Runtime value conversion отвечает за преобразование значения к целевому `TagValueType`.

Поддерживаются преобразования:

- numeric -> numeric;
- numeric -> boolean;
- boolean -> numeric;
- string -> numeric, если строка содержит число;
- string -> boolean, если строка содержит true/false/1/0/yes/no/on/off;
- any simple value -> string;
- any simple value -> enum string;
- any simple value -> json string на текущем этапе.

Если преобразование невозможно, runtime сохраняет исходное значение.

### Engineering transform

Engineering transform применяет формулу:

    engineering_value = raw_value * scale + offset

На текущем этапе transform применяется только к значениям, которые можно представить как число.

Для `String`, `Enum`, `Json` и пустых значений transform пропускается.

Если `scale` или `offset` некорректны, transform не применяется.

### RuntimeValueChangeKind

Тип изменения runtime value.

Поддерживаются значения:

- `None`;
- `Initialized`;
- `ValueChanged`;
- `QualityChanged`;
- `ValueAndQualityChanged`;
- `SourceChanged`.

### RuntimeValueChangeResult

Результат определения изменения.

Поля:

- `kind`;
- `changed`;
- `value_changed`;
- `quality_changed`;
- `source_changed`;
- `message`.

Методы:

- `has_changed()`;
- `has_value_changed()`;
- `has_quality_changed()`;
- `has_source_changed()`;
- `has_message()`.

### RuntimeEventType

Тип runtime-события.

Поддерживаются значения:

- `ValueInitialized`;
- `ValueChanged`;
- `QualityChanged`;
- `ValueAndQualityChanged`;
- `SourceChanged`.

### RuntimeValueEvent

Базовая модель runtime-события.

Поля:

- `tag_id`;
- `type`;
- `change_kind`;
- `previous_quality`;
- `current_quality`;
- `timestamp`;
- `change_counter`;
- `message`.

На текущем этапе событие только формируется и возвращается в `RuntimeValueApplyResult`.

Оно еще не публикуется в EventBus.

### RuntimeValueApplyResult

Результат применения значения к runtime store.

Поля:

- `tag_id`;
- `success`;
- `inserted`;
- `updated`;
- `changed`;
- `good_quality`;
- `last_good_updated`;
- `value_converted`;
- `engineering_transform_applied`;
- `change_kind`;
- `event`;
- `previous_change_counter`;
- `new_change_counter`;
- `quality`;
- `message`.

Методы:

- `is_success()`;
- `was_inserted()`;
- `was_updated()`;
- `has_changed()`;
- `has_good_quality()`;
- `was_last_good_updated()`;
- `was_value_converted()`;
- `was_engineering_transform_applied()`;
- `has_event()`;
- `has_message()`.

### RuntimeValueApplier

`RuntimeValueApplier` применяет результат чтения протокольного драйвера к `TagValueStore`.

Методы:

- `apply_protocol_read_result(read_result)`;
- `apply_protocol_read_result(read_result, tag)`.

Метод без `Tag` сохраняет совместимость:

    raw_value -> engineering_value

Метод с `Tag` использует конфигурацию тега:

- `Tag.value_type`;
- `Tag.scale`;
- `Tag.offset`.

Метод выполняет:

- проверку `tag_id`;
- проверку соответствия `read_result.tag_id` и `tag.id`;
- поиск предыдущего значения в `TagValueStore`;
- преобразование raw value к `Tag.value_type`;
- применение engineering transform;
- перенос `quality`;
- перенос `source_timestamp`;
- перенос `server_timestamp`;
- установку source в `Device`;
- обновление `last_good_value`, если quality хорошее;
- обновление `last_good_timestamp`, если quality хорошее;
- определение изменения value / quality / source;
- увеличение `change_counter` только при изменении;
- создание `RuntimeValueEvent`, если изменение есть;
- запись значения в `TagValueStore`.

## Repository-интерфейсы

На текущем этапе добавлены repository-интерфейсы runtime.

Они не содержат SQL и не знают о конкретном хранилище.

### IRuntimeValueSnapshotRepository

Интерфейс хранения snapshot текущих runtime-значений.

Отвечает за:

- получение всех runtime snapshots;
- поиск snapshot по `TagId`;
- сохранение snapshot;
- удаление snapshot по `TagId`;
- очистку snapshots.

### IRuntimeEventRepository

Интерфейс хранения runtime value events.

Отвечает за:

- получение всех runtime events;
- получение events по `TagId`;
- получение последних events;
- сохранение runtime event;
- очистку events.

Будущие реализации могут быть разными:

- in-memory repository для тестов;
- PostgreSQL repository для основной системы;
- mock repository для unit-тестов.

## База данных

На текущем этапе добавлена черновая миграция:

- `database/migrations/0005_runtime_values.sql`.

Она описывает будущие таблицы:

- `runtime_value_snapshots`;
- `runtime_value_events`.

Миграция не выполняется приложением автоматически.

## Runtime snapshots не являются Historian

`runtime_value_snapshots` хранит последнее известное runtime-состояние тега.

Это не временной ряд.

История тегов будет отдельной подсистемой Historian.

## Runtime events не являются полным Event Manager

`runtime_value_events` хранит базовые события изменения runtime value.

Это не заменяет будущий Event Manager.

Полный Event Manager будет отвечать за:

- lifecycle events;
- alarms;
- acknowledgements;
- operator actions;
- audit trail;
- system events;
- integrations.

## Как работает change detection

Change detection сравнивает:

- предыдущее `engineering_value`;
- новое `engineering_value`;
- предыдущее `quality`;
- новое `quality`;
- предыдущий `source`;
- новый `source`.

Если значение, качество и источник не изменились, `change_counter` не увеличивается.

Временные метки сами по себе не считаются изменением значения.

## Почему timestamp-only update не считается изменением

Если устройство возвращает то же значение с тем же качеством, но с новой временной меткой, это не всегда означает изменение процесса.

Поэтому на текущем этапе runtime event не создается только из-за изменения timestamp.

Позже можно будет добавить отдельный тип события для heartbeat/sample update.

## Правила last good value

`last_good_value` обновляется только если:

- quality является good runtime quality;
- значение не пустое.

Good runtime quality:

- `Good`;
- `Manual`;
- `Simulation`.

Если приходит плохое качество, например:

- `CommunicationError`;
- `Timeout`;
- `DeviceError`;
- `ProtocolError`;
- `Bad`;
- `Stale`;
- `Disabled`;

то текущее значение и quality обновляются, но `last_good_value` сохраняется прежним.

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
- нужен `Tag`;
- нужен `TagId`;
- нужен `TagCurrentValue`;
- нужен `TagValuePayload`;
- нужен `TagQuality`;
- нужен `ProtocolReadResult`.

## Основные файлы

- `include/scada_runtime/tag_value_store.h`
- `include/scada_runtime/runtime_quality.h`
- `include/scada_runtime/runtime_value_conversion.h`
- `include/scada_runtime/engineering_transform.h`
- `include/scada_runtime/runtime_value_change.h`
- `include/scada_runtime/runtime_event.h`
- `include/scada_runtime/runtime_value_applier.h`
- `include/scada_runtime/runtime_module.h`
- `include/scada_runtime/repository/IRuntimeValueSnapshotRepository.h`
- `include/scada_runtime/repository/IRuntimeEventRepository.h`
- `src/tag_value_store.cpp`
- `src/runtime_quality.cpp`
- `src/runtime_value_conversion.cpp`
- `src/engineering_transform.cpp`
- `src/runtime_value_change.cpp`
- `src/runtime_event.cpp`
- `src/runtime_value_applier.cpp`
- `src/runtime_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_runtime`.
- Добавлен `TagValueStore`.
- Добавлены runtime quality helpers.
- Добавлен runtime value conversion.
- Добавлен engineering transform.
- Добавлен runtime change detection.
- Добавлен runtime event foundation.
- Добавлен `RuntimeValueApplyResult`.
- Добавлен `RuntimeValueApplier`.
- Добавлены repository-интерфейсы runtime snapshots.
- Добавлены repository-интерфейсы runtime events.
- Добавлено применение `ProtocolReadResult` к `TagCurrentValue`.
- Добавлено преобразование raw value к `Tag.value_type`.
- Добавлено применение `Tag.scale` и `Tag.offset`.
- Добавлено обновление `last_good_value`.
- Добавлено обновление `last_good_timestamp`.
- Добавлено увеличение `change_counter` только при изменении.
- Добавлено создание `RuntimeValueEvent`.
- Добавлен `get_runtime_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем runtime-слой текущих значений.
- Готовим основу для применения результатов polling.
- Готовим основу для runtime value processing.
- Готовим runtime к будущему хранению snapshots и events.

### Нужно доделать

- Добавить документацию закрытия Sprint 006.
- Добавить интеграцию polling -> runtime в следующем крупном этапе.
- Добавить PostgreSQL repository implementations позже.
- Добавить EventBus integration позже.

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
- Deadband.
- Archive policy evaluation.

### Не входит в этот шаг

- Polling integration.
- EventBus publish.
- Deadband.
- Historian.
- Alarm engine.
- PostgreSQL implementation.
- HTTP API.
- Frontend.