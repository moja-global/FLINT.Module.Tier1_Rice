#include "moja/modules/tier1_rice/buildlandunitmodule.h"
#include "moja/modules/tier1_rice/riceevent.h"

#include <moja/flint/eventqueue.h>

#include <moja/flint/flintexceptions.h>
#include <moja/flint/iflintdata.h>
#include <moja/flint/ioperation.h>
#include <moja/flint/ivariable.h>
#include <moja/flint/operationdatapackage.h>
#include <moja/flint/timing.h>
#include <moja/flint/spatiallocationinfo.h>

#include <moja/notificationcenter.h>
#include <moja/signals.h>

#include <boost/format.hpp>

namespace moja {
namespace modules {
namespace tier1_rice{
    void BuildLandUnitModule::subscribe(NotificationCenter& notificationCenter) {
        notificationCenter.subscribe(signals::LocalDomainInit, &BuildLandUnitModule::onLocalDomainInit, *this);
        notificationCenter.subscribe(signals::PreTimingSequence, &BuildLandUnitModule::onPreTimingSequence, *this);
    }

    void BuildLandUnitModule::configure(const DynamicObject& config) {
    }

    void BuildLandUnitModule::onLocalDomainInit() {
        try {
            sim_area_id_var_ = _landUnitData->getVariable("admin_level_3");
            spatial_location_info_var_ = _landUnitData->getVariable("spatialLocationInfo");
            land_unit_build_success_var_ = _landUnitData->getVariable("landUnitBuildSuccess");
            rice_activity_var_ = _landUnitData->getVariable("rice_events");
        }
        catch (flint::VariableNotFoundException& exc) {
            const auto variable_name = *(boost::get_error_info<flint::VariableName>(exc));
            const auto str = ((boost::format("Variable not found: %1%") % variable_name).str());
            BOOST_THROW_EXCEPTION(flint::LocalDomainError()
                << flint::Details(str) << flint::LibraryName("moja.modules.tier1_rice")
                << flint::ModuleName(BOOST_CURRENT_FUNCTION) << flint::ErrorCode(1));
        }
        catch (...) {
            if (_landUnitData->hasVariable("spatialLocationInfo")) {
                auto spatial_location_info = std::static_pointer_cast<flint::SpatialLocationInfo>(
                    _landUnitData->getVariable("spatialLocationInfo")->value().extract<std::shared_ptr<flint::IFlintData>>());
                spatial_location_info->_library = "moja.modules.tier1_rice";
                spatial_location_info->_module = BOOST_CURRENT_FUNCTION;
                spatial_location_info->_errorCode = 1;
            }
            throw;
        }
    }

    void BuildLandUnitModule::onPreTimingSequence() {
        try {
            const bool land_unit_build_success = land_unit_build_success_var_->value();
            if (sim_area_id_var_->value().isEmpty()) {
                land_unit_build_success_var_->set_value(false);
                return;
            }

            const auto& spatial_location_info = std::static_pointer_cast<flint::SpatialLocationInfo>(
                spatial_location_info_var_->value().extract<std::shared_ptr<flint::IFlintData>>());
            
            if (!land_unit_build_success) return;

            auto event_queue = std::static_pointer_cast<flint::EventQueue>(
                _landUnitData->getVariable("eventqueue")->value<std::shared_ptr<flint::IFlintData>>());
            event_queue->clear();
            int event_id = 0;

            std::map<int, double> cum_prob;
            std::map<int, double> random_value;
            std::list<int> event_year_record;
            std::list<int> flag_random_set;

            auto rice_inputs = rice_activity_var_->value().extract<std::vector<DynamicObject>>();
            for (auto rice_input: rice_inputs){
                int event_year = rice_input["year"];
                bool year_found = (std::find(event_year_record.begin(), event_year_record.end(), 
                    event_year) != event_year_record.end());

                if (!year_found){
                    bool year_random_set = (std::find(flag_random_set.begin(), flag_random_set.end(),
                        event_year) != flag_random_set.end());
                    if (!year_random_set) {
                        std::uniform_real_distribution<> distribution(0.0, rice_input["total_frequency"]);
                        random_value[event_year] = distribution(spatial_location_info->_engCell);
                        flag_random_set.push_back(event_year);
                    }
  
                    cum_prob[event_year] += rice_input["frequency"];
                    if (cum_prob[event_year] < random_value[event_year]) {
                        continue;
                    }
                    else {
                        DateTime ev_date(rice_input["year"], 6, 1);
                        auto water_regime = rice_input["water_regime"];
                        auto rice_event = std::make_shared<RiceHarvestEvent>(event_id, "Rice Harvest Event");
                        rice_event->id = rice_input["id"];
                        rice_event->organic_amendment_is_applied = rice_input["organic_amendment_is_applied"];
                        rice_event->cropping_season = rice_input["cropping_season"];
                        rice_event->water_regime = rice_input["water_regime"];
                        rice_event->water_regime_prior = rice_input["water_regime_prior"];
                        rice_event->rice_area = rice_input["rice_area"];
                        DynamicVector rice_organic_amendment;
                        for (auto it = organic_amend_name_map.begin(); it != organic_amend_name_map.end(); it++)
                        {
                            DynamicObject amendment;
                            amendment["rate"] = rice_input[it->first];
                            amendment["type"] = it->second;
                            rice_organic_amendment.push_back(amendment);
                        }
                        rice_event->organic_amendment_input = rice_organic_amendment;
                        event_queue->emplace_back(ev_date, rice_event);
                        event_id++;
                        event_year_record.push_back(event_year);     
                    }  
                }
               
            }

            land_unit_build_success_var_->set_value(true);
        }
        catch (...) {
            if (_landUnitData->hasVariable("spatialLocationInfo")) {
                auto spatial_location_info = std::static_pointer_cast<flint::SpatialLocationInfo>(
                    _landUnitData->getVariable("spatialLocationInfo")->value().extract<std::shared_ptr<flint::IFlintData>>());
                spatial_location_info->_library = "moja.modules.tier1_rice";
                spatial_location_info->_module = BOOST_CURRENT_FUNCTION;
                spatial_location_info->_errorCode = 1;
            }
            throw;
        }
    }

}
}
}