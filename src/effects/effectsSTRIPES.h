static struct stripesState
{
    uint8_t active; // сколько элементов уже светится
    uint8_t maxActive;
    uint8_t nextTicks; // когда следующий
    uint8_t maxSteps;
    uint8_t color_shift;
    uint8_t maxSkip;
    uint8_t skip;
} stripesState;

static struct stripesValue
{
    bool active;
    uint8_t color;
    uint8_t value;
    uint8_t direction;
    uint8_t head;
    uint8_t step;
} stripes[LEDS_ROWS];


void stripesInit()
{
    stripesState.active = 0;
    stripesState.maxActive = LEDS_ROWS * 0.8;
    stripesState.nextTicks = 0;
    for (uint8_t i = 0; i < LEDS_ROWS; i++)
    {
        stripes[i].active = false;
    }
}

#define STRIPE_SIZE  5
const uint8_t stripeLine[STRIPE_SIZE] = {12,50,240,50,12};

void addNewStripe()
{
    if(stripesState.active == stripesState.maxActive) return;
    if(stripesState.nextTicks)
    {
        stripesState.nextTicks--;
        return;
    }

    uint8_t nextRow = LEDS_ROWS;
    while (nextRow == LEDS_ROWS)
    {
        uint8_t i = random(0,LEDS_ROWS);
        if(!stripes[i].active) 
        {
            nextRow = i;
        }
    }
    stripes[nextRow].active = true;
    stripes[nextRow].direction = random(0,2);
    stripes[nextRow].color = random(0,255);
    stripes[nextRow].value = random(120, 255);
    stripes[nextRow].step = 0;
    stripes[nextRow].head = 0;
    
    stripesState.active++;

    stripesState.nextTicks = random(1, 3) * stripesState.maxSteps * stripesState.active / 10;
}

void moveStripes()
{
    for (uint8_t i = 0; i < LEDS_ROWS; i++)
    {
        if(stripes[i].active)
        {
            
            if(stripes[i].step >= stripesState.maxSteps)
            {
                stripes[i].head++;
                stripes[i].step = 0;
            }

            uint8_t minValue = minValueFromColor(stripes[i].color);
            for(uint8_t j = 0; j < STRIPE_SIZE; j++)
            {
                if(stripes[i].direction == 0)
                {
                    uint8_t stepValue = stripeLine[j];
                    uint8_t destValue = j == 0 ? 12 : stripeLine[j - 1];
                    uint8_t gap = stepValue < destValue ? destValue - stepValue : stepValue - destValue;
                    uint8_t diff = gap  * (stripesState.maxSteps - stripes[i].step)  / stripesState.maxSteps;
                    
                    stepValue = stepValue < destValue ? destValue - diff : destValue + diff;
                    
                    matrixHSV(i, j + stripes[i].head - STRIPE_SIZE, stripes[i].color, 255, map(stepValue, 10, 255, minValue, stripes[i].value));
                }
                else
                {
                    uint8_t stepValue = j == 0 ? 12 : stripeLine[j - 1];
                    uint8_t destValue = stripeLine[j];
                    uint8_t gap = stepValue < destValue ? destValue - stepValue : stepValue - destValue;
                    uint8_t diff = gap  * (stripesState.maxSteps - stripes[i].step)  / stripesState.maxSteps;
                    
                    stepValue = stepValue < destValue ? destValue - diff : destValue + diff;
                    
                    matrixHSV(i, j + (LEDS_COLUMNS - stripes[i].head), stripes[i].color, 255, map(stepValue, 10, 255, minValue, stripes[i].value));
                }
            }
            if(stripes[i].head > LEDS_COLUMNS + STRIPE_SIZE)
            {
                stripes[i].active = 0;
                stripesState.active--;
            }
            else
            {
                stripes[i].step++;
            }
        }
    }
}

void moveStripesStar()
{
    if(!STAR_SIZE) return;

    for (uint8_t node = 0; node < 5; node++)
    {
      for (uint8_t shift = 0; shift < STAR_SIZE; shift++)
      {
          uint8_t color = shift * 15 + dropsState.color_shift;
          starHSV(node, shift, color, 255, 40);
      }
    }
    if(dropsState.skip == 0)
    {
        dropsState.color_shift += 3;
        dropsState.skip = dropsState.maxSkip;
    }
    else
    {
        dropsState.skip--;
    }
}

void effects_STRIPES_Tick()
{
    uint8_t maxSteps = map(255 - RemoteState.speed, 0, 255, 8, 25);
    uint8_t maxSkip = map(255 - RemoteState.speed, 0, 255, 1, 5);
    
    if(stripesState.maxSteps != maxSteps)
    {
        stripesState.maxSteps = maxSteps;
    }
    if(dropsState.maxSkip != maxSkip)
    {
        dropsState.maxSkip = maxSkip;
    }
    if(stipState.updateInterval != 1)
    {
        stipState.updateInterval = 1;
    }

    clearAll();
    addNewStripe();
    moveStripes();
    moveStripesStar();
}