#include "receiver_mitsubishi_heavy.h"
#include "heatpumpir.h"

#include <HeatpumpIRFactory.h>

#if defined(USE_ARDUINO) || defined(USE_ESP32)

namespace esphome::heatpumpir {

bool decode_mitsubishi_heavy_frame(HeatpumpIRClimate &climate, remote_base::RemoteReceiveData &data,
                                   uint8_t frame[11]) {
  static const uint8_t frame_prefix[5] = {0x52, 0xAE, 0xC3, 0x26, 0xD9};
  static const uint8_t mode_mask = 0x07;
  static const uint8_t temp_mask = 0x0F;
  static const uint8_t checksum_byte = 0xFF;

  if (!data.expect_item(MITSUBISHI_HEAVY_HDR_MARK, MITSUBISHI_HEAVY_HDR_SPACE))
    return false;

  for (uint8_t pos = 0; pos < 11; pos++) {
    uint8_t byte = 0;
    for (int8_t bit = 0; bit < 8; bit++) {
      if (data.expect_item(MITSUBISHI_HEAVY_BIT_MARK, MITSUBISHI_HEAVY_ONE_SPACE)) {
        byte |= 1 << bit;
      } else if (!data.expect_item(MITSUBISHI_HEAVY_BIT_MARK, MITSUBISHI_HEAVY_ZERO_SPACE)) {
        return false;
      }
    }
    frame[pos] = byte;

    if (pos < 5 && byte != frame_prefix[pos])
      return false;
  }

  if ((uint8_t) (frame[5] ^ frame[6]) != checksum_byte ||
      (uint8_t) (frame[7] ^ frame[8]) != checksum_byte ||
      (uint8_t) (frame[9] ^ frame[10]) != checksum_byte) {
    return false;
  }

  if (frame[9] & MITSUBISHI_HEAVY_MODE_OFF) {
    climate.mode = climate::CLIMATE_MODE_OFF;
  } else {
    switch (frame[9] & mode_mask) {
      case MITSUBISHI_HEAVY_MODE_AUTO:
        climate.mode = climate::CLIMATE_MODE_HEAT_COOL;
        break;
      case MITSUBISHI_HEAVY_MODE_HEAT:
        climate.mode = climate::CLIMATE_MODE_HEAT;
        break;
      case MITSUBISHI_HEAVY_MODE_COOL:
        climate.mode = climate::CLIMATE_MODE_COOL;
        break;
      case MITSUBISHI_HEAVY_MODE_DRY:
        climate.mode = climate::CLIMATE_MODE_DRY;
        break;
      case MITSUBISHI_HEAVY_MODE_FAN:
        climate.mode = climate::CLIMATE_MODE_FAN_ONLY;
        break;
      default:
        return false;
    }
  }

  climate.target_temperature = 17 + ((~(frame[9] >> 4)) & temp_mask);
  return true;
}

}  // namespace esphome::heatpumpir

#endif
