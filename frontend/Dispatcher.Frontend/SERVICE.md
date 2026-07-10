# Dispatcher.Frontend

## Назначение

`Dispatcher.Frontend` — браузерное frontend-приложение универсальной диспетчерской платформы Dispatcher.

Frontend предоставляет операторский интерфейс для наблюдения, управления, настройки и диагностики системы.

## Технологии

Frontend использует:

- .NET 10;
- Blazor WebAssembly;
- MudBlazor;
- `HttpClient`;
- `System.Text.Json`.

## Статус

Post-MVP vertical integration.

Страница «Система» подключена к реальному backend HTTP API.

Остальные прикладные страницы пока используют placeholder или demo-data до появления соответствующих backend endpoints.

## Язык интерфейса

Основной язык пользовательского интерфейса:

    русский

Новые пользовательские тексты должны добавляться на русском языке.

URL routes и внутренние технические идентификаторы пока остаются на английском языке.

## Текущие страницы

    /
    /system
    /objects
    /devices
    /tags
    /runtime
    /events
    /alarms
    /not-found

## Реальная System integration

Страница:

    /system

использует:

    ISystemApiClient

Доступны две реализации:

    SystemHttpApiClient
    MockSystemApiClient

### SystemHttpApiClient

Выполняет реальные HTTP-запросы:

    GET /api/system/health
    GET /api/system/modules

Возвращает frontend view models:

    BackendHealthViewModel
    BackendModuleViewModel

Использует transport DTO:

    SystemHealthApiResponse
    SystemModulesApiResponse
    SystemModuleApiDto

### MockSystemApiClient

Используется только при:

    DispatcherApi:UseMockData = true

Mock mode не должен автоматически включаться при ошибке backend.

Если real backend недоступен, UI должен явно показать состояние отсутствия соединения, а не подменять данные демонстрационными.

## Legacy API client

Пока сохраняются:

    IDispatcherApiClient
    DispatcherApiClient

Они предоставляют demo-data для:

- Runtime;
- Events;
- Alarms;
- части старых overview models.

Legacy client будет постепенно заменяться отдельными typed clients после появления соответствующих backend endpoints.

## API client boundaries

Frontend pages не должны напрямую вызывать `HttpClient`.

Допустимый поток:

    Razor page
        -> typed frontend API interface
        -> HTTP or mock implementation
        -> HttpClient
        -> backend API

Рекомендуемые будущие интерфейсы:

    IObjectApiClient
    IDeviceApiClient
    ITagApiClient
    IRuntimeApiClient
    IEventApiClient
    IAlarmApiClient

Не рекомендуется создавать один универсальный client для всех API областей.

## Dependency injection

Регистрация frontend services выполняется в:

    Program.cs

`DispatcherApiClientOptions` загружается из configuration section:

    DispatcherApi

На основании `UseMockData` выбирается реализация `ISystemApiClient`.

Real mode:

    ISystemApiClient -> SystemHttpApiClient

Mock mode:

    ISystemApiClient -> MockSystemApiClient

## API configuration

Файл development configuration:

    wwwroot/appsettings.json

Параметры:

    BaseUrl
    ApiBasePath
    UseMockData
    RequestTimeoutSeconds

Development values:

    BaseUrl = http://127.0.0.1:8080
    ApiBasePath = /api
    UseMockData = false
    RequestTimeoutSeconds = 10

`DispatcherApiClientOptions` отвечает за:

- нормализацию backend base URL;
- нормализацию API base path;
- формирование endpoint path;
- request timeout;
- выбор real или mock mode.

## Security rule

Blazor WebAssembly configuration загружается в браузер.

Запрещено помещать в frontend configuration:

- пароли;
- database connection strings;
- private API keys;
- bearer tokens;
- private certificates;
- другие server-side secrets.

Будущие access tokens должны выдаваться после authentication и храниться по отдельно принятой security policy.

## Error handling

Transport errors преобразуются в:

    DispatcherApiException

Категории:

    Connection
    Timeout
    HttpStatus
    InvalidResponse
    Configuration

UI должен работать с этими категориями и не зависеть от внутренних исключений `HttpClient`.

Отмена через `CancellationToken` не должна отображаться пользователю как backend error.

## System page states

Страница `/system` поддерживает:

- initial loading;
- connected;
- backend unhealthy;
- no connection;
- timeout;
- HTTP status error;
- invalid JSON;
- configuration error;
- empty modules response;
- mock mode.

Кнопка «Обновить» выполняет повторную загрузку health и modules.

Предыдущий запрос отменяется перед началом нового запроса.

## CORS

Development frontend обычно работает на:

    http://localhost:5030

Backend работает на:

    http://127.0.0.1:8080

Backend `scada_http` разрешает development origins и обрабатывает OPTIONS preflight через Drogon pre-routing advice.

Frontend не должен пытаться обходить browser CORS policy.

Production CORS policy будет настраиваться отдельно.

## UI state foundation

Общий компонент:

    UiStatePanel

Поддерживаемые состояния:

- loading;
- empty;
- warning;
- error;
- not implemented;
- no connection;
- mock data.

Пользовательские сообщения должны объяснять:

- что произошло;
- доступен ли backend;
- можно ли повторить операцию;
- какие действия ожидаются от пользователя.

## Local integration smoke-test

Скрипт:

    scripts/test-local-integration.ps1

Проверяет:

- `/api/system/health`;
- `/api/system/modules`;
- обязательные поля modules;
- CORS для разрешенного origin;
- OPTIONS preflight;
- отсутствие CORS header для неизвестного origin.

Успешный результат:

    SMOKE TEST PASSED

## Сборка

Из корня репозитория:

    dotnet build frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj
    dotnet build frontend/Dispatcher.Frontend.slnx

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

Development URL:

    http://localhost:5030

System page:

    http://localhost:5030/system

## Проверка с backend

Backend:

    .\out\build\x64-debug\backend\apps\dispatcher_server\Debug\dispatcher_server.exe

Smoke-test:

    powershell -ExecutionPolicy Bypass -File scripts/test-local-integration.ps1

Frontend:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

## Текущие ограничения

Пока нет:

- real object/device/tag clients;
- real runtime/events/alarms clients;
- realtime client;
- authentication;
- authorization;
- token handling;
- operator commands;
- alarm acknowledgement;
- dashboards;
- mimic diagrams;
- charts;
- production deployment configuration;
- production CORS configuration;
- localization engine;
- frontend automated tests.

## Следующее инженерное развитие

После завершения Sprint 013 планируется Engineering Baseline:

- frontend client tests;
- backend unit tests;
- Windows/Linux CI;
- stable JSON contract;
- unified API error envelope;
- correlation ID;
- актуальный current-status document.
