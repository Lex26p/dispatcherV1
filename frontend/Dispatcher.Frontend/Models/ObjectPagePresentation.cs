namespace Dispatcher.Frontend.Models;

public static class ObjectPagePresentation
{
    public static string FormatType(
        string? type
    )
    {
        return type switch
        {
            "Site" => "Площадка",
            "Building" => "Здание",
            "Floor" => "Этаж",
            "Room" => "Помещение",
            "Zone" => "Зона",
            "Cabinet" => "Шкаф",
            "Rack" => "Стойка",
            "Line" => "Линия",
            "EquipmentGroup" => "Группа оборудования",
            "Custom" => "Пользовательский объект",
            "Unknown" => "Неизвестный тип",
            null or "" => "Тип не указан",
            _ => type
        };
    }

    public static string FormatParent(
        string? parentId
    )
    {
        return string.IsNullOrWhiteSpace(parentId)
            ? "Корневой объект"
            : parentId;
    }

    public static int CalculateMaxDepth(
        IReadOnlyList<ObjectTreeNodeViewModel>? roots
    )
    {
        if (roots is null || roots.Count == 0)
        {
            return 0;
        }

        var maxDepth = 0;

        foreach (var root in roots)
        {
            maxDepth = Math.Max(
                maxDepth,
                CalculateNodeDepth(
                    root,
                    1
                )
            );
        }

        return maxDepth;
    }

    private static int CalculateNodeDepth(
        ObjectTreeNodeViewModel node,
        int currentDepth
    )
    {
        var maxDepth = currentDepth;

        foreach (var child in node.Children)
        {
            maxDepth = Math.Max(
                maxDepth,
                CalculateNodeDepth(
                    child,
                    currentDepth + 1
                )
            );
        }

        return maxDepth;
    }
}
