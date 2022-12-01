#include "moja/modules/tier1_rice/commondata.h"

namespace moja::modules::tier1_rice {

ObjectHolder& ObjectHolder::Instance() {
   static ObjectHolder instance;
   return instance;
}
}  // namespace moja::modules::tier1_rice
