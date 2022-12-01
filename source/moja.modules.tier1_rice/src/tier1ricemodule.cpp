#include "moja/modules/tier1_rice/tier1ricemodule.h"

#include <moja/flint/flintexceptions.h>
#include <moja/flint/iflintdata.h>
#include <moja/flint/ioperation.h>
#include <moja/flint/ivariable.h>
#include <moja/flint/operationdatapackage.h>
#include <moja/flint/timing.h>

#include <moja/notificationcenter.h>
#include <moja/signals.h>

#include <boost/format.hpp>

namespace moja {
namespace modules {
namespace tier1_rice{

	void Tier1RiceModule::configure(const DynamicObject& config) {}
	void Tier1RiceModule::subscribe(NotificationCenter& notificationCenter) {
		notificationCenter.subscribe(signals::LocalDomainInit, &Tier1RiceModule::onLocalDomainInit, *this);
		notificationCenter.subscribe(signals::TimingInit, &Tier1RiceModule::onTimingInit, *this);
		notificationCenter.subscribe(signals::TimingStep, &Tier1RiceModule::onTimingStep, *this);
	}

	void Tier1RiceModule::onLocalDomainInit() {
		CH4_ = _landUnitData->getPool("CH4");
		Rice_ = _landUnitData->getPool("Rice");
	}
	void Tier1RiceModule::onTimingInit() {

		

	}
	void Tier1RiceModule::onTimingStep() {}
}
}
}