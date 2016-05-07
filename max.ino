#include "max.h"

TonePhrase* vivaldi;
TonePhrase* beethoven;
const int buzzPin = 0;
const int fsrPin = 1;
const int greenPin = 3;
const int bluePin = 4;
const int redPin = 2;
const int vibePin = 5;
const int csdPin = 6;
int ledPins[] = {redPin, greenPin, bluePin};

unsigned long lastTimeOpened;
unsigned long lastTimeCardExtracted;
unsigned long timeToNeglect = 60000;
unsigned long timeToForget = 30000;
int maxCardExtract = 2; // # of times card can be extracted before Beethoven
int warningMargin = 1; // # of times less than max that card can be extracted before light change
int cardExtractCnt = 0; // # of times card has been extracted

int lightThreshold = 200;
int forceThreshold = 300;

int off[] = {LOW, LOW, LOW};
int red[] = {HIGH, LOW, LOW};
int green[] = {LOW, HIGH, LOW};
int yellow[] = {HIGH, HIGH, LOW};

void setup() {
    noTone(buzzPin);
    double beat = 300;
    int freqs1[] = {C5, 0, E5, 0, E5, 0, E5, 0, D5, C5, G5, 0};
    int durations1[] = {0.9*beat, 0.1*beat, 0.9*beat, 0.1*beat,0.9*beat, 0.1*beat,0.9*beat, 0.1*beat, 0.5*beat, 0.5*beat, 3*beat, beat};
    vivaldi = new TonePhrase(freqs1, durations1, 12);

    double beat2 = 400;
    //int freqs2[] = {E5/1.059463, 0, E5/1.059463, 0, E5/1.059463, 0, C5/1.059463, 0, D5/1.059463, 0, D5/1.059463, 0, D5/1.059463, 0, B4/1.059463, 0};
    int freqs2[] = {E5, 0, E5, 0, E5, 0, C5, 0, D5, 0, D5, 0, D5, 0, B4, 0};
    int durations2[] = {0.4*beat2, 0.1*beat2, 0.4*beat2, 0.1*beat2, 0.4*beat2, 0.1*beat2, 2*beat2, 0.5*beat2, 0.4*beat2, 0.1*beat2, 0.4*beat2, 0.1*beat2, 0.4*beat2, 0.1*beat2, 4*beat2, beat2};
    beethoven = new TonePhrase(freqs2, durations2, 16);

    pinMode(buzzPin, OUTPUT);
    pinMode(fsrPin, INPUT_PULLUP);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(vibePin, OUTPUT);
    pinMode(csdPin, INPUT_PULLUP);

    lastTimeOpened = millis();
}

void loop() {
    int light = 1023 - analogRead(csdPin);
    int force = 1023 - analogRead(fsrPin);

    // Check if wallet is open
    if (light > lightThreshold) {
        // Wallet is opened:
        // 1) Show appropriate colored LED
        // 2) Play appropriate tune
        // 3) Wait for force sensor (card extraction) or light sensor (wallet closing)

        // Show LED and play tunes
        // LED logic: good, warn, bad
        // Buzz logic: good, good, bad
        if (cardExtractCnt >= maxCardExtract) {
            ledColor(ledPins, red);
            beethoven->play(buzzPin);
        } else if (cardExtractCnt >= maxCardExtract - warningMargin) {
            ledColor(ledPins, yellow);
            vivaldi->play(buzzPin);
        } else {
            ledColor(ledPins, green);
            vivaldi->play(buzzPin);
        }

        // Wait for sensor change
        light = 1023 - analogRead(csdPin);
        force = 1023 - analogRead(fsrPin);
        bool cardExtracted = false;
        while (light > lightThreshold) {
            light = 1023 - analogRead(csdPin);
            force = 1023 - analogRead(fsrPin);
            
            if (force > forceThreshold && !cardExtracted) {
                if (cardExtractCnt < maxCardExtract) {
                    cardExtractCnt++;
                }
                lastTimeCardExtracted = millis();
                cardExtracted = true;
            }
        }

        if (millis() - lastTimeCardExtracted > timeToForget) {
            cardExtractCnt--;
            lastTimeCardExtracted = millis();
        }
        
        lastTimeOpened = millis();
    } else {
        // Turn LED off
        ledColor(ledPins, off);
        
        // Closed for more than 10 seconds (for testing purposes)
        if (millis() - lastTimeOpened > timeToNeglect) {
            neglectedVibe(vibePin);
        }

        if (millis() - lastTimeCardExtracted > timeToForget) {
            cardExtractCnt--;
            lastTimeCardExtracted = millis();
        }
    }
    
    
}
