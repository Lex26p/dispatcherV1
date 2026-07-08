#pragma once

namespace dispatcher::contracts
{

    template<typename TEntity, typename TId>
    class IRepository
    {
    public:

        virtual ~IRepository() = default;


        virtual bool exists(
            const TId& id
        ) const = 0;


        virtual void save(
            const TEntity& entity
        ) = 0;


        virtual void remove(
            const TId& id
        ) = 0;
    };

}