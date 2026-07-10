# ADR 0003 — Testing and CI Strategy

## Статус

Accepted

## Контекст

Dispatcher уже содержит несколько backend domain modules, HTTP transport и первый frontend/backend vertical slice.

До Sprint 014 проект не имел полноценного automated unit-test foundation и CI.

Продолжение расширения API без тестового baseline создает риски:

- незаметные regression;
- platform-specific ошибки;
- нестабильные transport contracts;
- дорогая ручная проверка;
- сложный переход на Linux;
- рост связности между mock и real implementations.

## Решение

### C++ unit tests

Использовать:

    Catch2 v3

Интеграция с CMake:

    CTest
    catch_discover_tests

Catch2 добавляется через vcpkg manifest.

Корневой CMake подключает:

    include(CTest)

Тестовые targets создаются только при:

    BUILD_TESTING=ON

### Frontend tests

Использовать:

    xUnit

Tests находятся в отдельном проекте:

    Dispatcher.Frontend.Tests

Запуск:

    dotnet test

Product frontend project не должен зависеть от xUnit.

### Test isolation

Unit tests не должны зависеть от:

- внешней сети;
- PostgreSQL;
- реального оборудования;
- system clock без abstraction;
- случайных задержек;
- порядка запуска тестов.

### Test organization

Backend tests группируются по backend module.

Frontend tests группируются по service или model.

Один test должен проверять одно наблюдаемое поведение.

### CI

Использовать GitHub Actions.

Обязательные environments:

    Windows
    Ubuntu Linux

Обязательные проверки:

- backend configure;
- backend build;
- CTest;
- frontend build;
- frontend tests;
- LF validation.

### Local integration test

Существующий PowerShell smoke-test остается отдельным уровнем проверки.

Он не заменяет unit tests.

CI может использовать отдельный non-interactive integration runner, если текущий console lifecycle backend мешает надежному запуску.

## Почему Catch2

Catch2 предоставляет зрелую интеграцию с CMake и позволяет регистрировать отдельные test cases в CTest.

Он подходит для модульных C++ libraries и не требует переноса product code в специальный test runner.

## Почему xUnit

xUnit поддерживается стандартным .NET testing workflow и запускается через `dotnet test`.

Он позволяет тестировать frontend services и DTO без запуска браузера.

## Отклоненные варианты

### Только ручное тестирование

Отклонено из-за невозможности надежно предотвращать regressions.

### Только local smoke-test

Отклонено, потому что smoke-test проверяет только запущенную систему и не локализует ошибки отдельных компонентов.

### Собственный C++ test framework

Отклонено из-за ненужной стоимости поддержки.

### Browser tests в качестве первого уровня

Отложено.

Browser tests полезны, но медленнее и сложнее unit tests. Сначала создается быстрый service-level baseline.

### Docker-based CI с первого шага

Отложено.

Docker не требуется для проверки чистых libraries, frontend services и basic cross-platform build.

## Последствия

Положительные:

- regressions обнаруживаются раньше;
- Windows и Linux проверяются регулярно;
- testable design становится архитектурным требованием;
- transport contracts получают executable specification;
- ручные проверки сокращаются.

Отрицательные:

- увеличится время первой сборки после добавления test dependencies;
- CI будет устанавливать vcpkg dependencies;
- часть текущего кода может потребовать небольшого refactoring для testability;
- потребуется поддерживать tests вместе с product code.

## Правило изменений

Если product code трудно тестировать без глобального состояния, скрытых зависимостей или реального времени, сначала согласуется небольшой refactoring.

Тесты не должны закреплять случайные implementation details.

Основной объект проверки:

    observable behavior
