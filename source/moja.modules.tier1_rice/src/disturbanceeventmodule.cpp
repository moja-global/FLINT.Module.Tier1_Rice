#include "moja/modules/tier1_rice/disturbanceeventmodule.h"

#include "moja/modules/tier1_rice/disturbanceevents.h"
#include "moja/modules/tier1_rice/riceevent.h"

#include <moja/flint/eventqueue.h>
#include <moja/flint/flintexceptions.h>
#include <moja/flint/iflintdata.h>
#include <moja/flint/ioperation.h>
#include <moja/flint/ivariable.h>

#include <moja/notificationcenter.h>
#include <moja/signals.h>

namespace moja {
namespace modules {
namespace tier1_rice {

	struct Water_Regime_Records {
		int id;
		std::string water_regime;
		double sf;
	};

	struct Water_Regime_Prior_Records {
		int id;
		std::string water_regime_prior;
		double sf;
	};

	struct Organic_Amendment_Records {
		int id;
		std::string organic_amendment;
		double cf;
	};

	void DisturbanceEventModule::configure(const DynamicObject& config) {}

	void DisturbanceEventModule::subscribe(NotificationCenter& notificationCenter) {
		notificationCenter.subscribe(signals::LocalDomainInit, &DisturbanceEventModule::onLocalDomainInit, *this);
		notificationCenter.subscribe(signals::TimingInit, &DisturbanceEventModule::TimingInit, *this);
		notificationCenter.subscribe(signals::DisturbanceEvent, &DisturbanceEventModule::disturbanceEventHandler, *this);
	}

	void DisturbanceEventModule::onLocalDomainInit() {
		CH4_ = _landUnitData->getPool("CH4");
		Rice_ = _landUnitData->getPool("Rice");
		region_ef_ = _landUnitData->getVariable("region_ef");
		cultivation_period_ = _landUnitData->getVariable("cultivation_period");
		organic_amendment_ = _landUnitData->getVariable("organic_amendment");
		water_regime_sf_ = _landUnitData->getVariable("water_regime_sf");
		water_regime_prior_sf_ = _landUnitData->getVariable("water_regime_prior_sf");

		organic_amendment =
			organic_amendment_->value().extract<std::vector<DynamicObject>>();

	    water_regime_sf =
			water_regime_sf_->value().extract<std::vector<DynamicObject>>();

		water_regime_prior_sf =
			water_regime_prior_sf_->value().extract<std::vector<DynamicObject>>();
	
	}

	void DisturbanceEventModule::TimingInit() {
		region_ = _landUnitData->getVariable("region");
		auto region_ef_record =
			region_ef_->value().extract<DynamicObject>();
		region_ef = region_ef_record["ef"];

		auto cultivation_period =
			cultivation_period_->value().extract<DynamicObject>();
		period = cultivation_period["period"];
	
	}

	void DisturbanceEventModule::simulate(const RiceHarvestEvent& harvest) {

		std::vector<Water_Regime_Records> water_regime_record_iter;
		for (const auto& record_var : water_regime_sf) {
				Water_Regime_Records record;
				record.id = record_var["id"];
				record.water_regime = record_var["water_regime"].extract<std::string>();
				record.sf = record_var["sf"];
				water_regime_record_iter.push_back(record);		
		}
		auto water_regime_sf_value = std::find_if(water_regime_record_iter.begin(), water_regime_record_iter.end(),
			[&](const Water_Regime_Records& c) { return c.id == harvest.water_regime; });

		std::vector<Water_Regime_Prior_Records> water_regime_prior_record_iter;
		for (const auto& record_var : water_regime_prior_sf) {
			Water_Regime_Prior_Records record;
			record.id = record_var["id"];
			record.water_regime_prior = record_var["water_regime_prior"].extract<std::string>();
			record.sf = record_var["sf"];
			water_regime_prior_record_iter.push_back(record);
		}
		auto water_regime_sf_prior_value = std::find_if(water_regime_prior_record_iter.begin(), water_regime_prior_record_iter.end(),
			[&](const Water_Regime_Prior_Records& c) { return c.id == harvest.water_regime_prior; });

		std::vector<Organic_Amendment_Records> organic_amendment_record_iter;
		for (const auto& record_var : organic_amendment) {
			Organic_Amendment_Records record;
			record.id = record_var["id"];
			record.organic_amendment = record_var["organic_amendment"].extract<std::string>();
			record.cf = record_var["cf"];
			organic_amendment_record_iter.push_back(record);
		}

		double sum_sf0 = 0;
		for (auto input: harvest.organic_amendment_input){
			double rate = input["rate"];
			int type_id = input["type"]; 
			auto organic_amendment_value = std::find_if(organic_amendment_record_iter.begin(), organic_amendment_record_iter.end(),
					[&](const Organic_Amendment_Records& c) { return c.id == type_id; });
			sum_sf0 += organic_amendment_value->cf * rate;

		}
		double sf0 = 0;
		if (harvest.organic_amendment_is_applied) {
			sf0 = std::pow(1 + sum_sf0, 0.59);
		}
		else {
			sf0 = 1;
		}
		double adjust_ef = region_ef *water_regime_sf_value->sf* water_regime_sf_prior_value->sf * sf0;
		double CH4_emission = harvest.cropping_season * adjust_ef * period * 1e-06;
		auto operation = _landUnitData->createStockOperation();
		operation->addTransfer(Rice_, CH4_, CH4_emission);

		_landUnitData->submitOperation(operation);
	}

	void DisturbanceEventModule::disturbanceEventHandler(const flint::EventQueueItem* event) {
		const auto disturbance_event = std::static_pointer_cast<const DisturbanceEventBase>(event->_event);
		disturbance_event->simulate(*this);
	}

}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja
