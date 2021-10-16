#include "M5Atom.h"
#include <elapsedMillis.h>

extern const unsigned char images[63][77];
extern const int sequences[][32];
extern const int numberOfSequences;
extern const int numberOfImages;
const int outputPin = 33;
int numberOfSteps = 32;
int currentSequence = 0;
int currentImage = 0;
int currentStep = 0;
int currentBrightness = 60;
bool stepIsTriggered = false;

bool isPlaying = false;
bool IMU6886Flag = false;

elapsedMillis sinceStepUpdate;
elapsedMillis sincePulseUpdate;
elapsedMillis sinceStepTrigger;

int millisecondsPerStep = 125;        // 120 bpm, 8 ticks per second
int millisecondsPerPulseUpdate = 12;  // Tune this to increase or decrease how long a pulse lasts

void setup(){
    M5.begin(true, false, true);
    pinMode(outputPin, OUTPUT);
    delay(50); 
}

void loop(){
  if (M5.Btn.wasPressed()) {
    // Start playing the sequence if it is not playing
    if (isPlaying == false) {
      isPlaying = true;
    } else {
      // Otherwise, increment the sequence number
      currentSequence += 1;

      if (currentSequence == numberOfSequences) {
        currentSequence = 0;
      }
    }
  }

  if (isPlaying == true) {
    if (sinceStepUpdate >= millisecondsPerStep) {
      sinceStepUpdate = 0;
                
      if ( sequences[currentSequence][currentStep] == 1) {
        // Send a trigger, set the trigger pin high
        digitalWrite(outputPin, HIGH);
        sinceStepTrigger = 0;

        // Set a random semiotic and start a display pulse
        currentImage = random(numberOfImages);
       
        currentBrightness = 60;
        M5.dis.setBrightness(currentBrightness);
        M5.dis.displaybuff((uint8_t *)(images[currentImage]));
      }
      
      // Advance the step sequencer
      currentStep += 1;

      if (currentStep == numberOfSteps) {
        currentStep = 0;
      }
    }

    if (sincePulseUpdate >= millisecondsPerPulseUpdate) {
      sincePulseUpdate = 0;
      if (currentBrightness >= 0) {
        M5.dis.setBrightness(currentBrightness);
        M5.dis.displaybuff((uint8_t *)(images[currentImage]));
        currentBrightness = currentBrightness - 2;
      } 
    }
    
    // Step triggers are 60 ms long
    if (sinceStepTrigger >= 60) {
      // Set the trigger pin low
      digitalWrite(outputPin, LOW);
      sinceStepTrigger = 0;
    }
  }

  M5.update(); 
}
 
