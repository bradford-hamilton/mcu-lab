#include "ssd1306.h"
#include "nano_gfx.h"

void setup()
{
  Serial.begin(9600);

  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();
  ssd1306_drawRect(0, 0, 127, 63);
  ssd1306_printFixed(8,  32, "Bradford's MCU Lab", STYLE_NORMAL);

  delay(3000);
}

int counter {0};

void loop()
{
  delay(1000);

  char buf[8];

  ssd1306_clearScreen();
  ssd1306_printFixed(64,  32, itoa(counter, buf, 10), STYLE_NORMAL);

  counter++;

  if (counter >= 127) {
    counter = 0;
  }
}
