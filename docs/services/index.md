# Dispatcher services index

## Назначение

Этот документ является навигационной картой backend-приложений и backend-модулей Dispatcher.

Перед изменением существующего backend-модуля нужно:

1. открыть этот индекс;
2. найти нужный модуль;
3. открыть его `SERVICE.md`;
4. проверить назначение, границы ответственности и статус реализации;
5. только после этого менять код.

## Правила документации модулей

Каждый backend-модуль и каждое backend-приложение должны иметь свой файл `SERVICE.md`.

Модуль нельзя считать завершенным, если у него нет `SERVICE.md`.

При изменении архитектуры или ответственности модуля нужно обновлять его `SERVICE.md`.

В каждом `SERVICE.md` должен быть раздел статуса реализации:

- реализовано сейчас;
- делаем сейчас;
- нужно доделать;
- расширить позже;
- не входит в MVP.

## Статусы

| Статус | Значение |
|---|---|
| Created | Модуль уже создан в репозитории |
| Planned | Модуль запланирован, но еще не создан |
| Partial | Модуль создан частично |
| Deferred | Модуль отложен на будущие этапы |
| Not MVP | Не входит в MVP |

## Backend applications

| Код | Путь | Назначение | Статус | Входит в MVP | Будущий микросервис |
|---|---|---|---|---|---|
| dispatcher_server | `backend/apps/dispatcher_server` | Основное backend-приложение Dispatcher. На первом этапе запускает все backend-модули внутри одного процесса. | Created | Да | Нет |
| dispatcher_polling_service | `backend/apps/dispatcher_polling_service` | Будущее отдельное приложение Polling Service. | Deferred | Нет | Да |
| dispatcher_historian_service | `backend/apps/dispatcher_historian_service` | Будущее отдельное приложение Historian Service. | Deferred | Нет | Да |
| dispatcher_notification_service | `backend/apps/dispatcher_notification_service` | Будущее отдельное приложение Notification Service. | Deferred | Нет | Да |
| dispatcher_script_service | `backend/apps/dispatcher_script_service` | Будущее отдельное приложение Script Service. | Deferred | Нет | Да |
| dispatcher_integration_service | `backend/apps/dispatcher_integration_service` | Будущее отдельное приложение Integration Service. | Deferred | Нет | Да |
| dispatcher_remote_polling_node | `backend/apps/dispatcher_remote_polling_node` | Будущее приложение удаленного узла опроса. | Deferred | Нет | Да |

## Backend libraries

| Код | Путь | Назначение | Статус | Входит в MVP | Будущий микросервис |
|---|---|---|---|---|---|
| scada_common | `backend/libs/scada_common` | Общие типы, ошибки, результат операций, идентификаторы, время и версия продукта. | Created | Да | Нет |
| scada_contracts | `backend/libs/scada_contracts` | DTO, REST/API envelopes, WebSocket envelopes и контракты обмена. | Created | Да | Нет |
| scada_core | `backend/libs/scada_core` | Ядро backend-приложения, `ApplicationContext`, lifecycle, базовые интерфейсы, EventBus/Settings/Health abstractions. | Created | Да | Нет |
| scada_settings | `backend/libs/scada_settings` | Системные настройки, setting definitions, runtime settings, hot reload, audit изменений. | Planned | Да | Нет |
| scada_objects | `backend/libs/scada_objects` | Объектная модель: площадки, здания, этажи, комнаты, зоны, шкафы, линии, группы оборудования. | Planned | Да | Нет |
| scada_devices | `backend/libs/scada_devices` | Устройства, подключения, черновики Конфигуратора оборудования, статусы устройств. | Planned | Да | Нет |
| scada_protocols | `backend/libs/scada_protocols` | `IProtocolDriver`, MockDriver, SimulatorDriver, Modbus TCP, SNMP basic и будущие драйверы. | Planned | Да | Нет |
| scada_tags | `backend/libs/scada_tags` | Теги, адреса тегов, текущие значения, качество, архивные политики, value extraction. | Planned | Да | Нет |
| scada_polling | `backend/libs/scada_polling` | Scheduler, queue, worker pool, опрос устройств, обновление `TagValueStore`. | Planned | Да | Да |
| scada_historian | `backend/libs/scada_historian` | Архивирование истории по `TagArchivePolicy`, буфер, batch writer, запросы истории. | Planned | Да | Да |
| scada_alarms | `backend/libs/scada_alarms` | Правила аварий, активные аварии, переходы, квитирование, комментарии. | Planned | Да | Нет |
| scada_events | `backend/libs/scada_events` | Единый диспетчер событий: аварии, команды, пользователи, панели, связь, система, аудит. | Planned | Да | Нет |
| scada_commands | `backend/libs/scada_commands` | Безопасное выполнение команд через Command Service, политики, очереди, результат выполнения. | Planned | Да | Нет |
| scada_security | `backend/libs/scada_security` | Пользователи, роли, права, сессии, service accounts, panel/workstation accounts. | Planned | Да | Нет |
| scada_audit | `backend/libs/scada_audit` | Аудит важных действий, audit records, change sets, security events. | Planned | Да | Нет |
| scada_dashboards | `backend/libs/scada_dashboards` | Dashboard, DashboardWindow, Widget, WidgetBinding, публикация, версии, доступы. | Planned | Да | Нет |
| scada_mimics | `backend/libs/scada_mimics` | Мнемосхемы, SVG-модель, элементы, привязки, state rules, zoom/pan basic. | Planned | Да | Нет |
| scada_notifications | `backend/libs/scada_notifications` | Notification Rules, центр уведомлений, delivery log, внутренние уведомления, email optional. | Planned | Да | Да |
| scada_scripts | `backend/libs/scada_scripts` | Скрипты, `IScriptEngine`, sandbox, триггеры, журнал выполнения. | Planned | Частично | Да |
| scada_scheduler | `backend/libs/scada_scheduler` | Планировщик задач, interval/daily jobs, retention, backup jobs, execution log. | Planned | Да | Да |
| scada_maintenance | `backend/libs/scada_maintenance` | ППР, MaintenanceObject, MaintenanceTask, комментарии, статусы, события. | Planned | Да | Нет |
| scada_integrations | `backend/libs/scada_integrations` | Integration API, service accounts, scopes, external tags, future webhooks. | Planned | Да | Да |
| scada_diagnostics | `backend/libs/scada_diagnostics` | SystemHealth, health checks, метрики модулей, диагностика БД, очередей, WebSocket, диска. | Planned | Да | Да |
| scada_files | `backend/libs/scada_files` | Абстракция файлового хранилища, metadata, links, LocalFileStorage. | Planned | Да | Нет |
| scada_backup | `backend/libs/scada_backup` | Backup/restore basic, backup jobs, backup history, manifest, audit. | Planned | Да | Нет |
| scada_reports | `backend/libs/scada_reports` | Будущий модуль отчетов. | Deferred | Нет | Да |
| scada_license | `backend/libs/scada_license` | Будущий модуль лицензирования и редакций продукта. | Deferred | Нет | Нет |

## MVP backend modules

В MVP входят:

- `scada_common`;
- `scada_contracts`;
- `scada_core`;
- `scada_settings`;
- `scada_objects`;
- `scada_devices`;
- `scada_protocols`;
- `scada_tags`;
- `scada_polling`;
- `scada_historian`;
- `scada_alarms`;
- `scada_events`;
- `scada_commands`;
- `scada_security`;
- `scada_audit`;
- `scada_dashboards`;
- `scada_mimics` basic;
- `scada_notifications` basic;
- `scada_scheduler` basic;
- `scada_maintenance` basic;
- `scada_integrations` basic;
- `scada_diagnostics`;
- `scada_files` basic;
- `scada_backup` basic.

## Not MVP

Не входят в MVP:

- `scada_reports` full;
- `scada_license`;
- отдельные микросервисные приложения;
- remote polling node app;
- advanced Integration Service;
- advanced Script Service;
- advanced Scheduler Service;
- advanced Diagnostics Service;
- plugin SDK;
- dynamic protocol driver loading.

## Рекомендуемый порядок разработки модулей

После фундамента модули разрабатываются в таком порядке:

1. `scada_common`;
2. `scada_core`;
3. `scada_contracts`;
4. `scada_objects`;
5. `scada_devices`;
6. `scada_tags`;
7. `scada_protocols`;
8. `scada_polling`;
9. `dispatcher_server` API layer;
10. frontend Current Values;
11. WebSocket layer;
12. `scada_historian`;
13. `scada_events`;
14. `scada_alarms`;
15. `scada_commands`;
16. `scada_security`;
17. `scada_audit`;
18. `scada_dashboards`;
19. `scada_mimics`;
20. `scada_notifications`;
21. `scada_scheduler`;
22. `scada_maintenance`;
23. `scada_integrations`;
24. `scada_settings`;
25. `scada_diagnostics`;
26. `scada_backup`;
27. Linux deploy.

## Первый вертикальный срез

Первый вертикальный срез:

```text
SimulatorDriver
-> Polling Service
-> TagValueStore
-> REST API
-> Blazor Current Values page
