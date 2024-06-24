#include <Servo.h>
Servo m;
Servo a;

// تعريف أرقام الدبابيس
const int dirPinX = 5;
const int stepPinX = 2;
const int dirPinY = 6;
const int stepPinY = 3;

// تعريف المسافات بين الأعمدة والصفوف
const int stepsPerColumn = 240;
const int stepsPerRow = 2500;

void getmd() {
  a.write(180);
  delay(1000);
  a.write(0);
  delay(1000);
}

void outarm() {
  m.write(120);
  delay(3000);
  m.write(0);
}

void zeroservo() {
  m.write(0);
  a.write(0);
  delay(0);
}

void xmove(const int xsteps) {
  xright(xsteps);
  xleft(xsteps);
}

void xright(const int xstepsRight) {
  digitalWrite(dirPinX, LOW);
  for (int x = 0; x < xstepsRight; x++) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(1000);
  }
}

void xleft(const int xstepsLeft) {
  digitalWrite(dirPinX, HIGH);
  for (int x = 0; x < xstepsLeft; x++) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(1000);
  }
}

void ymove(const int ysteps) {
  yup(ysteps);
  delay(500);
  ydown(ysteps);
}

void yup(const int ystepsUp) {
  digitalWrite(dirPinY, HIGH);
  for (int y = 0; y < ystepsUp; y++) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(700);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(700);
  }
}

void ydown(const int ystepsDown) {
  digitalWrite(dirPinY, LOW);
  for (int y = 0; y < ystepsDown; y++) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(700);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(700);
  }
}

void rof(const int columns[], const int rows[], int length) {
  for (int i = 0; i < length; i++) {
    int xsteps = columns[i] * stepsPerColumn;
    int ysteps = rows[i] * stepsPerRow;

    yup(ysteps);
    xleft(xsteps);
    getmd();
    delay(1000);

    if (i < length - 1) {
      int nextYsteps = (rows[i + 1] - rows[i]) * stepsPerRow;
      if (nextYsteps > 0) {
        yup(nextYsteps);
      } else {
        ydown(-nextYsteps);
      }

      int nextXsteps = (columns[i + 1] - columns[i]) * stepsPerColumn;
      if (nextXsteps > 0) {
        xleft(nextXsteps);
      } else {
        xright(-nextXsteps);
      }

      getmd();
      delay(1000);
    }
  }

  int xend = columns[length - 1] * stepsPerColumn;
  int yend = rows[length - 1] * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

void setup() {
  m.attach(20);
  a.attach(21);
  zeroservo();
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');

    if (input.startsWith("rof(")) {
      input = input.substring(4, input.length() - 1); // إزالة "rof(" و ")"

      int columns[10];
      int rows[10];
      int colIndex = 0;
      int rowIndex = 0;
      bool isColumn = true;

      for (int i = 0; i < input.length(); i++) {
        if (input.charAt(i) == ',') {
          isColumn = !isColumn;
        } else if (isColumn) {
          columns[colIndex] = input.substring(i, i + 1).toInt();
          colIndex++;
          isColumn = false;
        } else {
          rows[rowIndex] = input.substring(i, i + 1).toInt();
          rowIndex++;
          isColumn = true;
        }
      }

      int length = colIndex > rowIndex ? colIndex : rowIndex;
      rof(columns, rows, length);
    }
  }
}
