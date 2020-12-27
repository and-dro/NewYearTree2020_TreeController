enum effects
{
    OFF,
    SPARK,
    DROPS,
    STRIPES,
    MARKING,
    LAST_VALUE
};

struct stateControl
{
    bool ready;
    uint8_t totalBrightness;
    uint16_t updateInterval;
    effects currentEffect;
    unsigned long updateStripTime;
} stipState;

struct newModeControl
{
  bool active;
  effects newEffect;
  unsigned long downTime;
  unsigned long finalTime;
} NewMode;

struct pixelData {
  uint8_t mode, value;
}; // параметры одного элемента
static pixelData ledPoints[NUMLEDS];

struct remoteState
{
  uint8_t mode;
  uint8_t speed;
  uint8_t brightness;
  uint8_t reserv1;
  uint8_t reserv2;
  uint8_t reserv3;
};

static volatile remoteState RemoteState;

void updateState()
{
  if(RemoteState.mode <  effects::LAST_VALUE && stipState.currentEffect != (effects)RemoteState.mode)
  {
    // stipState.currentEffect = (effects)RemoteState.mode;
    NewMode.active = true;
    NewMode.newEffect = (effects)RemoteState.mode;
    if(stipState.currentEffect == OFF)
    {
      NewMode.downTime = 0;
    }
    else
    {
      NewMode.downTime = millis() + 500;
    }
    if(NewMode.newEffect == OFF)
    {
      NewMode.finalTime = 0;
    }
    else
    {
        NewMode.finalTime = (NewMode.downTime == 0 ? millis() : NewMode.downTime) + 500;
    }
  }
  #ifdef SERDEBUG 
    Serial.print("state update");
    Serial.println();
  #endif
}

template <typename T> int I2C_writeAnything (const T& value)
 {
   const byte * p = (const byte*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         Wire.write(*p++);
   return i;
 } 
 
template <typename T> int I2C_readAnything(T& value)
 {
   byte * p = (byte*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         *p++ = Wire.read();
   return i;
 }

void receiveEvent(int howMany) 
{
  if(howMany == sizeof RemoteState) 
  {
    I2C_readAnything (RemoteState);
    updateState();
  }
  else
  {
    while (Wire.available()) 
    {
        char c = Wire.read();
        #ifdef SERDEBUG 
          Serial.print(c);
        #endif
    }
  }
}

void requestEvent() {
 I2C_writeAnything(RemoteState);
}

uint8_t newModeBrightnessHandler(uint8_t brightness)
{
    if(!NewMode.active)
    {
      return brightness;
    }

    uint8_t interval = 10;

    if(NewMode.downTime != 0) // режим гашения
    {
        if(NewMode.downTime > millis())
        {
            interval = 1 + (NewMode.downTime - millis()) / 50;
        }
        else
        {
          NewMode.downTime = 0;
        }
    }

    if(NewMode.downTime == 0) // режим разжигания
    {
      // гашение завершено, перейдем к новому эффекту
      if(stipState.currentEffect != NewMode.newEffect) stipState.currentEffect = NewMode.newEffect;

      NewMode.active = false;

    }

  uint8_t upperBound = 0;
  switch (interval)
  {
  case 10:
    upperBound = 90;
    break;
  case 9:
    upperBound = 81;
    break;
  case 8:
    upperBound = 63;
    break;
  case 7:
    upperBound = 47;
    break;
  case 6:
    upperBound = 35;
    break;
  case 5:
    upperBound = 26;
    break;
  case 4:
    upperBound = 20;
    break;
  case 3:
    upperBound = 16;
    break;
  case 2:
    upperBound = 12;
    break;
  case 1:
    upperBound = 8;
    break;
  
  default:
    upperBound = 100;
    break;
  }
  return (1.0 * upperBound / 100.0) * brightness;
}