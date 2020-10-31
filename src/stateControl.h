enum effects
{
    OFF,
    SPARK,
    SPARK_FIXED,
    DROPS,
    LAST_VALUE
};

struct stateControl
{
    bool ready;
    uint8_t totalBrightness;
    uint8_t updateInterval;
    effects currentEffect;
    unsigned long updateStripTime;
} stipState;

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
  if(RemoteState.mode <  effects::LAST_VALUE)
  {
    stipState.currentEffect = (effects)RemoteState.mode;
  }
  Serial.print("state update");
  Serial.println();
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
        Serial.print(c);
    }
  }
}

void requestEvent() {
 Wire.write("i'm8, what u need?");
}
