#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/preferences.h"
#include <bsec.h>
#include <map>

namespace esphome {
namespace bme680_bsec {

enum BME680BSECIAQMode {
  BME680_IAQ_MODE_STATIC = 0,
  BME680_IAQ_MODE_MOBILE = 1,
};

class BME680BSECComponent : public Component, public i2c::I2CDevice {
 public:

  void set_temperature_offset(float offset);
  void set_iaq_mode(BME680BSECIAQMode iaq_mode);
  void set_state_save_interval(uint16_t interval);

  std::string calc_iaq_accuracy_text(uint8_t accuracy) const;

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) {
    temperature_sensor_ = temperature_sensor;
  }
  void set_pressure_sensor(sensor::Sensor *pressure_sensor) {
    pressure_sensor_ = pressure_sensor;
  }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) {
    humidity_sensor_ = humidity_sensor;
  }
  void set_gas_resistance_sensor(sensor::Sensor *gas_resistance_sensor) {
    gas_resistance_sensor_ = gas_resistance_sensor;
  }
  void set_iaq_sensor(sensor::Sensor *iaq_sensor) {
    iaq_sensor_ = iaq_sensor;
  }
  void set_iaq_accuracy_sensor(sensor::Sensor *iaq_accuracy_sensor) {
    iaq_accuracy_sensor_ = iaq_accuracy_sensor;
  }
  void set_co2_equivalent_sensor(sensor::Sensor *co2_equivalent_sensor) {
    co2_equivalent_sensor_ = co2_equivalent_sensor;
  }
  void set_breath_voc_equivalent_sensor(sensor::Sensor *breath_voc_equivalent_sensor) {
    breath_voc_equivalent_sensor_ = breath_voc_equivalent_sensor;
  }

  static std::map<uint8_t,BME680BSECComponent*> instances;
  static int8_t read_bytes_wrapper(uint8_t address, uint8_t a_register, uint8_t *data, uint16_t len);
  static int8_t write_bytes_wrapper(uint8_t address, uint8_t a_register, uint8_t *data, uint16_t len);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;

 protected:
  bool check_bsec_status_(void);
  void load_state_(void);
  void update_state_(void);
  uint8_t get_iaq_(void);
  uint8_t get_iaq_accuracy_(void);
  void publish_state_(sensor::Sensor *sensor, float value);

  Bsec bsec_;
  bsec_library_return_t last_bsec_status_{BSEC_OK};
  int8_t last_bme680_status_{BME680_OK};
  float temperature_offset_{0};
  uint16_t state_save_interval_{360}; // 360 minutes - 4 times a day
  ESPPreferenceObject bsec_state_;
  BME680BSECIAQMode iaq_mode_{BME680_IAQ_MODE_STATIC};
  uint16_t state_update_counter_{0};
  uint8_t sensor_push_num_{0};

  sensor::Sensor *temperature_sensor_;
  sensor::Sensor *pressure_sensor_;
  sensor::Sensor *humidity_sensor_;
  sensor::Sensor *gas_resistance_sensor_;
  sensor::Sensor *iaq_sensor_;
  sensor::Sensor *iaq_accuracy_sensor_;
  sensor::Sensor *co2_equivalent_sensor_;
  sensor::Sensor *breath_voc_equivalent_sensor_;
};

}  // namespace bme680_bsec
}  // namespace esphome