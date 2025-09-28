#include <LedControl.h>

// ขาต่อกับ MAX7219
const byte DIN_PIN = 12;   // DIN
const byte CLK_PIN = 11;   // CLK
const byte CS_PIN  = 10;   // CS/LOAD

// ใช้เมทริกซ์ 1 โมดูล
LedControl lc(DIN_PIN, CLK_PIN, CS_PIN, 1);

// ลาย 3x5 ของเลข 6 และ 9 (บิต 1=ติด) แต่ละแถวกว้าง 3 บิต: ซ้าย→ขวา คือ bit 2..0
const byte SIX_3x5[5]  = { B111, B100, B111, B101, B111 };
const byte NINE_3x5[5] = { B111, B101, B111, B001, B111 };

// วาดรูป 3x5 ที่ตำแหน่ง (topRow,leftCol)
void draw3x5(const byte pat[5], byte topRow, byte leftCol) {
  for (byte r = 0; r < 5; r++) {
    for (byte c = 0; c < 3; c++) {
      bool on = pat[r] & (1 << (2 - c));          // อ่านบิตซ้าย→ขวา
      lc.setLed(0, topRow + r, leftCol + c, on);  // device=0, row, col
    }
  }
}

void setup() {
  lc.shutdown(0, false);   // ปลุกชิป
  lc.setIntensity(0, 3);   // ความสว่าง 0..15
  lc.clearDisplay(0);

  // จัดวางให้อยู่กึ่งกลางแนวตั้ง: ใช้แถว 1..5
  // เลข '6' ที่คอลัมน์ 0..2, เว้น 1 คอลัมน์, เลข '9' ที่คอลัมน์ 4..6
  draw3x5(SIX_3x5,  1, 0);
  draw3x5(NINE_3x5, 1, 4);
}

void loop() {
  // วาดครั้งเดียวแล้วค้างไว้ ไม่ต้องทำอะไรต่อ
}
