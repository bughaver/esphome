#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome::heatpumpir {

class HeatpumpIRClimate;

bool decode_mitsubishi_heavy_frame(HeatpumpIRClimate &climate, remote_base::RemoteReceiveData &data, uint8_t frame[11]);

bool decode_mitsubishi_heavy_zmp(const uint8_t frame[11], HeatpumpIRClimate &climate);

}  // namespace esphome::heatpumpir
