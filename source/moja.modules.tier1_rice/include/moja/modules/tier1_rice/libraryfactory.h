

#pragma once

#include <moja/flint/librarymanager.h>

namespace moja {
namespace modules {
namespace tier1_rice {

extern "C" MOJA_LIB_API int getModuleRegistrations(moja::flint::ModuleRegistration* outModuleRegistrations);
extern "C" MOJA_LIB_API int getTransformRegistrations(moja::flint::TransformRegistration* outTransformRegistrations);
extern "C" MOJA_LIB_API int getFlintDataRegistrations(moja::flint::FlintDataRegistration* outFlintDataRegistrations);
extern "C" MOJA_LIB_API int getFlintDataFactoryRegistrations(
    moja::flint::FlintDataFactoryRegistration* outFlintDataFactoryRegistrations);

}  // namespace tier1_rice
}  // namespace modules
}  // namespace moja

