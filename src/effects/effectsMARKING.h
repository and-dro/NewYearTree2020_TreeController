uint8_t color_shift = 0;
void markingInit()
{
}

void effects_MARKING_Tick()
{
  for(uint8_t c = 0; c < LEDS_COLUMNS; c++)
  {
    for(uint8_t r = 0; r < LEDS_ROWS / 3; r++)
    {
      matrixHSV(r, c, c * (120 / LEDS_COLUMNS) + color_shift, 255, 40);
    }
    for(uint8_t r = LEDS_ROWS / 3; r < 2 * LEDS_ROWS / 3; r++)
    {
      matrixHSV(r, LEDS_COLUMNS - c - 1, c * (120 / LEDS_COLUMNS) + color_shift + 130, 255, 40);
    }
    for(uint8_t r = 2 * LEDS_ROWS / 3; r < LEDS_ROWS; r++)
    {
      matrixHSV(r, c, c * (120 / LEDS_COLUMNS) + color_shift + 80, 255, 40);
    }
  }
  color_shift += 2;
  // uint16_t updateInterval = map(255 - RemoteState.speed, 0, 255, 1, 40);
  uint16_t updateInterval = map(255 - 128, 0, 255, 1, 10);
  if(stipState.updateInterval != updateInterval)
  {
      stipState.updateInterval = updateInterval;
  }
}