#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome::heatpumpir {

class HeatpumpIRClimate;

/// Decode the shared Mitsubishi Heavy IR frame (header, bits, checksum, mode, temperature).
/// Writes climate.mode and climate.target_temperature. Fills frame[11] with raw bytes.
/// Returns true if a valid frame was decoded.
bool decode_mitsubishi_heavy_frame(HeatpumpIRClimate &climate, remote_base::RemoteReceiveData &data, uint8_t frame[11]);

/// Decode ZMP-specific frame bytes (fan, preset, swing) into `climate`.
bool decode_mitsubishi_heavy_zmp(const uint8_t frame[11], HeatpumpIRClimate &climate);

}  // namespace esphome::heatpumpir
