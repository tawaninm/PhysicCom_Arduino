// ====== รูปแบบไฟของแต่ละเลข (สำหรับ Common Cathode) ======
int num_array[10][7] = {
  {1,1,1,1,1,1,0},  // 0
  {0,1,1,0,0,0,0},  // 1
  {1,1,0,1,1,0,1},  // 2
  {1,1,1,1,0,0,1},  // 3
  {0,1,1,0,0,1,1},  // 4
  {1,0,1,1,0,1,1},  // 5
  {1,0,1,1,1,1,1},  // 6
  {1,1,1,0,0,0,0},  // 7
  {1,1,1,1,1,1,1},  // 8
  {1,1,1,0,0,1,1}   // 9
};

// ====== กำหนดขา segment ที่แชร์ร่วมกัน (ตามรูป) ======
const int segment_a = 2; 
const int segment_b = 3; 
const int segment_c = 4;
const int segment_d = 5;
const int segment_e = 6;
const int segment_f = 7;
const int segment_g = 8;

const int segmentPins[7] = {
  segment_a, segment_b, segment_c, segment_d, segment_e, segment_f, segment_g
};

// ====== ขาเลือกหลัก (Common Cathode) ======
const int digitL = 12;   // หลักซ้าย
const int digitR = 13;   // หลักขวา

void setup() {
  for (int i = 0; i < 7; i++) pinMode(segmentPins[i], OUTPUT);
  pinMode(digitL, OUTPUT);
  pinMode(digitR, OUTPUT);
  // ปิดทั้งสองหลักก่อน (CC: HIGH = ปิด)
  digitalWrite(digitL, HIGH);
  digitalWrite(digitR, HIGH);
}

void setSegments(uint8_t n) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], num_array[n][i] ? HIGH : LOW); // CC: HIGH = ติด
  }
}

inline void enableLeftDigit()  { digitalWrite(digitL, LOW);  } // CC เปิดด้วย LOW
inline void enableRightDigit() { digitalWrite(digitR, LOW);  }
inline void disableDigits()    { digitalWrite(digitL, HIGH); digitalWrite(digitR, HIGH); }

// มัลติเพล็กซ์เลขสองหลักเป็นระยะเวลาที่กำหนด (ms)
void displayTwoDigits(uint8_t tens, uint8_t ones, unsigned long duration_ms) {
  unsigned long tEnd = millis() + duration_ms;
  while (millis() < tEnd) {
    // หลักซ้าย
    setSegments(tens);
    enableLeftDigit();
    delayMicroseconds(1500);
    disableDigits();
    // หลักขวา
    setSegments(ones);
    enableRightDigit();
    delayMicroseconds(1500);
    disableDigits();
  }
  disableDigits(); // กัน ghost ท้ายรอบ
}

void loop() {
  // รหัสนักศึกษา: 67070069 -> แสดงทีละคู่: 67, 07, 00, 69
  const char id[] = "67070069";
  // คู่ละ 1 วินาที
  const unsigned long holdMs = 1000;

  for (int i = 0; i < 8; i += 2) {
    uint8_t left  = id[i]   - '0';  // หลักซ้ายของคู่นั้น
    uint8_t right = id[i+1] - '0';  // หลักขวาของคู่นั้น
    displayTwoDigits(left, right, holdMs);
  }
}
