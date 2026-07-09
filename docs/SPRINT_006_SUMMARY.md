# Sprint 006 Summary

## Название спринта

Runtime Values and Data Engine

## Статус

Closed

## Диапазон шагов

Шаги 43–49.

---

# Цель спринта

Создать runtime-слой текущих значений тегов.

Sprint 006 должен был заложить фундамент для цепочки:

    Polling -> ProtocolReadResult -> Runtime -> TagCurrentValue -> future EventBus / Historian / Alarms

---

# Выполненные шаги

## Шаг 43

Создан модуль `scada_runtime`.

Результат:

- создан каталог `backend/libs/scada_runtime`;
- добавлен `TagValueStore`;
- добавлен `get_runtime_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

`TagValueStore` умеет:

- хранить `TagCurrentValue` в памяти;
- искать значение по `TagId`;
- получать все текущие значения;
- проверять наличие значения;
- обновлять или добавлять значение;
- удалять значение;
- очищать store.

---

## Шаг 44

Добавлено применение `ProtocolReadResult` к `TagCurrentValue`.

Результат:

- добавлен `RuntimeValueApplyResult`;
- добавлен `RuntimeValueApplier`;
- добавлен первый runtime-processing слой;
- добавлено применение значения к `TagValueStore`.

Текущий flow:

    ProtocolReadResult -> TagCurrentValue -> TagValueStore

На этом шаге `engineering_value` временно был равен `raw_value`.

---

## Шаг 45

Добавлены runtime quality helpers и last good value handling.

Результат:

- добавлены runtime quality helpers;
- добавлено определение good runtime quality;
- добавлено обновление `last_good_value`;
- добавлено обновление `last_good_timestamp`;
- добавлен признак `good_quality`;
- добавлен признак `last_good_updated`.

Good runtime quality:

- `Good`;
- `Manual`;
- `Simulation`.

---

## Шаг 46

Добавлены value conversion и engineering transform.

Результат:

- добавлен `RuntimeValueConversionResult`;
- добавлен `convert_runtime_value()`;
- добавлен `infer_runtime_value_type()`;
- добавлен `runtime_value_to_double()`;
- добавлен `runtime_value_to_string()`;
- добавлен `RuntimeEngineeringTransformOptions`;
- добавлен `RuntimeEngineeringTransformResult`;
- добавлен `apply_engineering_transform()`.

`RuntimeValueApplier` получил два метода:

- `apply_protocol_read_result(read_result)`;
- `apply_protocol_read_result(read_result, tag)`.

Метод с `Tag` использует:

- `Tag.value_type`;
- `Tag.scale`;
- `Tag.offset`.

Engineering transform:

    engineering_value = raw_value * scale + offset

---

## Шаг 47

Добавлены runtime change detection и events foundation.

Результат:

- добавлен `RuntimeValueChangeKind`;
- добавлен `RuntimeValueChangeResult`;
- добавлен `detect_runtime_value_change()`;
- добавлен `RuntimeEventType`;
- добавлен `RuntimeValueEvent`;
- добавлен `make_runtime_value_event()`.

Runtime теперь сравнивает:

- предыдущее `engineering_value`;
- новое `engineering_value`;
- предыдущее `quality`;
- новое `quality`;
- предыдущий `source`;
- новый `source`.

`change_counter` увеличивается только при реальном изменении.

При изменении формируется `RuntimeValueEvent`.

---

## Шаг 48

Добавлены DTO, repository-интерфейсы и черновая миграция runtime.

Результат:

- добавлен `RuntimeValueDto`;
- добавлен `RuntimeValueEventDto`;
- добавлен `RuntimeValueApplyResultDto`;
- добавлен `RuntimeValueSnapshotDto`;
- добавлен `RuntimeValueValidationIssueDto`;
- добавлен `RuntimeValueValidationResultDto`;
- добавлен `IRuntimeValueSnapshotRepository`;
- добавлен `IRuntimeEventRepository`;
- создан `database/migrations/0005_runtime_values.sql`.

Миграция описывает таблицы:

- `runtime_value_snapshots`;
- `runtime_value_events`.

---

## Шаг 49

Обновлена документация и закрыт Sprint 006.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_006_SUMMARY.md`.

---

# Итоговые артефакты

## Backend runtime layer

Основной модуль:

- `backend/libs/scada_runtime`

Основные сущности:

- `TagValueStore`;
- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`;
- `RuntimeValueChangeKind`;
- `RuntimeValueChangeResult`;
- `RuntimeEventType`;
- `RuntimeValueEvent`.

Runtime helpers:

- runtime quality helpers;
- runtime value conversion;
- engineering transform;
- runtime change detection.

Repository-интерфейсы:

- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `RuntimeValueDto`;
- `RuntimeValueEventDto`;
- `RuntimeValueApplyResultDto`;
- `RuntimeValueSnapshotDto`;
- `RuntimeValueValidationIssueDto`;
- `RuntimeValueValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0005_runtime_values.sql`.

Миграция описывает таблицы:

- `runtime_value_snapshots`;
- `runtime_value_events`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_006_SUMMARY.md`.

---

# Что не входило в Sprint 006

В Sprint 006 сознательно не входили:

- потокобезопасность `TagValueStore`;
- интеграция polling -> runtime;
- EventBus publish;
- Historian;
- Alarm Engine;
- deadband;
- archive policy evaluation;
- PostgreSQL repository implementation;
- HTTP API;
- WebSocket;
- frontend;
- unit-тесты.

---

# Архитектурный результат

После Sprint 006 Dispatcher имеет foundation runtime values layer.

Текущая архитектурная цепочка:

    scada_common
        ↓
    scada_core
        ↓
    scada_objects
        ↓
    scada_devices
        ↓
    scada_tags
        ↓
    scada_protocols
        ↓
    scada_polling
        ↓
    scada_runtime
        ↓
    future historian / alarms / realtime gateway

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Текущий flow после Sprint 006

Текущий доступный runtime flow:

    ProtocolReadResult
        ↓
    RuntimeValueApplier
        ↓
    value conversion
        ↓
    engineering transform
        ↓
    quality handling
        ↓
    last good value handling
        ↓
    change detection
        ↓
    RuntimeValueEvent
        ↓
    TagValueStore

Пока flow не публикует события наружу.

Следующий спринт должен добавить historian foundation.

---

# Следующий спринт

## Sprint 007 — Historian Foundation

Предварительная цель:

Создать фундамент истории значений тегов.

Ожидаемые направления:

- `scada_historian`;
- `HistorySample`;
- archive decision foundation;
- historian buffer;
- batch write contract;
- history query contract;
- historian DTO;
- repository interfaces;
- database migration draft for tag history.

---

# Статус закрытия

Sprint 006 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.