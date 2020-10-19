enum effects
{
    OFF,
    SPARK,
    SPARK_FIXED,
    DROPS
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
