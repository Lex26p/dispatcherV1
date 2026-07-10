# Sprint 011 Summary

## Название спринта

Russian UI Cleanup and Frontend Terminology

## Статус

Closed

## Этап

Post-MVP development

## Диапазон шагов

Шаги 74–80.

---

# Цель спринта

Цель Sprint 011 — привести frontend-интерфейс Dispatcher к русскому языку по умолчанию и зафиксировать единую терминологию пользовательского интерфейса.

Sprint 011 был выполнен после закрытия MVP foundation.

Основная идея:

    frontend UI language
        -> русский по умолчанию

Английский язык оставлен как будущая локализация.

---

# Выполненные шаги

## Шаг 74 — Sprint 011 plan and UI terminology foundation

Созданы документы:

- `docs/SPRINT_011_PLAN.md`;
- `docs/UI_TERMINOLOGY.md`.

Зафиксировано:

- план Sprint 011;
- правило русского UI;
- базовая терминология;
- что переводим;
- что не переводим;
- route policy;
- future localization note.

Ключевое правило:

    новый пользовательский текст во frontend пишем на русском языке

---

## Шаг 75 — Translate layout and navigation

Переведены:

- AppBar;
- Drawer;
- sidebar navigation;
- operator status panel.

Результат:

- навигация отображается на русском;
- пункты меню:
  - Обзор;
  - Система;
  - Объекты;
  - Устройства;
  - Теги;
  - Текущие значения;
  - События;
  - Аварии.

Технические route paths остались английскими:

- `/system`;
- `/objects`;
- `/devices`;
- `/tags`;
- `/runtime`;
- `/events`;
- `/alarms`.

---

## Шаг 76 — Translate overview, system, objects, devices and tags pages

Переведены страницы:

- Overview;
- System;
- Objects;
- Devices;
- Tags.

Результат:

- overview page стала русскоязычной;
- System page показывает русские подписи и описания;
- Objects page переведена;
- Devices page переведена;
- Tags page переведена;
- mock-описания backend modules переведены.

Технические module codes не переводились:

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

---

## Шаг 77 — Translate runtime, events and alarms pages

Переведены страницы:

- Runtime;
- Events;
- Alarms.

Результат:

- Runtime page отображается как «Текущие значения»;
- Events page отображается как «События»;
- Alarms page отображается как «Аварии»;
- колонки таблиц переведены;
- summary cards переведены;
- quality/severity/priority/state отображаются на русском;
- demo-data сообщения переведены.

Технические коды тегов, событий и аварий не переводились.

---

## Шаг 78 — Add UI state text foundation

Добавлен общий UI state foundation.

Созданы:

- `frontend/Dispatcher.Frontend/Components/UiStatePanel.razor`;
- `frontend/Dispatcher.Frontend/Models/UiStateKind.cs`;
- `frontend/Dispatcher.Frontend/Services/UiText.cs`.

Добавлены UI states:

- Info;
- Loading;
- Empty;
- Warning;
- Error;
- MockData;
- NotImplemented;
- NoConnection.

Пользовательские тексты отображаются на русском.

Использование:

- System page использует общий блок «Демо-данные»;
- Runtime page использует общий блок «Демо-данные»;
- Events page использует общий блок «Демо-данные»;
- Alarms page использует общий блок «Демо-данные»;
- Objects/Devices/Tags pages используют общий блок «Функция пока не реализована».

---

## Шаг 79 — Update frontend docs and improve loading screen

Обновлены документы:

- `frontend/README.md`;
- `frontend/Dispatcher.Frontend/SERVICE.md`.

Улучшен стартовый экран загрузки Blazor WebAssembly:

- добавлен брендовый loading panel;
- добавлен логотип `D`;
- добавлен текст «Загрузка интерфейса»;
- добавлен spinner;
- добавлена анимированная полоса загрузки;
- добавлен русский текст стартового экрана.

Изменены:

- `frontend/Dispatcher.Frontend/wwwroot/index.html`;
- `frontend/Dispatcher.Frontend/wwwroot/css/app.css`.

Цель:

- при первом открытии `http://localhost:5030` пользователь видит аккуратный стартовый экран Dispatcher до загрузки WebAssembly.

---

## Шаг 80 — Sprint 011 docs update and closure

Создан:

- `docs/SPRINT_011_SUMMARY.md`.

Обновлены:

- `docs/development-log.md`;
- `docs/known-limitations.md`.

Sprint 011 закрыт.

---

# Итог Sprint 011

В результате Sprint 011 frontend стал русскоязычным по умолчанию.

Реализовано:

- UI terminology foundation;
- русская navigation;
- русские основные страницы;
- русские operator pages;
- русские UI state messages;
- общий `UiStatePanel`;
- улучшенный loading screen;
- обновленная frontend документация.

---

# Текущий frontend status

Frontend находится на этапе:

    post-MVP foundation

Текущий frontend поддерживает:

- русскоязычный operator shell;
- русскую navigation;
- русские страницы;
- demo-data предупреждения;
- not implemented состояния;
- стартовый loading screen;
- API client placeholder.

---

# Что осталось техническим и не переводилось

Не переводились:

- route paths;
- file names;
- class names;
- namespaces;
- method names;
- backend module codes;
- API paths;
- enum values в коде;
- commit messages;
- protocol names;
- internal IDs.

Причина:

- это технические контракты;
- они не являются пользовательскими UI-подписями.

---

# Что осталось временным

Временными остаются:

- demo-data;
- mock API client;
- frontend placeholders;
- английские технические status values внутри моделей;
- отсутствие localization engine;
- отсутствие real backend HTTP API;
- отсутствие realtime client.

---

# Принятая языковая политика

Основной язык UI:

    русский

Будущая локализация:

    английский позже

Правило:

    новый пользовательский frontend-текст пишем на русском языке

---

# Build/run status

Sprint 011 закрывается при выполнении проверок:

Frontend project:

    dotnet build frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Frontend solution:

    dotnet build frontend\Dispatcher.Frontend.slnx

Frontend run:

    dotnet run --project frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Backend configure/build:

    cmake --preset x64-debug
    cmake --build --preset x64-debug

Backend run:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

---

# Следующий рекомендуемый спринт

Следующий рекомендуемый спринт:

    Sprint 012 — Backend HTTP API Transport Foundation

Цель Sprint 012:

- добавить реальный backend HTTP transport;
- реализовать `/api/system/health`;
- реализовать `/api/system/modules`;
- подготовить замену frontend demo-data на реальные backend данные.

---

# Статус закрытия

Sprint 011 считается закрытым после выполнения условий:

- frontend build прошел;
- frontend run прошел;
- backend build прошел;
- backend run прошел;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.

Статус:

    Sprint 011 closed
