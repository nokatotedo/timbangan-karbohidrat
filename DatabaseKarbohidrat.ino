#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "makan-sehat-6dd48-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "QRUt7maP3odPCGkQhhUusKrWvMNX5z32K5A8wb4Q"
#define WIFI_SSID "PANDU"
#define WIFI_PASSWORD "peraiiii"

float kSerealia, kUmbi, kKacang, kSayur, kBuah, kDaging, kIkan, kTelur, kSusu;
float bSerealia, bUmbi, bKacang, bSayur, bBuah, bDaging, bIkan, bTelur, bSusu;
String hari;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected.");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  char jenis;
  float jenisHari;
  while(Serial.available()) {
    Serial.read();
  }
  while(Serial.available() == 0) {}
  jenis = Serial.read();
  
  if(jenis == 'a') {
    while(Serial.available()) {
      Serial.read();
    }
    while(Serial.available() == 0) {}
    jenisHari = Serial.parseFloat();

    String hariSekarang;
    if(jenisHari == 1.0) {
      hariSekarang = "Minggu";
    } else if(jenisHari == 2.0) {
      hariSekarang = "Senin";
    } else if(jenisHari == 3.0) {
      hariSekarang = "Selasa";
    } else if(jenisHari == 4.0) {
      hariSekarang = "Rabu";
    } else if(jenisHari == 5.0) {
      hariSekarang = "Kamis";
    } else if(jenisHari == 6.0) {
      hariSekarang = "Jum'at";
    } else if(jenisHari == 7.0) {
      hariSekarang = "Sabtu";
    }

    getHari();
    if(hariSekarang != hari) {
      Firebase.setFloat("makanan/m1/berat", 0); Firebase.setFloat("makanan/m1/karbohidrat", 0);
      Firebase.setFloat("makanan/m2/berat", 0); Firebase.setFloat("makanan/m2/karbohidrat", 0);
      Firebase.setFloat("makanan/m3/berat", 0); Firebase.setFloat("makanan/m3/karbohidrat", 0);
      Firebase.setFloat("makanan/m4/berat", 0); Firebase.setFloat("makanan/m4/karbohidrat", 0);
      Firebase.setFloat("makanan/m5/berat", 0); Firebase.setFloat("makanan/m5/karbohidrat", 0);
      Firebase.setFloat("makanan/m6/berat", 0); Firebase.setFloat("makanan/m6/karbohidrat", 0);
      Firebase.setFloat("makanan/m7/berat", 0); Firebase.setFloat("makanan/m7/karbohidrat", 0);
      Firebase.setFloat("makanan/m8/berat", 0); Firebase.setFloat("makanan/m8/karbohidrat", 0);
      Firebase.setFloat("makanan/m9/berat", 0); Firebase.setFloat("makanan/m9/karbohidrat", 0);
      Firebase.setString("waktu/hari", hariSekarang);
    }
  } else if(jenis == 'b') {
    for(int jumlah = 1; jumlah <= 3; jumlah++) {
      updateKarbohidrat();
    }
  } else if(jenis == 'c') {
    for(int jumlah = 1; jumlah <= 9; jumlah++) {
      getKarbohidrat(jumlah);
    }
    updateSaran();
  }
}

void updateKarbohidrat() {
  float jenis, karbohidrat, berat;

  while(Serial.available()) {
    Serial.read();
  }
  while(Serial.available() == 0) {}
  jenis = Serial.parseFloat();
  
  while(Serial.available()) {
    Serial.read();
  }
  while(Serial.available() == 0) {}
  karbohidrat = Serial.parseFloat();

  while(Serial.available()) {
    Serial.read();
  }
  while(Serial.available() == 0) {}
  berat = Serial.parseFloat();

  if(jenis == 1.0) {
    getKarbohidrat(1);
    getBerat(1);
    karbohidrat = karbohidrat + kSerealia;
    berat = berat + bSerealia;
    Firebase.setFloat("makanan/m1/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m1/berat", berat);
  } else if(jenis == 2.0) {
    getKarbohidrat(2);
    getBerat(2);
    karbohidrat = karbohidrat + kUmbi;
    berat = berat + bUmbi;
    Firebase.setFloat("makanan/m2/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m2/berat", berat);
  } else if(jenis == 3.0) {
    getKarbohidrat(3);
    getBerat(3);
    karbohidrat = karbohidrat + kKacang;
    berat = berat + bKacang;
    Firebase.setFloat("makanan/m3/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m3/berat", berat);
  } else if(jenis == 4.0) {
    getKarbohidrat(4);
    getBerat(4);
    karbohidrat = karbohidrat + kSayur;
    berat = berat + bSayur;
    Firebase.setFloat("makanan/m4/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m4/berat", berat);
  } else if(jenis == 5.0) {
    getKarbohidrat(5);
    getBerat(5);
    karbohidrat = karbohidrat + kBuah;
    berat = berat + bBuah;
    Firebase.setFloat("makanan/m5/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m5/berat", berat);
  } else if(jenis == 6.0) {
    getKarbohidrat(6);
    getBerat(6);
    karbohidrat = karbohidrat + kDaging;
    berat = berat + bDaging;
    Firebase.setFloat("makanan/m6/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m6/berat", berat);
  } else if(jenis == 7.0) {
    getKarbohidrat(7);
    getBerat(7);
    karbohidrat = karbohidrat + kIkan;
    berat = berat + bIkan;
    Firebase.setFloat("makanan/m7/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m7/berat", berat);
  } else if(jenis == 8.0) {
    getKarbohidrat(8);
    getBerat(8);
    karbohidrat = karbohidrat + kTelur;
    berat = berat + bTelur;
    Firebase.setFloat("makanan/m8/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m8/berat", berat);
  } else if(jenis == 9.0) {
    getKarbohidrat(9);
    getBerat(9);
    karbohidrat = karbohidrat + kSusu;
    berat = berat + bSusu;
    Firebase.setFloat("makanan/m9/karbohidrat", karbohidrat);
    Firebase.setFloat("makanan/m9/berat", berat);
  }
}

void updateSaran() {
  float karbohidrat;
  karbohidrat = kSerealia + kUmbi + kKacang + kSayur + kBuah + kDaging + kIkan + kTelur + kSusu;
  if(karbohidrat > 266.66) {
    Serial.print("Ikan");
  } else if(karbohidrat > 233.33) {
    Serial.print("Telur");
  } else if(karbohidrat > 200) {
    Serial.print("Daging");
  } else if(karbohidrat > 166.66) {
    Serial.print("Sayur");
  } else if(karbohidrat > 133.33) {
    Serial.print("Kacang");
  } else if(karbohidrat > 100) {
    Serial.print("Susu");
  } else if(karbohidrat > 66.66) {
    Serial.print("Buah");
  } else if(karbohidrat > 33.33) {
    Serial.print("Umbi");
  } else {
    Serial.print("Serealia");
  }
}

void getKarbohidrat(int jenis) {
  switch(jenis) {
    case 1:
    kSerealia = Firebase.getFloat("makanan/m1/karbohidrat");
    break;

    case 2:
    kUmbi = Firebase.getFloat("makanan/m2/karbohidrat");
    break;

    case 3:
    kKacang = Firebase.getFloat("makanan/m3/karbohidrat");
    break;

    case 4:
    kSayur = Firebase.getFloat("makanan/m4/karbohidrat");
    break;

    case 5:
    kBuah = Firebase.getFloat("makanan/m5/karbohidrat");
    break;

    case 6:
    kDaging = Firebase.getFloat("makanan/m6/karbohidrat");
    break;

    case 7:
    kIkan = Firebase.getFloat("makanan/m7/karbohidrat");
    break;

    case 8:
    kTelur = Firebase.getFloat("makanan/m8/karbohidrat");
    break;

    case 9:
    kSusu = Firebase.getFloat("makanan/m9/karbohidrat");
    break;
  }
}

void getBerat(int jenis) {
  switch(jenis) {
    case 1:
    bSerealia = Firebase.getFloat("makanan/m1/berat");
    break;

    case 2:
    bUmbi = Firebase.getFloat("makanan/m2/berat");
    break;

    case 3:
    bKacang = Firebase.getFloat("makanan/m3/berat");
    break;

    case 4:
    bSayur = Firebase.getFloat("makanan/m4/berat");
    break;

    case 5:
    bBuah = Firebase.getFloat("makanan/m5/berat");
    break;

    case 6:
    bDaging = Firebase.getFloat("makanan/m6/berat");
    break;

    case 7:
    bIkan = Firebase.getFloat("makanan/m7/berat");
    break;

    case 8:
    bTelur = Firebase.getFloat("makanan/m8/berat");
    break;

    case 9:
    bSusu = Firebase.getFloat("makanan/m9/berat");
    break;
  }
}

void getHari() {
  hari = Firebase.getString("waktu/hari");
}