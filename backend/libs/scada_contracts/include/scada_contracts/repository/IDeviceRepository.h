#pragma once

#include "scada_contracts/repository/IRepository.h"

#include "scada_common/entity_id.h"

namespace dispatcher::contracts
{

    struct DeviceDto;


    class IDeviceRepository :
        public IRepository<
        DeviceDto,
        dispatcher::common::DeviceId
        >
    {
    public:

        virtual ~IDeviceRepository() = default;
    };

}