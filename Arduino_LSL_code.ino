#include <Servo.h>
Servo spin_servo;

int newNum;
int runs = 0;
String sequence;
String userSequence;
String newDigit;
int i;
bool generatingCombination = true;
bool generatingUserCombination = true;
int dismiss;
bool calibrate = true;
int thresholdArray[]  = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  spin_servo.attach(5);
  spin_servo.write(90);
}

void loop() {
  int time = millis();
  //////////////////////////////////////////////////////////////////////////// Calibration //////////////////////////////////////////////////////////////////////////////////////////
  if (calibrate) {
    Serial.print("calibrating...");
    delay(7000);
    for (int i = 0; i < 6; i++) {
      thresholdArray[i] = 0;
    }
    // Master for loop
    for (i = 0; i < 10; i++) {
      int sensorVals[] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5)};

      //Adds 6 starting values together for loop
      for (int x = 0; x < 6; x++) {
        thresholdArray[x] = thresholdArray[x] + sensorVals[x];
      }
      //If we've reached the end of our 10 second calibration, find the mean of those values, subtract 100, thats our threshold for that phototransistor
      if (i == 9) {

        for (int y = 0; y < 6; y++) {
          thresholdArray[y] = (thresholdArray[y] / 10) - 100;
          //Serial.print("Threshold ");
          //Serial.print(y);
          //Serial.print(": ");
          //Serial.println(thresholdArray[y]);
        }
      }
      delay(1000);
    }

    calibrate = false;
    Serial.println(" Done.");
  }
  // End calibration

  ///////////////////////////////////////////////////////////////////////////// Random Sequence Generation //////////////////////////////////////////////////////////////////////////////
  Serial.println("R");
  runs = 0;
  while (generatingCombination) {
    randomSeed(analogRead(A5) + analogRead(A4) + analogRead(A3) + analogRead(A2) + analogRead(A1) + analogRead(A0));
    newNum = random(0, 6);
    newDigit = (String) newNum;
    dismiss = 0;
    if (runs == 0) {
      sequence = sequence + newDigit;
    }
    if (runs == 1) {
      for (i = 0; i < sequence.length(); i++) {
        if (newDigit.charAt(0) == sequence.charAt(i)) {
          dismiss = 1;
        }
        if (dismiss == 0 && i == (sequence.length() - 1)) {
          sequence = sequence + newDigit;
          break;
        }
      }
    }
    if (sequence.length() == 6) {
      generatingCombination = false;
      Serial.println("X" + sequence);
      break;
    }
    runs = 1;
  }


  ///////////////////////////////////////////////////////////////////////////// User Sequence Generation ////////////////////////////////////////////////////////////////////////////
  bool panelsUsed[] = {false, false, false, false, false, false};
  bool alreadyGreen[] = {false, false, false, false, false, false};
  while (generatingUserCombination) {

    for (i = 0; i < 6; i++) {
      int sensorVals[] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5)};

      String panelNumbers[] = {"0", "1", "2", "3", "4", "5"};
      // If sensors sense someone blocking, they send message to processing to paint a certain panel green
      if (sensorVals[i] <= thresholdArray[i]) {
        //Serial.print(i); 
        //Serial.print("'s detection value: ");
        //Serial.println(sensorVals[i]);

        if (panelsUsed[i] == false) {
          userSequence = userSequence + panelNumbers[i];
          panelsUsed[i] = true;
        }
      }
      // Sending userSequence digits over ash twin project
      if (panelsUsed[i] == true) {
        if (alreadyGreen[i] == false) {
          Serial.println("Y" + panelNumbers[i]);
          Serial.println("Z" + userSequence);
          digitalWrite(13, HIGH);
          alreadyGreen[i] = true;
        }
      }
    } // For loop end
    // If sequence has 6 digits, end panel input segment
    if (userSequence.length() == 6) {
      generatingUserCombination = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////// Verdict ///////////////////////////////////////////////////////////////////////////////////////////////
  if (sequence == userSequence) {
    // trim this in processing
    Serial.println("G");
    spin_servo.write(0);
    delay(7000);
    spin_servo.write(180);
  } else {
    Serial.println("D");
    delay(3000);
  }


  //////////////////////////////////////////////////////////////////////////// Reset ////////////////////////////////////////////////////////////////////////////////////////////////
  delay(1000);
  Serial.println("R");
  generatingCombination = true;
  sequence = "";
  generatingUserCombination = true;
  userSequence = "";
  if (time % 10800000 == 0) {
    calibrate = true;
  } else {
    calibrate = false;
  }


}
