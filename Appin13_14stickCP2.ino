/*
Thermometer with M5StickCPlus2
    needs:
    - thermistor: SEMITEC/ 103JT-050
    - 4.7k metal-glaze resistor
    wiring:
      M5Stick 3V3 <--> GPIO 25
      GPIO 25     <--> terminal A of thermistor
      GND         <--> terminal B of thermistor
*/

#include <M5StickCPlus2.h>
#include <math.h>
#include "constants.h"

const uint8_t tmp_left = 80;
const uint8_t tmp_f_size = 40;
const uint8_t label_f_size = 28;
const uint8_t avg_bottom = 55;
const uint8_t cur_bottom = 105;

int interval_r = 500;   // /ms
uint32_t prev_time_r = 0;

uint8_t const tmp_val_size = 15;
uint16_t tmp_val[tmp_val_size];
uint8_t count_tmp = 0;
bool progress_bar = true;
uint16_t prev_temperature = 0;
uint16_t prev_average = 0;

class Sensor{
  public:
    int a0;
};

Sensor sensor_values;
float Bvalue = 3435.f; //[kelvin] Bvalue of Thermistor
float resistance_0 = series_resistance0[STICK_CP2_ID];  // /k_ohm  Pull-up Series Resistance

float Calc_thermistorTemperature(float Bvalue, float Resistance, int ADCvalue)
{
  float ADCMax = 4095;
  float R0 = 10;
  float T0 = 25;
  float ABS0 = 273.25;
  float R_Thermistor = ADCvalue*Resistance/(ADCMax-ADCvalue);
  float temperature = 1.f / ((1.f / Bvalue) * log(R_Thermistor / R0) + 1.f / (T0 + ABS0)) - ABS0;
  return temperature;
}

int getADC_OverSampling(uint8_t pin, int nSamples)
{
	int i;
	int accumulator = 0;
	for (i = 0; i < nSamples;  i++) {
		accumulator += analogRead(pin);
		ets_delay_us(10);  // delay 10 microseconds
	}
	return accumulator / nSamples;
}

void Get_sensorValues()
{
  int ana0 = getADC_OverSampling(GPIO_THM, 16);
  float x1 = calibration[STICK_CP2_ID][0] * 10;
  float y1 = calibration[STICK_CP2_ID][1] * 10;
  float x2 = calibration[STICK_CP2_ID][2] * 10;
  float y2 = calibration[STICK_CP2_ID][3] * 10;
  sensor_values.a0 = (int) ((Calc_thermistorTemperature(Bvalue, resistance_0, ana0) * 10 - x1) * (y2 - y1) / (x2 - x1) + y1);
}

/*
void Send_onTime ()
{
  char str0[60];
  sprintf(str0, "%.1f℃", (float)sensor_values.a0 / 10);
  Serial.println(str0);
}
*/

void Set_text(String str, uint8_t x, uint8_t y, uint8_t datum, uint8_t size, uint16_t color)
{
  M5.Lcd.setTextColor(color);
  M5.Lcd.setTextDatum(datum);
  if(size >= 40) {
    M5.Lcd.setTextFont(&fonts::lgfxJapanGothicP_40);
  }else if(size >= 28) {
    M5.Lcd.setTextFont(&fonts::lgfxJapanGothicP_28);
  }else if(size >= 20) {
    M5.Lcd.setTextFont(&fonts::lgfxJapanGothicP_20);
  }
  M5.Lcd.drawString(str, x, y);
}


void setup(void)
{
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  M5.Lcd.setRotation(1);

  char str0[60];
  sprintf(str0, "StickCPlus2 ID = %d ", STICK_CP2_ID);
  Set_text(str0, M5.Lcd.width(), M5.Lcd.height(), bottom_right, 20, TFT_WHITE);
  Set_text("AVG:", tmp_left - 2, avg_bottom - 2, bottom_right, label_f_size, TFT_YELLOW);
  Set_text("CUR:", tmp_left - 2, cur_bottom - 2, bottom_right, label_f_size, TFT_WHITE);

  M5.Lcd.setTextDatum(middle_center);
  M5.Lcd.setTextFont(&fonts::lgfxJapanGothicP_40);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setBrightness(LCD_BRIGHTNESS);

  setCpuFrequencyMhz(80);
  Serial.begin(115200);
  pinMode(GPIO_THM, INPUT);
}

void loop(void)
{
  bool measure_r = false;
  uint32_t curr_time = millis();
  if(interval_r <= curr_time - prev_time_r) {
    prev_time_r = curr_time;
    Get_sensorValues();
    //Send_onTime();
    uint16_t temp = sensor_values.a0;
    if(temp != prev_temperature) {
      char str0[30];
      sprintf(str0, "%.1f℃", (float)sensor_values.a0 / 10);
      M5.Lcd.fillRect(tmp_left, cur_bottom - tmp_f_size - 3, M5.Lcd.width() - tmp_left, tmp_f_size + 3, TFT_BLACK);
      Set_text(str0, tmp_left, cur_bottom, bottom_left, tmp_f_size, TFT_WHITE);
    }
    prev_temperature = temp;

    uint16_t avg_sum = 0;
    for(int i = 0; i < tmp_val_size - 1; i++) {
      tmp_val[i] = tmp_val[i + 1];
      avg_sum += tmp_val[i];
    }
    tmp_val[tmp_val_size - 1] = sensor_values.a0;
    avg_sum += tmp_val[tmp_val_size - 1];
    uint16_t avg = round(avg_sum / tmp_val_size);
    if(count_tmp < tmp_val_size) {
      M5.Lcd.fillRect(0, 0, M5.Lcd.width() / tmp_val_size * count_tmp, 10, TFT_YELLOW);
      count_tmp++;
    }else {
      if(progress_bar) {
        progress_bar = false;
        M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 10, TFT_BLACK);
      }
      if(avg != prev_average) {
        char str1[30];
        sprintf(str1, "%.1f℃", (float)avg / 10);
        M5.Lcd.fillRect(tmp_left, avg_bottom - tmp_f_size - 3, M5.Lcd.width() - tmp_left, tmp_f_size + 3, TFT_BLACK);
        Set_text(str1, tmp_left, avg_bottom, bottom_left, tmp_f_size, YELLOW);
      }
      prev_average = avg;
    }
  }
  delay(10);
}

// cSpell:ignore THRMOSENS, thrmo