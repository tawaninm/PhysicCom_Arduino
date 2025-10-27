void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  pinMode(9, OUTPUT);   // Set pin 9 (Trig) as an output
  pinMode(10, INPUT);   // Set pin 10 (Echo) as an input
}

void loop() {
  digitalWrite(9, HIGH);              // Send a HIGH signal to trigger the sensor
  delayMicroseconds(10);               // Wait for 10 microseconds
  digitalWrite(9, LOW);               // Stop triggering the sensor

  long pulseWidth = pulseIn(10, HIGH); // Measure the duration of the pulse on the Echo pin (pin 10)
     // Print the pulse width to the serial monitor

  long distance = pulseWidth / 29 / 2;  // Calculate the distance based on pulse width
  Serial.print("Distance: ");
  Serial.println(distance);            // Print the distance to the serial monitor

  delay(1000);  // Delay for 1 second before taking the next measurement
}
