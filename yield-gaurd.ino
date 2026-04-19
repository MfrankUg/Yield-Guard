#include <DHT.h>

// --- Pin Definitions ---
#define DHTPIN 4        // DHT11 Data Pin
#define BUZZER_PIN 13   // Buzzer Pin

// --- Sensor Setup ---
#define DHTTYPE DHT11   // Using the DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

// --- Coffee Storage Optimal Ranges ---

const float MIN_TEMP = 15.0;  // Minimum safe temperature in Celsius
const float MAX_TEMP = 25.0;  // Maximum safe temperature in Celsius
const float MIN_HUM  = 40.0;  // Minimum safe humidity percentage
const float MAX_HUM  = 60.0;  // Maximum safe humidity percentage

void setup() {
  Serial.begin(115200); 
  Serial.println("Starting Coffee Storage Monitor...");

  // Initialize the pins and sensor
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  // The DHT11 needs about 2 seconds between readings
  delay(2000); 

  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();

  // Check if the sensor disconnected or failed
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Error: Failed to read from DHT sensor!"); 
    
    // Optional: Beep rapidly to indicate a broken sensor
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    return; 
  }

  // --- Print current conditions to the Serial Monitor ---
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C  |  Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  -->  Status: ");

  // --- The Alarm Logic ---
  // Check if temperature or humidity are OUTSIDE the optimal ranges
  bool isTempBad = (tempC < MIN_TEMP || tempC > MAX_TEMP);
  bool isHumBad  = (humidity < MIN_HUM || humidity > MAX_HUM);

  if (isTempBad || isHumBad) {
    // Conditions are bad! Turn on the buzzer.
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("WARNING! Sub-optimal conditions!");
  } else {
    // Conditions are perfect. Keep the buzzer off.
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Perfect.");
  }
}
