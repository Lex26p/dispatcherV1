using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Tests.Models;

public sealed class DeviceTagPagePresentationTests
{
    [Theory]
    [InlineData("Draft", "Черновик")]
    [InlineData("Commissioning", "Пусконаладка")]
    [InlineData("Active", "Активно")]
    [InlineData("Maintenance", "Обслуживание")]
    [InlineData("Disabled", "Отключено")]
    [InlineData("OutOfService", "Вне работы")]
    [InlineData(
        "Decommissioned",
        "Выведено из эксплуатации"
    )]
    [InlineData("Simulation", "Симуляция")]
    [InlineData("FutureState", "FutureState")]
    public void FormatDeviceState_ReturnsPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            DeviceTagPagePresentation
                .FormatDeviceState(source)
        );
    }

    [Theory]
    [InlineData("ModbusTcp", "Modbus TCP")]
    [InlineData("Snmp", "SNMP")]
    [InlineData("Simulator", "Симулятор")]
    [InlineData("Custom", "Пользовательский")]
    [InlineData("FutureProtocol", "FutureProtocol")]
    public void FormatProtocol_ReturnsPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            DeviceTagPagePresentation
                .FormatProtocol(source)
        );
    }

    [Theory]
    [InlineData("DeviceTag", "Тег устройства")]
    [InlineData("ManualTag", "Ручной тег")]
    [InlineData("CalculatedTag", "Расчетный тег")]
    [InlineData("SystemTag", "Системный тег")]
    [InlineData("VirtualTag", "Виртуальный тег")]
    [InlineData("CommandTag", "Командный тег")]
    [InlineData("SimulationTag", "Тег симуляции")]
    [InlineData("ExternalTag", "Внешний тег")]
    [InlineData("FutureTag", "FutureTag")]
    public void FormatTagType_ReturnsPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            DeviceTagPagePresentation
                .FormatTagType(source)
        );
    }

    [Theory]
    [InlineData("Boolean", "Логический")]
    [InlineData("Int32", "Целое 32")]
    [InlineData("Int64", "Целое 64")]
    [InlineData("Float", "Число Float")]
    [InlineData("Double", "Число Double")]
    [InlineData("String", "Строка")]
    [InlineData("Enum", "Перечисление")]
    [InlineData("Json", "JSON")]
    [InlineData("FutureValue", "FutureValue")]
    public void FormatValueType_ReturnsPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            DeviceTagPagePresentation
                .FormatValueType(source)
        );
    }

    [Theory]
    [InlineData("Disabled", "Не архивируется")]
    [InlineData("OnChange", "При изменении")]
    [InlineData("Periodic", "Периодически")]
    [InlineData(
        "OnChangeWithDeadband",
        "При изменении с зоной нечувствительности"
    )]
    [InlineData(
        "OnQualityChange",
        "При изменении качества"
    )]
    [InlineData("OnAlarm", "При аварии")]
    [InlineData(
        "AlwaysButThrottled",
        "Всегда с ограничением частоты"
    )]
    [InlineData("FuturePolicy", "FuturePolicy")]
    public void FormatArchivePolicy_ReturnsPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            DeviceTagPagePresentation
                .FormatArchivePolicy(source)
        );
    }

    [Fact]
    public void FormatRelationship_DistinguishesMissingValue()
    {
        Assert.Equal(
            "Не назначено",
            DeviceTagPagePresentation
                .FormatRelationship(null)
        );

        Assert.Equal(
            "Не назначено",
            DeviceTagPagePresentation
                .FormatRelationship(" ")
        );

        Assert.Equal(
            "room-control",
            DeviceTagPagePresentation
                .FormatRelationship("room-control")
        );
    }

    [Fact]
    public void DeviceCounters_ReturnExpectedValues()
    {
        DeviceViewModel[] devices =
        [
            new()
            {
                ObjectId = "room-control",
                RuntimeEnabled = true
            },
            new()
            {
                ObjectId = "line-water",
                RuntimeEnabled = true
            },
            new()
            {
                ObjectId = null,
                RuntimeEnabled = false
            }
        ];

        Assert.Equal(
            2,
            DeviceTagPagePresentation
                .CountRuntimeEnabled(devices)
        );

        Assert.Equal(
            2,
            DeviceTagPagePresentation
                .CountAssignedDevices(devices)
        );
    }

    [Fact]
    public void TagCounters_ReturnExpectedValues()
    {
        TagViewModel[] tags =
        [
            new()
            {
                DeviceId = "device-1",
                ArchivePolicy = "OnChange",
                Enabled = true
            },
            new()
            {
                DeviceId = null,
                ArchivePolicy = "Disabled",
                Enabled = true
            },
            new()
            {
                DeviceId = "device-2",
                ArchivePolicy = "Disabled",
                Enabled = false
            }
        ];

        Assert.Equal(
            2,
            DeviceTagPagePresentation
                .CountEnabledTags(tags)
        );

        Assert.Equal(
            1,
            DeviceTagPagePresentation
                .CountArchivedTags(tags)
        );

        Assert.Equal(
            2,
            DeviceTagPagePresentation
                .CountDeviceLinkedTags(tags)
        );
    }
}
