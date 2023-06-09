#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo_ESP32.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> 
#endif

// SCK 18, MISO 19, MOSI 23
#define RST_PIN   22         
#define SS_PIN   5 //chân SDA

#define servoPin_1  12 // UP_DOWN_SERVO
#define servoPin_2  13 // LEFT_RIGHT_SERVO
#define servoPin_3  14 // GARA
#define servoPin_4  17 // DOOR


#define LED_PIN_2   21     //LED NEO TRON
#define NUMPIXELS_2   12 

#define cambien  15
int Giatri;
unsigned long quakhu;
unsigned long quakhu_1;
unsigned long quakhu_2;

Servo_ESP32 servo1;
Servo_ESP32 servo2;
Servo_ESP32 servo3;
Servo_ESP32 servo4;


Adafruit_NeoPixel pixels_2(NUMPIXELS_2, LED_PIN_2, NEO_GRB + NEO_KHZ800);

MFRC522 mfrc522(SS_PIN, RST_PIN); 

int UID[4],i;
int ID1[4] = {27, 123, 25, 55};
int ID2[4] = {181, 180, 136, 172};

const char* ssid = "dhtdt_l0703";
const char* password = "nn7c1b14"; 
const char* mqtt_server = "10.41.7.90";
const unsigned int mqtt_port = 1883;

long now = millis();
long lastMeasure = 0;

void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Đang Kết Nối Tới ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiClient wifi;
PubSubClient client(mqtt_server,mqtt_port,callback,wifi);

void callback(char* topic, byte* payload, unsigned int length)
{
   String string;
   Serial.print("Message from [");
   Serial.print("]:");
   Serial.print(topic);
   Serial.print("] ");

   if(strstr(topic,"servo_1"))
   {  
      for (int i = 0; i < length; i++) 
      {
        string+=((char)payload[i]);
      }
      Serial.print(" ");
      int resultado_1 = string.toInt();   
      int pos_1 = map(resultado_1, 1, 100, 0, 180);
      Serial.println(pos_1);
      servo1.write(pos_1);
      delay(15);
    }
  
   else if(strstr(topic,"servo_2"))
   {
      for (int i = 0; i < length; i++) 
      {
        string+=((char)payload[i]);
      }
     Serial.print(" ");
     int resultado_2 = string.toInt();   
     int pos_2 = map(resultado_2, 1, 100, 0, 180);
     Serial.println(pos_2);
     servo2.write(pos_2);
     delay(15);
   }

   else if(strstr(topic,"servo_3"))  
   {
      for (int i = 0; i < length; i++) 
      {
        string+=((char)payload[i]);
      }
       Serial.print(" ");
       int resultado_3 = string.toInt();   
       int pos_3 = map(resultado_3, 1, 100, 0, 180);
       Serial.println(pos_3);
       servo3.write(pos_3);
       delay(15);
   }

   else if(strstr(topic,"servo_4"))   
   {
      for (int i = 0; i < length; i++) 
      {
        string+=((char)payload[i]);
      }
      Serial.print(" ");
      int resultado_4 = string.toInt();   
      int pos_4 = map(resultado_4, 1, 100, 0, 180);
      Serial.println(pos_4);
      servo4.write(pos_4);
      delay(15);
   }

 
  
   else if(strstr(topic,"Light_2"))
   {
      for (int i = 0; i < length; i++) 
      {
        Serial.print((char)payload[i]);
      } 
      Serial.println();
      
      if ((char)payload[0] == '1') 
      {  
        for(int i=0; i<NUMPIXELS_2; i++) 
        { 
           pixels_2.setPixelColor(i, pixels_2.Color(255, 255, 255));
           pixels_2.show();   
        } 
      }
       
      else 
      {
        for(int i=12; i>=0; i--) 
        { 
          pixels_2.setPixelColor(i, pixels_2.Color(0, 0, 0));
          pixels_2.show();   
        }
       
      }        
    }
} 
void setup() 
{
  Serial.begin(115200);

  SPI.begin();    
  mfrc522.PCD_Init();
  
  servo1.attach(servoPin_1);
  servo2.attach(servoPin_2);
  servo3.attach(servoPin_3);
  servo4.attach(servoPin_4);

  pinMode(cambien,INPUT);
  
  pixels_2.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  

}

void reconnect()
{
  while (!client.connected()) 
  {
    Serial.print(" Đang Kết Nối MQTT...");
 
    if (client.connect("ESP32Client")) 
    {
       Serial.println("Kết Nối Thành Công");
       Serial.println("MQTT Đã Kết Nối");
       client.subscribe("servo_1");
       client.subscribe("servo_2");
       client.subscribe("servo_3");
       client.subscribe("servo_4");
       client.subscribe("Light_2");
       delay(3000);
    }
    else
    {
       Serial.print("Kết Nối MQTT Thất Bại ");
       Serial.println("Thử Lại Trong 5 Giây");
       delay(5000);
     
    }
  }
}


void loop() 
{
   Giatri = digitalRead(cambien);
   if(Giatri==0)
   {
      unsigned long hientai = millis();
      if(hientai - quakhu = 5000)
      {
        quakhu=hientai;
        servo3.write(0);
      }
      servo3.write(67);
   }
   
   if (!client.connected())
   {
    reconnect();
   }
   if(!client.loop())
      client.connect("ESP32Client");
   
   now = millis();
   if (now - lastMeasure >=  1000)
   {
     lastMeasure = now;
     if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
          return;
      }
    
     if ( ! mfrc522.PICC_ReadCardSerial()) 
     {
            return;
     }
      
     for (byte i = 0; i <mfrc522.uid.size; i++)
     {
           Serial.print(mfrc522.uid.uidByte[i]< 0x10 ? " 0" : " ");
           UID[i] = mfrc522.uid.uidByte[i];
           Serial.print(UID[i]);
     }
     Serial.println();
          
     if (UID[i] == ID1[i]  )
     {
        client.publish("RFID", "27_123_25_55");
        Serial.println(" Cho phép truy cập ");
        Serial.println(); 
        unsigned long hientai = millis();
        if(hientai - quakhu_1 = 3000)
        {
          quakhu_1=hientai;
          servo3.write(90);
        } 
           servo4.write(0);
     }
     else if (UID[i] == ID2[i]  )
     {
          client.publish("RFID", "181_180_136_172");
          Serial.println(" Cho phép truy cập ");
          Serial.println();
          unsigned long hientai = millis();
          if(hientai - quakhu_2 = 3000)
          {
            quakhu_2=hientai;
            servo3.write(90);
          } 
             servo4.write(0); 
     }
     else   
     {
          Serial.println("Từ chối truy cập ");
          Serial.println();
          client.publish("RFID", "ID lạ");
     }
     mfrc522.PICC_HaltA();
     mfrc522.PCD_StopCrypto1();
     }

}
