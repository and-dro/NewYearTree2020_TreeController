#include <Arduino.h>

#define LED_PIN 4    // пин ленты
#define LEDS_ROWS 15  // кол-во строк светодиодов
#define LEDS_COLUMNS 15  // кол-во колонок светодиодов
#define NUMLEDS LEDS_ROWS * LEDS_COLUMNS  // кол-во светодиодов
#define ORDER_GRB     // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 3 // цветовая глубина: 1, 2, 3 (в байтах)

#include "..\lib\microLED\microLED.h" // разместить обязательно ПОСЛЕ определения параметров ленты

LEDdata leds[NUMLEDS];  // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, NUMLEDS, LED_PIN);  // объект лента
static byte brightness;

#include "stateControl.h"
#include "stripEffects.h"

void setup() {
  brightness = 10;
  Serial.begin(9600);
  Serial.print("init\r\n");
  pinMode(LED_BUILTIN, OUTPUT); 
  strip.fill(mRGB(162,32,16));
  strip.setBrightness(brightness);

  strip.show(); // выводим изменения на ленту
  delay(3000);
  Serial.print("ready\r\n");

  stripEffectsInit();

}

static bool state = false;

void loop() {
    
    if(state != ((millis() / 500) % 2 == 0))
    {
      state = !state;
      digitalWrite(LED_BUILTIN,  state ? HIGH : LOW );
      Serial.print('.');
    }
    
    stripEffectsTick();

}