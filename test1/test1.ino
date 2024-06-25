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
const int stepsPerRow = 4450;

void getmd() {
  a.write(170);
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

void rof(int c1, int r1) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y
  int xsteps = c1 * stepsPerColumn;
  int ysteps = r1 * stepsPerRow;

  yup(ysteps);
  xleft(xsteps);
  getmd();
  delay(1000);

  ydown(ysteps);
  xright(xsteps);
  outarm();
}

void rof(int c1, int r1, int c2, int r2) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  }
  if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (r1 == r2) {
    yup(0);
    ydown(0);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  }
  if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  if (c1 == c2) {
    xright(0);
    xleft(0);
  }
  getmd();
  int xend = c2 * stepsPerColumn;
  int yend = r2 * stepsPerRow;
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
  Serial.begin(115200); // بدء الاتصال التسلسلي بمعدل نقل 115200
  delay(3000); // تأخير لبدء التشغيل
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // قراءة الإدخال حتى newline

    // تحقق إذا كان الإدخال يبدأ بـ "x="
    if (input.startsWith("x=")) {
      int xsteps = input.substring(2).toInt(); // استخراج قيمة X وتحويلها إلى عدد صحيح
      xmove(xsteps); // استدعاء الدالة xmove بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "y="
    else if (input.startsWith("y=")) {
      int ysteps = input.substring(2).toInt(); // استخراج قيمة Y وتحويلها إلى عدد صحيح
      ymove(ysteps); // استدعاء الدالة ymove بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "xr="
    else if (input.startsWith("xr=")) {
      int xstepsRight = input.substring(3).toInt(); // استخراج قيمة xr وتحويلها إلى عدد صحيح
      xright(xstepsRight); // استدعاء الدالة xright بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "xl="
    else if (input.startsWith("xl=")) {
      int xstepsLeft = input.substring(3).toInt(); // استخراج قيمة xl وتحويلها إلى عدد صحيح
      xleft(xstepsLeft); // استدعاء الدالة xleft بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "up="
    else if (input.startsWith("up=")) {
      int ystepsUp = input.substring(3).toInt(); // استخراج قيمة up وتحويلها إلى عدد صحيح
      yup(ystepsUp); // استدعاء الدالة yup بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "down="
    else if (input.startsWith("down=")) {
      int ystepsDown = input.substring(5).toInt(); // استخراج قيمة down وتحويلها إلى عدد صحيح
      ydown(ystepsDown); // استدعاء الدالة ydown بالقيمة المدخلة
    }
    // تحقق إذا كان الإدخال يبدأ بـ "rof("
    else if (input.startsWith("rof(")) {
      int commaIndex1 = input.indexOf(',');
      int commaIndex2 = input.lastIndexOf(',');
      int closeIndex = input.indexOf(')');
      if (commaIndex1 != -1 && commaIndex2 != -1 && closeIndex != -1) {
        int c1 = input.substring(4, commaIndex1).toInt(); // استخراج قيمة العمود الأول
        int r1 = input.substring(commaIndex1 + 1, commaIndex2).toInt(); // استخراج قيمة الصف الأول
        int c2 = input.substring(commaIndex2 + 1, closeIndex).toInt(); // استخراج قيمة العمود الثاني
        int r2 = input.substring(closeIndex + 1).toInt(); // استخراج قيمة الصف الثاني
        rof(c1, r1, c2, r2); // استدعاء الدالة rof بالقيم المدخلة
      } else if (commaIndex1 != -1 && closeIndex != -1) {
        int c1 = input.substring(4, commaIndex1).toInt(); // استخراج قيمة العمود
        int r1 = input.substring(commaIndex1 + 1, closeIndex).toInt(); // استخراج قيمة الصف
        // rof(c1, r1); // استدعاء الدالة rof بالقيم المدخلة
      }
    }
  }
}

//3علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  }
  if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  }
  if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  }
  if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  }
  if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c3 * stepsPerColumn;
  int yend = r3 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

//4علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  } else if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  } else if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  } else if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  } else if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الرابعة
  if (r4 > r3) {
    int yr4 = (r4 - r3) * stepsPerRow;
    yup(yr4);
  } else if (r3 > r4) {
    int yr4 = (r3 - r4) * stepsPerRow;
    ydown(yr4);
  }
  if (c4 > c3) {
    int xc4 = (c4 - c3) * stepsPerColumn;
    xleft(xc4);
  } else if (c3 > c4) {
    int xc4 = (c3 - c4) * stepsPerColumn;
    xright(xc4);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c4 * stepsPerColumn;
  int yend = r4 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

//5 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  } else if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  } else if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  } else if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  } else if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الرابعة
  if (r4 > r3) {
    int yr4 = (r4 - r3) * stepsPerRow;
    yup(yr4);
  } else if (r3 > r4) {
    int yr4 = (r3 - r4) * stepsPerRow;
    ydown(yr4);
  }
  if (c4 > c3) {
    int xc4 = (c4 - c3) * stepsPerColumn;
    xleft(xc4);
  } else if (c3 > c4) {
    int xc4 = (c3 - c4) * stepsPerColumn;
    xright(xc4);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الخامسة
  if (r5 > r4) {
    int yr5 = (r5 - r4) * stepsPerRow;
    yup(yr5);
  } else if (r4 > r5) {
    int yr5 = (r4 - r5) * stepsPerRow;
    ydown(yr5);
  }
  if (c5 > c4) {
    int xc5 = (c5 - c4) * stepsPerColumn;
    xleft(xc5);
  } else if (c4 > c5) {
    int xc5 = (c4 - c5) * stepsPerColumn;
    xright(xc5);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c5 * stepsPerColumn;
  int yend = r5 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

//6 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, int c6, int r6) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  } else if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  } else if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  } else if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  } else if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الرابعة
  if (r4 > r3) {
    int yr4 = (r4 - r3) * stepsPerRow;
    yup(yr4);
  } else if (r3 > r4) {
    int yr4 = (r3 - r4) * stepsPerRow;
    ydown(yr4);
  }
  if (c4 > c3) {
    int xc4 = (c4 - c3) * stepsPerColumn;
    xleft(xc4);
  } else if (c3 > c4) {
    int xc4 = (c3 - c4) * stepsPerColumn;
    xright(xc4);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الخامسة
  if (r5 > r4) {
    int yr5 = (r5 - r4) * stepsPerRow;
    yup(yr5);
  } else if (r4 > r5) {
    int yr5 = (r4 - r5) * stepsPerRow;
    ydown(yr5);
  }
  if (c5 > c4) {
    int xc5 = (c5 - c4) * stepsPerColumn;
    xleft(xc5);
  } else if (c4 > c5) {
    int xc5 = (c4 - c5) * stepsPerColumn;
    xright(xc5);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة السادسة
  if (r6 > r5) {
    int yr6 = (r6 - r5) * stepsPerRow;
    yup(yr6);
  } else if (r5 > r6) {
    int yr6 = (r5 - r6) * stepsPerRow;
    ydown(yr6);
  }
  if (c6 > c5) {
    int xc6 = (c6 - c5) * stepsPerColumn;
    xleft(xc6);
  } else if (c5 > c6) {
    int xc6 = (c5 - c6) * stepsPerColumn;
    xright(xc6);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c6 * stepsPerColumn;
  int yend = r6 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

//7 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, 
         int c6, int r6, int c7, int r7) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5, c6, c7};
  int rows[] = {r1, r2, r3, r4, r5, r6, r7};

  // تنفيذ العمليات لكل زوج من الأعمدة والصفوف
  for (int i = 0; i < 10; i++) {
    int xsteps = columns[i] * stepsPerColumn;
    int ysteps = rows[i] * stepsPerRow;

    // حركة X و Y للوصول إلى الموقع
    yup(ysteps);
    xleft(xsteps);
    getmd();
    delay(1000);

    // إذا لم يكن هذا هو الزوج الأخير، قم بالانتقال إلى الزوج التالي
    if (i < 9) {
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

  // العودة إلى النقطة الأصلية
  int xend = c7 * stepsPerColumn;
  int yend = r7 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}
//8 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, int c6, int r6, int c7, int r7, int c8, int r8) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  } else if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  } else if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  } else if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  } else if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الرابعة
  if (r4 > r3) {
    int yr4 = (r4 - r3) * stepsPerRow;
    yup(yr4);
  } else if (r3 > r4) {
    int yr4 = (r3 - r4) * stepsPerRow;
    ydown(yr4);
  }
  if (c4 > c3) {
    int xc4 = (c4 - c3) * stepsPerColumn;
    xleft(xc4);
  } else if (c3 > c4) {
    int xc4 = (c3 - c4) * stepsPerColumn;
    xright(xc4);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الخامسة
  if (r5 > r4) {
    int yr5 = (r5 - r4) * stepsPerRow;
    yup(yr5);
  } else if (r4 > r5) {
    int yr5 = (r4 - r5) * stepsPerRow;
    ydown(yr5);
  }
  if (c5 > c4) {
    int xc5 = (c5 - c4) * stepsPerColumn;
    xleft(xc5);
  } else if (c4 > c5) {
    int xc5 = (c4 - c5) * stepsPerColumn;
    xright(xc5);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة السادسة
  if (r6 > r5) {
    int yr6 = (r6 - r5) * stepsPerRow;
    yup(yr6);
  } else if (r5 > r6) {
    int yr6 = (r5 - r6) * stepsPerRow;
    ydown(yr6);
  }
  if (c6 > c5) {
    int xc6 = (c6 - c5) * stepsPerColumn;
    xleft(xc6);
  } else if (c5 > c6) {
    int xc6 = (c5 - c6) * stepsPerColumn;
    xright(xc6);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة السابعة
  if (r7 > r6) {
    int yr7 = (r7 - r6) * stepsPerRow;
    yup(yr7);
  } else if (r6 > r7) {
    int yr7 = (r6 - r7) * stepsPerRow;
    ydown(yr7);
  }
  if (c7 > c6) {
    int xc7 = (c7 - c6) * stepsPerColumn;
    xleft(xc7);
  } else if (c6 > c7) {
    int xc7 = (c6 - c7) * stepsPerColumn;
    xright(xc7);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثامنة
  if (r8 > r7) {
    int yr8 = (r8 - r7) * stepsPerRow;
    yup(yr8);
  } else if (r7 > r8) {
    int yr8 = (r7 - r8) * stepsPerRow;
    ydown(yr8);
  }
  if (c8 > c7) {
    int xc8 = (c8 - c7) * stepsPerColumn;
    xleft(xc8);
  } else if (c7 > c8) {
    int xc8 = (c7 - c8) * stepsPerColumn;
    xright(xc8);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c8 * stepsPerColumn;
  int yend = r8 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}


//9 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, int c6, int r6, int c7, int r7, int c8, int r8, int c9, int r9) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y للنقطة الأولى
  int xc1 = c1 * stepsPerColumn;
  int yr1 = r1 * stepsPerRow;

  yup(yr1);
  xleft(xc1);
  delay(2000);
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثانية
  if (r2 > r1) {
    int yr2 = (r2 - r1) * stepsPerRow;
    yup(yr2);
  } else if (r1 > r2) {
    int yr2 = (r1 - r2) * stepsPerRow;
    ydown(yr2);
  }
  if (c2 > c1) {
    int xc2 = (c2 - c1) * stepsPerColumn;
    xleft(xc2);
  } else if (c1 > c2) {
    int xc2 = (c1 - c2) * stepsPerColumn;
    xright(xc2);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثالثة
  if (r3 > r2) {
    int yr3 = (r3 - r2) * stepsPerRow;
    yup(yr3);
  } else if (r2 > r3) {
    int yr3 = (r2 - r3) * stepsPerRow;
    ydown(yr3);
  }
  if (c3 > c2) {
    int xc3 = (c3 - c2) * stepsPerColumn;
    xleft(xc3);
  } else if (c2 > c3) {
    int xc3 = (c2 - c3) * stepsPerColumn;
    xright(xc3);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الرابعة
  if (r4 > r3) {
    int yr4 = (r4 - r3) * stepsPerRow;
    yup(yr4);
  } else if (r3 > r4) {
    int yr4 = (r3 - r4) * stepsPerRow;
    ydown(yr4);
  }
  if (c4 > c3) {
    int xc4 = (c4 - c3) * stepsPerColumn;
    xleft(xc4);
  } else if (c3 > c4) {
    int xc4 = (c3 - c4) * stepsPerColumn;
    xright(xc4);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الخامسة
  if (r5 > r4) {
    int yr5 = (r5 - r4) * stepsPerRow;
    yup(yr5);
  } else if (r4 > r5) {
    int yr5 = (r4 - r5) * stepsPerRow;
    ydown(yr5);
  }
  if (c5 > c4) {
    int xc5 = (c5 - c4) * stepsPerColumn;
    xleft(xc5);
  } else if (c4 > c5) {
    int xc5 = (c4 - c5) * stepsPerColumn;
    xright(xc5);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة السادسة
  if (r6 > r5) {
    int yr6 = (r6 - r5) * stepsPerRow;
    yup(yr6);
  } else if (r5 > r6) {
    int yr6 = (r5 - r6) * stepsPerRow;
    ydown(yr6);
  }
  if (c6 > c5) {
    int xc6 = (c6 - c5) * stepsPerColumn;
    xleft(xc6);
  } else if (c5 > c6) {
    int xc6 = (c5 - c6) * stepsPerColumn;
    xright(xc6);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة السابعة
  if (r7 > r6) {
    int yr7 = (r7 - r6) * stepsPerRow;
    yup(yr7);
  } else if (r6 > r7) {
    int yr7 = (r6 - r7) * stepsPerRow;
    ydown(yr7);
  }
  if (c7 > c6) {
    int xc7 = (c7 - c6) * stepsPerColumn;
    xleft(xc7);
  } else if (c6 > c7) {
    int xc7 = (c6 - c7) * stepsPerColumn;
    xright(xc7);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة الثامنة
  if (r8 > r7) {
    int yr8 = (r8 - r7) * stepsPerRow;
    yup(yr8);
  } else if (r7 > r8) {
    int yr8 = (r7 - r8) * stepsPerRow;
    ydown(yr8);
  }
  if (c8 > c7) {
    int xc8 = (c8 - c7) * stepsPerColumn;
    xleft(xc8);
  } else if (c7 > c8) {
    int xc8 = (c7 - c8) * stepsPerColumn;
    xright(xc8);
  }
  getmd();
  delay(1000);

  // الانتقال إلى النقطة التاسعة
  if (r9 > r8) {
    int yr9 = (r9 - r8) * stepsPerRow;
    yup(yr9);
  } else if (r8 > r9) {
    int yr9 = (r8 - r9) * stepsPerRow;
    ydown(yr9);
  }
  if (c9 > c8) {
    int xc9 = (c9 - c8) * stepsPerColumn;
    xleft(xc9);
  } else if (c8 > c9) {
    int xc9 = (c8 - c9) * stepsPerColumn;
    xright(xc9);
  }
  getmd();

  // الانتقال إلى النقطة النهائية والعودة إلى الأصل
  int xend = c9 * stepsPerColumn;
  int yend = r9 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}

//10علب 
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, 
         int c6, int r6, int c7, int r7, int c8, int r8, int c9, int r9, int c10, int r10) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5, c6, c7, c8, c9, c10};
  int rows[] = {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10};

  // تنفيذ العمليات لكل زوج من الأعمدة والصفوف
  for (int i = 0; i < 10; i++) {
    int xsteps = columns[i] * stepsPerColumn;
    int ysteps = rows[i] * stepsPerRow;

    // حركة X و Y للوصول إلى الموقع
    yup(ysteps);
    xleft(xsteps);
    getmd();
    delay(1000);

    // إذا لم يكن هذا هو الزوج الأخير، قم بالانتقال إلى الزوج التالي
    if (i < 9) {
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

  // العودة إلى النقطة الأصلية
  int xend = c10 * stepsPerColumn;
  int yend = r10 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}