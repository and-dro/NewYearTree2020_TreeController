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
  setHSV(column * LEDS_ROWS + (column % 2 == 0 ? row : LEDS_ROWS - row - 1  ), h, s, v);
}


#include "effects/effectsSPARK.h"
#include "effects/effectsDROPS.h"

void stripEffectsInit()
{
    stipState.currentEffect = DROPS;

    stipState.totalBrightness = 220;
    stipState.updateInterval = 50;
    
    sparkInit();
    dropsInit();
}

void stripEffectsTick()
{
  if(!stipState.ready)
  {
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

        case SPARK_FIXED:
            effects_SPARK_Tick();
            updateStripDelay();
            break;

        case DROPS:
            effects_DROPS_Tick();
            updateStripDelay();
            break;
    }
    strip.setBrightness(stipState.totalBrightness);
    strip.show(); // выводим изменения на ленту
  }
  if(stipState.updateStripTime != 0 && stipState.updateStripTime < millis())
  {
    updateStrip();
    updateStripDelay();
  }

}