# Dispatcher Frontend Objects Page

## Статус

Real backend integration создана в Sprint 015, шаг 114.

## Route

    /objects

## Data sources

Страница использует:

    IConfigurationApiClient.GetObjectTreeAsync
    IConfigurationApiClient.GetObjectsAsync

Запросы выполняются параллельно.

Frontend не содержит Object demo-data.

## Представления

Страница показывает:

- summary cards;
- Object tree;
- flat Object collection.

Summary:

    total object count
    root count
    maximum tree depth

## Object tree

Recursive component:

    ObjectTreeNodeView

Component отображает:

- type icon;
- name;
- ID;
- localized type;
- code;
- description;
- nested children.

Hierarchy приходит готовой из backend.

Frontend не реконструирует tree из flat collection.

## Flat collection

Таблица отображает:

- code;
- name;
- description;
- localized type;
- parent ID;
- object ID.

Flat collection является техническим дополнительным представлением.

## Loading

При начальной загрузке и refresh отображается progress state.

Кнопка refresh блокируется во время активного request.

## Cancellation

Перед новым refresh:

- предыдущий CancellationTokenSource отменяется;
- предыдущий CancellationTokenSource освобождается;
- создается новый request scope.

При уничтожении component активный request отменяется.

## Empty state

Если Object tree и Object collection пусты:

    UiStateKind.Empty

Пустой результат не считается backend error.

## Error states

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

    ObjectPagePresentation

Он отвечает за:

- русские Object type labels;
- root/parent presentation;
- maximum tree depth calculation.

## Object type labels

Поддерживаются:

    Site
    Building
    Floor
    Room
    Zone
    Cabinet
    Rack
    Line
    EquipmentGroup
    Custom

Неизвестный будущий type отображается исходной строкой.

## Ограничения

На шаге 114 отсутствуют:

- Object selection;
- search;
- filtering UI;
- pagination controls;
- create;
- update;
- delete;
- drag-and-drop;
- commands;
- authorization;
- persisted UI state.
