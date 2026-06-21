#include "receiver_mitsubishi_heavy.h"
#include "heatpumpir.h"

#include <HeatpumpIRFactory.h>

#if defined(USE_ARDUINO) || defined(USE_ESP32)

namespace esphome::heatpumpir {

bool decode_mitsubishi_heavy_zmp(const uint8_t frame[11], HeatpumpIRClimate &climate) {
  static const uint8_t FAN_MASK = 0xE0;
  static const uint8_t SWING_V_MASK5 = 0x02;
  static const uint8_t SWING_V_MASK7 = 0x18;

  uint8_t fan = frame[7] & FAN_MASK;
  if (fan == MITSUBISHI_HEAVY_ZMP_FAN_AUTO) {
    climate.fan_mode = climate::CLIMATE_FAN_AUTO;
    climate.preset = climate::CLIMATE_PRESET_NONE;
  } else if (fan == MITSUBISHI_HEAVY_ZMP_FAN1) {
    climate.fan_mode = climate::CLIMATE_FAN_LOW;
    climate.preset = climate::CLIMATE_PRESET_NONE;
  } else if (fan == MITSUBISHI_HEAVY_ZMP_FAN2) {
    climate.fan_mode = climate::CLIMATE_FAN_MEDIUM;
    climate.preset = climate::CLIMATE_PRESET_NONE;
  } else if (fan == MITSUBISHI_HEAVY_ZMP_FAN3) {
    climate.fan_mode = climate::CLIMATE_FAN_HIGH;
    climate.preset = climate::CLIMATE_PRESET_NONE;
  } else if (fan == MITSUBISHI_HEAVY_ZMP_HIPOWER) {
    climate.preset = climate::CLIMATE_PRESET_BOOST;
  } else if (fan == MITSUBISHI_HEAVY_ZMP_ECONO) {
    climate.preset = climate::CLIMATE_PRESET_ECO;
  }

  static const uint8_t HS_MASK = 0xDC;
  static const uint8_t HS_SWING = 0x5C;

  uint8_t swing_h = frame[5] & HS_MASK;
  uint8_t swing_v = (frame[5] & SWING_V_MASK5) | (frame[7] & SWING_V_MASK7);
  bool h_swing = (swing_h == HS_SWING);
  bool v_swing = (swing_v == MITSUBISHI_HEAVY_ZMP_VS_SWING);

  if (h_swing && v_swing) {
    climate.swing_mode = climate::CLIMATE_SWING_BOTH;
  } else if (h_swing) {
    climate.swing_mode = climate::CLIMATE_SWING_HORIZONTAL;
  } else if (v_swing) {
    climate.swing_mode = climate::CLIMATE_SWING_VERTICAL;
  } else {
    climate.swing_mode = climate::CLIMATE_SWING_OFF;
  }

  return true;
}

}  // namespace esphome::heatpumpir

#endif
