#include <U8glib.h>
#include <Keypad.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Wire.h>
#include <HX711.h>
#include <LedControl.h>

//pin
const int LCD_e = 13;
const int LCD_rs = 12;
const int LCD_rw = 11;
const int KEY_c4 = 30;
const int KEY_c3 = 32;
const int KEY_c2 = 34;
const int KEY_c1 = 36;
const int KEY_r4 = 38;
const int KEY_r3 = 40;
const int KEY_r2 = 42;
const int KEY_r1 = 44;
const int PUSH_b1 = A10;
const int PUSH_b2 = A12;
const int LCELL_dout = 3;
const int LCELL_sck = 2;
const int SEVEN_din = 51;
const int SEVEN_clk = 52;
const int SEVEN_load = A15;
const int LED_1 = A0;
const int LED_2 = A1;
const int LED_3 = A2;
const int LED_4 = A3;
const int LED_5 = A4;
const int LED_6 = A5;

//inisialisasi
U8GLIB_ST7920_128X64 lcd(LCD_e, LCD_rw, LCD_rs, U8G_PIN_NONE);
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8',' 9', 'C'},
  {'*', '0', '#', 'D'}
};
byte baris[4] = {KEY_r1, KEY_r2, KEY_r3, KEY_r4};
byte kolom[4] = {KEY_c1, KEY_c2, KEY_c3, KEY_c4};
Keypad keypad = Keypad(makeKeymap(keys), baris, kolom, 4, 4);
HX711 hx711;
LedControl seven = LedControl(SEVEN_din, SEVEN_clk, SEVEN_load, 1);

//data
int a, b, d1, d2, d3, d4, d5, b1, b2;
char jenis;
float berat, karbohidrat;

void setup() {
  Serial.begin(9600);

  Serial3.begin(9600);
  mp3_set_serial(Serial3);
  delay(10);

  //printWarning

  hx711.begin(LCELL_dout, LCELL_sck);
  hx711.set_scale(440.0);
  hx711.tare(100);

  seven.shutdown(0, false);
  seven.setIntensity(0, 2);

  pinMode(PUSH_b1, INPUT);
  pinMode(PUSH_b2, INPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);

  a = 0;
  b = 0;
}

void loop() {
  lcdPrint(a);
  sevenMAX7219();

  //Pengenalan Alat
  if(a == 0) {
    bool buttonTimbangan = digitalRead(A10);
    bool buttonSaran = digitalRead(A12);
    if(buttonTimbangan) {
      a = 1;
    }
  }

  //Jenis Makanan 1
  if(a == 1) {
    char key = keypad.getKey();
    switch(key) {
      case 'A':
      a = 0;
      break;

      case 'B':
      a = 2;
      break;

      case '1':
      a = 4;
      lcdPrint(a);
      play(1, 8000);
      break;

      case '2':
      a = 5;
      lcdPrint(a);
      play(2, 7000);
      break;

      case '3':
      a = 6;
      lcdPrint(a);
      play(3, 9000);
      break;
    }
    b = a;
  }

  //Jenis Makanan 2
  if(a == 2) {
    char key = keypad.getKey();
    switch(key) {
      case 'A':
      a = 1;
      break;

      case 'B':
      a = 3;
      break;

      case '4':
      a = 7;
      lcdPrint(a);
      play(4, 7000);
      break;

      case '5':
      a = 8;
      lcdPrint(a);
      play(5, 5000);
      break;

      case '6':
      a = 9;
      lcdPrint(a);
      play(6, 7000);
      break;
    }
    b = a;
  }

  //Jenis Makanan 3
  if(a == 3) {
    char key = keypad.getKey();
    switch(key) {
      case 'A':
      a = 2;
      break;

      case '7':
      a = 10;
      lcdPrint(a);
      play(7, 5000);
      break;

      case '8':
      a = 11;
      lcdPrint(a);
      play(8, 6000);
      break;

      case '9':
      a = 12;
      lcdPrint(a);
      play(9, 5000);
      break;
    }
    b = a;
  }

  //Serealia
  if(a == 4) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 1;
    } else if(key == 'B') {
      jenis = '1';
      a = 13;
    }
  }

  //Umbi
  if(a == 5) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 1;
    } else if(key == 'B') {
      jenis = '2';
      a = 13;
    }
  }

  //Kacang
  if(a == 6) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 1;
    } else if(key == 'B') {
      jenis = '3';
      a = 13;
    }
  }

  //Sayur
  if(a == 7) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 2;
    } else if(key == 'B') {
      jenis = '4';
      a = 13;
    }
  }

  //Buah
  if(a == 8) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 2;
    } else if(key == 'B') {
      jenis = '5';
      a = 13;
    }
  }

  //Daging
  if(a == 9) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 2;
    } else if(key == 'B') {
      jenis = '6';
      a = 13;
    }
  }

  //Ikan
  if(a == 10) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 3;
    } else if(key == 'B') {
      jenis = '7';
      a = 13;
    }
  }

  //Telur
  if(a == 11) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 3;
    } else if(key == 'B') {
      jenis = '8';
      a = 13;
    }
  }

  //Susu
  if(a == 12) {
    char key = keypad.getKey();
    if(key == 'A') {
      a = 3;
    } else if(key == 'B') {
      jenis = '9';
      a = 13;
    }
  }

  //Proses
  if(a == 13) {
    sensorBerat(jenis);
    char key = keypad.getKey();
    if(key == 'A') {
      kosongkanBerat();
      a = b;
    } else if(key == 'B') {
      a = 14;
    }
  }

  if(a == 14) {
    kosongkanBerat();
    LED(0);
    char key = keypad.getKey();
    if(key == 'A') {
      a = 13;
    }
  }
}

void lcdPrint(int print) {
  lcd.firstPage();
  do {
    menu(print);
  } while(lcd.nextPage());
}

void menu(int pilihanMenu) {
  switch(pilihanMenu) {
    case 0:
    pengenalanAlat();
    break;

    case 1:
    jenisMakanan("1. Serealia", "2. Umbi", "3. Kacang", true);
    break;

    case 2:
    jenisMakanan("4. Sayur", "5. Buah", "6. Daging", true);
    break;

    case 3:
    jenisMakanan("7. Ikan", "8. Telur", "9. Susu", false);
    break;

    case 4:
    contohMakanan(39, "SEREALIA", 52, "BERAS", 49, "TERIGU", 39, "ROTI MANIS");
    break;

    case 5:
    contohMakanan(51, "UMBI", 47, "KENTANG", 44, "SINGKONG", 29, "TEPUNG TAPIOKA");
    break;

    case 6:
    contohMakanan(41, "KACANG", 54, "TAHU", 52, "TEMPE", 34, "KACANG TANAH");
    break;

    case 7:
    contohMakanan(46, "SAYUR", 37, "TOMAT MERAH", 37, "NANGKA MUDA", 32, "DAUN SINGKONG");
    break;

    case 8:
    contohMakanan(50, "BUAH", 37, "JERUK MANIS", 34, "PISANG KAPOK", 34, "PISANG AMBON");
    break;

    case 9:
    contohMakanan(43, "DAGING", 39, "BAKSO SAPI", 37, "DAGING SAPI", 37, "DAGING AYAM");
    break;

    case 10:
    contohMakanan(51, "IKAN", 54, "LELE", 47, "TONGKOL", 47, "BANDENG");
    break;
    
    case 11:
    contohMakanan(48, "TELUR", 39, "TELUR AYAM", 37, "TELUR BEBEK", 37, "TELUR PUYUH");
    break;

    case 12:
    contohMakanan(51, "SUSU", 27, "UHT VANILA CAIR", 27, "UHT COKLAT CAIR", 22, "SUSU KENTAL MANIS");
    break;

    case 13:
    proses();
    break;

    case 14:
    karbohidratTotal();
    break;
  }
}

void pengenalanAlat() {
  lcd.setFont(u8g_font_helvB08);
  lcd.setPrintPos(31, 30);
  lcd.print("TIMBANGAN");
  lcd.setPrintPos(26, 42);
  lcd.print("KARBOHIDRAT");
}

void jenisMakanan(String satu, String dua, String tiga, bool lanjut) {
  lcd.setFont(u8g_font_helvB08);
  lcd.setPrintPos(5, 18);
  lcd.print(satu);
  lcd.setPrintPos(5, 30);
  lcd.print(dua);
  lcd.setPrintPos(5, 42);
  lcd.print(tiga);
  lcd.setFont(u8g_font_5x7);
  lcd.setPrintPos(5, 59);
  lcd.print("A.KEMBALI");
  if(lanjut) {
    lcd.setPrintPos(84, 59);
    lcd.print("B.LANJUT");
  }
}

void contohMakanan(int p1, String satu, int p2, String dua, int p3, String tiga, int p4, String empat) {
  lcd.setFont(u8g_font_helvB08);
  lcd.setPrintPos(p1, 17);
  lcd.print(satu);
  lcd.setFont(u8g_font_5x7);
  lcd.setPrintPos(p2, 27);
  lcd.print(dua);
  lcd.setPrintPos(p3, 35);
  lcd.print(tiga);
  lcd.setPrintPos(p4, 43);
  lcd.print(empat);
  lcd.setPrintPos(5, 59);
  lcd.print("A.KEMBALI");
  lcd.setPrintPos(84, 59);
  lcd.print("B.LANJUT");
}

void proses() {
  lcd.setFont(u8g_font_helvB08);
  lcd.setPrintPos(19, 18);
  lcd.print("LANJUTKAN JIKA");
  lcd.setPrintPos(13, 30);
  lcd.print("MAKANAN SELESAI");
  lcd.setPrintPos(34, 42);
  lcd.print("DITIMBANG");
  lcd.setFont(u8g_font_5x7);
  lcd.setPrintPos(5, 59);
  lcd.print("A.KEMBALI");
  lcd.setPrintPos(84, 59);
  lcd.print("B.LANJUT");
}

void karbohidratTotal() {
  lcd.drawFrame(0, 0, 128, 64);
  lcd.drawFrame(1, 1, 126, 62);
  lcd.drawBox(64, 0, 2, 64);
  lcd.setFont(u8g_font_5x7);
  char *nama = jenisMakan(jenis);
  char karbohidratString[10];

  Serial.print("Karbohidrat : ");
  Serial.print(karbohidrat);
  Serial.print("     Berat : ");
  Serial.println(berat);
  
  dtostrf(karbohidrat, 4, 2, karbohidratString);
  tengahX(nama, 13, 2, 64);
  tengahX(karbohidratString, 13, 66, 128);
}

void play(int judul, int lama) {
  mp3_play(judul);
  delay(lama);
}

void sensorBerat(char jenis) {
  berat = hx711.get_units(10);
  berat = -(berat);
  if(berat < 0.20) {
    berat = 0.00;
  }

  float pengali = karbohidratMakanan(jenis);
  karbohidrat = berat*pengali;

  int lampu = persen(berat, jenis);
  LED(lampu);

  int depan = berat;
  int belakang = (berat - depan) * 100;
  if(belakang < 0) {
    belakang = -(belakang);
  }

  d1 = (depan%10);
  d2 = (depan/10)%10;
  d3 = (depan/100)%10;
  d4 = (depan/1000)%10;
  d5 = (depan/10000)%10;
  b1 = (belakang%10);
  b2 = (belakang/10)%10;
}

void kosongkanBerat() {
  d1 = 0;
  d2 = 0;
  d3 = 0;
  d4 = 0;
  d5 = 0;
  b1 = 0;
  b2 = 0;
}

void sevenMAX7219() {
  seven.setDigit(0, 0, b1, false);
  seven.setDigit(0, 1, b2, false);
  seven.setChar(0, 2, '.', true);
  seven.setDigit(0, 2, d1, true);
  seven.setDigit(0, 3, d2, false);
  seven.setDigit(0, 4, d3, false);
  seven.setDigit(0, 5, d4, false);
  seven.setDigit(0, 6, d5, false);
  seven.setDigit(0, 7, 0, false);
  seven.setDigit(0, 8, 0, false);
}

void LED(int jumlah) {
  switch(jumlah) {
    case 0:
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);
    break;

    case 1:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);
    break;

    case 2:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);
    break;

    case 3:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);
    break;

    case 4:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);
    break;

    case 5:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    digitalWrite(LED_5, HIGH);
    digitalWrite(LED_6, LOW);
    break;

    case 6:
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    digitalWrite(LED_5, HIGH);
    digitalWrite(LED_6, HIGH);
    break;
  }
}

int persen(float beratan, char jenis) {
  int persentase;
  switch(jenis) {
    case '1':
    persentase = persenan(beratan, 5.95, 11.90, 17.85, 23.80, 29.75, 35.71);
    break;
    
    case '2':
    persentase = persenan(beratan, 15.58, 31.17, 46.76, 62.35, 77.93, 93.52);
    break;

    case '3':
    persentase = persenan(beratan, 45.28, 90.57, 135.86, 181.15, 226.44, 271.73);
    break;

    case '4':
    persentase = persenan(beratan, 62.94, 125.88, 188.82, 251.76, 314.70, 377.64);
    break;

    case '5':
    persentase = persenan(beratan, 19.23, 38.47, 57.71, 76.94, 96.18, 115.42);
    break;

    case '6':
    persentase = persenan(beratan, 165.34, 330.68, 496.03, 661.37, 826.71, 992.06);
    break;

    case '7':
    persentase = 0;
    break;

    case '8':
    persentase = persenan(beratan, 478.92, 957.85, 1436.78, 1915.70, 2394.63, 2873.56);
    break;

    case '9':
    persentase = persenan(beratan, 29.84, 59.69, 89.54, 119.38, 149.23, 179.08);
    break;
  }
  return persentase;
}

int persenan(float beratan, float satu, float dua, float tiga, float empat, float lima, float enam) {
  if(beratan >= enam) {
    return 6;
  } else if(beratan >= lima) {
    return 5;
  } else if(beratan >= empat) {
    return 4;
  } else if(beratan >= tiga) {
    return 3;
  } else if(beratan >= dua) {
    return 2;
  } else if(beratan >= satu) {
    return 1;
  } else {
    return 0;
  }
}

char *jenisMakan(char jenis) {
  char *makan;
  switch(jenis) {
    case '1':
    makan = "Serealia";
    break;
    
    case '2':
    makan = "Umbi";
    break;

    case '3':
    makan = "Kacang";
    break;

    case '4':
    makan = "Sayur";
    break;

    case '5':
    makan = "Buah";
    break;

    case '6':
    makan = "Daging";
    break;

    case '7':
    makan = "Ikan";
    break;

    case '8':
    makan = "Telur";
    break;

    case '9':
    makan = "Susu";
    break;
  }
  return makan;
}

float karbohidratMakanan(char jenis) {
  float makan;
  switch(jenis) {
    case '1':
    makan = 0.70;
    break;
    
    case '2':
    makan = 0.26;
    break;

    case '3':
    makan = 0.09;
    break;

    case '4':
    makan = 0.06;
    break;

    case '5':
    makan = 0.21;
    break;

    case '6':
    makan = 0.02;
    break;

    case '7':
    makan = 0;
    break;

    case '8':
    makan = 0;
    break;

    case '9':
    makan = 0.13;
    break;
  }
  return makan;
}

void tengahX(char *tulisan, int y, int min, int maks) {
  int total = maks - min;
  int tX = (total - (lcd.getStrWidth(tulisan)))/2;
  int x = tX + min;
  lcd.setPrintPos(x, y);
  lcd.print(tulisan);
}