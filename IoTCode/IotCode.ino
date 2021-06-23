#include <MQ7.h>
#include <MQ2.h>
#include <SoftwareSerial.h>                               //SoftwareSerial kütüphanimizi ekliyoruz.
#include <DHT.h> 



String agAdi = "patates";                 //Ağımızın adını buraya yazıyoruz.    
String agSifresi = "gizem1912325990348";           //Ağımızın şifresini buraya yazıyoruz.



int rxPin = 6;                                               //ESP8266 RX pini
int txPin = 5;                                               //ESP8266 TX pini
int pin = 13;
int lpg, co, smoke;



MQ2 mq2(pin);

MQ7 mq7(A1,5.0);

#define DHTPIN 9



String ip = "184.106.153.149";  




#define DHTTYPE DHT11
DHT ibrahim(DHTPIN,DHTTYPE);

SoftwareSerial esp(rxPin, txPin);                             //Seri haberleşme pin ayarlarını yapıyoruz.

void setup() {  
  mq2.begin();
  Serial.begin(9600);  //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Started");
  esp.begin(115200);                                          //ESP8266 ile seri haberleşmeyi başlatıyoruz.
  esp.println("AT");                                          //AT komutu ile modül kontrolünü yapıyoruz.
  Serial.println("AT Yollandı");
  while(!esp.find("OK")){                                     //Modül hazır olana kadar bekliyoruz.
  esp.println("AT");
  Serial.println("ESP8266 Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                                 //ESP8266 modülünü client olarak ayarlıyoruz.
  while(!esp.find("OK")){                                     //Ayar yapılana kadar bekliyoruz.
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    //Ağımıza bağlanıyoruz.
  while(!esp.find("OK"));                                     //Ağa bağlanana kadar bekliyoruz.
  Serial.println("Aga Baglandi.");
  delay(1000);
}
void loop() {
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //Thingspeak'e bağlanıyoruz.
  if(esp.find("Error")){                                      //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("AT+CIPSTART Error");
  }
 Serial.println(mq7.getPPM());
 float sicaklik = ibrahim.readHumidity();
 float nem = ibrahim.readTemperature();
 float* values= mq2.read(true);
 lpg = mq2.readLPG();
 co = mq2.readCO();
 smoke =mq2.readSmoke();
 
 
  String veri = "GET https://api.thingspeak.com/update?api_key=HUKK931IV9OA57GG";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.                                   //Göndereceğimiz sıcaklık değişkeni
  veri += "&field1=";
  veri += String(nem);
  veri += "&field2=";
  veri += String(sicaklik);                                        //Göndereceğimiz nem değişkeni
  veri += "&field3=";
  veri += String(lpg);
  veri += "&field4=";
  veri += String(co);
  veri += "&field5=";
  veri += String(smoke);


  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
  esp.println(veri.length()+2);
  delay(200);
  if(esp.find(">")){                                          //ESP8266 hazır olduğunda içindeki komutlar çalışıyor.
    esp.print(veri);                                          //Veriyi gönderiyoruz.
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(100);
    Serial.print("Nem: ");
Serial.print(sicaklik);
Serial.println(" %");

Serial.print("Temperature: ");
Serial.print(nem);
Serial.println(" *C");

  }
  Serial.println("Baglantı Kapatildi.");
  esp.println("AT+CIPCLOSE");                                //Bağlantıyı kapatıyoruz
  delay(500);                                               //Yeni veri gönderimi için 1 dakika bekliyoruz.
}
