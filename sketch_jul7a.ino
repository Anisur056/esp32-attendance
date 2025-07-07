#include <rdm6300.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Initialize the TFT display

#define RDM6300_RX_PIN 5
#define READ_LED_PIN 2

/* Set Wifi info */
const char *ssid = "ANIS-WIFI";
const char *password = "12345678";

/* Set SERVER info */
String URL = "https://azadfamily.org/sirikotia/api/getdata.php";
String getData, Link;
Rdm6300 rdm6300;

void setup(){
  //Serial.begin(115200);
  pinMode(READ_LED_PIN, OUTPUT);
  rdm6300.begin(RDM6300_RX_PIN);

  tft.begin(); // Initialize the TFT display
  tft.setRotation(0); // Set the display rotation

  tft.setCursor(10, 20);
  tft.setTextSize(2); // Set the text size to 1
  tft.fillScreen(TFT_BLACK); // Clear the screen
  tft.setTextColor(TFT_WHITE); // Set the text color to white
  tft.setTextWrap(true, false);

  tft.print("Welcome to Sirikotia Madrasha");
  tft.print("\n");
  // tft.print("Place Your Card....");
  // tft.print("\n");
  delay(1000);
}

void loop(){


  //check if there's a connection to Wi-Fi or not
  if(!WiFi.isConnected()){
    connectToWiFi();    //Retry to connect to Wi-Fi
  }

  /* get_new_tag_id returns the tag_id of a "new" near tag,
  following calls will return 0 as long as the same tag is kept near. */
  if (rdm6300.get_new_tag_id()){
    String CardID ="";

    CardID += rdm6300.get_tag_id();
    //Serial.println(CardID);

    tft.fillScreen(TFT_BLACK); // Clear the screen
    tft.setCursor(10, 20);
    tft.print(CardID);
    tft.print("\n");
    tft.print("\n");

    SendCardID(CardID);
    digitalWrite(READ_LED_PIN, HIGH);
  }
 
  //delay(1000);
  digitalWrite(READ_LED_PIN, LOW);
}

//************send the Card UID to the website*************
void SendCardID( String Card_uid ){
  //Serial.println("Sending the Card ID");

 
  tft.print("Sending the Card ID");
  tft.print("\n");
  tft.print("\n");
 
  HTTPClient http;    //Declare object of class HTTPClient
  //GET Data
  getData = "?id=" + String(Card_uid) ; // Add the Card ID to the GET array in order to send it
  //GET methode
  Link = URL + getData;

  http.begin(Link); //initiate HTTP request   //Specify content-type header
 
  int httpCode = http.GET();   //Send the request
  String payload = http.getString();    //Get the response payload



  //Serial.println(Link);   //Print HTTP return code

  tft.print("\n");
  tft.print(payload);

  // if(httpCode){
  //   tft.print("Card Uploded Successfully.");
  //   tft.print("\n");
  // }else{
  //   tft.print("\n");
  //   tft.print("Upload Failed..");
  // }

  delay(500);

  tft.fillScreen(TFT_BLACK); // Clear the screen

  tft.setCursor(10, 20);
  tft.print("Place Your ID Card");
  http.end();  //Close connection


}

//********************connect to the WiFi******************
void connectToWiFi(){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
   
    tft.fillScreen(TFT_BLACK); // Clear the screen
    tft.setCursor(10, 20);
    tft.print("Connecting to ....");
    tft.print("\n");
    tft.print(ssid);
    tft.print("\n");

    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      tft.print(".");
    }

    tft.fillScreen(TFT_BLACK); // Clear the screen
    tft.setCursor(10, 20);

    tft.print("Connected");

    tft.print("\n");
    tft.print("IP address:");
    tft.print("\n");

    tft.print(WiFi.localIP());  //IP address assigned to your ESP

    delay(5000);

    tft.fillScreen(TFT_BLACK); // Clear the screen
    tft.setCursor(10, 20);
    tft.print("Place Your ID Card");
}
