const int irSensorPin1 = 32;
const int irSensorPin2 = 33;
const int irSensorPin3 = 25;
const int irSensorPin4 = 26;
const int irSensorPin5 = 27;
const int irSensorPin6 = 14;

int sensorValue1, sensorValue2, sensorValue3, sensorValue4, sensorValue5, sensorValue6;

void setup() {
  Serial.begin(115200);

  // Initialize IR sensor pins
  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irSensorPin3, INPUT);
  pinMode(irSensorPin4, INPUT);
  pinMode(irSensorPin5, INPUT);
  pinMode(irSensorPin6, INPUT);
}

void loop() {
  // Read data from each IR sensor
  sensorValue1 = analogRead(irSensorPin1);
  sensorValue2 = analogRead(irSensorPin2);
  sensorValue3 = analogRead(irSensorPin3);
  sensorValue4 = analogRead(irSensorPin4);
  sensorValue5 = analogRead(irSensorPin5);
  sensorValue6 = analogRead(irSensorPin6);

  // Print individual sensor values to the serial monitor
  Serial.println("IR Sensor 1: " + String(sensorValue1));
  Serial.println("IR Sensor 2: " + String(sensorValue2));
  Serial.println("IR Sensor 3: " + String(sensorValue3));
  Serial.println("IR Sensor 4: " + String(sensorValue4));
  Serial.println("IR Sensor 5: " + String(sensorValue5));
  Serial.println("IR Sensor 6: " + String(sensorValue6));

  // Add a delay to avoid reading too frequently
  delay(1000);
}
