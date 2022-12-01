#include "moja/modules/tier1_rice/libraryfactory.h"

#include "moja/modules/tier1_rice/disturbanceeventmodule.h"
#include "moja/modules/tier1_rice/buildlandunitmodule.h"

#include "moja/modules/tier1_rice/aggregatorerror.h"
#include "moja/modules/tier1_rice/aggregatorlandunit.h"
#include "moja/modules/tier1_rice/commondata.h"
#include "moja/modules/tier1_rice/landunitsqlitewriter.h"

#include "moja/modules/tier1_rice/runstatistics.h"

#include "moja/modules/tier1_rice/riceevent.h"

#include <moja/flint/imodule.h>
#include <moja/flint/mojalibapi.h>

// Modules
// Transforms
// Flint Data

namespace moja {
namespace modules {
namespace tier1_rice {

extern "C" {
// Flint Data

MOJA_LIB_API int getModuleRegistrations(flint::ModuleRegistration* outModuleRegistrations) {
   auto index = 0;
   outModuleRegistrations[index++] = flint::ModuleRegistration{
       "AggregatorError",
       []() -> flint::IModule* { return new AggregatorError(ObjectHolder::Instance().systemSettings); } };
   outModuleRegistrations[index++] = flint::ModuleRegistration{
       "AggregatorLandUnit", []() -> flint::IModule* { return new AggregatorLandUnit(ObjectHolder::Instance()); } };
   outModuleRegistrations[index++] = flint::ModuleRegistration{
       "LandUnitSQLiteWriter", []() -> flint::IModule* { return new LandUnitSQLiteWriter(ObjectHolder::Instance()); } };
   outModuleRegistrations[index++] = flint::ModuleRegistration{
       "DisturbanceEventModule", []() -> flint::IModule* { return new DisturbanceEventModule(); }};
   outModuleRegistrations[index++] = flint::ModuleRegistration{
    "BuildLandUnitModule", []() -> flint::IModule* { return new BuildLandUnitModule(); } };

   return index;
}

MOJA_LIB_API int getTransformRegistrations(flint::TransformRegistration* outTransformRegistrations) {
   auto index = 0;
  
   return index;
}

MOJA_LIB_API int getFlintDataRegistrations(flint::FlintDataRegistration* outFlintDataRegistrations) {
   auto index = 0;
   outFlintDataRegistrations[index++] = flint::FlintDataRegistration{
    "SimulationUnitData", []() -> flint::IFlintData* { return new SimulationUnitData(); } };
   outFlintDataRegistrations[index++] =
       flint::FlintDataRegistration{ "RunStatistics", []() -> flint::IFlintData* { return new RunStatistics(); } };

   return index;
}

MOJA_LIB_API int getFlintDataFactoryRegistrations(
    flint::FlintDataFactoryRegistration* outFlintDataFactoryRegistrations) {
   auto index = 0;
   outFlintDataFactoryRegistrations[index++] =
       flint::FlintDataFactoryRegistration{ "internal.flint", "EventQueue", &createRiceEventsFactory };
   return index;
}
}

}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja