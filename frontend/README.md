# Dispatcher Frontend

## Назначение

Каталог `frontend` содержит frontend-часть платформы Dispatcher.

Основной проект:

    Dispatcher.Frontend

Технологии:

- .NET 10;
- Blazor WebAssembly;
- MudBlazor.

## Текущий статус

Frontend находится на этапе post-MVP vertical integration.

Реализовано:

- операторский UI shell;
- русскоязычная навигация;
- страницы основных подсистем;
- общий компонент отображения состояний;
- real backend integration для страницы «Система»;
- mock foundation для подсистем, backend API которых еще не реализован.

## Язык интерфейса

Основной язык пользовательского интерфейса:

    русский

Английский язык будет добавлен позже как локализация.

Технические URL routes пока остаются на английском языке.

## Структура

    frontend/
        Dispatcher.Frontend/
        Dispatcher.Frontend.slnx
        README.md

`Dispatcher.Frontend` — Blazor WebAssembly приложение.

`Dispatcher.Frontend.slnx` — отдельная frontend solution.

## Доступные страницы

    /
    /system
    /objects
    /devices
    /tags
    /runtime
    /events
    /alarms

Страница `/system` подключена к реальному backend API.

Остальные прикладные страницы пока используют placeholder или demo-data, пока соответствующие backend endpoints не реализованы.

## Backend integration

Frontend использует отдельный service layer.

Для страницы «Система»:

    ISystemApiClient
        SystemHttpApiClient
        MockSystemApiClient

`SystemHttpApiClient` выполняет реальные HTTP-запросы:

    GET /api/system/health
    GET /api/system/modules

`MockSystemApiClient` используется только при явно включенном mock mode.

Legacy-клиент:

    IDispatcherApiClient
    DispatcherApiClient

пока обслуживает demo-data для Runtime, Events и Alarms.

Он будет постепенно заменяться отдельными typed API clients по мере появления backend endpoints.

## API configuration

Настройки находятся в:

    frontend/Dispatcher.Frontend/wwwroot/appsettings.json

Секция:

    DispatcherApi

Параметры:

    BaseUrl
    ApiBasePath
    UseMockData
    RequestTimeoutSeconds

Development defaults:

    BaseUrl = http://127.0.0.1:8080
    ApiBasePath = /api
    UseMockData = false
    RequestTimeoutSeconds = 10

Файл `appsettings.json` Blazor WebAssembly доступен браузеру.

В него нельзя помещать:

- пароли;
- API keys;
- private tokens;
- connection strings;
- другие секреты.

## CORS

Frontend обычно запускается на:

    http://localhost:5030

Backend запускается на:

    http://127.0.0.1:8080

Так как адреса имеют разные origins, backend должен разрешать development CORS.

Текущая backend CORS policy включает frontend localhost origins и поддерживает OPTIONS preflight.

## Состояния System page

Страница `/system` поддерживает:

- loading;
- connected;
- no connection;
- timeout;
- backend API error;
- invalid response;
- empty modules list;
- mock mode.

Страница позволяет повторно выполнить запрос кнопкой «Обновить».

После остановки и повторного запуска backend соединение восстанавливается без перезапуска frontend.

## Обработка ошибок

Frontend service layer различает:

    Connection
    Timeout
    HttpStatus
    InvalidResponse
    Configuration

Ошибки HTTP transport преобразуются в:

    DispatcherApiException

UI не должен напрямую обрабатывать `HttpRequestException` или детали `HttpClient`.

## Сборка

Из корня репозитория:

    dotnet build frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj
    dotnet build frontend/Dispatcher.Frontend.slnx

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

Фактический адрес отображается в выводе `dotnet run`.

Обычный development address:

    http://localhost:5030

## Рекомендуемый порядок локального запуска

Первый терминал:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

Второй терминал:

    powershell -ExecutionPolicy Bypass -File scripts/test-local-integration.ps1

Третий терминал:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

После этого открыть:

    http://localhost:5030/system

## Local integration smoke-test

Скрипт:

    scripts/test-local-integration.ps1

Проверяет:

- system health;
- system modules;
- структуру ответа modules;
- CORS GET response;
- OPTIONS preflight;
- отсутствие CORS-разрешения для неизвестного origin.

Успешный результат:

    SMOKE TEST PASSED

## Ограничения

Пока не реализованы:

- real Objects API integration;
- real Devices API integration;
- real Tags API integration;
- real Runtime API integration;
- real Events API integration;
- real Alarms API integration;
- realtime WebSocket/SSE client;
- authentication;
- authorization;
- dashboards;
- mimic diagrams;
- alarm acknowledgement;
- command execution;
- полноценная localization infrastructure.

## Правило развития API clients

Новые подсистемы не должны добавляться в один универсальный frontend client.

Рекомендуемая структура:

    IObjectApiClient
    IDeviceApiClient
    ITagApiClient
    IRuntimeApiClient
    IEventApiClient
    IAlarmApiClient

Каждый client должен отвечать только за свою API область.
