Thermometer with M5StickCPlus2.

   board manager:
   
      https://raw.githubusercontent.com/espressif/arduino-esp32/ghpages/package_esp32_dev_index.json
      
   needs:

    - M5StickC Plus2                  https://docs.m5stack.com/en/core/M5StickC%20PLUS2
    - thermistor: SEMITEC/ 103JT-050  https://akizukidenshi.com/goodsaffix/jt_thermistor.pdf    
    - 4.7k (metal-glaze) resistor
    
   wiring:
   
      M5StickCP2
        3V3     <-->  4.7k resister  <-->  GPIO 25      
        GPIO 25 <-->  terminal A of thermistor      
        GND     <-->  terminal B of thermistor

   
