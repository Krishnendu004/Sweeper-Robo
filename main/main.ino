// --- SPEED PINS ---
const int ena = 3;  // Back Drive Wheel Speed (OUT1/OUT2)
const int enb = 11; // Rotating Disks Speed (OUT3/OUT4)

// --- BACK DRIVE WHEEL PINS ---
const int in1 = 5;
const int in2 = 6;

// --- FRONT ROTATING DISKS PINS ---
const int in3 = 9;
const int in4 = 10;

// --- ULTRASONIC SENSOR PINS ---
const int trigPin = 12;
const int echoPin = 13;

// Set to 255 for maximum power to prevent stalling
int driveSpeed = 255; 
int diskSpeed = 255;  

void setup() {
  Serial.begin(9600); 
  
  pinMode(ena, OUTPUT); pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Apply power to both motor channels
  analogWrite(ena, driveSpeed);
  analogWrite(enb, diskSpeed);
  
  Serial.println("Sweeper Mode Ready!");
  Serial.println("Back wheel reversed. Auto-brake at 15cm.");
}

void loop() {
  // 1. --- MEASURE DISTANCE ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 25000); 
  long distance = duration * 0.034 / 2;

  // 2. --- ACTION LOGIC ---
  if (distance > 0 && distance < 15) {
    // WALL DETECTED! Hit the brakes on everything.
    Serial.print("Wall at "); Serial.print(distance); Serial.println("cm. BRAKING.");
    
    // Stop Back Drive Wheel
    digitalWrite(in1, LOW); digitalWrite(in2, LOW);
    
    // Stop Front Rotating Disks
    digitalWrite(in3, LOW); digitalWrite(in4, LOW);
    
  } else {
    // PATH IS CLEAR! Turn everything on.
    
    // REVERSED: Back Drive Wheel
    digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
    
    // Spin Front Rotating Disks (Untouched)
    digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  }
  
  delay(50); // Tiny delay to stabilize the sensor readings
}