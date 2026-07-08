#pragma once

#include "scada_common/lifecycle_state.h"
#include "scada_common/audit_info.h"

namespace dispatcher::core
{

    template<typename TId>
    class EntityBase
    {
    public:

        virtual ~EntityBase() = default;


        [[nodiscard]]
        const TId& id() const noexcept
        {
            return id_;
        }


        void set_id(const TId& value)
        {
            id_ = value;
        }


        [[nodiscard]]
        dispatcher::common::LifecycleState lifecycle_state() const noexcept
        {
            return lifecycle_state_;
        }


        void set_lifecycle_state(
            dispatcher::common::LifecycleState state
        ) noexcept
        {
            lifecycle_state_ = state;
        }


        [[nodiscard]]
        const dispatcher::common::AuditInfo& audit() const noexcept
        {
            return audit_;
        }


        dispatcher::common::AuditInfo& audit() noexcept
        {
            return audit_;
        }


    protected:

        TId id_;

        dispatcher::common::LifecycleState lifecycle_state_ =
            dispatcher::common::LifecycleState::Active;


        dispatcher::common::AuditInfo audit_;
    };

}