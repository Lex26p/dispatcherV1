# Dispatcher Frontend Devices and Tags Pages

## Статус

Real backend integration создана в Sprint 015, шаг 115.

## Routes

    /devices
    /tags

## Data sources

Devices page:

    IConfigurationApiClient.GetDevicesAsync

Tags page:

    IConfigurationApiClient.GetTagsAsync

Frontend Device и Tag demo-data не используются.

## Devices page

Страница показывает:

- total device count;
- runtime enabled count;
- object-assigned count;
- code;
- name;
- ID;
- protocol;
- configuration state;
- object relationship;
- connection summary;
- IP mode;
- network interface;
- runtime state.

## Device disabled filter

По умолчанию backend скрывает:

    Disabled
    Decommissioned

UI toggle:

    Показывать отключенные

изменяет:

    DeviceReadRequest.IncludeDisabled

Изменение toggle автоматически выполняет новый request.

## Runtime state

Runtime enabled определяется backend model.

Frontend отображает готовый:

    runtimeEnabled

Frontend не вычисляет runtime eligibility из state самостоятельно.

## Tags page

Страница показывает:

- total tag count;
- enabled tag count;
- archived tag count;
- device-linked tag count;
- code;
- name;
- ID;
- Tag type;
- value type;
- object relationship;
- device relationship;
- engineering unit;
- scale;
- offset;
- address;
- archive policy;
- enabled state.

## Tag disabled filter

По умолчанию backend скрывает:

    enabled = false

UI toggle:

    Показывать отключенные

изменяет:

    TagReadRequest.IncludeDisabled

Archive policy `Disabled` не считается disabled configuration state.

## Loading and refresh

Обе страницы поддерживают:

- initial loading;
- manual refresh;
- loading indicator;
- disabled refresh button;
- last update timestamp.

## Cancellation

Перед новым request:

- предыдущий CancellationTokenSource отменяется;
- предыдущий CancellationTokenSource освобождается;
- создается новый request scope.

При уничтожении component активный request отменяется.

## Empty state

Валидный пустой collection отображается через:

    UiStateKind.Empty

Пустой collection не считается backend error.

## Error state

Connection и timeout:

    UiStateKind.NoConnection

Другие ошибки:

    UiStateKind.Error

Для backend errors отображаются:

- message;
- endpoint;
- API error code;
- correlation ID.

## Presentation helper

Создан:

    DeviceTagPagePresentation

Он отвечает за:

- русские Device state labels;
- protocol labels;
- Tag type labels;
- value type labels;
- archive policy labels;
- relationship fallback;
- Device counters;
- Tag counters.

Неизвестные будущие string values отображаются без потери исходного значения.

## Ограничения

На шаге 115 отсутствуют:

- create;
- update;
- delete;
- device commands;
- tag commands;
- current values;
- quality;
- timestamps;
- history;
- alarms;
- realtime subscriptions;
- search;
- advanced filters;
- pagination controls;
- object and device name lookup;
- authorization.
