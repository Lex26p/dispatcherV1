namespace Dispatcher.Frontend.Services;

public static class UiText
{
    public const string DemoDataTitle = "Демо-данные";

    public const string NotImplementedTitle = "Функция пока не реализована";

    public const string NoConnectionTitle = "Нет подключения";

    public const string LoadingTitle = "Загрузка";

    public const string EmptyTitle = "Нет данных";

    public const string ErrorTitle = "Ошибка";

    public const string SystemDemoDataMessage =
        "На этой странице пока отображаются демо-данные. Реальное подключение к backend API будет добавлено после реализации HTTP transport.";

    public const string RuntimeDemoDataMessage =
        "На этой странице пока отображаются демо-данные. Реальное подключение к /api/runtime/values будет добавлено позже.";

    public const string EventsDemoDataMessage =
        "На этой странице пока отображаются демо-события. Реальное подключение к /api/events будет добавлено позже.";

    public const string AlarmsDemoDataMessage =
        "На этой странице пока отображаются демо-аварии. Реальное подключение к /api/alarms/active будет добавлено позже.";

    public const string ObjectsNotImplementedMessage =
        "Реальное дерево объектов будет подключено после реализации backend HTTP API и репозиториев.";

    public const string DevicesNotImplementedMessage =
        "Реальные устройства будут подключены после появления API и слоя хранения данных.";

    public const string TagsNotImplementedMessage =
        "Реальные теги и текущие значения будут подключены через backend API на следующих этапах.";

    public const string GenericLoadingMessage =
        "Данные загружаются. Подождите несколько секунд.";

    public const string GenericEmptyMessage =
        "Нет данных для отображения.";

    public const string GenericErrorMessage =
        "Не удалось загрузить данные. Проверьте подключение к backend и повторите попытку.";

    public const string GenericNoConnectionMessage =
        "Связь с backend пока недоступна. Проверьте адрес API и состояние сервера.";
}
