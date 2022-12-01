#pragma once

#include "moja/modules/tier1_rice/_modules.tier1_rice_exports.h"
#include <moja/flint/modulebase.h>

namespace moja {
namespace modules {
namespace tier1_rice {
    class BuildLandUnitModule : public flint::ModuleBase {
    public:
        BuildLandUnitModule()
            : spatial_location_info_var_(nullptr),
            land_unit_build_success_var_(nullptr) {
        }

        virtual ~BuildLandUnitModule() = default;

        void configure(const DynamicObject&) override;
        void subscribe(NotificationCenter& notificationCenter) override;

        void onLocalDomainInit() override;
        void onPreTimingSequence() override;

    private:
        struct varValueList {
            std::string _name;
            const flint::IVariable* _var;
            std::vector<int> _values;
        };

        const flint::IVariable* land_unit_area_var_;
        const flint::IVariable* management_data_var_;

        flint::IVariable* spatial_location_info_var_;
        flint::IVariable* land_unit_build_success_var_;
        flint::IVariable* event_queue_var_;
        flint::IVariable* rice_activity_var_;
        flint::IVariable* sim_area_id_var_;
       

        std::map<std::string, int> organic_amend_name_map {
            {"straw_shortly_rate", 1},
            {"straw_long_rate", 2},
            {"compost_rate", 3},
            {"farmyard_manure", 4},
            {"green_manure", 5}
        };
    };

}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja