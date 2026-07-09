# Dispatcher MVP Stabilization Checklist

## Назначение документа

Этот документ содержит чеклист стабилизации MVP Dispatcher.

Чеклист используется перед закрытием Sprint 010 и MVP foundation stage.

---

# 1. Repository status

## Проверить git status

Команда:

    git status

Ожидаемый результат:

- нет случайных build artifacts;
- нет `bin`;
- нет `obj`;
- нет `out`;
- нет `.vs`;
- нет временных файлов;
- все изменения понятны.

## Проверить .gitignore

Ожидается, что `.gitignore` исключает:

- Visual Studio temporary files;
- CMake output;
- C++ build artifacts;
- .NET bin/obj;
- frontend generated artifacts;
- logs;
- local secrets;
- local database files.

---

# 2. Backend build

## CMake configure

Команда:

    cmake --preset x64-debug

Ожидаемый результат:

- configure проходит без ошибок.

## Backend build

Команда:

    cmake --build --preset x64-debug

Ожидаемый результат:

- backend собирается без ошибок;
- warnings не блокируют сборку;
- все backend modules собираются.

---

# 3. Backend run

## Запуск dispatcher_server

Команда:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

Ожидаемый результат:

- приложение запускается;
- выводит название Dispatcher;
- выводит version;
- выводит mode;
- выводит список modules.

## Ожидаемые modules

Список должен содержать:

- `scada_common`;
- `scada_contracts`;
- `scada_core`;
- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`;
- `scada_polling`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`;
- `scada_api`;
- `scada_realtime`;
- `scada_app`.

## Ожидаемые statuses

Для MVP foundation ожидается:

    Running

для всех зарегистрированных modules.

---

# 4. Frontend build

## Project build

Команда:

    dotnet build frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Ожидаемый результат:

- frontend project собирается без ошибок.

## Solution build

Команда:

    dotnet build frontend\Dispatcher.Frontend.slnx

Ожидаемый результат:

- frontend solution собирается без ошибок.

---

# 5. Frontend run

## Запуск frontend

Команда:

    dotnet run --project frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Ожидаемый результат:

- Blazor WebAssembly запускается;
- terminal выводит адрес приложения;
- browser открывает Dispatcher.

## Проверка правильного адреса

Открывать нужно адрес из строки:

    Now listening on: ...

Не использовать случайный localhost-порт.

---

# 6. Frontend UI checks

## Layout

Проверить:

- AppBar отображается;
- Drawer/sidebar отображается;
- navigation отображается;
- фон темный;
- активный пункт navigation подсвечивается;
- страница адаптируется по ширине.

## Navigation

Проверить переходы:

- `/`;
- `/system`;
- `/objects`;
- `/devices`;
- `/tags`;
- `/runtime`;
- `/events`;
- `/alarms`.

## Overview page

Проверить:

- отображается Dispatcher title;
- отображается Sprint 010 badge;
- отображаются cards backend/frontend/MVP focus.

## System page

Проверить:

- отображаются summary cards;
- отображается API foundation summary;
- отображается realtime foundation summary;
- отображается backend modules table;
- в таблице есть `scada_api`;
- в таблице есть `scada_realtime`;
- в таблице есть `scada_app`;
- видно, что данные mock.

## Runtime page

Проверить:

- отображаются summary cards;
- отображается current values table;
- есть значения с Good quality;
- есть значение с Uncertain или CommunicationError;
- видно, что данные mock.

## Events page

Проверить:

- отображаются summary cards;
- отображается event journal;
- есть event severity;
- есть source type;
- есть correlation id;
- видно, что данные mock.

## Alarms page

Проверить:

- отображаются summary cards;
- отображается active alarms table;
- есть critical alarm;
- есть acknowledged/unacknowledged status;
- видно, что данные mock.

## Objects page

Проверить:

- страница открывается;
- отображается placeholder.

## Devices page

Проверить:

- страница открывается;
- отображается placeholder.

## Tags page

Проверить:

- страница открывается;
- отображается placeholder.

---

# 7. Browser console

## Проверить browser console

Ожидаемый результат:

- нет красных ошибок Blazor;
- нет ошибок MudBlazor;
- нет ошибок загрузки `_framework/blazor.webassembly.js`;
- нет ошибок загрузки `_content/MudBlazor/MudBlazor.min.css`;
- нет ошибок загрузки `_content/MudBlazor/MudBlazor.min.js`.

## Не считать ошибкой проекта

Если открыт неправильный localhost-порт, browser может показать внешнюю страницу ошибки.

В таком случае нужно открыть адрес из terminal output:

    Now listening on: ...

---

# 8. Documentation checks

Проверить наличие документов:

- `docs/PROJECT_SPECIFICATION.txt`;
- `docs/VERSION_1_ROADMAP.txt`;
- `docs/DEFERRED_FEATURES.txt`;
- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/MVP_STATUS.md`;
- `docs/MVP_SCOPE.md`;
- `docs/MVP_STABILIZATION_CHECKLIST.md`;
- `docs/SPRINT_009_SUMMARY.md`;
- будущий `docs/SPRINT_010_SUMMARY.md`.

Проверить, что markdown files:

- открываются нормально;
- не содержат случайно сломанных fenced blocks;
- не содержат устаревших утверждений о production-ready статусе.

---

# 9. Known limitations checks

Проверить, что known limitations явно фиксируют отсутствие:

- real HTTP API;
- real WebSocket/SSE gateway;
- PostgreSQL repository implementations;
- real Modbus TCP driver;
- real SNMP driver;
- background polling loop;
- Event Manager;
- Alarm Manager;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- command execution.

---

# 10. MVP scope checks

Проверить, что MVP scope не обещает:

- production deployment;
- промышленный real-time monitoring;
- real device control;
- reliable historian;
- alarm acknowledgement;
- user/role system;
- secure operator access;
- external integrations.

---

# 11. Manual cleanup before commit

Проверить и удалить, если появились:

- `frontend/Dispatcher.Frontend/bin`;
- `frontend/Dispatcher.Frontend/obj`;
- `out`;
- `.vs`;
- temporary files.

Если они есть в рабочей директории, но игнорируются `.gitignore`, это нормально.

Главное — они не должны попасть в commit.

---

# 12. Final commit check

Перед commit выполнить:

    git status

Проверить, что staged/unstaged files соответствуют текущему шагу.

Для шага 72 ожидаются только docs changes.

---

# 13. Step 72 acceptance criteria

Шаг 72 считается выполненным, если:

- создан `docs/MVP_STATUS.md`;
- создан `docs/MVP_STABILIZATION_CHECKLIST.md`;
- создан `docs/MVP_SCOPE.md`;
- frontend build прошел;
- backend build прошел;
- backend run прошел;
- frontend run прошел;
- изменения закоммичены;
- изменения отправлены в `master`.

---

# 14. Sprint 010 closure readiness

Sprint 010 готов к закрытию на шаге 73, если:

- шаги 68–72 выполнены;
- frontend foundation стабилен;
- backend foundation стабилен;
- MVP docs добавлены;
- known limitations понятны;
- Sprint 010 summary можно написать без противоречий.