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
const int PUSH_b1 = A10;
const int PUSH_b2 = A12;
const int KEY_c4 = 30;
const int KEY_c3 = 32;
const int KEY_c2 = 34;
const int KEY_c1 = 36;
const int KEY_r4 = 38;
const int KEY_r3 = 40;
const int KEY_r2 = 42;
const int KEY_r1 = 44;
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
HX711 loadcell;
LedControl seven = LedControl(SEVEN_din, SEVEN_clk, SEVEN_load, 1);

//data
int menuSekarang, menuSebelumnya;
int belakang1, belakang2, depan1, depan2, depan3, depan4, depan5, jumlah;
char *jenisMakanan, *makanan1, *makanan2, *makanan3;
float pengali, karbohidratMakanan, beratMakanan, totalBerat, totalKarbohidrat;
float berat1, berat2, berat3;
float karbohidrat1, karbohidrat2, karbohidrat3;

void setup() {
  Serial.begin(9600);
  
  //lcd
  lcdPrint(0);
  delay(4990);

  //push button
  pinMode(PUSH_b1, INPUT);
  pinMode(PUSH_b2, INPUT);

  //mp3
  Serial3.begin(9600);
  mp3_set_serial(Serial3);
  mp3_set_volume(15);
  delay(10);

  //HX711
  loadcell.begin(LCELL_dout, LCELL_sck);
  loadcell.set_scale(440.0);
  loadcell.tare(100);

  //max7219
  seven.shutdown(0, false);
  seven.setIntensity(0, 2);
  sevenReset();

  //led
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);

  menuSekarang = 1;
}

void loop() {
  jumlah = 1;

  lcdPrint(menuSekarang);
  sevenPrint();

  //pengenalan alat
  if(menuSekarang == 1) {
    bool buttonTimbangan = digitalRead(A10);
    bool buttonSaran = digitalRead(A12);
    if(buttonTimbangan) {
      menuSekarang = 15;
    }
  }

  //menimbang piring
  if(menuSekarang == 15) {
    totalBerat = 0;
    berat1 = 0;
    berat2 = 0;
    berat3 = 0;
    totalKarbohidrat = 0;
    karbohidrat1 = 0;
    karbohidrat2 = 0;
    karbohidrat3 = 0;
    makanan1 = "";
    makanan2 = "";
    makanan3 = "";
    lcdPrint(menuSekarang);
    for(int timbang = 0; timbang < 10; timbang++) {
      sevenPrint();
      loadcellOn(false);
      totalBerat = beratMakanan;
    }
    sevenReset();
    menuSekarang = 2;
  }

  if(menuSekarang == 2) {
    do {
      lcdPrint(menuSekarang);
      sevenPrint();

      //jenis makanan 1-9
      keypadJenisMakanan(2, true, '1', '2', '3', 5, 8000, 7000, 9000);
      keypadJenisMakanan(3, true, '4', '5', '6', 8, 7000, 5000, 7000);
      keypadJenisMakanan(4, false, '7', '8', '9', 11, 5000, 6000, 5000);

      //contoh makanan
      keypadContohMakanan(5, 2);
      keypadContohMakanan(6, 2);
      keypadContohMakanan(7, 2);
      keypadContohMakanan(8, 3);
      keypadContohMakanan(9, 3);
      keypadContohMakanan(10, 3);
      keypadContohMakanan(11, 4);
      keypadContohMakanan(12, 4);
      keypadContohMakanan(13, 4);

      //menimbang makanan
      if(menuSekarang == 14) {
        loadcellOn(true);

        int lampu = persenBerat(beratMakanan, menuSebelumnya - 1);
        ledPersen(lampu);

        char key = keypad.getKey();
        if(key == 'A') {
          ledPersen(0);
          sevenReset();
          menuSekarang = menuSebelumnya;
        } else if(key == 'B') {
          menuSekarang = 16;
          if(jumlah == 1) {
            makanan1 = jenisMakanan;
            berat1 = beratMakanan;
            karbohidrat1 = karbohidratMakanan;
          } else if(jumlah == 2) {
            makanan2 = jenisMakanan;
            berat2 = beratMakanan;
            karbohidrat2 = karbohidratMakanan;
          } else if(jumlah == 3) {
            makanan3 = jenisMakanan;
            berat3 = beratMakanan;
            karbohidrat3 = karbohidratMakanan;
          }
        }
      }

      if(menuSekarang == 16) {
        ledPersen(0);
        sevenReset();
        char key = keypad.getKey();
        if(key == 'A') {
          menuSekarang = 14;
        } else if(key == 'B') {
          menuSekarang = 17;
        }
      }

      if(menuSekarang == 17) {
        totalKarbohidrat = karbohidrat1 + karbohidrat2 + karbohidrat3;
        int lampu = persen(totalKarbohidrat, 16.66, 33.33, 50, 66.66, 83.33, 100);
        ledPersen(lampu);
        //mp3Play
        char key = keypad.getKey();
        if(key == 'A') {
          menuSekarang = 16;
        } else if(key == 'B') {
          if(jumlah < 3) {
            jumlah++;
            totalBerat = totalBerat + beratMakanan;
            ledPersen(0);
            menuSekarang = 2;
          }
        } else if(key == 'C') {
          ledPersen(0);
          menuSekarang = 1;
        }
      }

      if(menuSekarang == 1) {
        jumlah = 4;
      }
    } while (jumlah <= 3);
    Serial.print("Jenis 1 : ");
    Serial.print(makanan1);
    Serial.print("  Berat : ");
    Serial.print(berat1);
    Serial.print("  Karbohidrat : ");
    Serial.println(karbohidrat1);
    Serial.print("Jenis 2 : ");
    Serial.print(makanan2);
    Serial.print("  Berat : ");
    Serial.print(berat2);
    Serial.print("  Karbohidrat : ");
    Serial.println(karbohidrat2);
    Serial.print("Jenis 3 : ");
    Serial.print(makanan3);
    Serial.print("  Berat : ");
    Serial.print(berat3);
    Serial.print("  Karbohidrat : ");
    Serial.println(karbohidrat3);
    menuSekarang = 1;
  }
}

void lcdPrint(int print) {
  lcd.firstPage();
  do {
    lcdMenu(print);
  } while(lcd.nextPage());
}

void lcdMenu(int menu) {
  switch(menu) {
    case 0:
    lcdPeringatan();
    break;

    case 1:
    lcdPengenalanAlat();
    break;

    case 2:
    lcdJenisMakanan("1. Serealia", "2. Umbi", "3. Kacang", true);
    break;

    case 3:
    lcdJenisMakanan("4. Sayur", "5. Buah", "6. Daging", true);
    break;

    case 4:
    lcdJenisMakanan("7. Ikan", "8. Telur", "9. Susu", false);
    break;

    case 5:
    lcdContohMakanan("SEREALIA", "BERAS", "TERIGU", "ROTI MANIS");
    break;

    case 6:
    lcdContohMakanan("UMBI", "KENTANG", "SINGKONG", "TEPUNG TAPIOKA");
    break;

    case 7:
    lcdContohMakanan("KACANG", "TAHU", "TEMPE", "KACANG TANAH");
    break;

    case 8:
    lcdContohMakanan("SAYUR", "TOMAT MERAH", "NANGKA MUDA", "DAUN SINGKONG");
    break;

    case 9:
    lcdContohMakanan("BUAH", "JERUK MANIS", "PISANG KAPOK", "PISANG AMBON");
    break;

    case 10:
    lcdContohMakanan("DAGING", "BAKSO SAPI", "DAGING SAPI", "DAGING AYAM");
    break;

    case 11:
    lcdContohMakanan("IKAN", "LELE", "TONGKOL", "BANDENG");
    break;
    
    case 12:
    lcdContohMakanan("TELUR", "TELUR AYAM", "TELUR BEBEK", "TELUR PUYUH");
    break;

    case 13:
    lcdContohMakanan("SUSU", "UHT VANILA CAIR", "UHT COKLAT CAIR", "SUSU KENTAL MANIS");
    break;

    case 14:
    lcdMenimbang();
    break;

    case 15:
    lcdMenimbangPiring();
    break;

    case 16:
    lcdTabelKarbohidrat();
    break;

    case 17:
    lcdHasil(jumlah);
    break;

    case 18:
    lcdError();
    break;
  }
}

void lcdPeringatan() {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("PERINGATAN!", 28, 1, 128);
  lcd.setFont(u8g_font_5x7);
  lcdTengahX("KOSONGKAN TIMBANGAN", 36, 1, 128);
  lcdTengahX("SEBELUM DIGUNAKAN", 44, 1, 128);
}

void lcdPengenalanAlat() {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("TIMBANGAN", 30, 1, 128);
  lcdTengahX("KARBOHIDRAT", 42, 1, 128);
}

void lcdJenisMakanan(String satu, String dua, String tiga, bool lanjut) {
  lcd.setFont(u8g_font_helvB08);
  lcd.setPrintPos(5, 18);
  lcd.print(satu);
  lcd.setPrintPos(5, 30);
  lcd.print(dua);
  lcd.setPrintPos(5, 42);
  lcd.print(tiga);
  lcd.setFont(u8g_font_5x7);
  if((menuSekarang == 2) && (jumlah > 1)) {
    lcdBawah(false, lanjut);
  } else {
    lcdBawah(true, lanjut);
  }
}

void lcdContohMakanan(char *satu, char *dua, char *tiga, char *empat) {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX(satu, 17, 1, 128);
  lcd.setFont(u8g_font_5x7);
  lcdTengahX(dua, 27, 1, 128);
  lcdTengahX(tiga, 35, 1, 128);
  lcdTengahX(empat, 43, 1, 128);
  lcdBawah(true, true);
}

void lcdMenimbang() {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("LANJUTKAN JIKA", 18, 1, 128);
  lcdTengahX("MAKANAN SELESAI", 30, 1, 128);
  lcdTengahX("DITIMBANG", 42, 1, 128);
  lcd.setFont(u8g_font_5x7);
  lcdBawah(true, true);
}

void lcdMenimbangPiring() {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("LETAKKAN PIRING", 30, 1, 128);
  lcdTengahX("UNTUK DITIMBANG", 42, 1, 128);
}

void lcdTabelKarbohidrat() {
  lcd.drawFrame(0, 0, 128, 48);
  lcd.drawFrame(1, 1, 126, 46);
  lcd.drawBox(64, 0, 2, 48);
  lcd.setFont(u8g_font_5x7);
  char *nama1 = makanan1;
  char *nama2 = makanan2;
  char *nama3 = makanan3;
  char k1[10], k2[10], k3[10];
  dtostrf(karbohidrat1, 4, 2, k1);
  dtostrf(karbohidrat2, 4, 2, k2);
  dtostrf(karbohidrat3, 4, 2, k3);
  lcdTengahX(nama1, 16, 3, 64);
  lcdTengahX(nama2, 27, 3, 64);
  lcdTengahX(nama3, 38, 3, 64);
  if(karbohidrat1 != 0.00) {
    lcdTengahX(k1, 16, 67, 128);
  }
  if(karbohidrat2 != 0.00) {
    lcdTengahX(k2, 27, 67, 128);
  }
  if(karbohidrat3 != 0.00) {
    lcdTengahX(k3, 38, 67, 128);
  }
  lcdBawah(true, true);
}

void lcdHasil(int jumlah) {
  char karbohidrat[10];
  dtostrf(totalKarbohidrat, 4, 2, karbohidrat);
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("TOTAL KARBOHIDRAT", 18, 1, 128);
  lcdTengahX(karbohidrat, 31, 1, 128);
  lcd.setFont(u8g_font_5x7);
  if(jumlah == 3) {
    lcdBawah(true, false);
    lcd.setPrintPos(79, 59);
  } else {
    lcdBawah(true, true);
    lcd.setPrintPos(79, 48);
  }
  lcd.print("C.SELESAI");
}

void lcdError() {
  lcd.setFont(u8g_font_helvB08);
  lcdTengahX("ERROR!", 28, 1, 128);
  lcd.setFont(u8g_font_5x7);
  lcdTengahX("MAKANAN/PIRING", 36, 1, 128);
  lcdTengahX("SEBELUMNYA TERANGKAT", 44, 1, 128);
}

void lcdTengahX(char *tulisan, int y, int min, int maks) {
  int total = maks - min;
  int totalX = (total - (lcd.getStrWidth(tulisan)))/2;
  int x = totalX + min;
  lcd.setPrintPos(x, y);
  lcd.print(tulisan);
}

void lcdBawah(bool a, bool b) {
  if(a) {
    lcd.setPrintPos(5, 59);
    lcd.print("A.KEMBALI");
  }
  if(b) {
    lcd.setPrintPos(84, 59);
    lcd.print("B.LANJUT");
  }
}

void sevenPrint() {
  seven.setDigit(0, 0, belakang1, false);
  seven.setDigit(0, 1, belakang2, false);
  seven.setChar(0, 2, '.', true);
  seven.setDigit(0, 2, depan1, true);
  seven.setDigit(0, 3, depan2, false);
  seven.setDigit(0, 4, depan3, false);
  seven.setDigit(0, 5, depan4, false);
  seven.setDigit(0, 6, depan5, false);
  seven.setDigit(0, 7, 0, false);
  seven.setDigit(0, 8, 0, false);
}

void sevenReset() {
  belakang1 = 0;
  belakang2 = 0;
  depan1 = 0;
  depan2 = 0;
  depan3 = 0;
  depan4 = 0;
  depan5 = 0;
}

void keypadJenisMakanan(int menu, bool b, char keypad1, char keypad2, char keypad3, int menuSelanjutnya, int lama1, int lama2, int lama3) {
  if(menuSekarang == menu) {
    char key = keypad.getKey();
    if(key == 'A') {
      if(!((menuSekarang == 2) && (jumlah > 1))) {
        menuSekarang--;
      }
    } else if((key == 'B') && b) {
      menuSekarang++;
    } else if(key == keypad1) {
      menuSekarang = menuSelanjutnya;
      lcdPrint(menuSekarang);
      mp3Play(menuSekarang - 4, lama1);
    } else if(key == keypad2) {
      menuSekarang = menuSelanjutnya + 1;
      lcdPrint(menuSekarang);
      mp3Play(menuSekarang - 4, lama2);
    } else if(key == keypad3) {
      menuSekarang = menuSelanjutnya + 2;
      lcdPrint(menuSekarang);
      mp3Play(menuSekarang - 4, lama3);
    }
    menuSebelumnya = menuSekarang;
  }
}

void keypadContohMakanan(int menu, int sebelumnya) {
  if(menuSekarang == menu) {
    char key = keypad.getKey();
    if(key == 'A') {
      menuSekarang = sebelumnya;
    } else if(key == 'B') {
      makanan(menuSekarang - 1);
      menuSekarang = 14;
    }
  }
}

void mp3Play(int judul, int lama) {
  mp3_play(judul);
  delay(lama);
}

void loadcellOn(bool makanan) {
  beratMakanan = loadcell.get_units(10);
  beratMakanan = -(beratMakanan);

  if(makanan) {
    beratMakanan = beratMakanan - totalBerat;
    if(beratMakanan < -1.20) {
      beratMakanan = 0.00;
      lcdPrint(18);
      delay(1200);
    }
  }

  if(beratMakanan < 0.20) {
    beratMakanan = 0.00;
  }

  karbohidratMakanan = pengali * beratMakanan;
  
  int depan = beratMakanan;
  int belakang = (beratMakanan - depan) * 100;
  if(belakang < 0) {
    belakang = -(belakang);
  }

  depan1 = (depan%10);
  depan2 = (depan/10)%10;
  depan3 = (depan/100)%10;
  depan4 = (depan/1000)%10;
  depan5 = (depan/10000)%10;
  belakang1 = (belakang%10);
  belakang2 = (belakang/10)%10;
}

void ledOn(bool satu, bool dua, bool tiga, bool empat, bool lima, bool enam) {
  digitalWrite(LED_1, satu);
  digitalWrite(LED_2, dua);
  digitalWrite(LED_3, tiga);
  digitalWrite(LED_4, empat);
  digitalWrite(LED_5, lima);
  digitalWrite(LED_6, enam);
}

void ledPersen(int led) {
  switch(led) {
    case 0:
    ledOn(false, false, false, false, false, false);
    break;

    case 1:
    ledOn(true, false, false, false, false, false);
    break;

    case 2:
    ledOn(true, true, false, false, false, false);
    break;

    case 3:
    ledOn(true, true, true, false, false, false);
    break;

    case 4:
    ledOn(true, true, true, true, false, false);
    break;

    case 5:
    ledOn(true, true, true, true, true, false);
    break;

    case 6:
    ledOn(true, true, true, true, true, true);
    break;
  }
}

void makanan(int jenis) {
  jenis = jenis - 3;
  switch(jenis) {
    case 1:
    jenisMakanan = "Serealia";
    pengali = 0.70;
    break;

    case 2:
    jenisMakanan = "Umbi";
    pengali = 0.26;
    break;

    case 3:
    jenisMakanan = "Kacang";
    pengali = 0.09;
    break;

    case 4:
    jenisMakanan = "Sayur";
    pengali = 0.06;
    break;

    case 5:
    jenisMakanan = "Buah";
    pengali = 0.21;
    break;

    case 6:
    jenisMakanan = "Daging";
    pengali = 0.02;
    break;

    case 7:
    jenisMakanan = "Ikan";
    pengali = 0;
    break;

    case 8:
    jenisMakanan = "Telur";
    pengali = 0; //0.0087
    break;

    case 9:
    jenisMakanan = "Susu";
    pengali = 0.13;
    break;
  }
}

int persenBerat(float beratan, int jenis) {
  jenis = jenis - 3;
  int persentase;
  switch(jenis) {
    case 1:
    persentase = persen(beratan, 7.87, 15.75, 23.62, 31.50, 39.38, 47.25);
    break;
    
    case 2:
    persentase = persen(beratan, 20.78, 41.56, 62.34, 83.12, 103.90, 124.69);
    break;

    case 3:
    persentase = persen(beratan, 60.38, 120.76, 181.14, 241.52, 301.90, 362.28);
    break;

    case 4:
    persentase = persen(beratan, 83.91, 167.82, 251.73, 335.64, 419.56, 503.47);
    break;

    case 5:
    persentase = persen(beratan, 25.64, 51.29, 76.93, 102.58, 128.23, 153.87);
    break;

    case 6:
    persentase = persen(beratan, 220.43, 440.87, 661.30, 881.74, 1102.18, 1322.61);
    break;

    case 7:
    persentase = 0;
    break;

    case 8:
    persentase = 0;
    break;

    case 9:
    persentase = persen(beratan, 39.79, 79.58, 119.37, 159.16, 198.96, 238.75);
    break;
  }
  return persentase;
}

int persen(float beratan, float satu, float dua, float tiga, float empat, float lima, float enam) {
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