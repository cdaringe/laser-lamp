// install FastRunningMedian from here: https://github.com/robotican/lizi_robot/blob/master/lizi_arduino/Arduino/libraries/FastRunningMedian/FastRunningMedian.h
#include "median.h"
byte LED = 1;
byte echoPin = 3;
byte trigPin = 4;

byte currentBrightness = 0;
unsigned int distance;

byte minDistance=5;  // cm (min distance === full brightness)
byte maxDistance=50; // cm (max distance === least brightness)

void setup(){
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, currentBrightness);
}

void loop() {
  byte n;
  FastRunningMedian<long, 10, 0> samples;

  /*PART1: calculate distance through sensor, dont change*/
  for (n=0 ; n<10 ; ++n) {
    samples.addValue(sample_distance());
  }
  distance = samples.getMedian();
  if (distance < 0.3) return; // clearly no one meant to do anything with their hand

  if (distance < minDistance) {
    fadeOff();
    delay(3000);
  } else if (distance <= maxDistance) {
    currentBrightness = map(distance, minDistance, maxDistance, 0, 255);
    analogWrite(LED, currentBrightness);
  }
}

void fadeOff() {
    float fadeBlockSize = (currentBrightness / 255.0);
    float tBrightness = (float) currentBrightness;
    byte n;
    for (n=0 ; n < 255 ; ++n) {
      tBrightness = tBrightness - fadeBlockSize;
      analogWrite(LED, tBrightness);
      delay(3);
    }
    analogWrite(LED, 0);
}

long sample_distance() {
  // reset ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // start measurement
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return (long) (pulseIn(echoPin, HIGH)/58.0); // cm's,  https://docs.google.com/document/d/1Y-yZnNhMYy7rwhAgyL_pfa39RsB-x2qR4vP8saG73rE
}
