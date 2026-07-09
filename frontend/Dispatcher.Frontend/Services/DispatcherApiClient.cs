using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiClient : IDispatcherApiClient
{
    private readonly HttpClient _httpClient;
    private readonly DispatcherApiClientOptions _options;

    public DispatcherApiClient(
        HttpClient httpClient
    )
    {
        _httpClient = httpClient;

        _options = new DispatcherApiClientOptions
        {
            UseMockData = true
        };
    }

    public Task<IReadOnlyList<BackendModuleViewModel>> GetBackendModulesAsync(
        CancellationToken cancellationToken = default
    )
    {
        IReadOnlyList<BackendModuleViewModel> modules =
        [
            new() { Code = "scada_common", Name = "Общие типы", Description = "Общие типы, ошибки, результаты, идентификаторы и временные метки.", Status = "Running" },
            new() { Code = "scada_contracts", Name = "Контракты", Description = "DTO и контракты для обмена данными между слоями системы.", Status = "Running" },
            new() { Code = "scada_core", Name = "Ядро", Description = "Application context, module contracts и базовые интерфейсы ядра.", Status = "Running" },
            new() { Code = "scada_objects", Name = "Объекты", Description = "Основа дерева объектов, типов объектов и зон ответственности.", Status = "Running" },
            new() { Code = "scada_devices", Name = "Устройства", Description = "Модель устройств, подключений и диагностической информации.", Status = "Running" },
            new() { Code = "scada_tags", Name = "Теги", Description = "Модель тегов, адресации, качества и текущего значения.", Status = "Running" },
            new() { Code = "scada_protocols", Name = "Протоколы", Description = "Контракты драйверов протоколов, capabilities и драйвер симулятора.", Status = "Running" },
            new() { Code = "scada_polling", Name = "Опрос", Description = "Основа задач опроса, групп опроса и scheduler foundation.", Status = "Running" },
            new() { Code = "scada_runtime", Name = "Текущие значения", Description = "Хранилище текущих значений и применение результатов чтения.", Status = "Running" },
            new() { Code = "scada_historian", Name = "История", Description = "Исторические значения, политика архивирования и буфер записи.", Status = "Running" },
            new() { Code = "scada_events", Name = "События", Description = "Основа записей событий, категорий, важности и источников.", Status = "Running" },
            new() { Code = "scada_alarms", Name = "Аварии", Description = "Основа аварий, жизненного цикла, правил и переходов состояния.", Status = "Running" },
            new() { Code = "scada_api", Name = "API-слой", Description = "Маршруты API, response model, mapper foundation и каталог endpoints чтения.", Status = "Running" },
            new() { Code = "scada_realtime", Name = "Реальное время", Description = "Модели сообщений, подписок и каналов обновлений в реальном времени.", Status = "Running" },
            new() { Code = "scada_app", Name = "Приложение", Description = "Application composition и план запуска gateway foundation.", Status = "Running" }
        ];

        return Task.FromResult(modules);
    }

    public Task<GatewaySummaryViewModel> GetGatewaySummaryAsync(
        CancellationToken cancellationToken = default
    )
    {
        var summary = new GatewaySummaryViewModel
        {
            Mode = "ApiAndRealtime",
            StartupStatus = "Prepared",
            BindAddress = "127.0.0.1",
            HttpPort = 8080,
            RealtimePort = 8080,
            ApiBasePath = "/api",
            RealtimePath = "/api/realtime",
            ApiEnabled = true,
            RealtimeEnabled = true,
            RealTransportAvailable = false,
            Message = "План запуска шлюза подготовлен, но реальный HTTP/WebSocket transport пока не реализован."
        };

        return Task.FromResult(summary);
    }

    public Task<ApiRouteSummaryViewModel> GetApiRouteSummaryAsync(
        CancellationToken cancellationToken = default
    )
    {
        var summary = new ApiRouteSummaryViewModel
        {
            RouteCount = 7,
            ReadEndpointCount = 11,
            RealtimeChannelCount = 6,
            UsesMockData = _options.UseMockData,
            Notes = "Количество маршрутов и endpoints чтения основано на foundation-каталогах Sprint 009."
        };

        return Task.FromResult(summary);
    }

    public Task<IReadOnlyList<RuntimeValueViewModel>> GetRuntimeValuesAsync(
        CancellationToken cancellationToken = default
    )
    {
        var now = DateTimeOffset.Now;

        IReadOnlyList<RuntimeValueViewModel> values =
        [
            new()
            {
                TagCode = "PUMP_01_STATUS",
                TagName = "Насос 01: состояние",
                ObjectPath = "/Демо/Насосная станция/Насос 01",
                DeviceName = "Устройство-симулятор",
                Value = "Работает",
                Unit = "",
                Quality = "Good",
                Source = "Демо-источник",
                Timestamp = now.AddSeconds(-4)
            },
            new()
            {
                TagCode = "PUMP_01_PRESSURE",
                TagName = "Насос 01: давление",
                ObjectPath = "/Демо/Насосная станция/Насос 01",
                DeviceName = "Устройство-симулятор",
                Value = "4.20",
                Unit = "бар",
                Quality = "Good",
                Source = "Демо-источник",
                Timestamp = now.AddSeconds(-6)
            },
            new()
            {
                TagCode = "TANK_01_LEVEL",
                TagName = "Резервуар 01: уровень",
                ObjectPath = "/Демо/Резервуарный парк/Резервуар 01",
                DeviceName = "Устройство-симулятор",
                Value = "72.5",
                Unit = "%",
                Quality = "Good",
                Source = "Демо-источник",
                Timestamp = now.AddSeconds(-8)
            },
            new()
            {
                TagCode = "LINE_01_FLOW",
                TagName = "Линия 01: расход",
                ObjectPath = "/Демо/Трубопровод/Линия 01",
                DeviceName = "Устройство-симулятор",
                Value = "18.7",
                Unit = "м³/ч",
                Quality = "Uncertain",
                Source = "Демо-источник",
                Timestamp = now.AddSeconds(-12)
            },
            new()
            {
                TagCode = "REMOTE_DEVICE_SIGNAL",
                TagName = "Удаленное устройство: сигнал",
                ObjectPath = "/Демо/Удаленный объект/Устройство 01",
                DeviceName = "Удаленный симулятор",
                Value = "Н/Д",
                Unit = "",
                Quality = "CommunicationError",
                Source = "Демо-источник",
                Timestamp = now.AddSeconds(-30)
            }
        ];

        return Task.FromResult(values);
    }

    public Task<IReadOnlyList<EventRecordViewModel>> GetEventsAsync(
        CancellationToken cancellationToken = default
    )
    {
        var now = DateTimeOffset.Now;

        IReadOnlyList<EventRecordViewModel> events =
        [
            new()
            {
                Id = "EVT-0001",
                Timestamp = now.AddMinutes(-1),
                Category = "Runtime",
                Severity = "Info",
                SourceType = "Tag",
                SourceName = "PUMP_01_STATUS",
                Message = "Состояние насоса 01 изменилось на «Работает».",
                CorrelationId = "mock-runtime-0001"
            },
            new()
            {
                Id = "EVT-0002",
                Timestamp = now.AddMinutes(-2),
                Category = "Polling",
                Severity = "Warning",
                SourceType = "Device",
                SourceName = "REMOTE_DEVICE_SIGNAL",
                Message = "Качество опроса удаленного устройства изменилось на «Ошибка связи».",
                CorrelationId = "mock-polling-0002"
            },
            new()
            {
                Id = "EVT-0003",
                Timestamp = now.AddMinutes(-4),
                Category = "Historian",
                Severity = "Info",
                SourceType = "Tag",
                SourceName = "TANK_01_LEVEL",
                Message = "Историческое значение принято буфером истории.",
                CorrelationId = "mock-historian-0003"
            },
            new()
            {
                Id = "EVT-0004",
                Timestamp = now.AddMinutes(-7),
                Category = "Alarms",
                Severity = "Critical",
                SourceType = "Alarm",
                SourceName = "TANK_01_HIGH_LEVEL",
                Message = "Возникла авария высокого уровня резервуара 01.",
                CorrelationId = "mock-alarm-0004"
            }
        ];

        return Task.FromResult(events);
    }

    public Task<IReadOnlyList<ActiveAlarmViewModel>> GetActiveAlarmsAsync(
        CancellationToken cancellationToken = default
    )
    {
        var now = DateTimeOffset.Now;

        IReadOnlyList<ActiveAlarmViewModel> alarms =
        [
            new()
            {
                Id = "ALM-0001",
                RaisedAt = now.AddMinutes(-7),
                ObjectPath = "/Демо/Резервуарный парк/Резервуар 01",
                SourceName = "TANK_01_HIGH_LEVEL",
                Severity = "Critical",
                Priority = "High",
                State = "Active",
                Message = "Уровень резервуара 01 выше верхнего аварийного порога.",
                Acknowledged = false
            },
            new()
            {
                Id = "ALM-0002",
                RaisedAt = now.AddMinutes(-2),
                ObjectPath = "/Демо/Удаленный объект/Устройство 01",
                SourceName = "REMOTE_DEVICE_SIGNAL",
                Severity = "Warning",
                Priority = "Medium",
                State = "Active",
                Message = "Качество связи с удаленным устройством плохое.",
                Acknowledged = true
            }
        ];

        return Task.FromResult(alarms);
    }
}