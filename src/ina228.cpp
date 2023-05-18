#include "ina228.h"
#include <Wire.h>

// ======  INA228_conf  ======
#define DEFINE_FLAG_METHODS(flagName, flagMask) \
  void INA228_conf::set##flagName(const bool doSet) { \
    if (doSet) { \
      raw |= flagMask; \
    } else { \
      raw &= ~flagMask; \
    } \
  } \
  bool INA228_conf::get##flagName() const { \
    return (raw & flagMask) != 0; \
  }

DEFINE_FLAG_METHODS(Reset, INA228_CONF_RST_FLAG)
DEFINE_FLAG_METHODS(ResetAcc, INA228_CONF_RSTACC_FLAG)
DEFINE_FLAG_METHODS(TempComp, INA228_CONF_TEMPCOMP_FLAG)
DEFINE_FLAG_METHODS(ADCRangeLow, INA228_CONF_ADCRANGE_FLAG)

#undef DEFINE_FLAG_METHODS

void INA228_conf::setConvDly(const uint16_t mills) {
  raw = (raw & ~INA228_CONF_CONVDLY_MASK) | (((mills / 2) << 6) & INA228_CONF_CONVDLY_MASK);
}

uint16_t INA228_conf::getConvDly() const {
  return ((raw & INA228_CONF_CONVDLY_MASK) >> 6) * 2;
}

String INA228_conf::toString() const {
  String result = "=== INA228_conf ===\n";
  result += "Reset:       " + String(getReset() ? "true" : "false") + "\n";
  result += "ResetAcc:    " + String(getResetAcc() ? "true" : "false") + "\n";
  result += "TempComp:    " + String(getTempComp() ? "true" : "false") + "\n";
  result += "ADCRangeLow: " + String(getADCRangeLow() ? "true" : "false") + "\n";
  result += "ConvDly:     " + String(getConvDly()) + "ms\n";
  result += "====================";
  return result;
}
// ====  INA228_adc_conf  ====

String INA228_adc_conf::CTToString(INA228_CT ct) const {
  switch (ct) {
    case INA228_CT::CT_0050us: return "50us";
    case INA228_CT::CT_0084us: return "84us";
    case INA228_CT::CT_0150us: return "150us";
    case INA228_CT::CT_0280us: return "280us";
    case INA228_CT::CT_0540us: return "540us";
    case INA228_CT::CT_1052us: return "1052us";
    case INA228_CT::CT_2074us: return "2074us";
    case INA228_CT::CT_4120us: return "4120us";
    default: return "err";
  }
}

String INA228_adc_conf::ADCAVGToString(INA228_ADC_AVG avg) const {
  switch (avg) {
    case INA228_ADC_AVG::AVG_0001: return "1";
    case INA228_ADC_AVG::AVG_0004: return "4";
    case INA228_ADC_AVG::AVG_0016: return "16";
    case INA228_ADC_AVG::AVG_0064: return "64";
    case INA228_ADC_AVG::AVG_0128: return "128";
    case INA228_ADC_AVG::AVG_0256: return "256";
    case INA228_ADC_AVG::AVG_0512: return "512";
    case INA228_ADC_AVG::AVG_1024: return "1024";
    default: return "err";
  }
}

String INA228_adc_conf::ModeToString(INA228_ADC_MODE mode) const {
  switch (mode) {
    case INA228_ADC_MODE::SHUTDOWN: return "SHUTDOWN";
    case INA228_ADC_MODE::TRIG_BUS: return "TRIG_BUS";
    case INA228_ADC_MODE::TRIG_SHUNT: return "TRIG_SHUNT";
    case INA228_ADC_MODE::TRIG_BUS_SHUNT: return "TRIG_BUS_SHUNT";
    case INA228_ADC_MODE::TRIG_TEMP: return "TRIG_TEMP";
    case INA228_ADC_MODE::TRIG_TEMP_BUS: return "TRIG_TEMP_BUS";
    case INA228_ADC_MODE::TRIG_TEMP_SHUNT: return "TRIG_TEMP_SHUNT";
    case INA228_ADC_MODE::TRIG_TEMP_BUS_SHUNT: return "TRIG_TEMP_BUS_SHUNT";
    case INA228_ADC_MODE::CONT_BUS: return "CONT_BUS";
    case INA228_ADC_MODE::CONT_SHUNT: return "CONT_SHUNT";
    case INA228_ADC_MODE::CONT_BUS_SHUNT: return "CONT_BUS_SHUNT";
    case INA228_ADC_MODE::CONT_TEMP: return "CONT_TEMP";
    case INA228_ADC_MODE::CONT_TEMP_BUS: return "CONT_TEMP_BUS";
    case INA228_ADC_MODE::CONT_TEMP_SHUNT: return "CONT_TEMP_SHUNT";
    case INA228_ADC_MODE::CONT_TEMP_BUS_SHUNT: return "CONT_TEMP_BUS_SHUNT";
    default: return "err";
  }
}

#define DEFINE_CT_METHODS(methodName, mask, shift) \
  void INA228_adc_conf::set##methodName(const INA228_CT ct) { \
    raw = (raw & ~mask) | (static_cast<uint16_t>(ct) << shift); \
  } \
\
  INA228_CT INA228_adc_conf::get##methodName() const { \
    return static_cast<INA228_CT>((raw & mask) >> shift); \
  }

DEFINE_CT_METHODS(VBUSCT, INA228_ADC_VBUSCT_MASK, 9)
DEFINE_CT_METHODS(VTCT, INA228_ADC_VTCT_MASK, 3)
DEFINE_CT_METHODS(VSHCT, INA228_ADC_VSHCT_MASK, 6)

#undef DEFINE_CT_METHODS

void INA228_adc_conf::setADCAVG(const INA228_ADC_AVG count) {
  raw = (raw & ~INA228_ADC_AVG_MASK) | (static_cast<uint16_t>(count));
}

INA228_ADC_AVG INA228_adc_conf::getADCAVG() const {
  return static_cast<INA228_ADC_AVG>((raw & INA228_ADC_AVG_MASK));
}

void INA228_adc_conf::setMode(const INA228_ADC_MODE mode) {
  raw = (raw & ~INA228_ADC_MODE_MASK) | static_cast<uint16_t>(mode);
}
INA228_ADC_MODE INA228_adc_conf::getMode() const {
  return static_cast<INA228_ADC_MODE>(raw & INA228_ADC_MODE_MASK);
}

String INA228_adc_conf::toString() const {
  String dump = "=== INA228_adc_conf ===";
  dump += "\nVBUSCT: " + CTToString(getVBUSCT());
  dump += "\nVTCT:   " + CTToString(getVTCT());
  dump += "\nVSHCT:  " + CTToString(getVSHCT());
  dump += "\nADCAVG: " + ADCAVGToString(getADCAVG());
  dump += "\nMode:   " + ModeToString(getMode());
  dump += "\n=======================\n";
  return dump;
}

// ======  INA228_diag  ======
bool INA228_diag::get(INA228_DIAG_CONF diag) const {
  return (raw & static_cast<uint16_t>(diag)) != 0;
}

void INA228_diag::set(INA228_DIAG_CONF diag, bool value) {
  if (value) {
    raw |= static_cast<uint16_t>(diag);
  } else {
    raw &= ~static_cast<uint16_t>(diag);
  }
}

bool INA228_diag::has(INA228_DIAG diag) const {
  return (raw & static_cast<uint16_t>(diag)) != 0;
}

void INA228_diag::clear(INA228_DIAG diag) {
  raw &= ~static_cast<uint16_t>(diag);
}

String INA228_diag::toString() const {
  String dump = "===== DIAG =====\nALATCH:    ";
  dump += get(INA228_DIAG_CONF::ALATCH) ? "true" : "false";
  dump += "\nCNVR:      ";
  dump += get(INA228_DIAG_CONF::CNVR) ? "true" : "false";
  dump += "\nSLOWALERT: ";
  dump += get(INA228_DIAG_CONF::SLOWALERT) ? "true" : "false";
  dump += "\nAPOL:      ";
  dump += get(INA228_DIAG_CONF::APOL) ? "true" : "false";
  dump += "\n===== ALRT =====";
  if (has(INA228_DIAG::ENERGYOF)) {
    dump += "\nENERGYOF";
  }
  if (has(INA228_DIAG::CHARGEOF)) {
    dump += "\nCHARGEOF";
  }
  if (has(INA228_DIAG::MATHOF)) {
    dump += "\nMATHOF";
  }
  if (has(INA228_DIAG::TMPOL)) {
    dump += "\nTMPOL";
  }
  if (has(INA228_DIAG::SHNTOL)) {
    dump += "\nSHNTOL";
  }
  if (has(INA228_DIAG::SHNTUL)) {
    dump += "\nSHNTUL";
  }
  if (has(INA228_DIAG::BUSOL)) {
    dump += "\nBUSOL";
  }
  if (has(INA228_DIAG::BUSUL)) {
    dump += "\nBUSUL";
  }
  if (has(INA228_DIAG::POL)) {
    dump += "\nPOL";
  }
  if (has(INA228_DIAG::CNVRF)) {
    dump += "\nCNVRF";
  }
  if (has(INA228_DIAG::MEMSTAT)) {
    dump += "\nMEMSTAT";
  }
  dump += "\n================";
  return dump;
}
// ===========================



#define EXPECT(value, expect) \
  if (value != expect) return false;

#define FIX_20BIT(arg) arg & (1 << 19)?-((~arg&0xFFFFF) + 1):arg
#define FIX_24BIT(arg) arg & (1 << 23)?-((~arg&0xFFFFFF) + 1):arg

INA228::INA228(uint8_t _addr) : addr(_addr), rangeMultiplier(4) {}

void INA228::setConfig(INA228_conf conf, INA228_adc_conf adc_conf) {
  rangeMultiplier = conf.getADCRangeLow()?1:4;
  writeReg(INA228_REG_CONF, &conf, 2);
  writeReg(INA228_REG_ADC_CONF, &adc_conf, 2);
}

INA228_conf INA228::pollConfig() {
  INA228_conf conf{};
  readReg(INA228_REG_CONF, &conf, 2);
  return conf;
}

INA228_adc_conf INA228::pollADCConfig() {
  INA228_adc_conf conf{};
  readReg(INA228_REG_ADC_CONF, &conf, 2);
  return conf;
}

bool INA228::readReg(const uint8_t reg, void* value, const uint8_t size) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  EXPECT(Wire.endTransmission(false), 0);
  EXPECT(Wire.requestFrom(addr, size), size);
  for (int i = size; 0 < i; i--) {
    uint8_t slice = Wire.read();
    ((uint8_t*)value)[i - 1] = slice;
  }
  return true;
}

bool INA228::writeReg(const uint8_t reg, void* value, const uint8_t size) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  for (int i = size; 0 < i; i--) {
    uint8_t slice = ((uint8_t*)value)[i - 1];
    Wire.write(slice);
  }
  EXPECT(Wire.endTransmission(), 0);
  return true;
}

double INA228::pollVBus() {
  int32_t res = 0;
  readReg(INA228_REG_VBUS, &res, 3);
  res = FIX_20BIT(res >> 4);
  return res * VBUS_LSB;
}

double INA228::pollVShunt() {
  int32_t res = 0;
  readReg(INA228_REG_VSHUNT, &res, 3);
  res = FIX_24BIT(res);
  return res * VSHUNT_LSB * rangeMultiplier;
}

double INA228::pollCurrent() {
  int32_t res = 0;
  readReg(INA228_REG_CURRENT, &res, 3);
  res = FIX_20BIT(res >> 4);
  return res * CURRENT_LSB * rangeMultiplier;
}

double INA228::pollPower() {
  int32_t res = 0;
  readReg(INA228_REG_POWER, &res, 3);
  res = FIX_24BIT(res);
  return res * CURRENT_LSB * 3.2 * rangeMultiplier;
}

uint64_t INA228::pollEnergy() {
  uint64_t res = 0;
  readReg(INA228_REG_ENERGY, &res, 5);
  return res * CURRENT_LSB * 16.0 * 3.2 * rangeMultiplier;
}

double INA228::pollCharge() {
  int64_t res = 0;
  readReg(INA228_REG_CHARGE, &res, 5);
  if(res >> 39){
    res = -((~res&0xFFFFFFFFFFLL) + 1);
  }
  return res* CURRENT_LSB * rangeMultiplier;
}

float INA228::pollTemp() {
  int16_t res = 0;
  readReg(INA228_REG_DIETEMP, &res, 2);
  return res * TEMP_LSB;
}

void INA228::setDiag(INA228_diag diag) {
  writeReg(INA228_REG_DIAG_ALRT, &diag, 2);
}

INA228_diag INA228::pollDiag() {
  INA228_diag diag{};
  readReg(INA228_REG_DIAG_ALRT, &diag, 2);
  return diag;
}

#undef EXPECT
#undef FIX_20BIT
#undef FIX_24BIT

