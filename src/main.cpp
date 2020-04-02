// Program to demonstrate the MD_Parola library
//
// Demonstrate use of effects on different zones to create an animated sign
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME  25
#define PAUSE_TIME  1000

int moleCnt[] = {0,0};

const char *countdown[] = {
  "- 1 -",
  "- 2 -",
  "- 3 -",
};

textEffect_t  effect[] =
{
  PA_SCROLL_LEFT,
  PA_WIPE,
  PA_RANDOM,
  PA_FADE,
  PA_OPENING_CURSOR,
  PA_CLOSING,
  PA_SCAN_VERT,
  PA_WIPE_CURSOR,
  PA_OPENING,
  PA_CLOSING_CURSOR,
  PA_SCROLL_DOWN_RIGHT,
  PA_SCROLL_RIGHT,
  PA_SCROLL_DOWN,
};

void doCountdown()
{
  for (int i = 2; i >= 0; i--)
  {
    P.displayZoneText(0, countdown[i], PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_OPENING_CURSOR, PA_PRINT);
    P.displayZoneText(1, countdown[i], PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_OPENING_CURSOR, PA_PRINT);
    while (!P.getZoneStatus(0))
      P.displayAnimate();
  }
  
}

void setup(void)
{
  P.begin(2);
  P.setZone(0,0,3);
  P.setZone(1,4,7);
  randomSeed(analogRead(0));
}

void incCount(uint8_t id){
  moleCnt[id]++;
  char buffer1[32];
  char buffer2[32];
  itoa(moleCnt[0], buffer1, 10);
  P.displayZoneText(0, buffer1, PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
  itoa(moleCnt[1], buffer2, 10);
  P.displayZoneText(1, buffer2, PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
  P.displayAnimate();
}

void loop(void)
{
  P.displayZoneText(0, "Mole 1", PA_CENTER, SPEED_TIME, PAUSE_TIME, effect[random(13)], PA_PRINT);
  P.displayZoneText(1, "Mole 2", PA_CENTER, SPEED_TIME, PAUSE_TIME, effect[random(13)], PA_PRINT);
  while (!P.getZoneStatus(0))
    P.displayAnimate();
  
  delay(2000);
  
  doCountdown();

  while (true)
  {
    incCount(random(50)%2);
    if (moleCnt[0] == 100 || moleCnt[1] == 100) {
      if (moleCnt[0] > moleCnt[1]) {
        P.displayZoneText(0, "Winner!", PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
        P.displayZoneText(1, "Lost!", PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
      } else {
        P.displayZoneText(0, "Lost!", PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
        P.displayZoneText(1, "Winner!", PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
      }
      P.displayAnimate();
      moleCnt[0] = 0;
      moleCnt[1] = 0;
      delay(5000);
      P.displayClear();
      break;
    }
    delay(80);  
  }
}