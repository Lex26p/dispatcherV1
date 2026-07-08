#pragma once

#include "scada_common/id_types.h"

#include <string>

namespace dispatcher::common
{
    template<typename TTag>
    class TypedId
    {
    public:

        TypedId() = default;

        explicit TypedId(Uuid value)
            : value_(std::move(value))
        {
        }

        static TypedId from_string(std::string_view value)
        {
            return TypedId{
                Uuid::from_string(value)
            };
        }

        [[nodiscard]] bool empty() const noexcept
        {
            return value_.empty();
        }

        [[nodiscard]] const Uuid& uuid() const noexcept
        {
            return value_;
        }

        [[nodiscard]] const std::string& str() const noexcept
        {
            return value_.str();
        }

        friend bool operator==(const TypedId&, const TypedId&) noexcept = default;

    private:

        Uuid value_;
    };


    struct ObjectIdTag {};
    struct LocationIdTag {};
    struct EquipmentIdTag {};
    struct DeviceIdTag {};
    struct TagIdTag {};
    struct EventIdTag {};
    struct AlarmIdTag {};
    struct CommandIdTag {};
    struct UserIdTag {};


    using ObjectId = TypedId<ObjectIdTag>;

    using LocationId = TypedId<LocationIdTag>;

    using EquipmentId = TypedId<EquipmentIdTag>;

    using DeviceId = TypedId<DeviceIdTag>;

    using TagId = TypedId<TagIdTag>;

    using EventId = TypedId<EventIdTag>;

    using AlarmId = TypedId<AlarmIdTag>;

    using CommandId = TypedId<CommandIdTag>;

    using UserId = TypedId<UserIdTag>;
}