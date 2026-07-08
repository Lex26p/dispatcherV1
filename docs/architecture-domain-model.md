# Dispatcher domain model architecture

## Назначение документа

Этот документ описывает базовую доменную архитектуру Dispatcher.

Цель документа:

- определить основные сущности системы;
- определить связи между сущностями;
- определить границы ответственности модулей;
- избежать неправильного проектирования сервисов;
- зафиксировать архитектурные решения до начала реализации основных SCADA-модулей.

Этот документ является основой для разработки:

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

Dispatcher строится вокруг модели реального инженерного объекта.

Основная цепочка:

```
Location
    |
    +-- Equipment
            |
            +-- Device
                    |
                    +-- Tag
```

Логика:

- Location определяет физическое расположение.
- Equipment определяет инженерную систему или оборудование.
- Device определяет физическое устройство обмена данными.
- Tag определяет конкретный параметр контроля или управления.

---

# Главный принцип разделения сущностей

Важно разделять:

```
Оборудование
```

и

```
Устройство связи
```

Они не являются одним объектом.

Пример:

```
Equipment:

Насосная установка №1


Device:

PLC Siemens S7-1200


Tags:

Pump1.Status

Pump1.Speed

Pump1.Fault
```

Один объект оборудования может иметь:

- несколько контроллеров;
- несколько устройств связи;
- десятки и сотни тегов.

---

# Location model

## Назначение

Location описывает расположение объектов.

Нельзя использовать жесткую структуру:

```
Site
 |
 Building
 |
 Room
```

потому что реальные объекты имеют разные структуры.

Пример:

```
Площадка

├── Корпус А
│     └── Электрощитовая
│            └── Щит №12
│
├── Насосная станция
│
├── КТП наружная
│
└── Резервуарная зона
```

Поэтому используется универсальная модель Location.

---

# Location types

Базовые типы:

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

Описание:

| Тип | Назначение |
|---|---|
| SITE | Территория или объект |
| BUILDING | Здание или корпус |
| ROOM | Помещение |
| ZONE | Технологическая зона |
| AREA | Участок |
| OUTDOOR | Наружная зона |
| CABINET | Шкаф или стойка |
| LINE | Производственная линия |
| CUSTOM | Пользовательский тип |

В будущем пользователь может создавать свои типы Location.

---

# Equipment model

## Назначение

Equipment — это инженерное оборудование.

Примеры:

```
Насос

Вентилятор

Компрессор

Чиллер

Котел

Электрощит

Станция водоснабжения
```

Equipment может иметь:

- Location;
- Device;
- Tags;
- Maintenance;
- Alarm rules;
- Documentation;
- Commands.

---

# Device model

## Назначение

Device — физическое устройство, через которое Dispatcher получает данные или отправляет команды.

Примеры:

```
PLC

Controller

Power meter

Gateway

Sensor

Frequency converter
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

# Device connection model

Устройство не должно напрямую содержать логику протокола.

Модель:

```
Device

 |

Connection

 |

Protocol Driver
```

Пример:

```
Device:

PLC-01


Connection:

192.168.1.10:502


Protocol:

Modbus TCP
```

---

# Tag model

## Назначение

Tag — минимальная единица данных Dispatcher.

Tag описывает параметр:

- измерения;
- состояния;
- управления;
- вычисления.

---

# Tag structure

Tag содержит:

```
TagId

Name

Description

DataType

Address

AccessMode

CurrentValue

Quality

Timestamp
```

---

# Типы Tag

```
INPUT

OUTPUT

CALCULATED

VIRTUAL

SYSTEM
```

Описание:

| Тип | Назначение |
|---|---|
| INPUT | Значение от устройства |
| OUTPUT | Управляющий параметр |
| CALCULATED | Расчетное значение |
| VIRTUAL | Внутренний тег |
| SYSTEM | Системный параметр |

---

# TagValue model

Значение тега никогда не должно храниться только как число.

Полная модель:

```
Value

Timestamp

Quality

Source
```

Пример:

```
Value:

25.4


Timestamp:

2026-07-08T12:00:00Z


Quality:

GOOD


Source:

ModbusTCP
```

---

# Quality model

SCADA система должна различать:

```
GOOD

BAD

UNCERTAIN

COMMUNICATION_ERROR

DEVICE_ERROR

MANUAL

SIMULATED
```

Причина:

Одинаковое значение может иметь разный смысл.

Например:

```
25.0 GOOD
```

и

```
25.0 COMMUNICATION_ERROR
```

это разные состояния.

---

# Event model

Event — любое значимое событие системы.

Примеры:

```
Device offline

User login

Command executed

Configuration changed

Alarm activated
```

Event содержит:

```
EventId

Timestamp

Source

Severity

Message

CorrelationId

UserId
```

---

# Alarm model

Alarm является специализированным событием.

Alarm содержит:

```
AlarmId

Rule

Priority

State

Timestamp

Acknowledgement
```

Состояния:

```
ACTIVE

ACKNOWLEDGED

CLEARED

ARCHIVED
```

---

# Command model

Command — команда управления объектом.

Примеры:

```
Start pump

Stop ventilation

Change setpoint

Open valve
```

Command содержит:

```
CommandId

Target

User

Timestamp

Result

AuditInfo
```

---

# User model

User используется для:

- авторизации;
- управления;
- аудита;
- уведомлений.

User связан с:

```
Role

Permission

Audit

Notification settings
```

---

# Dashboard model

Dashboard отвечает за отображение данных.

Dashboard содержит:

```
Dashboard

Widgets

Tags

Mimics

Permissions
```

Dashboard не владеет данными.

Он только отображает данные других модулей.

---

# Entity common fields

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

# Lifecycle model

Физическое удаление сущностей запрещается для основных объектов.

Используется жизненный цикл:

```
ACTIVE

DISABLED

ARCHIVED

DELETED
```

Причина:

Нужно сохранять:

- историю;
- аварии;
- события;
- аудит;
- связь с прошлыми конфигурациями.

---

# Audit model

Все важные изменения должны иметь информацию:

```
CreatedAt

CreatedBy

UpdatedAt

UpdatedBy

ChangeReason
```

Применяется для:

- устройств;
- тегов;
- пользователей;
- настроек;
- правил аварий;
- команд.

---

# Entity identifiers

Каждая сущность должна иметь свой тип идентификатора.

Не использовать:

```
Uuid id;
```

во всех классах.

Использовать:

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

Внутри все они могут использовать UUID.

Цель:

- повысить читаемость;
- избежать ошибок;
- упростить разработку.

---

# Module boundaries

## scada_objects

Отвечает за:

- Location;
- Equipment;
- объектную иерархию.


Не отвечает за:

- протоколы;
- опрос;
- значения тегов.

---

## scada_devices

Отвечает за:

- Device;
- Connection;
- состояние связи;
- конфигурацию устройств.


---

## scada_tags

Отвечает за:

- Tag;
- TagValue;
- Quality;
- адресацию тегов.


---

## scada_protocols

Отвечает за:

- драйверы;
- протоколы;
- обмен данными.


---

## scada_polling

Отвечает за:

- расписание опроса;
- очереди;
- worker pool;
- обновление текущих значений.


---

## scada_events

Отвечает за:

- события;
- журнал событий;
- корреляцию действий.


---

## scada_alarms

Отвечает за:

- аварийные правила;
- состояния аварий;
- квитирование.


---

# Database strategy

На уровне архитектуры предусматривается разделение:

## Configuration data

Хранится в:

```
PostgreSQL
```

Примеры:

- пользователи;
- устройства;
- настройки;
- конфигурация;
- аварии;
- события.


## Time series data

Хранится отдельно:

```
TimescaleDB
```

Примеры:

- история значений тегов;
- архивы измерений.

---

# Архитектурный принцип разработки

Новые модули должны:

- зависеть от интерфейсов;
- не зависеть от конкретных технологий;
- иметь собственный SERVICE.md;
- иметь документацию;
- иметь описание текущего состояния.

---

# Статус документа

Создано:

O3-001

Статус:

Completed

Следующий шаг:

O3-002

Добавление типизированных идентификаторов и базовых доменных типов в:

```
scada_common
```