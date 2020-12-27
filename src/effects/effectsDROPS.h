static struct dropsState
{
    uint8_t active; // сколько элементов уже светится
    uint8_t maxActive;
    uint16_t nextTicks; // когда следующий
    uint8_t maxSteps;
    uint8_t color_shift;
    uint8_t maxSkip;
    uint8_t skip;
} dropsState;

#define DROP_SIZE  8
const uint8_t dropLine[DROP_SIZE] = {10,255,240,150,100,50,15,10};

static struct dropValue
{
    bool active;
    uint8_t color;
    uint8_t value;
    uint8_t head; 
    uint8_t step;
} drops[LEDS_COLUMNS];

void dropsInit()
{
    dropsState.active = 0;
    dropsState.maxActive = LEDS_COLUMNS - 1;
    dropsState.nextTicks = 0;
    dropsState.maxSteps = 5;
    for (uint8_t i = 0; i < LEDS_COLUMNS; i++)
    {
        drops[i].active = false;
    }
}

void addNewDrop()
{
    if(dropsState.active == dropsState.maxActive) return;
    if(dropsState.nextTicks)
    {
        dropsState.nextTicks--;
        return;
    }
    uint8_t nextColumn = LEDS_COLUMNS;
    while (nextColumn == LEDS_COLUMNS)
    {
        uint8_t i = random(0,LEDS_COLUMNS-1);
        if(!drops[i].active) 
        {
            nextColumn = i;
        }
    }
    drops[nextColumn].active = true;
    drops[nextColumn].head = LEDS_ROWS + DROP_SIZE * 2;
    drops[nextColumn].color = random(0,255);
    drops[nextColumn].value = random(120,255);
    drops[nextColumn].step = 0;
    
    dropsState.active++;

    dropsState.nextTicks = random(6, 8) * dropsState.maxSteps * dropsState.active / 10;
}

void moveDrops()
{
    for (uint8_t i = 0; i < LEDS_COLUMNS; i++)
    {
        if(drops[i].active)
        {
            if(drops[i].step >= dropsState.maxSteps)
            {
                drops[i].head--;
                drops[i].step = 0;
            }
            uint8_t minValue = minValueFromColor(drops[i].color);
            for(uint8_t j = 0; j < DROP_SIZE; j++)
            {
                uint8_t stepValue = j == 0 ? 10 : dropLine[j - 1];
                uint8_t destValue = dropLine[j];
                uint8_t gap = stepValue < destValue ? destValue - stepValue : stepValue - destValue;
                uint8_t diff = gap  * (dropsState.maxSteps - drops[i].step)  / dropsState.maxSteps;
                 
                stepValue = stepValue < destValue ? destValue - diff : destValue + diff;

                matrixHSV(drops[i].head + j - DROP_SIZE, i, drops[i].color, 255, map(stepValue, 10, 255, minValue, drops[i].value));
            }
            if(drops[i].head == 0)
            {
                drops[i].active = false;
                dropsState.active--;

                uint16_t nextTicks = random(6, 8) * dropsState.maxSteps * dropsState.active / 10;
                if(nextTicks < dropsState.nextTicks) dropsState.nextTicks = nextTicks;
            }
            else
            {
                drops[i].step++;
            }
            
        }
    }
}



void moveDropsStar()
{
    if(!STAR_SIZE) return;

    for (uint8_t node = 0; node < 5; node++)
    {
      for (uint8_t shift = 0; shift < STAR_SIZE; shift++)
      {
        starHSV(node, shift, node * 51 + shift * 5 + dropsState.color_shift, 255, 40);
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

void effects_DROPS_Tick()
{
    uint8_t maxSteps = map(255 - RemoteState.speed, 0, 255, 3, 15);
    uint8_t maxSkip = map(255 - RemoteState.speed, 0, 255, 1, 5);

    if(dropsState.maxSteps != maxSteps)
    {
        dropsState.maxSteps = maxSteps;
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
    addNewDrop();
    moveDrops();
    moveDropsStar();
}
