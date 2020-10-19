void dropsInit()
{

}

static uint8_t dropLine[8][2] = {{3,10},{23,100},{23,90},{18,80},{12,70},{7,50},{3,30},{3,0}};

void effects_DROPS_Tick()
{
    clearAll();
    for(uint8_t i = 0; i<LEDS_COLUMNS; i++)
    {
        uint8_t head = 2;
        for(uint8_t j = 0; j<8; j++)
            matrixHSV(head + j, i, i * 25, dropLine[j][1] + 155, dropLine[j][0]);
    }

}