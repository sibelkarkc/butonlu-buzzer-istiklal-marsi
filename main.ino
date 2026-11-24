const int buzzerPin = 14;
const int buttonPin = 13;

// Nota frekansları (Hz)
#define c   523
#define cd  554
#define d   587
#define dd  622
#define e   659
#define f   698
#define fd  740
#define g   784
#define gd  830
#define a   880
#define ad  932
#define b   988
#define c2  1046
#define cd2 1108
#define d2  1174
#define dd2 1244
#define e2  1318
#define f2  1396

// İstiklal Marşı notaları ve süreleri (ms)
const int notalar[] = {
  c, f, g, gd, e, g, f,
  f, ad, c2, cd2, a, c2, ad,
  c2, ad, c2, g, g, c2, ad, gd, g, f, e, f, g, c,
  c, c, g, f, e, d, e, f, g, gd, g, f, e,
  c, f, g, gd, e, g, f, d2, c2, ad, gd, g, f, e, f, g, c, f
};

const int sureler[] = {
  800, 800, 800, 800, 400, 200, 1600,
  800, 800, 800, 800, 400, 200, 1600,
  200, 200, 200, 400, 200, 800, 400, 200, 400, 200, 400, 200, 400, 800,
  400, 200, 800, 400, 200, 400, 200, 400, 200, 800, 400, 200, 1600,
  800, 800, 800, 800, 400, 200, 800, 400, 200, 400, 200, 400, 200, 400, 200, 800, 400, 1600
};

const int toplamNota = sizeof(notalar) / sizeof(notalar[0]);

bool caliyor = false;
int notaIndeksi = 0;
unsigned long notaBaslangic = 0;

bool sonButonDurumu = HIGH;
unsigned long sonBasmaZamani = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // GND bağlantısı için (3.3V kullanıyorsan INPUT_PULLDOWN yap)
}

void loop() {
  // 1. Buton kontrolü (basma anını yakala)
  bool butonDurumu = digitalRead(buttonPin);
  
  // 3.3V (INPUT_PULLDOWN) kullanıyorsan aşağıdaki LOW yerine HIGH, HIGH yerine LOW yaz
  if (sonButonDurumu == HIGH && butonDurumu == LOW && (millis() - sonBasmaZamani > 200)) {
    sonBasmaZamani = millis();
    caliyor = !caliyor; // Çalıyorsa durdur, duruyorsa başlat

    if (caliyor) {
      notaIndeksi = 0;
      tone(buzzerPin, notalar[0], sureler[0]);
      notaBaslangic = millis();
    } else {
      noTone(buzzerPin); // Sesi anında kes
    }
  }
  sonButonDurumu = butonDurumu;

  // 2. Melodi çalma kontrolü
  if (caliyor) {
    unsigned long beklemeSuresi = sureler[notaIndeksi] * 1.1;
    if (millis() - notaBaslangic >= beklemeSuresi) {
      notaIndeksi++;
      if (notaIndeksi >= toplamNota) {
        // Marş bitti, başa dönüp dur
        caliyor = false;
        noTone(buzzerPin);
      } else {
        tone(buzzerPin, notalar[notaIndeksi], sureler[notaIndeksi]);
        notaBaslangic = millis();
      }
    }
  }
}
