#pragma once

#include "scada_contracts/repository/IRepository.h"

#include "scada_common/entity_id.h"

namespace dispatcher::contracts
{

    struct TagDto;


    class ITagRepository :
        public IRepository<
        TagDto,
        dispatcher::common::TagId
        >
    {
    public:

        virtual ~ITagRepository() = default;
    };

}