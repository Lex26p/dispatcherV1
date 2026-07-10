# Sprint 013 Plan

## Название

Frontend Real API Client Integration

## Статус

Completed

## Этап

Post-MVP vertical integration

## Диапазон шагов

Шаги 91–98.

## Цель

Подключить Blazor WebAssembly frontend Dispatcher к реальному backend HTTP API, созданному в Sprint 012.

Первая область реальной интеграции:

    /system

Backend endpoints:

    GET /api/system/health
    GET /api/system/modules

## Результат

Цель спринта достигнута.

Страница «Система»:

- получает health из реального backend;
- получает список зарегистрированных backend-модулей;
- показывает loading state;
- показывает состояние соединения;
- показывает ошибки HTTP и JSON;
- показывает timeout;
- показывает пустой список модулей;
- поддерживает явный mock mode;
- восстанавливает соединение после повторного запуска backend;
- позволяет повторить загрузку кнопкой «Обновить».

## Реализованная frontend-архитектура

Для System API введена отдельная граница:

    ISystemApiClient
        SystemHttpApiClient
        MockSystemApiClient

`SystemHttpApiClient` отвечает за реальные HTTP-запросы.

`MockSystemApiClient` используется только при явно включенном mock mode.

Legacy-клиент:

    IDispatcherApiClient
    DispatcherApiClient

временно сохраняется для demo-data Runtime, Events и Alarms.

Frontend pages не должны напрямую использовать `HttpClient`.

## API configuration

Настройки frontend находятся в:

    frontend/Dispatcher.Frontend/wwwroot/appsettings.json

Секция:

    DispatcherApi

Development configuration:

    BaseUrl = http://127.0.0.1:8080
    ApiBasePath = /api
    UseMockData = false
    RequestTimeoutSeconds = 10

Blazor WebAssembly configuration доступна браузеру и не должна содержать секреты.

## DTO и view models

Добавлены transport models:

    SystemHealthApiResponse
    SystemModulesApiResponse
    SystemModuleApiDto

Добавлена health view model:

    BackendHealthViewModel

Backend transport DTO отделены от пользовательских view models.

## Обработка ошибок

Добавлены:

    DispatcherApiException
    DispatcherApiErrorKind

Категории ошибок:

    Connection
    Timeout
    HttpStatus
    InvalidResponse
    Configuration

UI не зависит напрямую от исключений `HttpClient`.

## CORS

В backend `scada_http` добавлена development CORS policy.

Development origins:

    http://localhost:5030
    http://127.0.0.1:5030
    https://localhost:5030

OPTIONS preflight обрабатывается на уровне Drogon pre-routing advice.

Неизвестные origins не получают `Access-Control-Allow-Origin`.

## Local integration smoke-test

Добавлен скрипт:

    scripts/test-local-integration.ps1

Скрипт проверяет:

- system health;
- system modules;
- обязательные поля модулей;
- количество модулей;
- CORS GET response;
- OPTIONS preflight;
- отсутствие CORS-разрешения для неизвестного origin.

Успешный результат:

    SMOKE TEST PASSED

## Выполненные шаги

### Шаг 91

Создан план Sprint 013 и зафиксированы правила frontend/backend integration.

### Шаг 92

Добавлен backend development CORS foundation.

### Шаг 93

Добавлены frontend API settings, DTO и view models.

### Шаг 94

Добавлены отдельные real и mock System API clients.

### Шаг 95

Страница «Система» подключена к реальному backend API.

### Шаг 96

Добавлен local integration smoke-test и исправлена обработка CORS preflight.

### Шаг 97

Актуализирована frontend service documentation.

### Шаг 98

Обновлена документация проекта и Sprint 013 закрыт.

## Acceptance criteria

Выполнено:

- backend собирается;
- frontend собирается;
- backend запускается;
- frontend запускается;
- browser frontend вызывает backend API;
- System page показывает real backend health;
- System page показывает real backend modules;
- CORS работает;
- OPTIONS preflight работает;
- неизвестный origin не получает CORS-разрешение;
- состояние отключенного backend отображается корректно;
- соединение восстанавливается без перезапуска frontend;
- mock mode сохранен как явная настройка;
- smoke-test проходит;
- документация обновлена.

## Следующий спринт

    Sprint 014 — Engineering Baseline

Основные направления:

- backend unit-test foundation;
- frontend client tests;
- CTest integration;
- Windows CI;
- Linux CI;
- Linux CMake preset;
- единый JSON serialization foundation;
- единый API error envelope;
- correlation ID;
- актуальный current-status document.

Object, Device и Tag Read API переносятся на Sprint 015.
