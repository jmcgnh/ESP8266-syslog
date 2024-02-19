#include <Arduino.h>
#include <Wire.h>
#include <blink_modl.h>

//variables for blinking an LED with Millis
const int led = LED_BUILTIN; // ESP8266 Pin to which onboard LED is connected
#define LED_OFF HIGH
#define LED_ON LOW
int ledState = LED_ON; // initial ledState
int ledSeqPos = 0;     // blink pattern position counter
// blink pattern: 1L - 2 - 2 - 2 - 1L - 2 - 2  - 2 ...
const int blinkSeq[] = {
    1000, 100, // 1 long
    800,  1,  // 2 short
    250,  1,
    500,  1,  // then 2 short
    250,  1,
    500,  1,  // then another 2 short
    250,  1,  
    0, 0}; // sentinal for end of sequence
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
                    // needident = 1;
                }
        }


}

