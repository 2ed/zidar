#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
// triangle512_int8.h
// #include <tables/square_no_alias_2048_int8.h>
#include <tables/sin8192_int8.h>
#include <tables/saw8192_int8.h> 
#include <tables/smoothsquare8192_int8.h>
#include <tables/triangle_warm8192_int8.h>
// #include <tables/square_analogue512_int8.h>
// #include <tables/sin256_int8.h>
// #include <tables/sin8192_int8.h> 
// saw_analogue512_int8.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 256
#define OSC1_DATA SAW8192_DATA

const int TABLE_CELLS = 8192;
//const int TABLE_CELLS = 2048;

/* 
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil(SQUARE_NO_ALIAS_2048_DATA); // SQUARE_ANALOGUE512_DATA);; //SAW8192_DATA);; // SAW_ANALOGUE512_DATA);;
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil2(SQUARE_NO_ALIAS_2048_DATA); // SQUARE_ANALOGUE512_DATA);; // SAW8192_DATA);; //AW_ANALOGUE512_DATA);;
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y(SQUARE_NO_ALIAS_2048_DATA); // SQUARE_ANALOGUE512_DATA);; // SAW8192_DATA);; //AW_ANALOGUE512_DATA);;
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y2(SQUARE_NO_ALIAS_2048_DATA); // SQUARE_ANALOGUE512_DATA);; // SAW8192_DATA);; //AW_ANALOGUE512_DATA);;
// */
/*
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil(SAW8192_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil2(SAW8192_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y(SAW8192_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y2(SAW8192_DATA); //AW_ANALOGUE512_DATA);
// */
/*
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil(SMOOTHSQUARE8192_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil2(SMOOTHSQUARE8192_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y(SMOOTHSQUARE8192_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y2(SMOOTHSQUARE8192_DATA); //AW_ANALOGUE512_DATA);
// */
/*
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil(sin8192_uint_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil2(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y2(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
// */
/*
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil(sin8192_uint_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil2(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y2(sin8192_uint_DATA); //AW_ANALOGUE512_DATA);
// */
//*
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil(OSC1_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil2(OSC1_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y(OSC1_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> aOscil_y2(OSC1_DATA); //AW_ANALOGUE512_DATA);
// */
//*
// Oscil <TABLE_CELLS, AUDIO_RATE> bOscil(OSC2_DATA); // SAW_ANALOGUE512_DATA);
// Oscil <TABLE_CELLS, AUDIO_RATE> bOscil2(OSC2_DATA); //AW_ANALOGUE512_DATA);
// Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y(OSC2_DATA); //AW_ANALOGUE512_DATA);
// Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y2(OSC2_DATA); //AW_ANALOGUE512_DATA);
// */
// for triggering the envelope
EventDelay noteDelay;

ADSR <CONTROL_RATE, AUDIO_RATE> envelope;
ADSR <CONTROL_RATE, AUDIO_RATE> envelope2;
ADSR <CONTROL_RATE, AUDIO_RATE> envelope_y;
ADSR <CONTROL_RATE, AUDIO_RATE> envelope_y2;
// ADSR <CONTROL_RATE, AUDIO_RATE> envelope_y2;

const char INPUT_X = A3; 
const char INPUT_X2 = A4; 
const char INPUT_Y = A2;
const char INPUT_Y2 = A5;
const char INPUT_LINE = A0;
const char INPUT_LINE2 = A1;
// const char INPUT_Y = A1; 
const char PIN_WAVE = 0;
const char PIN_NOTE_HIGH_DOWN = 11;
const char PIN_NOTE_HIGH_UP = 2;
const char PIN_NOTE_LOW_DOWN = 3;
const char PIN_NOTE_LOW_UP = 4;
const char PIN_OCTAVE_HIGH_DOWN = 5;
const char PIN_OCTAVE_HIGH_UP = 6;
const char PIN_OCTAVE_LOW_DOWN = 7;
const char PIN_OCTAVE_LOW_UP = 8;

bool prevPressedWave = false;
bool prevPressedNoteHighDown = false;
bool prevPressedNoteHighUp = false;
bool prevPressedNoteLowDown = false;
bool prevPressedNoteLowUp = false;
bool prevPressedOctaveHighDown = false;
bool prevPressedOctaveHighUp = false;
bool prevPressedOctaveLowDown = false;
bool prevPressedOctaveLowUp = false;

bool modAction = false;
bool modSustain = false;

int waveTableNum = 0;

byte volume = 0, volume2 =0, volume_y = 0, volume_y2 = 0;
byte prev_x = 0, prev_x2 = 0, prev_y = 0, prev_y2 = 0 ; // previous volume level to measure velocity
int vx, vx2, vy, vy2; // controller velocity
// byte gain_x, gain_x2, gain_y, gain_y2 // for control rate envelope values
/*
boolean note_is_on = true;
boolean trigger = false;
*/

float open_note = 55;
float open_note_y = 164.81; // approx x3 first freq, two octaves higher than base

float centsToFreq(float freq, float cents) {
  return freq*pow(2,cents/1200);
//  return result;
}

byte exponential(float vol){
  float res = 0;
  if (vol > 0){
    res = 600*(1 - exp(-vol/510)); // Nice and smooth, 255 is a *limit* // But not really. Clicking hazard!
  }
  return floor(res);
}

void playPluck(const ADSR <CONTROL_RATE, AUDIO_RATE>& env,const Oscil <TABLE_CELLS, AUDIO_RATE>& osc, byte vol, float freq, byte& prev, int& v) {
    if (!modSustain) {
        if (vol > 200){
            if (prev <= 200) {
                //         Serial.print("freq ");
                //         Serial.println(freq);
                /*
                Serial.print("freq ");
                Serial.println(freq);
                Serial.print("~~~~~~~ POW ~~~~~~~");
                Serial.println(-(v));
                // */
                // env.setADLevels(exponential(prev + 50), exponential((prev + 50)/3));
    
                env.setTimes(20, 20, 200, 1000);
                env.setLevels(((vol)), ((vol)), ((vol)>>2),0);
                env.noteOn(); 
                // byte midi_note = 66; // rand(107) + 20;
                // vx = exponential(vol);
                // osc2.setFreq(freq);
            }
        osc.setFreq(freq);
        } 
    } else {
        if (vol > 200){
            if (prev <= 200) {
                env.setTimes(20, 20, 60000, 20);
                env.setLevels(((vol)), ((vol)), ((vol)>>2),0);
                env.noteOn(); 
            }
            osc.setFreq(freq);
        }  else {
         //  if (prev >= 200) {
              env.noteOff();
         // }
        }
    }
}

int volumeRange(int input, int low, int low_zero, int high_zero, int high) {
  int level;
  if (input > high_zero and input < high){
    level = map(input, high_zero, high, 0, 255);
  } else if (input < low_zero and input > low){
    level = map(input, low, low_zero, 255, 0);
  } else if (input <= high_zero and input >= low_zero){
    level = 0;
  } else if (input <= low or input >= high){
    level = 255;
  } ;
  return level;
}

/*
void updateButtonState(bool indicator, char newState, ){
      if (!prevPressedWave) {
        Serial.println("wave");
        prevPressedWave = !prevPressedWave;
      } 
}
*/

void switchWave(int8_t* newTable){
    aOscil.setTable(newTable);
    aOscil2.setTable(newTable);
    aOscil_y.setTable(newTable);
    aOscil_y2.setTable(newTable);
}

void buttonsTune(bool pinState, bool * prevState, float * noteOpen, int cents){
  if (pinState) {                                         // if currently released
      if (*prevState) {                                   // being pressed previously
            //     Serial.print(*noteOpen);                 
            //            Serial.print("  ");
         if (!modAction) {                                // only do things if not mode switch
			 *noteOpen = centsToFreq(*noteOpen, cents);   
	     }
         // Serial.print(*noteOpen);
         // Serial.print("  ");
         // Serial.println(*prevState);
         *prevState = false;
      } 
  } else {
      if (!*prevState) {
		modAction = false;
        //  Serial.println("  turning off");
       //   Serial.println(prevState);
        //        Serial.print("  ");
       //  Serial.println(*prevState);
        *prevState = true;
      }
  }
}

void buttonsControl() {
    // Serial.print(modAction);
    // Serial.println(modSustain);
  if (!digitalRead(PIN_WAVE)) {                   // wave button is pressed
      if (prevPressedWave) {                      // for a while
        if (!digitalRead(PIN_OCTAVE_LOW_DOWN)){   // if low oct is pressed
            if (!prevPressedOctaveLowDown) {      // if detected first time
                modAction = true;
				open_note = 55;                   // set efault pitch parameters
                open_note_y = 164.81;
                // waveTableNum = 0;
                prevPressedOctaveLowDown = true;
            }
            // prevPressedOctaveLowDown = true;			
        }
        
        if (!digitalRead(PIN_OCTAVE_LOW_UP)) {    //  low oct up mode pressed (sustain switch)
            if (!prevPressedOctaveLowUp) {  
                modAction = true;
                modSustain = !modSustain;
                prevPressedOctaveLowUp = true;
            }
        }

      // prevPressedWave = false;	
	  } else {
            prevPressedWave = true;     // update state
            modAction = false;
	  } 
  } else {                                    // wave button released          
      if (prevPressedWave) {                  // if was pressed previously
        if (!modAction) {
            if (++waveTableNum == 2) {        // switch waveform
				waveTableNum = 0;
			} 
			// Serial.println(waveTableNum);
			switch(waveTableNum) {
				case 0:
					switchWave(SAW8192_DATA);
				break;
				case 1:
					switchWave(SMOOTHSQUARE8192_DATA);
				break;  
                /*    case 2:
                switchWave(TRIANGLE_WARM8192_DATA);
			        break; 
		        case 3: 
                switchWave(TRIANGLE_WARM8192_DATA);SMOOTHSQUARE8192_DATA SIN8192_DATA TRIANGLE_WARM8192_DATA SAW8192_DATA
		        break; */
            }
        }
        prevPressedWave = false;
      }
      buttonsTune(digitalRead(PIN_NOTE_HIGH_DOWN), &prevPressedNoteHighDown, &open_note_y, -100);
      buttonsTune(digitalRead(PIN_NOTE_HIGH_UP), &prevPressedNoteHighUp, &open_note_y, 100);
      buttonsTune(digitalRead(PIN_NOTE_LOW_DOWN), &prevPressedNoteLowDown, &open_note, -100);
      buttonsTune(digitalRead(PIN_NOTE_LOW_UP), &prevPressedNoteLowUp, &open_note, 100);
      buttonsTune(digitalRead(PIN_OCTAVE_HIGH_DOWN), &prevPressedOctaveHighDown, &open_note_y, -1200);
      buttonsTune(digitalRead(PIN_OCTAVE_HIGH_UP), &prevPressedOctaveHighUp, &open_note_y, 1200);
      buttonsTune(digitalRead(PIN_OCTAVE_LOW_DOWN), &prevPressedOctaveLowDown, &open_note, -1200);
      buttonsTune(digitalRead(PIN_OCTAVE_LOW_UP), &prevPressedOctaveLowUp, &open_note, 1200);
  }
  

  /*
  if (!digitalRead(PIN_NOTE_HIGH_DOWN)) {
      if (!prevPressedNoteHighDown) {
          open_note_y = centsToFreq(open_note_y, -100);
        // Serial.print(open_note_y);
        // Serial.print("  ");
        // Serial.println(prevPressedNoteHighDown);
          prevPressedNoteHighDown = true;
      } 
  } else {
      if (prevPressedNoteHighDown) {
        //  Serial.println("  turning off");
       //   Serial.println(prevPressedNoteHighDown);
        prevPressedNoteHighDown = false;
      }
  }
  */ 
  // buttonsTune(PIN_NOTE_HIGH_UP, &prevPressedNoteHighUp, &open_note_y, 100);
  // Serial.print(digitalRead(PIN_WAVE));
  // Serial.print(" ");
  // Serial.print(digitalRead(PIN_NOTE_HIGH_DOWN));
  // Serial.print(digitalRead(PIN_NOTE_HIGH_UP));
  // Serial.print(digitalRead(PIN_NOTE_LOW_DOWN));
  // Serial.print(digitalRead(PIN_NOTE_LOW_UP));
  // Serial.print(digitalRead(PIN_OCTAVE_HIGH_DOWN));
  // Serial.print(digitalRead(PIN_OCTAVE_HIGH_UP));
  // Serial.print(digitalRead(PIN_OCTAVE_LOW_DOWN));
  // Serial.print(digitalRead(PIN_OCTAVE_LOW_UP));
  // digitalRead(PIN_NOTE_HIGH_DOWN);
  // digitalRead(PIN_NOTE_HIGH_UP);
  // digitalRead(PIN_NOTE_LOW_DOWN);
  // digitalRead(PIN_NOTE_LOW_UP);
  // digitalRead(PIN_OCTAVE_HIGH_DOWN);
  // digitalRead(PIN_OCTAVE_HIGH_UP);
  // digitalRead(PIN_OCTAVE_LOW_DOWN);
  // digitalRead(PIN_OCTAVE_LOW_UP);
  //  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);

  pinMode(PIN_WAVE, INPUT_PULLUP);
  pinMode(PIN_NOTE_HIGH_DOWN, INPUT_PULLUP);
  pinMode(PIN_NOTE_HIGH_UP, INPUT_PULLUP);
  pinMode(PIN_NOTE_LOW_DOWN, INPUT_PULLUP);
  pinMode(PIN_NOTE_LOW_UP, INPUT_PULLUP);
  pinMode(PIN_OCTAVE_HIGH_DOWN, INPUT_PULLUP);
  pinMode(PIN_OCTAVE_HIGH_UP, INPUT_PULLUP);
  pinMode(PIN_OCTAVE_LOW_DOWN, INPUT_PULLUP);
  pinMode(PIN_OCTAVE_LOW_UP, INPUT_PULLUP);
  digitalWrite(INPUT_LINE,INPUT_PULLUP); // We need pullup.
  digitalWrite(INPUT_LINE2,INPUT_PULLUP); // Also here.
  vx = vx2 = vy = vy2 = 0;
  randSeed(); // fresh random
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  buttonsControl();
  int x_value = mozziAnalogRead(INPUT_X); // value is 0-1023
  int x2_value = mozziAnalogRead(INPUT_X2); // value is 0-1023
  int y_value = mozziAnalogRead(INPUT_Y); // value is 0-1023
  int y2_value = mozziAnalogRead(INPUT_Y2); // value is 0-1023
  //  int A = map(mozziAnalogRead(1), 0, 1024, 0, 255); // Attack
  //  int S = map(mozziAnalogRead(2), 0, 1024, 0, 255); // Sustain
  int cents = map(mozziAnalogRead(INPUT_LINE), 0, 1024, 1250,0); // a little longer
  int cents_y = map(mozziAnalogRead(INPUT_LINE2), 0, 1024, 1240,0);
  float lineTone = centsToFreq(open_note, cents); // value is 0-1023
  float lineTone_y = centsToFreq(open_note_y, cents_y); // value is 0-1023

  volume    = volumeRange(x_value,  220, 480, 580, 830);
  volume2   = volumeRange(x2_value, 200, 470, 550, 810);
  volume_y  = volumeRange(y_value,  230, 430, 540, 810);
  volume_y2 = volumeRange(y2_value, 210, 460, 590, 830);

  playPluck(envelope, aOscil, volume, lineTone, prev_x, vx);
  playPluck(envelope2, aOscil2, volume2, lineTone*2, prev_x2, vx2);
  playPluck(envelope_y, aOscil_y, volume_y, lineTone_y, prev_y, vy);
  playPluck(envelope_y2, aOscil_y2, volume_y2, lineTone_y*2, prev_y2, vy2);
  
  envelope.update();
  envelope2.update();
  envelope_y.update();
  envelope_y2.update(); 
  /*
  gain_x = envelope.next();
  gain_x2 = envelope2.next();
  gain_y = envelope_y.next();
  gain_y2 = envelope_y2.next();
  // */
  prev_x  = volume;
  prev_x2 = volume2;
  prev_y  = volume_y;
  prev_y2 = volume_y2;

}

int updateAudio() {
    byte ex  = envelope.next(); 
    byte ex2 = envelope2.next(); 
    byte ey  = envelope_y.next();
    byte ey2 = envelope_y2.next();
    // /*
    long asig = (long)
    /*
    ex * bOscil.next() +
    ex2* bOscil2.next() +
    ey* bOscil_y.next() +
    ey2 * bOscil_y2.next() + 
    // */    
    ex * aOscil.next() + // * exponential(volume) +   
    ex2 * aOscil2.next() +
    ey * aOscil_y.next() +
    ey2 * aOscil_y2.next(); // * exponential(volume_y2) ; 
    // */
    /*
    aOscil.next() * vx +
    aOscil2.next() * vx2 + 
    aOscil_y.next() * vy +
    aOscil_y2.next() * vy2; 
    // */
  asig >>=9;
  return (int) asig;
  // return (int) (envelope.next() * aOscil.next())>>8;
}

void loop() {
  // put your main code here, to run repeatedly:
  audioHook();
}
