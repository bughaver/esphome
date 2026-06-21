#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome::heatpumpir {

class HeatpumpIRClimate;

bool decode_mitsubishi_heavy_zmp(HeatpumpIRClimate &climate, remote_base::RemoteReceiveData &data);

}  // namespace esphome::heatpumpir
