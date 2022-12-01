#pragma once

#include "moja/modules/tier1_rice/disturbanceevents.h"

#include <moja/flint/iflintdata.h>

#include <moja/dynamic.h>

namespace moja {
namespace modules {
namespace tier1_rice {
    std::shared_ptr<flint::IFlintData> createRiceEventsFactory(const std::string& eventTypeStr, int id,
        const std::string& name, const DynamicObject& other);

    class RiceHarvestEvent final : public DisturbanceEventBase {
    public:
        RiceHarvestEvent(int id, const std::string& name) : DisturbanceEventBase(id, "tier1_rice.RiceHarvestEventEvent", name) {}
        virtual ~RiceHarvestEvent() = default;

        void configure(DynamicObject config, const flint::ILandUnitController& landUnitController,
            datarepository::DataRepository& dataRepository) override;
        DynamicObject exportObject() const override;
        void simulate(DisturbanceEventHandler& event_handler) const override;

        int id;
        bool organic_amendment_is_applied;
        int cropping_season;
        int water_regime;
        int water_regime_prior;
        int rice_area;
        DynamicVector organic_amendment_input;

    };


}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja
