#include <WiFi.h>
#include <WebServer.h>

//The WiFi identifier + LED configuration
const char* ssid = "Your WiFi name";
const char* password = "Your password";
const int LED_PIN = 18;

//Creating the web server
WebServer server(80);
bool ledState = false;
String page = R"rawliteral(
  <!DOCTYPE html>
  <html>

  <head>
      <meta charset="UTF-8">
      <meta name ="viewport" content="width=device-width,initial-scale=1">
      <title>ESP32 Smart Home</title>

      <style>

        body {
            background-color: #111827;
            color: white;
            font-family: Arial;
            margin: 0;
            padding: 20px;

        }
        .card {
            background-color: #1e293b;
            max-width: 400px;
            margin: 50px auto;
            padding: 25px;
            border-radius: 15px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.3);
            text-align: center;
        }

        h1 {
            font-size: 26px;
        }

        p {
            color: #cbd5e1;
        }

        .status {
            background-color: #334155;
            padding: 15px;
            border-radius: 10px;
            margin: 25px 0;
        }

        .button {
            display: block;
            padding: 15px;
            margin: 12px 0;
            border-radius: 10px;
            text-decoration: none;
            color: white;
            font-weight: bold;
        }

        .on {
          color: #22c55e;
          font-weight: bold;
        }

        .off {
          color: #ef4444;
          font-weight: bold;
        }

    </style>
  </head>

  <body>

    <div class="card">

        <h1>ESP32 Smart Home</h1>

        <p>Control Panel</p>

        <div class="status">
          <p>LED STATUS:</p>
          <p class="LED_CLASS">LED_STATUS</p>
        </div>

        <a class="button on" href="/on">
            TURN ON THE LED
        </a>

        <a class="button off" href="/off">
            TURN OFF THE LED
        </a>

    </div>

  </body>
  </html>
)rawliteral";


void setup() {
  Serial.begin(115200);

  //LED configuration
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //Connecting to WiFI
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  //Verification step
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connection established !");
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());

  //The main page
  server.on("/", HTTP_GET, []() {
      String html = page;

      if (ledState == true) {
        html.replace("LED_STATUS", "ON");
        html.replace("LED_CLASS", "on");
      }
      else {
        html.replace("LED_STATUS", "OFF");
        html.replace("LED_CLASS", "off");
      }
      server.send(200, "text/html", html);
  });

  //To turn on the LED
  server.on("/on", HTTP_GET, []() {
    digitalWrite(LED_PIN, HIGH);
    ledState = true;

    server.sendHeader("Location", "/");
    server.send(303);
    });

  //To turn off the LED
  server.on("/off", HTTP_GET, [](){
    digitalWrite(LED_PIN, LOW);
    ledState = false;

    server.sendHeader("Location", "/");
    server.send(303);
    });

  //Server startup
  server.begin();
  Serial.println("Web server started !");
  
}

void loop() {
  //To check for any request
  server.handleClient();  
}
