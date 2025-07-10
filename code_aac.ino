// Motor Pins
const int MR1 = 5;  // Right Motor Forward (PWM)
const int MR2 = 6;  // Right Motor Backward
const int ML1 = 9;  // Left Motor Backward
const int ML2 = 10; // Left Motor Forward (PWM)

// Potentiometer and Ultrasonic Pins
const int potPin = A0;
const int trigPin = 2;
const int echoPin = 3;

long duration;
int distance;
int baseSpeed = 0;
int adjustedSpeed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Stop();
}

void loop() {
  // Read potentiometer and map to base speed
  int potValue = analogRead(potPin); // 0–1023
  baseSpeed = map(potValue, 0, 1023, 0, 255); // 0–255

  // Measure distance using ultrasonic
  distance = getDistance();

  // Adjust speed relative to how close the object is
  if (distance <= 15) {
    adjustedSpeed = 0; // Stop immediately if object is very close
  } else if (distance > 15 && distance < 100) {
    // Linearly scale from 0.1x to 1.0x based on distance
    float factor = (distance - 15) / 85.0; // range from 0 to 1
    adjustedSpeed = baseSpeed * factor;
  } else {
    adjustedSpeed = baseSpeed; // Object far enough, go at full speed
  }

  // Debug info
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Base Speed: ");
  Serial.print(baseSpeed);
  Serial.print(" | Adjusted Speed: ");
  Serial.println(adjustedSpeed);

  // Drive forward with adjusted speed
  analogWrite(MR1, adjustedSpeed);
  digitalWrite(MR2, LOW);
  analogWrite(ML2, adjustedSpeed);
  digitalWrite(ML1, LOW);

  delay(100);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000); // timeout to avoid hang
  return duration * 0.034 / 2; // in cm
}

void Stop() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
}
