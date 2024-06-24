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

void rof(int c1, int r1) {
  // حساب عدد الخطوات المطلوبة لكل من المحاور X و Y
  int xsteps = c1 * stepsPerColumn;
  int ysteps = r1 * stepsPerRow;

  yup(ysteps);
  xleft(xsteps);
  getmd();
  delay(1500);

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
  delay(1000); // تأخير لبدء التشغيل
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
        rof(c1, r1); // استدعاء الدالة rof بالقيم المدخلة
      }
    }
  }
}

//3علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3};
  int rows[] = {r1, r2, r3;

  // تنفيذ العمليات لكل زوج من الأعمدة والصفوف
  for (int i = 0; i < 3; i++) {
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
  int xend = c3 * stepsPerColumn;
  int yend = r3 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}
//4علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4};
  int rows[] = {r1, r2, r3, r4, r5};

  // تنفيذ العمليات لكل زوج من الأعمدة والصفوف
  for (int i = 0; i < 4; i++) {
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
  int xend = c4 * stepsPerColumn;
  int yend = r4 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}
//5 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5};
  int rows[] = {r1, r2, r3, r4, r5};

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
  int xend = c5 * stepsPerColumn;
  int yend = c5 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}
//6 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, 
         int c6, int r6) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5, c6};
  int rows[] = {r1, r2, r3, r4, r5, r6};

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
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, 
         int c6, int r6, int c7, int r7, int c8, int r8) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5, c6, c7, c8};
  int rows[] = {r1, r2, r3, r4, r5, r6, r7, r8};

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
  int xend = c8 * stepsPerColumn;
  int yend = r8 * stepsPerRow;
  ydown(yend);
  xright(xend);
  outarm();
}
//9 علب
void rof(int c1, int r1, int c2, int r2, int c3, int r3, int c4, int r4, int c5, int r5, 
         int c6, int r6, int c7, int r7, int c8, int r8, int c9, int r9) {
  // مصفوفة الأعمدة والصفوف
  int columns[] = {c1, c2, c3, c4, c5, c6, c7, c8, c9};
  int rows[] = {r1, r2, r3, r4, r5, r6, r7, r8, r9};

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