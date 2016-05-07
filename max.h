#include <Wire.h>
#include "music_notes.h"
#define MAX_PHRASE_BUFFER 16

/**
 * Store and play a sequence of tones or vibrations
 */
class Phrase {
public:
    Phrase(int f[], int d[], int n) : length(n) {
        for (int i = 0; i < n; i++) {
            if (i >= MAX_PHRASE_BUFFER) {
                return; // Yes this isn't the best error handling but I don't care
            }
            freqs[i] = f[i];
            durations[i] = d[i];
        }
    };

    virtual void play(int pin) {}; // Implemented differently depending on whether it's tones or vibes

protected:
    int freqs[MAX_PHRASE_BUFFER];
    int durations[MAX_PHRASE_BUFFER]; // Array of durations of each "word"
    int length; // Number of "words"
};

/**
 * Store and play a sequence of tones
 */
class TonePhrase: public Phrase {
public:
    TonePhrase(int freq[], int duration[], int n):Phrase(freq, duration, n) {};

    void play(int pin) {
        for (int i = 0; i < length; i++) {
            tone(pin, freqs[i]);
            delay(durations[i]);
        }
        noTone(pin);
    }
};

/**
 * Store and play a sequence of vibrations
 */
class VibePhrase: public Phrase {
public:
    VibePhrase(int freq[], int duration[], int n) : Phrase(freq, duration, n) {};

    void play(int pin) {
        for (int i = 0; i < length; i++) {
            digitalWrite(pin, freqs[i]);
            delay(durations[i]);
        }
    }
};

void sadSweepTone(int pin) {
    int freq = G3;
    while (freq > C3) {
        tone(pin, freq);
        delay(15);
        freq--;
    }
    delay(500);
    tone(pin, 0);
}

void happySweepTone(int pin) {
    int freq = C6;
    int stopFreq = G6;
    while (freq < stopFreq) {
        tone(pin, freq);
        delay(20);
        freq += 40;
    }
    delay(100);
    tone(pin, 0);
    delay(100);
    tone(pin, C6);
    delay(100);
    tone(pin, 0);
    delay(100);
    tone(pin, C6);
    delay(100);
    tone(pin, 0);
}

void excitedTone(int pin) {
    int freq1 = 2350;
    int freq2 = 2500;
    tone(pin, freq2/2);
    delay(200);
    int freq = freq2;
    while (freq > freq1) {
        tone(pin, freq);
        delay(30);
        freq -= 30;
    }
    delay(10);
    tone(pin, 0);
}

void neglectedVibe(int pin) {
    for (int i = 0; i < 2; i++) {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(100);
    }
}

void ledColor(int pins[], int states[]) {
    digitalWrite(pins[0], states[0]); // red
    digitalWrite(pins[1], states[1]); // green
    digitalWrite(pins[2], states[2]); // blue
}

