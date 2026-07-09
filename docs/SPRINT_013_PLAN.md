# Sprint 013 Plan

## Название спринта

Frontend Real API Client Integration

## Статус

Planned

## Этап

Post-MVP vertical integration

## Диапазон шагов

Шаги 91–98.

---

# Цель спринта

Цель Sprint 013 — подключить frontend Dispatcher к реальному backend HTTP API, созданному в Sprint 012.

После Sprint 012 backend уже запускает Drogon HTTP server и предоставляет первые endpoints:

    GET /api/system/health
    GET /api/system/modules

После Sprint 013 frontend должен начать использовать реальные backend-данные хотя бы на странице:

    /system

---

# Главный результат спринта

После Sprint 013 страница frontend:

    Система

должна получать данные из backend:

    http://127.0.0.1:8080/api/system/health
    http://127.0.0.1:8080/api/system/modules

и показывать реальные состояния:

- backend доступен;
- backend недоступен;
- загрузка;
- ошибка;
- список backend-модулей из backend API.

---

# Текущая ситуация

Frontend сейчас имеет:

- русскоязычный UI;
- `IDispatcherApiClient`;
- `DispatcherApiClient`;
- `DispatcherApiClientOptions`;
- mock/demo data для System/Runtime/Events/Alarms;
- `UiStatePanel`;
- страницы с loading/mock/not implemented state foundation.

Backend сейчас имеет:

- Drogon HTTP server;
- `/api/system/health`;
- `/api/system/modules`;
- vcpkg + Drogon foundation;
- localhost HTTP API на порту 8080.

---

# Архитектурное решение

Frontend должен использовать typed API client:

    IDispatcherApiClient
        -> DispatcherApiClient
            -> HttpClient
            -> backend HTTP API

Frontend UI не должен напрямую вызывать `HttpClient`.

Страницы должны работать через:

    IDispatcherApiClient

---

# Frontend API settings

В Sprint 013 нужно добавить frontend-side API settings.

Рекомендуемая модель:

    DispatcherApiClientOptions
        BaseUrl
        ApiBasePath
        UseMockData
        RequestTimeoutSeconds

На старте:

    BaseUrl = http://127.0.0.1:8080
    ApiBasePath = /api
    UseMockData = false

Mock-data можно оставить как fallback mode, но не как основной режим System page.

---

# CORS

Так как Blazor WebAssembly frontend запускается на своем localhost port, а backend API работает на другом port, frontend будет делать cross-origin request.

Поэтому Sprint 013 должен добавить development CORS policy на backend.

Development policy:

- разрешить frontend localhost origins;
- разрешить GET;
- разрешить basic headers;
- пока не открывать production CORS policy.

Минимально ожидаемые origins:

    http://localhost:5030
    http://127.0.0.1:5030
    https://localhost:5030

Точный порт может отличаться по launchSettings, поэтому policy должна быть легко расширяемой.

---

# API response mapping

Backend `/api/system/health` возвращает JSON health response.

Frontend должен маппить его в отдельную модель:

    BackendHealthViewModel

Backend `/api/system/modules` возвращает JSON со списком модулей.

Frontend должен маппить его в модели:

    BackendModulesResponse
    BackendModuleViewModel

---

# UI states

System page должна поддерживать состояния:

- Loading;
- Connected;
- NoConnection;
- Error;
- Empty.

Используем существующий `UiStatePanel`.

---

# In scope

В Sprint 013 входит:

- создать Sprint 013 plan;
- добавить backend development CORS foundation;
- добавить frontend API response models;
- добавить frontend real HTTP client calls;
- подключить `/api/system/health`;
- подключить `/api/system/modules`;
- обновить System page;
- добавить loading/no connection/error states;
- сохранить mock mode как fallback/development option;
- обновить документацию;
- закрыть Sprint 013.

---

# Out of scope

В Sprint 013 не входит:

- runtime API integration;
- events API integration;
- alarms API integration;
- object/device/tag API endpoints;
- authentication;
- authorization;
- token handling;
- role-based UI;
- WebSocket/SSE;
- production CORS;
- OpenAPI/Swagger;
- deployment configuration.

---

# План шагов

## Шаг 91 — Sprint 013 plan and frontend API integration rules

Результат:

- создан `docs/SPRINT_013_PLAN.md`;
- правила интеграции frontend/backend зафиксированы.

## Шаг 92 — Add backend development CORS foundation

Результат:

- backend добавляет CORS headers для development frontend origins;
- frontend сможет обращаться к backend API из браузера.

## Шаг 93 — Add frontend API DTOs and options

Результат:

- добавлены DTO для `/api/system/health`;
- добавлены DTO для `/api/system/modules`;
- расширены API client options.

## Шаг 94 — Implement real System API client calls

Результат:

- `DispatcherApiClient` умеет вызывать реальный backend;
- mock mode остается как fallback option.

## Шаг 95 — Update System page for real backend states

Результат:

- System page показывает loading state;
- показывает backend health;
- показывает real modules;
- показывает no connection/error state.

## Шаг 96 — End-to-end frontend/backend local integration check

Результат:

- backend запущен на 127.0.0.1:8080;
- frontend запущен через dotnet run;
- System page получает реальные backend данные.

## Шаг 97 — Frontend service docs update

Результат:

- обновлены frontend service docs;
- описан API base URL;
- описан real/mock mode;
- описаны ограничения Sprint 013.

## Шаг 98 — Sprint 013 docs update and closure

Результат:

- создан `docs/SPRINT_013_SUMMARY.md`;
- обновлены `docs/development-log.md`;
- обновлены `docs/known-limitations.md`;
- Sprint 013 закрыт.

---

# Acceptance criteria

Sprint 013 считается выполненным, если:

- backend продолжает собираться;
- frontend продолжает собираться;
- backend запускается;
- frontend запускается;
- browser frontend может вызвать backend API;
- `/system` показывает real backend modules;
- `/system` показывает состояние backend health;
- при остановленном backend frontend показывает no connection/error state;
- mock mode не удален полностью;
- docs обновлены;
- изменения отправлены в `master`.

---

# Проверка backend API

Backend:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

PowerShell:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/health
    Invoke-RestMethod http://127.0.0.1:8080/api/system/modules

---

# Проверка frontend

Frontend:

    dotnet run --project frontend\Dispatcher.Frontend\Dispatcher.Frontend.csproj

Browser:

    http://localhost:5030/system

Ожидаемый результат:

- страница открывается;
- нет CORS ошибки;
- нет demo-only сообщения для System modules;
- виден список backend-модулей из реального API;
- visible state объясняет, что подключение к backend активно.

---

# Следующий спринт после Sprint 013

Рекомендуемый следующий спринт:

    Sprint 014 — Backend Read API Foundation for Object/Device/Tag

Цель:

- добавить backend read endpoints для objects/devices/tags;
- подготовить frontend pages к real data beyond system page.
