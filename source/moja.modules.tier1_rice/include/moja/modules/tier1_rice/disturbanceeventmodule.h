#pragma once

#include "moja/modules/tier1_rice/_modules.tier1_rice_exports.h"
#include "moja/modules/tier1_rice/disturbanceevents.h"

#include <moja/flint/modulebase.h>

namespace moja {
namespace flint {
class IPool;
class IVariable;
}  // namespace flint
namespace modules {
namespace tier1_rice {

    class TIER1_RICE_API DisturbanceEventModule : public flint::ModuleBase, DisturbanceEventHandler {
    public:
        DisturbanceEventModule() = default;
        virtual ~DisturbanceEventModule() = default;

        void configure(const DynamicObject& config) override;
        void subscribe(NotificationCenter& notificationCenter) override;

        void onLocalDomainInit() override;
        void TimingInit();

        void disturbanceEventHandler(const flint::EventQueueItem* event);

    private:
        void simulate(const RiceHarvestEvent& harvest) override;

        const flint::IPool* CH4_ = nullptr;
        const flint::IPool* Rice_ = nullptr;

        flint::IVariable* region_ = nullptr;
        flint::IVariable* region_ef_ = nullptr;
        flint::IVariable* organic_amendment_ = nullptr;
        flint::IVariable* cultivation_period_ = nullptr;
        flint::IVariable* water_regime_sf_ = nullptr;
        flint::IVariable* water_regime_prior_sf_ = nullptr;
        const flint::IVariable* land_unit_area_var_;


        DynamicVar period;
        double region_ef;

        std::vector<DynamicObject> water_regime_sf;
        std::vector<DynamicObject> water_regime_prior_sf;
        std::vector<DynamicObject> organic_amendment;
    
 
};

}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja
