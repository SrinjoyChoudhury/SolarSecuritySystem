import processing.serial.*;
Serial myPort;
/*
boolean [] keys =  new boolean[128];

void keyPressed() {
  keys[key] = true;
  //println(key);
}

void keyReleased() {
  keys[key] = false;
  //println(key);
}
*/

PImage grid;
PImage greenSquare;
PImage graySquare;
int coordinatesX[] = {100, 285, 475, 100, 285, 475};
int coordinatesY[] = {135, 135, 135, 310, 310, 310};
boolean firstTime = true;
String val = "Srinjoy";
String yeet;
int panel;
void setup() {
  size(800, 600);
  //String portName = Serial.list()[0];
  myPort = new Serial(this, "COM3", 9600);
  delay(10000);
}

void draw() {

  try {
    grid = loadImage("lastgrid.jpg");
    greenSquare = loadImage("greenSquare.jpg");
    graySquare = loadImage("graySquare.jpg");  
    if ( myPort.available() > 0) {  
      yeet = myPort.readStringUntil('\n');
      val = trim(yeet);
      println(val);
    }


    if (firstTime == true) {
      image(grid, 0, 0);
      firstTime = false;
      textSize(27);
      fill(255, 255, 0);
      rect(40, 22, 500, 37);
      fill(0, 0, 255);
      text(" Sequence: ", 40, 50);

      textSize(27);
      fill(255, 255, 0);
      rect(40, 62, 500, 37);
      fill(0, 0, 255);
      text(" UserSequence: ", 40, 90);
    }

    if (val.charAt(0) == 'X') {
      textSize(27);
      fill(255, 0, 0);
      text(val.substring(1), 185, 50);
    }

    if (val.charAt(0) == 'Z') {
      textSize(27);
      fill(255, 0, 0);
      text(val.substring(1), 250, 90);
    }

    if (val.charAt(0) == 'R') {
      firstTime = true;
    }
    if (val.charAt(0) == 'Y') {
      panel = Integer.parseInt(val.substring(1));
      image(greenSquare, coordinatesX[panel], coordinatesY[panel]);
      fill(255, 255, 255);
      textSize(50);
      text(val.charAt(1), coordinatesX[panel] + 60, coordinatesY[panel] + 90);
    } 
    /*
     else if (val.charAt(0) == 'Y') {
     panel = Integer.parseInt(val.substring(1));
     image(graySquare, coordinatesX[panel], coordinatesY[panel]);
     }
     */



    if (val.charAt(0) == 'G') {
      fill(0, 150, 0);
      rect(0, 500, 800, 100);
      textSize(50);
      fill(255, 255, 255);
      text("ACCESS GRANTED", 175, 570);
    } else if (val.charAt(0) == 'D') {
      fill(255, 0, 0);
      rect(0, 500, 800, 100);
      textSize(50);
      fill(255, 255, 255);
      text("ACCESS DENIED", 175, 570);
    }
  } // Try end 
  catch (NullPointerException e) {
    println("NullPointerException");
  }
}
