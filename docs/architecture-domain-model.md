# Dispatcher domain model architecture

## Назначение документа

Этот документ описывает базовую доменную архитектуру Dispatcher.

Цель:

- определить основные сущности;
- определить связи между сущностями;
- определить границы ответственности модулей;
- подготовить основу для реализации SCADA-функциональности.

Документ является основой для:

- scada_objects;
- scada_devices;
- scada_tags;
- scada_protocols;
- scada_polling;
- scada_events;
- scada_alarms;
- scada_commands;
- scada_dashboards;
- scada_maintenance.

---

# Основной принцип модели

Dispatcher строится вокруг физической модели инженерного объекта.

Главная цепочка:

```
Location
    |
    +-- Equipment
            |
            +-- Device
                    |
                    +-- Tag
```

---

# Location

Location описывает расположение объекта.

Используется универсальная модель.

Пример:

```
Площадка

├── Корпус А
│
├── Электрощитовая
│
├── Насосная станция
│
└── Наружная зона
```

---

# Location types

```
SITE

BUILDING

ROOM

ZONE

AREA

OUTDOOR

CABINET

LINE

CUSTOM
```

---

# Equipment

Equipment — инженерное оборудование.

Примеры:

```
Насос

Вентилятор

Компрессор

Чиллер

Электрощит
```

Equipment может иметь:

- Location;
- Device;
- Tags;
- Maintenance;
- Alarm rules.

---

# Device

Device — физическое устройство обмена данными.

Примеры:

```
PLC

Controller

Gateway

Sensor

Power meter
```

Device содержит:

```
DeviceId

Name

Model

Manufacturer

Connection

Protocol

Status
```

---

# Device и Equipment

Они разделяются.

Пример:

```
Equipment:

Насос №1


Device:

PLC Siemens


Tags:

Status

Speed

Fault
```

---

# Tag

Tag — минимальная единица данных Dispatcher.

Содержит:

```
TagId

Name

DataType

Address

AccessMode

CurrentValue

Quality

Timestamp
```

---

# TagValue

Значение тега:

```
Value

Timestamp

Quality

Source
```

---

# Quality

Поддерживаемые состояния:

```
GOOD

BAD

UNCERTAIN

COMMUNICATION_ERROR

DEVICE_ERROR

MANUAL

SIMULATED
```

---

# Event

Event — событие системы.

Примеры:

```
Device offline

User login

Command executed

Configuration changed
```

Содержит:

```
EventId

Timestamp

Source

Severity

Message

CorrelationId
```

---

# Alarm

Alarm — специализированное событие.

Состояния:

```
ACTIVE

ACKNOWLEDGED

CLEARED

ARCHIVED
```

---

# Command

Command — команда управления.

Примеры:

```
Start pump

Stop pump

Change setpoint
```

---

# User

User используется для:

- доступа;
- управления;
- аудита;
- уведомлений.

---

# Dashboard

Dashboard отображает данные.

Содержит:

```
Widgets

Tags

Mimics

Permissions
```

Не владеет данными.

---

# Entity identifiers

Все сущности имеют собственные идентификаторы.

Используются:

```
ObjectId

LocationId

EquipmentId

DeviceId

TagId

EventId

AlarmId

CommandId

UserId
```

Внутри используется UUID.

Пример:

```cpp
DeviceId device_id;

TagId tag_id;
```

Преимущества:

- читаемость;
- безопасность типов;
- уменьшение ошибок.

---

# Common entity fields

Все основные сущности должны иметь:

```
Id

CreatedAt

CreatedBy

UpdatedAt

UpdatedBy

Version

LifecycleState
```

---

# Lifecycle

Основные сущности не удаляются физически.

Используются состояния:

```
ACTIVE

DISABLED

ARCHIVED

DELETED
```

---

# Audit

Изменения должны сохранять:

```
CreatedAt

CreatedBy

UpdatedAt

UpdatedBy

ChangeReason
```

---

# Module boundaries

## scada_objects

Отвечает за:

- Location;
- Equipment;
- иерархию объектов.

---

## scada_devices

Отвечает за:

- Device;
- Connection;
- связь.

---

## scada_tags

Отвечает за:

- Tag;
- TagValue;
- Quality.

---

## scada_protocols

Отвечает за:

- драйверы;
- протоколы.

---

## scada_polling

Отвечает за:

- расписание;
- очереди;
- worker pool.

---

## scada_events

Отвечает за:

- события;
- историю событий.

---

## scada_alarms

Отвечает за:

- аварии;
- правила;
- квитирование.

---

# Статус документа

O3-002 completed.

Добавлено:

- доменная модель;
- правила идентификаторов;
- Typed IDs.

Следующий шаг:

O3-003.

Добавление:

- LifecycleState;
- общих полей жизненного цикла.