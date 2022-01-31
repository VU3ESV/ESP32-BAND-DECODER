#include <WiFi.h>

const char* ssid = "<SSID HERE>";        // Wifi ssid
const char* pass = "PASSWORD Here";              // Wifi password

const IPAddress serverIP(192, 168, 1, 123); // TCP Server IP
uint16_t serverPort = 7355;                 // Port

WiFiClient client;

String frequencyData;    //read data kenwood string
long freq = 0;
int BAND;
bool beginRead = false;
int arrayIndex = 0;

//IO port definition for bands, the numbers are based on the ESP32 pin definition
#define LPF_160    16
#define LPF_80A    17
#define LPF_80B    18
#define LPF_40_30  19
#define LPF_20     25
#define LPF_17_30  26
#define PTTBypass  27
#define DummyOut1  32
#define DummyOut2  33

void SetBand(long frequency);
void SetOutput(int bandCode);
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wifi Connected");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.println("Connecting to TCP Socket...");
  while (!client.connected())
  {
    client.connect(serverIP, serverPort);
    delay(1000);
  }
  Serial.println("TCP Socket Connected");
  
  pinMode(LPF_160, OUTPUT);
  pinMode(LPF_80A, OUTPUT);
  pinMode(LPF_80B, OUTPUT);
  pinMode(LPF_40_30, OUTPUT);
  pinMode(LPF_20, OUTPUT);
  pinMode(LPF_17_30, OUTPUT);
  pinMode(PTTBypass, OUTPUT);
  pinMode(DummyOut1, OUTPUT);
  pinMode(DummyOut1, OUTPUT);

  digitalWrite(LPF_160, HIGH);
  digitalWrite(LPF_80A, HIGH);
  digitalWrite(LPF_80B, HIGH);
  digitalWrite(LPF_40_30, HIGH);
  digitalWrite(LPF_20, HIGH);
  digitalWrite(LPF_17_30, HIGH);
  digitalWrite(PTTBypass, HIGH);
  digitalWrite(DummyOut1, HIGH);
  digitalWrite(DummyOut1, HIGH);
}

void loop()
{
  while (!client.connected())
  {
    client.connect(serverIP, serverPort);
    delay(1000);
    Serial.println("TCP Socket Reconnecting");
  }

  while (client.available()) {
    String readBuffer = client.readStringUntil(';');
    //Serial.println(readBuffer);
    if (readBuffer.startsWith("IF"))
    {
      frequencyData = readBuffer.substring(2, 13);
      //Serial.println(frequencyData);
      freq = frequencyData.toInt();
      SetBand(freq);    

      Serial.println(freq);
      Serial.println(BAND);
      frequencyData = "";
    }
  }

  if (client.connected()) {
    client.print("IF;");
    delay(300);
  }
}

void SetBand(long frequency)
{
  if (frequency >=   1810000 && frequency <=   2000000 )
  {
    BAND = 160; // 160m
  }

  else if (frequency >=   3500000 && frequency <=   3800000 )
  {
    BAND = 80; //  80m
  }
  else if (frequency >=   7000000 && frequency <=   7200000 )
  {
    BAND = 40; //  40m
  }
  else if (frequency >=  10100000 && frequency <=  10150000 )
  {
    BAND = 30; //  30m
  }
  else if (frequency >=  14000000 && frequency <=  14350000 )
  {
    BAND = 20; //  20m
  }
  else if (frequency >=  18068000 && frequency <=  18168000 )
  {
    BAND = 17; //  17m
  }
  else if (frequency >=  21000000 && frequency <=  21450000 )
  {
    BAND = 15; //  15m
  }
  else if (frequency >=  24890000 && frequency <=  24990000 )
  {
    BAND = 12; //  12m
  }
  else if (frequency >=  28000000 && frequency <=  29700000 )
  {
    BAND = 10; //  10m
  }
  else if (frequency >=  50000000 && frequency <=  52000000 )
  {
    BAND = 6; //   6m
  }
  else if (frequency >= 144000000 && frequency <= 146000000 )
  {
    BAND = 2; //   2m
  }
  else
  {
    BAND = 0; // out of range
  }
  SetOutput(BAND);
}

void SetOutput(int bandCode)
{
  // 160m band
  if (bandCode == 160)
  {
    digitalWrite(LPF_160, LOW);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);
        
    digitalWrite(PTTBypass, HIGH);
  }
  // 80m band
  if (bandCode == 80)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, LOW);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);    

    digitalWrite(PTTBypass, HIGH);
  }
  // 40m band
  if (bandCode == 40)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, LOW);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);    

    digitalWrite(PTTBypass, LOW);
  }
  // 30m band
  if (bandCode == 30)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, LOW);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);    

    digitalWrite(PTTBypass, HIGH);
  }
  // 20m band
  if (bandCode == 20)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, LOW);
    digitalWrite(LPF_17_30, HIGH);    

    digitalWrite(PTTBypass, LOW);
  }
  // 17m band
  if (bandCode == 17)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, LOW);    

    digitalWrite(PTTBypass, LOW);
  }
  // 15m band
  if (bandCode == 15)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, LOW);    

    digitalWrite(PTTBypass, LOW);
  }
  // 12m band
  if (bandCode == 12)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, LOW);
    
    digitalWrite(PTTBypass, LOW);
  }
  // 10m band
  if (bandCode == 10)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, LOW);

    digitalWrite(PTTBypass, LOW);
  }
  //6m band
  if (bandCode == 6)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);
    
    digitalWrite(PTTBypass, HIGH);
  }
  if (bandCode == 2)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);    

    digitalWrite(PTTBypass, HIGH);
  }
  if (bandCode == 70)
  {
    digitalWrite(LPF_160, HIGH);
    digitalWrite(LPF_80A, HIGH);
    digitalWrite(LPF_80B, HIGH);
    digitalWrite(LPF_40_30, HIGH);
    digitalWrite(LPF_20, HIGH);
    digitalWrite(LPF_17_30, HIGH);

    digitalWrite(PTTBypass, HIGH);
  }
}
