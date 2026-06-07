#define SDA_PIN PC1
#define SCL_PIN PC2

// === MINIMALISTYCZNY STEROWNIK I2C (BIT-BANG) ===

void i2c_start() {
  digitalWrite(SDA_PIN, HIGH); digitalWrite(SCL_PIN, HIGH);
  digitalWrite(SDA_PIN, LOW);  digitalWrite(SCL_PIN, LOW);
}

void i2c_stop() {
  digitalWrite(SDA_PIN, LOW);  digitalWrite(SCL_PIN, LOW);
  digitalWrite(SCL_PIN, HIGH); digitalWrite(SDA_PIN, HIGH);
}

void i2c_write(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SDA_PIN, (data & 0x80) ? HIGH : LOW);
    data <<= 1;
    digitalWrite(SCL_PIN, HIGH); 
    digitalWrite(SCL_PIN, LOW);
  }
  digitalWrite(SDA_PIN, HIGH); 
  digitalWrite(SCL_PIN, HIGH); 
  digitalWrite(SCL_PIN, LOW);
}

// === OBSŁUGA EKRANU OLED SSD1306 ===

void oled_cmd(uint8_t cmd) {
  i2c_start();
  i2c_write(0x78); 
  i2c_write(0x00); 
  i2c_write(cmd);
  i2c_stop();
}

void oled_init() {
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);

  const uint8_t init_sequence[] = {
    0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
    0x8D, 0x14, 0x20, 0x02, 0xA1, 0xC8, 0xDA, 0x12, 
    0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF 
  };

  for(int i = 0; i < sizeof(init_sequence); i++) {
    oled_cmd(init_sequence[i]);
  }
}

void oled_fill(uint8_t pattern) {
  for (uint8_t page = 0; page < 8; page++) {
    oled_cmd(0xB0 + page); 
    oled_cmd(0x00);        
    oled_cmd(0x10);        
    
    i2c_start();
    i2c_write(0x78);
    i2c_write(0x40);       
    for (uint8_t i = 0; i < 128; i++) {
      i2c_write(pattern);
    }
    i2c_stop();
  }
}

void draw_bitmap(uint8_t x, uint8_t page, const uint8_t* bitmap, uint8_t width) {
  oled_cmd(0xB0 + page);            
  oled_cmd(x & 0x0F);               
  oled_cmd(0x10 | (x >> 4));        
  
  i2c_start();
  i2c_write(0x78);
  i2c_write(0x40); 
  for(uint8_t i = 0; i < width; i++) {
    i2c_write(bitmap[i]);
  }
  i2c_stop();
}

// Mapa bitowa dla słowa "Antek" (każda litera ma 5 pikseli szerokości + 1 piksel odstępu)
const uint8_t antek_bitmap[] = {
  0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, // A
  0x7C, 0x04, 0x04, 0x04, 0x78, 0x00, // n
  0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, // t
  0x38, 0x54, 0x54, 0x54, 0x18, 0x00, // e
  0x7F, 0x08, 0x14, 0x22, 0x41, 0x00  // k
};

// === GŁÓWNA STRUKTURA PROGRAMU ===

void setup() { 
  oled_init();      // Uruchom wyświetlacz
  oled_fill(0x00);  // Wyczyść ekran
  
  // Słowo ma 30 pikseli szerokości (5 liter * 6 bajtów). 
  // Aby wyśrodkować je na ekranie o szerokości 128px: (128 - 30) / 2 = 49.
  // Rysujemy na pozycji X = 49, strona (wysokość) = 3
  draw_bitmap(49, 3, antek_bitmap, sizeof(antek_bitmap));
} 

void loop() { 
  // Pusta pętla - obraz na wyświetlaczu OLED jest statyczny
}