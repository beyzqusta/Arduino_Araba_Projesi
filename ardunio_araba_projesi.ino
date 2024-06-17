#define echoPinSag 12 //Ultrasonik sensörün echo pini Arduino'nun 12.pinine
#define trigPinSag 13
#define echoPinSol 7 //Ultrasonik sensörün echo pini Arduino'nun 12.pinine
#define trigPinSol 6
#define echoPinOrta 8 //Ultrasonik sensörün echo pini Arduino'nun 12.pinine
#define trigPinOrta 11
#define MotorR1 5 //sağ geri
#define MotorR2 4 //sağ ileri
#define MotorRE 10  // Motor pinlerini tanımlıyoruz.
#define MotorL1 3  //sol ileri
#define MotorL2 2  //sol geri
#define MotorLE 9
int LDRPin = A0;
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(0, 1);
long sureSag, uzaklikSag, sureSol, uzaklikSol, sureOrta, uzaklikOrta ; //süre ve uzaklık diye iki değişken tanımlıyoruz.
int command;
int isik;
int threshold = 250;     // Tünelin karanlık seviyesini belirlemek için eşik değeri
int tunnelCount = 0;    // Tünel sayacını tutan değişken
bool inTunnel = false;
void setup() {
  // ultrasonik sensör Trig pininden ses dalgaları gönderdiği için OUTPUT (Çıkış),
  // bu dalgaları Echo pini ile geri aldığı için INPUT (Giriş) olarak tanımlanır.

  pinMode(echoPinSag, INPUT);
  pinMode(trigPinSag, OUTPUT);
  pinMode(echoPinSol, INPUT);
  pinMode(trigPinSol, OUTPUT);
  pinMode(echoPinOrta, INPUT);
  pinMode(trigPinOrta, OUTPUT);

  pinMode(MotorL1, OUTPUT);
  pinMode(MotorL2, OUTPUT);
  pinMode(MotorLE, OUTPUT); //Motorlarımızı çıkış olarak tanımlıyoruz.
  pinMode(MotorR1, OUTPUT);
  pinMode(MotorR2, OUTPUT);
  pinMode(MotorRE, OUTPUT);
  Serial.begin(9600);
}
void loop() {

  if (Serial.available() > 0) {
  command = Serial.read();
 }

  switch (command) {
    case '1':
      {
        while (command=='1') {
          digitalWrite(trigPinSag, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinSag, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPinSag, LOW);
  sureSag = pulseIn(echoPinSag, HIGH); //ses dalgasının geri dönmesi için geçen sure ölçülüyor
  uzaklikSag = sureSag / 29.1 / 2; //ölçülen süre uzaklığa çevriliyor
  delayMicroseconds(5);
  digitalWrite(trigPinSol, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinSol, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPinSol, LOW);
  sureSol = pulseIn(echoPinSol, HIGH);
  uzaklikSol = sureSol / 29.1 / 2;
  delayMicroseconds(5);
  digitalWrite(trigPinOrta, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinOrta, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPinOrta, LOW);
  sureOrta = pulseIn(echoPinOrta, HIGH);
  uzaklikOrta = sureOrta / 29.1 / 2;

  Serial.print("sağ: ");
  Serial.println(uzaklikSag);
  Serial.print("sol: ");
  Serial.println(uzaklikSol);
  Serial.print("Orta: ");
  Serial.println(uzaklikOrta);
  
  
  
  if (uzaklikOrta > 30) {
    if(uzaklikSag>8 && uzaklikSag<150 && uzaklikSol > 8 && uzaklikSol < 150) {
      ileri();
    }
    else if(uzaklikSol<=6 && uzaklikSag>5) {
      dur();  
      delay(150);
      geri();
      delay(150);
      dur();
      delay(150);
      sag();
      delay(100);
    } 
    else if(uzaklikSol>5 && uzaklikSag<=6) {
      dur();
      delay(150);
      geri();
      delay(150);
      dur();
      delay(150);
      sol();
      delay(100);
    }
    

  }
  else{
    if(uzaklikOrta <=30 && uzaklikSag <= 30 && uzaklikSol <= 30){
      dur();
      delay(1000);
    }
      

    dur();
    delay(500);
    geri();
    delay(200);
    dur();
    delay(200);

    if(uzaklikSag > uzaklikSol){
      sag();
      delay(530);
    }else{
      sol();
      delay(530);
    }

  }
  /*if (isik < threshold) {
    if (!inTunnel) {
      inTunnel = true;
      tunnelCount++;
      if (tunnelCount == 1) {
        dur();
        delay(5000);
      }
      Serial.print("Arac tünele girdi. Tünel Sayisi: ");
      Serial.println(tunnelCount);
    }
  } else {
    inTunnel = false;
    Serial.print("Arac tünele girmedi . Tünel Sayisi: ");
    Serial.println(tunnelCount);
  }
  delay(100);
  */

int isik_degeri = analogRead(LDRPin);
Serial.println(isik_degeri);
delay(100);  
 if(isik_degeri<200)
 {
    tunnelCount++;
    if(tunnelCount==1) {
      dur();
      delay(5000);
      ileri(); 
    }
    else {
      ileri();
    }
    
 }
        
          
}}
  }

}
void dur() {

  digitalWrite(MotorR1, 0); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, 0); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 0); // Sağ motorun hızı 150
  digitalWrite(MotorL1, 0); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, 0); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 0); // Sol motorun hızı 150

}

void ileri(){  // Robotun ileri yönde hareketi için fonksiyon tanımlıyoruz.
  digitalWrite(MotorR1, 0); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, 1); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 90); // Sağ motorun hızı 150
  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 90); // Sol motorun hızı 150
  
  
}
void sag(){ // Robotun sağa dönme hareketi için fonksiyon tanımlıyoruz.
  digitalWrite(MotorR1, 1); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, 0); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 75); // Sağ motorun hızı 0 (Motor duruyor)
  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 75); // Sol motorun hızı 150
  
  
}

void sol(){ // Robotun sağa dönme hareketi için fonksiyon tanımlıyoruz.
  digitalWrite(MotorR1, 0); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, 1); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 75); // Sağ motorun hızı 0 (Motor duruyor)
  digitalWrite(MotorL1, 0); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, 1); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 75); // Sol motorun hızı 150

  
}
void geri(){ // Robotun geri yönde hareketi için fonksiyon tanımlıyoruz.
  digitalWrite(MotorR1, 1); // Sağ motorun ileri hareketi pasif
  digitalWrite(MotorR2, 0); // Sağ motorun geri hareketi aktif
  analogWrite(MotorRE, 75); // Sağ motorun hızı 150
  digitalWrite(MotorL1, LOW); // Sol motorun ileri hareketi pasif
  digitalWrite(MotorL2, HIGH); // Sol motorun geri hareketi aktif
  analogWrite(MotorLE, 75); // Sol motorun hızı 150

}

onst int motorA1  = 5;  // L298N'in IN3 Girişi
  const int motorA2  = 6;  // L298N'in IN1 Girişi
  const int motorB1  = 10; // L298N'in IN2 Girişi
  const int motorB2  = 9;  // L298N'in IN4 Girişi


  int i=0; //Döngüler için atanan rastgele bir değişken
  int j=0; //Döngüler için atanan rastgele bir değişken
  int state; //Bluetooth cihazından gelecek sinyalin değişkeni
  int vSpeed=255;     // Standart Hız, 0-255 arası bir değer alabilir

void setup() {
    // Pinlerimizi belirleyelim
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);    
    // 9600 baud hızında bir seri port açalım
    Serial.begin(9600);
}
 
void loop() {
  /*Bluetooth bağlantısı koptuğunda veya kesildiğinde arabayı durdur.
 (Aktif etmek için alt satırın "//" larını kaldırın.)*/
//     if(digitalRead(BTState)==LOW) { state='S'; }

  //Gelen veriyi 'state' değişkenine kaydet
    if(Serial.available() > 0){     
      state = Serial.read();   
    }
  
  /* Uygulamadan ayarlanabilen 4 hız seviyesi.(Değerler 0-255 arasında olmalı)*/
    if (state == '0'){
      vSpeed=0;}
    else if (state == '1'){
      vSpeed=100;}
    else if (state == '2'){
      vSpeed=180;}
    else if (state == '3'){
      vSpeed=200;}
    else if (state == '4'){
      vSpeed=255;}
     
  /********İleri*********/
  //Gelen veri 'F' ise araba ileri gider.
    if (state == 'F') {
      analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0);
        analogWrite(motorB1, vSpeed);      analogWrite(motorB2, 0); 
    }
  /*******İleri Sol*********/
  //Gelen veri 'G' ise araba ileri sol(çapraz) gider.
    else if (state == 'G') {
      analogWrite(motorA1,vSpeed ); analogWrite(motorA2, 0);  
        analogWrite(motorB1, 100);    analogWrite(motorB2, 0); 
    }
  /*******İleri Sağ*********/
  //Gelen veri 'I' ise araba ileri sağ(çapraz) gider.
    else if (state == 'I') {
        analogWrite(motorA1, 100); analogWrite(motorA2, 0); 
        analogWrite(motorB1, vSpeed);      analogWrite(motorB2, 0); 
    }
  /********Geri*********/
  //Gelen veri 'B' ise araba geri gider.
    else if (state == 'B') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, vSpeed); 
    }
  /*******Geri Sol*********/
  //Gelen veri 'H' ise araba geri sol(çapraz) gider
    else if (state == 'H') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, 100); 
        analogWrite(motorB1, 0); analogWrite(motorB2, vSpeed); 
    }
  /*******Geri Sağ*********/
  //Gelen veri 'J' ise araba geri sağ(çapraz) gider
    else if (state == 'J') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, 100); 
    }
  /**********Sol**********/
  //Gelen veri 'L' ise araba sola gider.
    else if (state == 'L') {
      analogWrite(motorA1, vSpeed);   analogWrite(motorA2, 150); 
        analogWrite(motorB1, 0); analogWrite(motorB2, 0); 
    }
  /**********Sağ**********/
  //Gelen veri 'R' ise araba sağa gider
    else if (state == 'R') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, 0); 
        analogWrite(motorB1, vSpeed);   analogWrite(motorB2, 150);     
    }
  
  /*********Stop**********/
  //Gelen veri 'S' ise arabayı durdur.
    else if (state == 'S'){
        analogWrite(motorA1, 0);  analogWrite(motorA2, 0); 
        analogWrite(motorB1, 0);  analogWrite(motorB2, 0);
    }  
}