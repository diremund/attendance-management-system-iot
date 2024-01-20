#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>

#define GREEN_LED D0
#define RED_LED D1

const char *ssid = "indigo";
const char *password = "11112222";
const char *apiUrl = "http://192.168.43.210:9090/api/check-uid";

#define SS_PIN 4 // sda
#define RST_PIN 2 //also set D5 -> SCK | D7 -> MOSI | D6 -> MISO

MFRC522 mfrc522(RST_PIN, SS_PIN);

int readsuccess = 0;
byte readcard[4];
char str[32] = "";
String StrUID;

// Function declarations
int getid();
void array_to_string(byte array[], unsigned int len, char buffer[]);
void setColor (int redValue, int greenValue, int blueValue);

void setup()
{
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  pinMode(GREEN_LED, OUTPUT);
   pinMode(RED_LED, OUTPUT);
  mfrc522.PCD_Init(); // Init MFRC522
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop()
{

  do
  {
    readsuccess = getid(); // Check if a card has been read successfully
  } while (!readsuccess);


  HTTPClient http; // Declare object of class HTTPClient

  // Create a WiFiClient object
  WiFiClient client;

  String UIDresultSend, postData;
  UIDresultSend = StrUID;

  // Post Data
  postData = UIDresultSend;

  http.begin(client, "http://192.168.43.210:9090/api/check-uid"); // Specify request destination
  http.addHeader("Content-Type", "application/json");             // Specify content-type header

  int httpCode = http.POST(postData); // Send the request
  String payload = http.getString();  // Get the response payload

  Serial.println(UIDresultSend);
  Serial.println(httpCode); // Print HTTP return code
  Serial.println(payload);  // Print request response payload

  http.end(); // Close connection

 setColor(0,255,0);
  delay(500);
 setColor(0,0,0);
}

int getid()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return 0;
  }

  Serial.print("THE UID OF THE SCANNED CARD IS : ");

  for (int i = 0; i < 4; i++)
  {
    readcard[i] = mfrc522.uid.uidByte[i]; // storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}

void setColor (int redValue, int greenValue, int blueValue){
analogWrite(RED_LED, redValue);
analogWrite(GREEN_LED, greenValue);
//analoqWrite(bluePin, blueValue);
}