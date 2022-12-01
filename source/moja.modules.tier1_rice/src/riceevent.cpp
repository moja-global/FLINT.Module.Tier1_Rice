#include "moja/modules/tier1_rice/disturbanceevents.h"

#include "moja/modules/tier1_rice/riceevent.h"

#include <moja/flint/ilandunitcontroller.h>
#include <moja/flint/ivariable.h>


namespace moja {
namespace modules {
namespace tier1_rice {
    std::shared_ptr<flint::IFlintData> createRiceEventsFactory(const std::string& eventTypeStr, int id,
        const std::string& name, const DynamicObject& other) {
        if (eventTypeStr == "tier1_rice.RiceHarvestEvent") return std::make_shared<RiceHarvestEvent>(id, name);
        return nullptr;
    }

    void RiceHarvestEvent::configure(DynamicObject config, const flint::ILandUnitController& landUnitController,
        datarepository::DataRepository& dataRepository) {
        DisturbanceEventBase::configure(config, landUnitController, dataRepository);
        id = config["id"];
        organic_amendment_is_applied = config["organic_amendment_is_applied"];
        cropping_season = config["cropping_season"];
        water_regime = config["water_regime"];
        water_regime_prior = config["water_regime_prior"];
        rice_area = config["rice_area"];
        organic_amendment_input = config["organic_amendment_input"].extract<DynamicVector>();

    }

    DynamicObject RiceHarvestEvent::exportObject() const {
        auto object = DisturbanceEventBase::exportObject();
        object["id"] = id;
        object["organic_amendment_is_applied"] = organic_amendment_is_applied;
        object["cropping_season"] = cropping_season;
        object["water_regime"] = water_regime;
        object["water_regime_prior"] = water_regime_prior;
        object["rice_area"] = rice_area;
        object["organic_amendment_input"] = organic_amendment_input;
        return object;
    }

    void RiceHarvestEvent::simulate(DisturbanceEventHandler& event_handler) const { event_handler.simulate(*this); }
    




}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja
