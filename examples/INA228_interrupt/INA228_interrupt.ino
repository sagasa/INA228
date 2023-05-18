#include <Wire.h>
#include "ina228.h"


INA228 ina228(0x4A);


void read(){
  Serial.print("VBus:");
  Serial.println(ina228.pollVBus());

  Serial.print("Current:");
  Serial.println(ina228.pollCurrent());

  Serial.print("Power:");
  Serial.println(ina228.pollPower());

  Serial.print("Charge:");
  Serial.println(ina228.pollCharge());

  ina228.pollDiag();
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  while(!Serial){}
  
  INA228_conf conf{};
  INA228_adc_conf adcconf{};
  INA228_diag diag{};
  
  conf.setADCRangeLow(true);
  conf.setResetAcc(true);
  adcconf.setADCAVG(INA228_ADC_AVG::AVG_0064);
  adcconf.setVBUSCT(INA228_CT::CT_0050us);
  adcconf.setVSHCT(INA228_CT::CT_0050us);
  adcconf.setMode(INA228_ADC_MODE::CONT_BUS_SHUNT);

  diag.set(INA228_DIAG_CONF::CNVR, true);
  diag.set(INA228_DIAG_CONF::ALATCH, true);

  ina228.setConfig(conf, adcconf);
  ina228.setDiag(diag);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), read, FALLING);

  delay(50);
  ina228.pollDiag();
}

void loop()
{

}
