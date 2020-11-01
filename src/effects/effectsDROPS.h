static struct dropsState
{
    uint8_t active; // сколько элементов уже светится
    uint8_t maxActive;
    uint8_t nextTicks; // когда следующий
} dropsState;

static struct dropValue
{
    bool active;
    uint8_t color;
    uint8_t value;
    uint8_t head; 
    uint8_t wait;
} drops[LEDS_COLUMNS];


void dropsInit()
{
    dropsState.active = 0;
    dropsState.maxActive = LEDS_COLUMNS * 0.4;
    dropsState.nextTicks = 0;
    for (uint8_t i = 0; i < LEDS_COLUMNS; i++)
    {
        drops[i].active = false;
        drops[i].wait = 0;
    }
}

#define DROP_SIZE  8
const uint8_t dropLine[DROP_SIZE] = {10,255,240,150,100,50,15,10};

void addNewDrop()
{
    if(dropsState.active == dropsState.maxActive) return;
    if(dropsState.nextTicks)
    {
        dropsState.nextTicks--;
        return;
    }
    for (uint8_t i = 0; i < LEDS_COLUMNS; i++)
    {
        if(drops[i].wait) drops[i].wait--;
    }
    uint8_t nextColumn = LEDS_COLUMNS;
    while (nextColumn == LEDS_COLUMNS)
    {
        uint8_t i = random(0,LEDS_COLUMNS-1);
        if(!drops[i].active && drops[i].wait == 0) 
        {
            nextColumn = i;
        }
    }
    drops[nextColumn].active = true;
    drops[nextColumn].head = LEDS_ROWS + DROP_SIZE;
    drops[nextColumn].color = random(0,255);
    drops[nextColumn].value = random(10,120);
    
    dropsState.nextTicks = random(LEDS_ROWS * 0.1, LEDS_ROWS * 0.3);
    dropsState.active++;
}

void moveDrops()
{
    for (uint8_t i = 0; i < LEDS_COLUMNS; i++)
    {
        if(drops[i].active)
        {
            drops[i].head--;
            uint8_t minValue = minValueFromColor(drops[i].color);
            for(uint8_t j = 0; j < DROP_SIZE; j++)
                matrixHSV(drops[i].head + j, i, drops[i].color, 255, map(dropLine[j], 10, 255, minValue, drops[i].value));
            
            if(drops[i].head == 0)
            {
                drops[i].active = false;
                drops[i].wait = (LEDS_COLUMNS - dropsState.maxActive) / 2;
                dropsState.active--;
            }
        }
    }
}

void effects_DROPS_Tick()
{
    clearAll();
    addNewDrop();
    moveDrops();

    uint16_t updateInterval = map(255 - RemoteState.speed, 0, 255, 5, 200);
    if(stipState.updateInterval != updateInterval)
    {
        stipState.updateInterval = updateInterval;
    }
}
