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
                TagName = "Pump 01 Status",
                ObjectPath = "/Demo/Pump Station/Pump 01",
                DeviceName = "Simulator Device",
                Value = "Running",
                Unit = "",
                Quality = "Good",
                Source = "Mock runtime",
                Timestamp = now.AddSeconds(-4)
            },
            new()
            {
                TagCode = "PUMP_01_PRESSURE",
                TagName = "Pump 01 Pressure",
                ObjectPath = "/Demo/Pump Station/Pump 01",
                DeviceName = "Simulator Device",
                Value = "4.20",
                Unit = "bar",
                Quality = "Good",
                Source = "Mock runtime",
                Timestamp = now.AddSeconds(-6)
            },
            new()
            {
                TagCode = "TANK_01_LEVEL",
                TagName = "Tank 01 Level",
                ObjectPath = "/Demo/Tank Farm/Tank 01",
                DeviceName = "Simulator Device",
                Value = "72.5",
                Unit = "%",
                Quality = "Good",
                Source = "Mock runtime",
                Timestamp = now.AddSeconds(-8)
            },
            new()
            {
                TagCode = "LINE_01_FLOW",
                TagName = "Line 01 Flow",
                ObjectPath = "/Demo/Pipe Line/Line 01",
                DeviceName = "Simulator Device",
                Value = "18.7",
                Unit = "m³/h",
                Quality = "Uncertain",
                Source = "Mock runtime",
                Timestamp = now.AddSeconds(-12)
            },
            new()
            {
                TagCode = "REMOTE_DEVICE_SIGNAL",
                TagName = "Remote Device Signal",
                ObjectPath = "/Demo/Remote Site/Device 01",
                DeviceName = "Remote Simulator",
                Value = "N/A",
                Unit = "",
                Quality = "CommunicationError",
                Source = "Mock runtime",
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
                Message = "Pump 01 status changed to Running.",
                CorrelationId = "mock-runtime-0001"
            },
            new()
            {
                Id = "EVT-0002",
                Timestamp = now.AddMinutes(-2),
                Category = "Polling",
                Severity = "Warning",
                SourceType = "Device",
                SourceName = "Remote Simulator",
                Message = "Remote simulator polling quality changed to CommunicationError.",
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
                Message = "History sample accepted by historian buffer.",
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
                Message = "High level alarm raised for Tank 01.",
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
                ObjectPath = "/Demo/Tank Farm/Tank 01",
                SourceName = "TANK_01_HIGH_LEVEL",
                Severity = "Critical",
                Priority = "High",
                State = "Active",
                Message = "Tank 01 level is above high threshold.",
                Acknowledged = false
            },
            new()
            {
                Id = "ALM-0002",
                RaisedAt = now.AddMinutes(-2),
                ObjectPath = "/Demo/Remote Site/Device 01",
                SourceName = "REMOTE_DEVICE_SIGNAL",
                Severity = "Warning",
                Priority = "Medium",
                State = "Active",
                Message = "Remote device communication quality is bad.",
                Acknowledged = true
            }
        ];

        return Task.FromResult(alarms);
    }
}