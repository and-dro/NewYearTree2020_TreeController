void updateStrip()
{
  stipState.ready = false;
}

void updateStripDelay()
{
  stipState.updateStripTime = millis() + stipState.updateInterval;
}
void clearAll()
{
  strip.fill(BLACK);
}

void setColor(uint8_t index, COLORS color)
{
  strip.setColor(index, color);
}
void setHSV(uint8_t index, uint8_t h, uint8_t s, uint8_t v) 
{
  strip.setHSV(index, h, s, v);
}
void matrixHSV(uint8_t row, uint8_t column, uint8_t h, uint8_t s, uint8_t v) 
{
  if(row < LEDS_ROWS && column < LEDS_COLUMNS)
  {
    setHSV(column * LEDS_ROWS + (column % 2 == 0 ? row : LEDS_ROWS - row - 1  ), h, s, v);
  }
}

uint8_t minValueFromColor(uint8_t color)
{
    if(color<8) return 2;
    if(color<15) return 10;
    if(color<25) return 9;
    if(color<30) return 4;
    if(color<90) return 3;
    if(color<110) return 10;
    if(color<180) return 6;
    if(color<245) return 10;
    return 2;
}

#include "effects/effectsSPARK.h"
#include "effects/effectsDROPS.h"
#include "effects/effectsSTRIPES.h"

void stripEffectsInit()
{
    stipState.currentEffect = STRIPES;

    stipState.totalBrightness = 220;
    stipState.updateInterval = 50;
    
    strip.setBrightness(stipState.totalBrightness);
    sparkInit();
    dropsInit();
    stripesInit();
}

void stripEffectsTick()
{
  if(!stipState.ready)
  {
    uint8_t brightness =  map(RemoteState.brightness, 0, 255, 50, 255);
    if(brightness != stipState.totalBrightness && !NewMode.active)
    {
      stipState.totalBrightness = brightness;
      strip.setBrightness(stipState.totalBrightness);
    }
    uint8_t changeModeBrightness = newModeBrightnessHandler(brightness);
    if(changeModeBrightness != brightness)strip.setBrightness(changeModeBrightness);

    stipState.ready = true;
    switch (stipState.currentEffect)
    {
        case OFF:
            strip.fill(BLACK);
            break;

        case SPARK:
            effects_SPARK_Tick();
            updateStripDelay();
            break;

        case DROPS:
            effects_DROPS_Tick();
            updateStripDelay();
            break;

        case STRIPES:
            effects_STRIPES_Tick();
            updateStripDelay();
            break;

        case LAST_VALUE:
            break;
    }
    strip.show(); // выводим изменения на ленту
  }
  if(stipState.updateStripTime != 0 && stipState.updateStripTime < millis())
  {
    updateStrip();
    updateStripDelay();
  }

}