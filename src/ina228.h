#ifndef __INA228_H__
#define __INA228_H__

#include "Arduino.h"
// INA228 Registers
#define INA228_REG_CONF 0x00
#define INA228_REG_ADC_CONF 0x01
#define INA228_REG_SHUNT_CAL 0x02
#define INA228_REG_SHUNT_TEMPCO 0x03
#define INA228_REG_VSHUNT 0x04
#define INA228_REG_VBUS 0x05
#define INA228_REG_DIETEMP 0x06
#define INA228_REG_CURRENT 0x07
#define INA228_REG_POWER 0x08
#define INA228_REG_ENERGY 0x09
#define INA228_REG_CHARGE 0x0A
#define INA228_REG_DIAG_ALRT 0x0B
#define INA228_REG_SOVL 0x0C
#define INA228_REG_SUVL 0x0D
#define INA228_REG_BOVL 0x0E
#define INA228_REG_BUVL 0x0F
#define INA228_REG_TEMP_LIMIT 0x10
#define INA228_REG_PWR_LIMIT 0x11
#define INA228_REG_MANUFACTURER_ID 0x3E
#define INA228_REG_DEVICE_ID 0x3F

// INA228 conf
#define INA228_CONF_RST_FLAG 0x8000
#define INA228_CONF_RSTACC_FLAG 0x4000
#define INA228_CONF_CONVDLY_MASK 0x3FC0
#define INA228_CONF_TEMPCOMP_FLAG 0x0020
#define INA228_CONF_ADCRANGE_FLAG 0x0010

// INA228 adc conf
#define INA228_ADC_MODE_MASK 0xF000
#define INA228_ADC_VBUSCT_MASK (7 << 9)
#define INA228_ADC_VSHCT_MASK (7 << 6)
#define INA228_ADC_VTCT_MASK (7 << 3)
#define INA228_ADC_AVG_MASK (7 << 0)


class INA228_conf {
  uint16_t raw = 0;
public:

#define DEFINE_FLAG_METHODS(flagName, flagMask) \
  void set##flagName(const bool doSet); \
  bool get##flagName() const;

  DEFINE_FLAG_METHODS(Reset, INA228_CONF_RST_FLAG)
  DEFINE_FLAG_METHODS(ResetAcc, INA228_CONF_RSTACC_FLAG)
  DEFINE_FLAG_METHODS(TempComp, INA228_CONF_TEMPCOMP_FLAG)
  DEFINE_FLAG_METHODS(ADCRangeLow, INA228_CONF_ADCRANGE_FLAG)

#undef DEFINE_FLAG_METHODS

  void setConvDly(const uint16_t mills);
  uint16_t getConvDly() const;

  String toString() const;
};


enum class INA228_CT : uint16_t {
  CT_0050us = 0x0,  //0.05ms  (20000/s)
  CT_0084us = 0x1,  //0.084ms (11905/s)
  CT_0150us = 0x2,  //0.15ms  (6667/s)
  CT_0280us = 0x3,  //0.28ms  (3571/s)
  CT_0540us = 0x4,  //0.54ms  (1852/s)
  CT_1052us = 0x5,  //1.052ms (951/s) [default]
  CT_2074us = 0x6,  //2.074ms (482/s)
  CT_4120us = 0x7   //4.12ms  (243/s)
};

enum class INA228_ADC_MODE : uint16_t {
  SHUTDOWN = 0x0000,             // Shutdown
  TRIG_BUS = 0x1000,             // Triggered bus voltage, single shot
  TRIG_SHUNT = 0x2000,           // Triggered shunt voltage, single shot
  TRIG_BUS_SHUNT = 0x3000,       // Triggered shunt voltage and bus voltage, single shot
  TRIG_TEMP = 0x4000,            // Triggered temperature, single shot
  TRIG_TEMP_BUS = 0x5000,        // Triggered temperature and bus voltage, single shot
  TRIG_TEMP_SHUNT = 0x6000,      // Triggered temperature and shunt voltage, single shot
  TRIG_TEMP_BUS_SHUNT = 0x7000,  // Triggered bus voltage, shunt voltage and temperature, single shot
  CONT_BUS = 0x9000,             // Continuous bus voltage only
  CONT_SHUNT = 0xA000,           // Continuous shunt voltage only
  CONT_BUS_SHUNT = 0xB000,       // Continuous shunt and bus voltage
  CONT_TEMP = 0xC000,            // Continuous temperature only
  CONT_TEMP_BUS = 0xD000,        // Continuous bus voltage and temperature
  CONT_TEMP_SHUNT = 0xE000,      // Continuous temperature and shunt voltage
  CONT_TEMP_BUS_SHUNT = 0xF000   // Continuous bus voltage, shunt voltage and temperature [default]
};

enum class INA228_ADC_AVG : uint16_t {
  AVG_0001 = 0,  // 1 [default]
  AVG_0004 = 1,  // 4
  AVG_0016 = 2,  // 16
  AVG_0064 = 3,  // 64
  AVG_0128 = 4,  // 128
  AVG_0256 = 5,  // 256
  AVG_0512 = 6,  // 512
  AVG_1024 = 7   // 1024
};

class INA228_adc_conf {
  uint16_t raw = 0xFB68;

  String CTToString(INA228_CT ct) const;
  String ADCAVGToString(INA228_ADC_AVG avg) const;
  String ModeToString(INA228_ADC_MODE mode) const;

public:

#define DEFINE_CT_METHODS(methodName, mask, shift) \
  void set##methodName(const INA228_CT ct); \
  INA228_CT get##methodName() const;

  DEFINE_CT_METHODS(VBUSCT, INA228_ADC_VBUSCT_MASK, 9)
  DEFINE_CT_METHODS(VTCT, INA228_ADC_VTCT_MASK, 3)
  DEFINE_CT_METHODS(VSHCT, INA228_ADC_VSHCT_MASK, 6)

#undef DEFINE_CT_METHODS

  void setADCAVG(const INA228_ADC_AVG count);
  INA228_ADC_AVG getADCAVG() const;

  void setMode(const INA228_ADC_MODE mode);
  INA228_ADC_MODE getMode() const;

  String toString() const;
};

enum class INA228_DIAG_CONF : uint16_t {
  ALATCH = 1 << 15,     // Alert Latch Enable: 0=Transparent, 1=Latched
  CNVR = 1 << 14,       // Conversion Ready Notify Flag: 0=Disable, 1=Enable
  SLOWALERT = 1 << 13,  // ALERT Comparison: 0=Non-averaged (ADC) value, 1=Averaged value
  APOL = 1 << 12,       // Alert Polarity: 0=Normal (Active-low, open-drain), 1=Inverted (active-high, open-drain)
};

enum class INA228_DIAG : uint16_t {
  ENERGYOF = 1 << 11,  // ENERGY Register Overflow: 0=Normal, 1=Overflow
  CHARGEOF = 1 << 10,  // CHARGE Register Overflow: 0=Normal, 1=Overflow
  MATHOF = 1 << 9,     // Arithmetic Operation Overflow: 0=Normal, 1=Overflow

  TMPOL = 1 << 7,   // Over Temperature Event: 0=Normal, 1=Over Temp Event
  SHNTOL = 1 << 6,  // Over Shunt Voltage Event: 0=Normal, 1=Over Shunt Voltage Event
  SHNTUL = 1 << 5,  // Under Shunt Voltage Event: 0=Normal, 1=Under Shunt Voltage Event
  BUSOL = 1 << 4,   // Bus Over-Limit Event: 0=Normal, 1=Bus Over-Limit Event
  BUSUL = 1 << 3,   // Bus Under-Limit Event: 0=Normal, 1=Bus Under-Limit Event
  POL = 1 << 2,     // Power Over-Limit Event: 0=Normal, 1=Power Over-Limit Event
  CNVRF = 1 << 1,   // Conversion Ready Flag: 0=Disable, 1=Enable
  MEMSTAT = 1 << 0  // Memory Status: 0=Disable, 1=Enable
};

class INA228_diag {
  uint16_t raw = 0x0;
public:

  bool get(INA228_DIAG_CONF diag) const;
  void set(INA228_DIAG_CONF diag, bool value);
  bool has(INA228_DIAG diag) const;
  void clear(INA228_DIAG diag);

  String toString() const;
};

class INA228 {
public:
  INA228(uint8_t _addr);
  
  void setConfig(INA228_conf conf, INA228_adc_conf adc_conf);
  INA228_conf pollConfig();
  INA228_adc_conf pollADCConfig();

  double pollVBus();
  double pollVShunt();
  double pollCurrent();
  double pollPower();
  uint64_t pollEnergy();
  double pollCharge();
  float pollTemp();

  void setDiag(INA228_diag diag);
  INA228_diag pollDiag();

protected:
  bool readReg(const uint8_t reg, void* value, const uint8_t size);
  bool writeReg(const uint8_t reg, void* value, const uint8_t size);

private:
  uint8_t addr;
  int8_t rangeMultiplier;
  
  double CURRENT_LSB = 0.0390625D;
  const double VBUS_LSB = 0.1953125D;
  const double VSHUNT_LSB = 0.078125D;
  const float TEMP_LSB = 7.8125;
};

#endif