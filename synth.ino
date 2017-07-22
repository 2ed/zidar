#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
// triangle512_int8.h
// #include <tables/square_no_alias_2048_int8.h>
#include <tables/sin8192_int8.h>
#include <tables/saw8192_int8.h> 
#include <tables/smoothsquare8192_int8.h>
// #include <tables/square_analogue512_int8.h>
// #include <tables/sin256_int8.h>
// #include <tables/sin8192_int8.h> 
// saw_analogue512_int8.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 256
#define OSC1_DATA SAW8192_DATA
#define OSC2_DATA SIN8192_DATA

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
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil(OSC2_DATA); // SAW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil2(OSC2_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y(OSC2_DATA); //AW_ANALOGUE512_DATA);
Oscil <TABLE_CELLS, AUDIO_RATE> bOscil_y2(OSC2_DATA); //AW_ANALOGUE512_DATA);
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

byte volume = 0, volume2 =0, volume_y = 0, volume_y2 = 0;
byte prev_x = 0, prev_x2 = 0, prev_y = 0, prev_y2 = 0 ; // previous volume level to measure velocity
int vx, vx2, vy, vy2; // controller velocity
// byte gain_x, gain_x2, gain_y, gain_y2 // for control rate envelope values
/*
boolean note_is_on = true;
boolean trigger = false;
*/

int open_note = 55;
int open_note_y = 3*open_note; // two octaves higher than base

float centsToFreq(int freq, float cents) {
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

void playPluck(const ADSR <CONTROL_RATE, AUDIO_RATE>& env,const Oscil <TABLE_CELLS, AUDIO_RATE>& osc, const Oscil <TABLE_CELLS, AUDIO_RATE>& osc2, byte vol, int freq, byte& prev, int& v) {
   if (vol > 200){
    if (prev < 200) {
      /*
      Serial.print("freq ");
      Serial.println(freq);
      Serial.print("~~~~~~~ POW ~~~~~~~");
      Serial.println(-(v));
      // */
      // env.setADLevels(exponential(prev + 50), exponential((prev + 50)/3));
      env.setLevels(((vol)), ((vol)), ((vol)>>2),0);
      // env.setTimes(attack,decay,sustain,release_ms);    
      // env.setTimes(A, A, S, S);
      env.setTimes(20, 20, 200, 1000);
      env.noteOn(); 
      // byte midi_note = 66; // rand(107) + 20;
      // vx = exponential(vol);
      // osc2.setFreq(freq);
    }
   osc.setFreq(freq);
   }
}


void playPluckOld(const ADSR <CONTROL_RATE, AUDIO_RATE>& env,const Oscil <TABLE_CELLS, AUDIO_RATE>& osc, byte vol, int freq, byte& prev, int& v) {
   if (abs(vol - prev) > 5 or (vol < 5 and v != 0)) {
    if (v < 0 and (vol - prev) < 0) {
      v = (v + vol-prev);
    } else {
      v = vol - prev;
    }
    /*
    Serial.print("vol ");
    Serial.print(vol);
    Serial.print(" prev ");
    Serial.print(prev);
    Serial.print(" v ");
    Serial.println(v);
    // */
    if (vol < 5 and v < -10) {
      /*
      Serial.print("~~~~~~~ POW ~~~~~~~");
      Serial.println(-(v));
      // */
      // env.setADLevels(exponential(prev + 50), exponential((prev + 50)/3));
      env.setLevels((-v), ((-v)), ((-v)/4),0);
      // env.setTimes(attack,decay,sustain,release_ms);    
      // env.setTimes(A, A, S, S);
      env.setTimes(20, 20, 200, 1000);
      env.noteOn(); 
      // byte midi_note = 66; // rand(107) + 20;
      // vx = exponential(vol);
      osc.setFreq(freq);
      /*
      Serial.print("vol ");
      Serial.println(vol);
      // */
    }
    /*
    Serial.print(" prev new ");
    Serial.print(prev);
    Serial.println();
    // */
   }
}

/* Controller states:
 *  0 — nothing to do
 *  1 — pluck a note
 *  2 — dunno lol
 */
char controllerState(){
  
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  digitalWrite(INPUT_LINE,INPUT_PULLUP); // We need pullup.
  digitalWrite(INPUT_LINE2,INPUT_PULLUP); // Also here.
  vx = vx2 = vy = vy2 = 0;
  randSeed(); // fresh random
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  int x_value = mozziAnalogRead(INPUT_X); // value is 0-1023
  int x2_value = mozziAnalogRead(INPUT_X2); // value is 0-1023
  int y_value = mozziAnalogRead(INPUT_Y); // value is 0-1023
  int y2_value = mozziAnalogRead(INPUT_Y2); // value is 0-1023
  //  int A = map(mozziAnalogRead(1), 0, 1024, 0, 255); // Attack
  //  int S = map(mozziAnalogRead(2), 0, 1024, 0, 255); // Sustain
  int cents = map(mozziAnalogRead(INPUT_LINE), 0, 1024, 1250,0);
  int cents_y = map(mozziAnalogRead(INPUT_LINE2), 0, 1024, 1250,0);
  int lineTone = centsToFreq(open_note, cents); // value is 0-1023
  int lineTone_y = centsToFreq(open_note_y, cents_y); // value is 0-1023

  volume    = volumeRange(x_value,  220, 480, 580, 830);
  volume2   = volumeRange(x2_value, 200, 470, 550, 810);
  volume_y  = volumeRange(y_value,  230, 430, 540, 810);
  volume_y2 = volumeRange(y2_value, 210, 460, 590, 830);

  playPluck(envelope, aOscil, bOscil, volume, lineTone, prev_x, vx);
  playPluck(envelope2, aOscil2, bOscil2, volume2, lineTone*2, prev_x2, vx2);
  playPluck(envelope_y, aOscil_y, bOscil_y, volume_y, lineTone_y, prev_y, vy);
  playPluck(envelope_y2, aOscil_y2, bOscil_y2, volume_y2, lineTone_y*2, prev_y2, vy2);
  
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

  /*
  updatePrev(prev_x, volume);
  updatePrev(prev_x2, volume2);
  updatePrev(prev_y, volume_y);
  updatePrev(prev_y2, volume_y2);
  // */ 
  /*
  Serial.print("X = ");
  Serial.print(x_value);
  Serial.print(" X2 = ");
  Serial.print(x2_value);
  Serial.print(" Y = ");
  Serial.print(y_value);
  Serial.print(" Y2 = ");
  Serial.print(y2_value); 
  Serial.print((volume));
  Serial.print(" volume2 = ");
  Serial.print((volume2));
  Serial.print(" volume_y = ");
  Serial.print((volume_y));
  Serial.print(" volume_y2 = ");
  Serial.print((volume_y2));
  Serial.println(); 

  // */
  /*  Serial.print(" A = "); // Attack and sustain
  Serial.print(A);
  Serial.print(" S = ");
  Serial.print(S); // */ 
  /*
  Serial.print(" cents = ");
  Serial.print(cents);
  Serial.print(" cents_y = ");
  Serial.print(cents_y);
  Serial.print(" line = ");
  Serial.print(lineTone);
  Serial.print("Hz line_y = ");
  Serial.print(lineTone_y);
  Serial.print("Hz volume = ");
  Serial.print(exponential(volume));
  Serial.print(" volume2 = ");
  Serial.print(exponential(volume2));
  Serial.print(" volume_y = ");
  Serial.print(exponential(volume_y));
  Serial.print(" volume_y2 = ");
  Serial.print(exponential(volume_y2));
  Serial.println(); // */
  /*  
  Serial.print("| TEST ");
  Serial.print(centsToFreq(open_note,cents)); // Debug only
  Serial.println(centsToFreq(open_note,1200)); 
  Serial.println();
  // */
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
