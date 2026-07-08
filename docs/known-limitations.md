# Dispatcher known limitations

## Назначение

Этот файл фиксирует текущие ограничения реализации Dispatcher.

Ограничения — это не ошибки, а осознанные временные рамки текущего этапа разработки.

Если функция заложена архитектурно, но не реализована сейчас, она должна быть отражена здесь, в `docs/DEFERRED_FEATURES.txt` или в будущем `docs/backlog.md`.

## Текущие ограничения после Sprint 001

### Общие ограничения

- Проект находится на этапе технического фундамента.
- Реализованы только стартовые backend-модули.
- Полноценная SCADA-логика еще не реализована.
- Данные пока не сохраняются в PostgreSQL.
- REST API еще не реализован.
- WebSocket еще не реализован.
- Frontend еще не реализован.
- Linux-развертывание еще не реализовано.

### dispatcher_server

- `dispatcher_server` пока только запускает минимальный `ApplicationContext`.
- Нет bootstrap-конфигурации.
- Нет чтения конфигурационных файлов.
- Нет логирования через `spdlog`.
- Нет REST API.
- Нет WebSocket.
- Нет graceful shutdown.
- Нет health endpoints.
- Нет подключения к PostgreSQL.
- Нет runtime-настроек.

### scada_common

- `Uuid` пока является строковой оболочкой без генерации настоящего UUID.
- `CorrelationId` пока не генерируется автоматически для каждого запроса.
- Валидация UUID пока отсутствует.
- `Timestamp` пока имеет только базовое форматирование UTC.
- Unit-тесты для common-типов еще не добавлены.

### scada_contracts

- Контракты пока минимальные.
- Реализован только базовый `ApiEnvelope`.
- Нет `ErrorResponse`.
- Нет `PagedResponse`.
- Нет `OperationResponse`.
- Нет `WebSocketEnvelope`.
- Нет DTO для тегов, устройств, аварий, событий и команд.

### scada_core

- `ApplicationContext` пока минимальный.
- Нет полноценного lifecycle модулей.
- Нет настоящего `ModuleRegistry`.
- Нет реального `InMemoryEventBus`.
- Нет settings provider реализации.
- Нет health check runtime.
- Нет startup checks.
- Нет graceful shutdown.
- Нет embedded/external service mode.

### CMake и сборка

- Настроена только базовая сборка.
- Тестовая инфраструктура еще не подключена.
- `vcpkg` еще не подключен.
- Внешние зависимости еще не добавлены.
- CI/CD еще не настроен.
- Linux-сборка еще не проверялась.

### Документация

- Создана базовая документация.
- Часть markdown-файлов пока является заготовками.
- `docs/backlog.md` пока не заполнен полноценно.
- `docs/mvp-specification.md` пока не заполнен полноценно.
- `docs/architecture.md` пока не заполнен полноценно.
- `docs/roadmap.md` пока не заполнен полноценно.

## Не реализовано на текущем этапе

На текущем этапе еще не реализованы:

- объектная модель;
- устройства;
- теги;
- текущие значения;
- `TagValueStore`;
- протокольные драйверы;
- `IProtocolDriver`;
- `MockDriver`;
- `SimulatorDriver`;
- Polling Service;
- Modbus TCP;
- SNMP;
- Historian;
- Alarm Service;
- Event Service;
- Command Service;
- Security;
- Audit;
- Dashboard;
- Mimic;
- PanelDashboard;
- Workstation Mode;
- Notifications;
- Scheduler;
- Maintenance;
- Integrations;
- Settings UI;
- Diagnostics;
- Backup;
- Frontend.

## Следующее ожидаемое снятие ограничений

Ближайшие шаги должны начать снимать ограничения в таком порядке:

1. создать `scada_objects`;
2. создать `scada_devices`;
3. создать `scada_tags`;
4. создать `IProtocolDriver`;
5. создать `MockDriver`;
6. создать `SimulatorDriver`;
7. создать `TagValueStore`;
8. создать Polling basic;
9. создать REST current values;
10. создать первый frontend экран.