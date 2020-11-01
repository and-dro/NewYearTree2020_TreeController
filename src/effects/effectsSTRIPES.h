static struct stripesState
{
    uint8_t active; // сколько элементов уже светится
    uint8_t maxActive;
    uint8_t nextTicks; // когда следующий
} stripesState;

static struct stripesValue
{
    bool active;
    uint8_t color;
    uint8_t value;
    uint8_t direction;
    uint8_t head;
    uint8_t wait;
} stripes[LEDS_ROWS];


void stripesInit()
{
    stripesState.active = 0;
    stripesState.maxActive = LEDS_ROWS * 0.6;
    stripesState.nextTicks = 0;
    for (uint8_t i = 0; i < LEDS_ROWS; i++)
    {
        stripes[i].active = false;
        stripes[i].wait = 0;
    }
}

#define STRIPE_SIZE  5
const uint8_t stripeLine[STRIPE_SIZE] = {10,150,240,150,10};

void addNewStripe()
{
    if(stripesState.active == stripesState.maxActive) return;
    if(stripesState.nextTicks)
    {
        stripesState.nextTicks--;
        return;
    }
    for (uint8_t i = 0; i < LEDS_ROWS; i++)
    {
        if(stripes[i].wait) stripes[i].wait--;
    }

    uint8_t nextRow = LEDS_ROWS;
    while (nextRow == LEDS_ROWS)
    {
        uint8_t i = random(0,LEDS_ROWS);
        if(!stripes[i].active && stripes[i].wait == 0) 
        {
            nextRow = i;
        }
    }
    stripes[nextRow].active = true;
    stripes[nextRow].direction = random(0,2);
    stripes[nextRow].color = random(0,255);
    stripes[nextRow].value = random(10,120);

    stripes[nextRow].head = 0;
    
    stripesState.nextTicks = random(LEDS_ROWS * 0.1, LEDS_ROWS * 0.3);
    
    stripesState.active++;
}

void moveStripes()
{
    for (uint8_t i = 0; i < LEDS_ROWS; i++)
    {
        if(stripes[i].active)
        {
            stripes[i].head++;
            uint8_t minValue = minValueFromColor(stripes[i].color);
            for(uint8_t j = 0; j <= STRIPE_SIZE; j++)
            {
                if(stripes[i].direction == 0)
                {
                    matrixHSV(i, j + stripes[i].head - STRIPE_SIZE, stripes[i].color, 255, map(stripeLine[j], 10, 255, minValue, stripes[i].value));
                }
                else
                {
                    matrixHSV(i, j + (LEDS_COLUMNS - stripes[i].head) - STRIPE_SIZE, stripes[i].color, 255, map(stripeLine[j], 10, 255, minValue, stripes[i].value));
                }
            }
            if(stripes[i].head > LEDS_COLUMNS + STRIPE_SIZE)
            {
                stripes[i].active = 0;
                stripes[i].wait = (LEDS_ROWS - stripesState.maxActive) / 2;
                stripesState.active--;
            }
        }
    }
}

void effects_STRIPES_Tick()
{
    clearAll();
    addNewStripe();
    moveStripes();

    uint16_t updateInterval = map(255 - RemoteState.speed, 0, 255, 35, 200);
    if(stipState.updateInterval != updateInterval)
    {
        stipState.updateInterval = updateInterval;
    }
}