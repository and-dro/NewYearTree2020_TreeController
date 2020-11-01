static struct sparkState
{
    uint8_t active; // сколько элементов уже светится
    uint8_t maxActive;
    uint8_t ticksToNew;
} sparkState;

void sparkInit()
{
    sparkState.maxActive = NUMLEDS / 5;
    sparkState.ticksToNew = 0;
}

void sparkAddNewLamp();
void sparkChangeValue();

void effects_SPARK_Tick(){
  sparkAddNewLamp();
  sparkChangeValue();


    uint16_t updateInterval = map(255 - RemoteState.speed, 0, 255, 1, 40);
    if(stipState.updateInterval != updateInterval)
    {
        stipState.updateInterval = updateInterval;
    }
}

uint8_t getNewPosition()
{
  uint8_t newPosition = 0;
  while (true)
  {
    newPosition = random(NUMLEDS); // берем рандомайзер
    bool checked = true;
    for (byte i = 0; i < NUMLEDS; i++)
    {
      if (ledPoints[i].mode > 0)
      {
        checked = checked && abs(i - newPosition) > 1; // рядом нельзя, слишком часто нельзя
      }
    }
    if (checked)
    {
      // сделаем другие элементы доступными для выбора (т.к. при гашении они блокируются для выбора)
      for (uint8_t j = 0; j < NUMLEDS; j++)
      {
        // Ограничение частоты выбора случайного элемента, устанавливается после гашения в макс(128, 255 - число активных элементов)
        if (ledPoints[j].mode == 0xFF) {
          ledPoints[j].value--;
          if (ledPoints[j].value == 0) ledPoints[j].mode = 0;
        }
      }
      break;
    }
  }
  return newPosition;
}

void sparkAddNewLamp()
{
    if(sparkState.active >= sparkState.maxActive) return;
    //  у нас 74 тика на цикл по одной лампе и 150 ламп, из них 30 горит
    //  скорость разжигания должна быть одна лампа за 2-3 тика
    if(sparkState.ticksToNew !=0)
    {
      sparkState.ticksToNew--;
      return;
    }

    uint8_t newIndex = getNewPosition();
    ledPoints[newIndex].value = random(0, 255);
    ledPoints[newIndex].mode = 0x81;

    sparkState.active ++;
    
    sparkState.ticksToNew = sparkState.active < 15 ? 0 : 1;

}

void sparkChangeValue()
{
  // раззожем и потушим
  for (uint8_t index = 0; index < NUMLEDS; index++)
  {
    if (ledPoints[index].mode == 0 || ledPoints[index].mode == 0xFF)
    {
      setColor(index, BLACK);
    }
    else
    {
      uint8_t val = ledPoints[index].mode & 0x3F;
      if ((ledPoints[index].mode & 0xC0) == 0x40)
      {
        // гасим
        val -= (1 + (val > 20 ? 1 : 0) + (val > 40 ? 1 : 0));
        if (val > 1)
        {
          setHSV(index, ledPoints[index].value, 255, val);
          ledPoints[index].mode = 0x40 | val;
        }
        else
        {
          // отправляем в очередь запрета розжига
          sparkState.active--;

          ledPoints[index].mode = 0xFF;
          ledPoints[index].value = max(1, NUMLEDS - sparkState.maxActive * 4);

        }
      }
      if ((ledPoints[index].mode & 0xC0) == 0x80)
      {
        // разжигаем
        val += (1 + (val > 20 ? 1 : 0) + (val > 40 ? 1 : 0));
        setHSV(index, ledPoints[index].value, 255, val);

        ledPoints[index].mode = 0x80 | val;
        if (val >= 63) {
          ledPoints[index].mode = 0x40 | 0x3F;
        }
      }
    }
  }
}
