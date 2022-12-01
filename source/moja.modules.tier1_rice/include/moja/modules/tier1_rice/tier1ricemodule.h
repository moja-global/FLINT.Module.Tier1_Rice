#pragma once

#include "moja/modules/tier1_rice/_modules.tier1_rice_exports.h"
#include <moja/flint/modulebase.h>

namespace moja {
namespace flint {
class IPool;
class IVariable;
}  // namespace flint
namespace modules {
namespace tier1_rice {

	class TIER1_RICE_API Tier1RiceModule : public moja::flint::ModuleBase {
	public:
		Tier1RiceModule() = default;

		virtual ~Tier1RiceModule() = default;

		void configure(const DynamicObject& config) override;
		void subscribe(NotificationCenter& notificationCenter) override;

		void onLocalDomainInit() override;
		void onTimingInit() override;
		void onTimingStep() override;

	private:
		const flint::IPool* CH4_ = nullptr;
		const flint::IPool* Rice_ = nullptr;

	};
}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja