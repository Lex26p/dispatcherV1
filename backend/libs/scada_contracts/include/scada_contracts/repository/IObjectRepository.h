#pragma once

#include "scada_contracts/repository/IRepository.h"

#include "scada_common/entity_id.h"

namespace dispatcher::contracts
{

    struct ObjectDto;


    class IObjectRepository :
        public IRepository<
        ObjectDto,
        dispatcher::common::ObjectId
        >
    {
    public:

        virtual ~IObjectRepository() = default;
    };

}