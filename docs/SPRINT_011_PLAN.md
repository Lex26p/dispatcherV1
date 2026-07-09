# Sprint 011 Plan

## Название спринта

Russian UI Cleanup and Frontend Terminology

## Статус

Planned

## Этап

Post-MVP development

## Диапазон шагов

Шаги 74–80.

---

# Цель спринта

Цель Sprint 011 — привести frontend-интерфейс Dispatcher к русскому языку по умолчанию и зафиксировать единую терминологию пользовательского интерфейса.

После закрытия MVP foundation стало понятно, что frontend нужно развивать как русскоязычный операторский интерфейс.

Английский язык остается как будущая локализация, но не как основной язык интерфейса.

---

# Главная идея

До Sprint 011 frontend содержит много временных английских подписей.

Sprint 011 должен заменить их на русские и подготовить UI к дальнейшей реальной интеграции с backend API.

Правило:

    новый пользовательский текст во frontend пишем на русском языке

---

# Почему этот спринт нужен сейчас

Этот спринт нужен до real API integration, потому что:

- frontend пока небольшой;
- терминологию еще легко привести к единому виду;
- будущие API-экраны будут опираться на эти термины;
- operator UI должен быть понятен русскоязычному пользователю;
- английский лучше добавлять позже как локализацию, а не смешивать с основным интерфейсом.

---

# In scope

В Sprint 011 входит:

- создание UI terminology document;
- перевод layout/navigation;
- перевод overview page;
- перевод System page;
- перевод Objects/Devices/Tags placeholder pages;
- перевод Runtime page;
- перевод Events page;
- перевод Alarms page;
- добавление UI state terminology;
- добавление loading/empty/error text foundation;
- обновление frontend docs;
- обновление sprint summary.

---

# Out of scope

В Sprint 011 не входит:

- real backend HTTP API;
- real frontend API integration;
- WebSocket/SSE;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- charts;
- command execution;
- alarm acknowledgement action;
- localization engine;
- resource files;
- culture switching.

---

# Языковая политика

## Основной язык

Основной язык пользовательского интерфейса:

    русский

## Будущая локализация

Английский язык будет добавлен позже как дополнительная локализация.

На Sprint 011 не внедряем полноценную систему локализации.

## Что переводим

Переводим пользовательские строки:

- названия пунктов меню;
- заголовки страниц;
- подписи карточек;
- описания;
- подсказки;
- сообщения warning/info/error;
- placeholder-тексты;
- статусы, если они отображаются пользователю.

## Что не переводим

Не переводим технические элементы:

- имена файлов;
- имена классов;
- имена методов;
- namespace;
- route paths;
- backend module codes;
- protocol names;
- enum names в коде;
- commit messages;
- internal IDs;
- API paths.

---

# Route policy

URL routes пока оставляем английскими.

Примеры:

- `/system`;
- `/objects`;
- `/devices`;
- `/tags`;
- `/runtime`;
- `/events`;
- `/alarms`.

Причина:

- route paths являются техническим контрактом;
- русские URL могут создать лишние проблемы с совместимостью;
- пользовательский текст на странице будет русским.

---

# UI terminology policy

Базовые термины:

- Object — Объект;
- Device — Устройство;
- Tag — Тег;
- Runtime — Текущие значения;
- Event — Событие;
- Alarm — Авария;
- History — История;
- Quality — Качество;
- Source — Источник;
- Severity — Важность;
- Priority — Приоритет;
- State — Состояние;
- Acknowledgement — Квитирование.

Полный словарь фиксируется в:

    docs/UI_TERMINOLOGY.md

---

# План шагов

## Шаг 74 — Sprint 011 plan and UI terminology foundation

Создать:

- `docs/SPRINT_011_PLAN.md`;
- `docs/UI_TERMINOLOGY.md`.

Результат:

- план Sprint 011 зафиксирован;
- UI terminology foundation создан.

## Шаг 75 — Translate layout and navigation

Перевести:

- AppBar;
- Drawer;
- navigation;
- operator status panel;
- common layout labels.

Результат:

- основная навигация на русском языке.

## Шаг 76 — Translate overview, system, objects, devices and tags pages

Перевести:

- Overview page;
- System page;
- Objects page;
- Devices page;
- Tags page.

Результат:

- основные справочные страницы на русском языке.

## Шаг 77 — Translate runtime, events and alarms pages

Перевести:

- Runtime page;
- Events page;
- Alarms page;
- mock data display labels.

Результат:

- операторские страницы текущих значений, событий и аварий на русском языке.

## Шаг 78 — Add UI state text foundation

Добавить базовые тексты для состояний UI:

- loading;
- empty;
- error;
- mock-data warning;
- no connection;
- not implemented.

Результат:

- единый стиль сообщений интерфейса.

## Шаг 79 — Update frontend docs

Обновить:

- `frontend/README.md`;
- `frontend/Dispatcher.Frontend/SERVICE.md`;
- при необходимости `docs/MVP_STATUS.md`;
- при необходимости `docs/known-limitations.md`.

Результат:

- frontend docs отражают русскую UI-политику.

## Шаг 80 — Sprint 011 docs update and closure

Создать:

- `docs/SPRINT_011_SUMMARY.md`.

Обновить:

- `docs/development-log.md`;
- `docs/known-limitations.md`.

Результат:

- Sprint 011 закрыт.

---

# Acceptance criteria

Sprint 011 считается выполненным, если:

- создан `docs/UI_TERMINOLOGY.md`;
- весь видимый frontend UI foundation переведен на русский;
- navigation на русском;
- основные страницы на русском;
- warning/info/empty/loading тексты на русском;
- frontend build проходит;
- frontend run проходит;
- backend build проходит;
- backend run проходит;
- документация обновлена;
- изменения отправлены в `master`.

---

# Build checks

Frontend:

    dotnet build frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Frontend solution:

    dotnet build frontend\Dispatcher.Frontend.slnx

Backend:

    cmake --preset x64-debug
    cmake --build --preset x64-debug

Backend run:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

---

# Expected result after Sprint 011

После Sprint 011 Dispatcher должен иметь русскоязычный operator UI foundation.

Ожидаемый результат:

- пользователь видит русский интерфейс;
- терминология единая;
- английские UI-подписи удалены или помечены как технические;
- frontend готов к real API integration;
- следующая логичная задача — backend HTTP API transport foundation.

---

# Следующий спринт после Sprint 011

Рекомендуемый следующий спринт:

    Sprint 012 — Backend HTTP API Transport Foundation

Цель:

- добавить реальный backend HTTP transport;
- реализовать `/api/system/health`;
- реализовать `/api/system/modules`;
- подготовить frontend к замене mock System page на реальные backend данные.
