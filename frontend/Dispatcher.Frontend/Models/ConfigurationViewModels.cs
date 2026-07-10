namespace Dispatcher.Frontend.Models;

public sealed class ConfigurationPage<T>
{
    public IReadOnlyList<T> Items { get; init; } = [];

    public int Count { get; init; }

    public long Total { get; init; }

    public int Offset { get; init; }

    public int Limit { get; init; }

    public bool HasMore { get; init; }

    public bool IsEmpty => Items.Count == 0;
}

public sealed class ObjectViewModel
{
    public string Id { get; init; } = string.Empty;

    public string? ParentId { get; init; }

    public string Type { get; init; } = string.Empty;

    public string Code { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public string Description { get; init; } = string.Empty;

    public bool IsRoot { get; init; }

    public bool HasParent =>
        !string.IsNullOrWhiteSpace(ParentId);
}

public sealed class ObjectTreeNodeViewModel
{
    public string Id { get; init; } = string.Empty;

    public string? ParentId { get; init; }

    public string Type { get; init; } = string.Empty;

    public string Code { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public string Description { get; init; } = string.Empty;

    public IReadOnlyList<ObjectTreeNodeViewModel> Children { get; init; } = [];

    public bool HasChildren => Children.Count > 0;
}

public sealed class ObjectTreeViewModel
{
    public IReadOnlyList<ObjectTreeNodeViewModel> Roots { get; init; } = [];

    public long Count { get; init; }

    public bool IsEmpty => Roots.Count == 0;
}

public sealed class DeviceConnectionViewModel
{
    public string Host { get; init; } = string.Empty;

    public int Port { get; init; }

    public string IpMode { get; init; } = string.Empty;

    public string NetworkInterface { get; init; } = string.Empty;

    public string Summary
    {
        get
        {
            if (string.IsNullOrWhiteSpace(Host))
            {
                return "Локальное или виртуальное подключение";
            }

            return Port > 0
                ? $"{Host}:{Port}"
                : Host;
        }
    }
}

public sealed class DeviceViewModel
{
    public string Id { get; init; } = string.Empty;

    public string? ObjectId { get; init; }

    public string Code { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public string Description { get; init; } = string.Empty;

    public string Protocol { get; init; } = string.Empty;

    public string State { get; init; } = string.Empty;

    public bool RuntimeEnabled { get; init; }

    public DeviceConnectionViewModel Connection { get; init; } = new();

    public bool HasObject =>
        !string.IsNullOrWhiteSpace(ObjectId);
}

public sealed class TagViewModel
{
    public string Id { get; init; } = string.Empty;

    public string? ObjectId { get; init; }

    public string? DeviceId { get; init; }

    public string Type { get; init; } = string.Empty;

    public string ValueType { get; init; } = string.Empty;

    public string ArchivePolicy { get; init; } = string.Empty;

    public string Code { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public string Description { get; init; } = string.Empty;

    public string EngineeringUnit { get; init; } = string.Empty;

    public double Scale { get; init; } = 1.0;

    public double Offset { get; init; }

    public string Address { get; init; } = string.Empty;

    public bool Enabled { get; init; }

    public bool HasObject =>
        !string.IsNullOrWhiteSpace(ObjectId);

    public bool HasDevice =>
        !string.IsNullOrWhiteSpace(DeviceId);
}
