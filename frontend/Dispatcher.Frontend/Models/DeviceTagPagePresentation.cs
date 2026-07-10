namespace Dispatcher.Frontend.Models;

public static class DeviceTagPagePresentation
{
    public static string FormatDeviceState(
        string? state
    )
    {
        return state switch
        {
            "Draft" => "Черновик",
            "Commissioning" => "Пусконаладка",
            "Active" => "Активно",
            "Maintenance" => "Обслуживание",
            "Disabled" => "Отключено",
            "OutOfService" => "Вне работы",
            "Decommissioned" => "Выведено из эксплуатации",
            "Simulation" => "Симуляция",
            "Unknown" => "Неизвестно",
            null or "" => "Состояние не указано",
            _ => state
        };
    }

    public static string FormatProtocol(
        string? protocol
    )
    {
        return protocol switch
        {
            "ModbusTcp" => "Modbus TCP",
            "Snmp" => "SNMP",
            "Simulator" => "Симулятор",
            "Custom" => "Пользовательский",
            "Unknown" => "Неизвестный",
            null or "" => "Протокол не указан",
            _ => protocol
        };
    }

    public static string FormatTagType(
        string? type
    )
    {
        return type switch
        {
            "DeviceTag" => "Тег устройства",
            "ManualTag" => "Ручной тег",
            "CalculatedTag" => "Расчетный тег",
            "SystemTag" => "Системный тег",
            "VirtualTag" => "Виртуальный тег",
            "CommandTag" => "Командный тег",
            "SimulationTag" => "Тег симуляции",
            "ExternalTag" => "Внешний тег",
            "Unknown" => "Неизвестный тип",
            null or "" => "Тип не указан",
            _ => type
        };
    }

    public static string FormatValueType(
        string? valueType
    )
    {
        return valueType switch
        {
            "Boolean" => "Логический",
            "Int32" => "Целое 32",
            "Int64" => "Целое 64",
            "Float" => "Число Float",
            "Double" => "Число Double",
            "String" => "Строка",
            "Enum" => "Перечисление",
            "Json" => "JSON",
            "Unknown" => "Неизвестный тип",
            null or "" => "Тип значения не указан",
            _ => valueType
        };
    }

    public static string FormatArchivePolicy(
        string? policy
    )
    {
        return policy switch
        {
            "Disabled" => "Не архивируется",
            "OnChange" => "При изменении",
            "Periodic" => "Периодически",
            "OnChangeWithDeadband" =>
                "При изменении с зоной нечувствительности",
            "OnQualityChange" =>
                "При изменении качества",
            "OnAlarm" => "При аварии",
            "AlwaysButThrottled" =>
                "Всегда с ограничением частоты",
            "Unknown" => "Неизвестная политика",
            null or "" => "Политика не указана",
            _ => policy
        };
    }

    public static string FormatRelationship(
        string? id
    )
    {
        return string.IsNullOrWhiteSpace(id)
            ? "Не назначено"
            : id;
    }

    public static string FormatRuntimeState(
        bool enabled
    )
    {
        return enabled
            ? "Runtime включен"
            : "Runtime выключен";
    }

    public static string FormatTagState(
        bool enabled
    )
    {
        return enabled
            ? "Тег включен"
            : "Тег отключен";
    }

    public static int CountRuntimeEnabled(
        IReadOnlyList<DeviceViewModel> devices
    )
    {
        ArgumentNullException.ThrowIfNull(devices);

        return devices.Count(
            static device =>
                device.RuntimeEnabled
        );
    }

    public static int CountAssignedDevices(
        IReadOnlyList<DeviceViewModel> devices
    )
    {
        ArgumentNullException.ThrowIfNull(devices);

        return devices.Count(
            static device =>
                device.HasObject
        );
    }

    public static int CountEnabledTags(
        IReadOnlyList<TagViewModel> tags
    )
    {
        ArgumentNullException.ThrowIfNull(tags);

        return tags.Count(
            static tag =>
                tag.Enabled
        );
    }

    public static int CountArchivedTags(
        IReadOnlyList<TagViewModel> tags
    )
    {
        ArgumentNullException.ThrowIfNull(tags);

        return tags.Count(
            static tag =>
                !string.Equals(
                    tag.ArchivePolicy,
                    "Disabled",
                    StringComparison.Ordinal
                )
        );
    }

    public static int CountDeviceLinkedTags(
        IReadOnlyList<TagViewModel> tags
    )
    {
        ArgumentNullException.ThrowIfNull(tags);

        return tags.Count(
            static tag =>
                tag.HasDevice
        );
    }
}
