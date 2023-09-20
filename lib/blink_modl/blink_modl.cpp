#include <Arduino.h>
#include <Wire.h>
#include <blink_modl.h>

//variables for blinking an LED with Millis
const int led = LED_BUILTIN; // ESP8266 Pin to which onboard LED is connected
#define LED_OFF HIGH
#define LED_ON LOW
int ledState = LED_ON; // initial ledState
int ledSeqPos = 0;     // blink pattern position counter
// blink pattern: 1L - 2 - 1 - 5 - 1L - 2 - 1  - 5 ...
const int blinkSeqNormal[] = {
    1000, 100, // 1 long
    800,  1,  // 2 short
    300,  1,
    800,  1,  // then 1 short
    800,  1,  // then 5 short
    300,  1,
    300,  1,
    300,  1,
    300,  1,
 
    0, 0}; // sentinal for end of sequence
const int blinkSeqSOS[] = {
    2000, 1, // 3 short
    250, 1,
    250, 1,
    700, 30, // 3 long
    500, 30,
    500, 30,
    700, 1, // 3 short
    250, 1,
    250, 1,

    0, 0}; // sentinal for end of sequence

int *blinkSeq = (int *) blinkSeqSOS;

unsigned long nextLedTransition = 0;
unsigned long blinkseqcount = 0;

void blink_setup()
{
    pinMode(led, OUTPUT);
}

void blink_handler()
{

    // loop to blink without invoking delay()
    if (currentMillis > nextLedTransition)
        {
            ledState ^= 1; // toggle ledState;
            // set the LED with the ledState of the variable:
            digitalWrite(led, ledState);
            // Serial.print("blink ");
            // Serial.println( ledState);

            nextLedTransition = currentMillis + blinkSeq[ledSeqPos++];
            if (blinkSeq[ledSeqPos] == 0)
                {
                    ledSeqPos = 0;
                }
        }
}

void blink_change( const int *seq) {
    ledSeqPos = 0;
    blinkSeq = (int *) seq;
    ledState = LED_ON;
}