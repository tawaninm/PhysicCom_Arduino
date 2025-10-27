#include "SPI.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

const int buttonPin = 34;  // Pin for push button
bool buttonPressed = false;  // Variable to track button state
bool screenState = false;  // false for "Click Start", true for "Synchro"

// Setup function
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("TFT_eSPI library test!");

  tft.init();
  tft.setRotation(2);  // Set screen rotation to portrait mode (320x480)
  tft.fillScreen(TFT_WHITE);  // Fill screen with white

  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with pull-up resistor

  displayStartScreen();  // Initial screen display
}

// Loop function
void loop() {
  if (digitalRead(buttonPin) == LOW && !buttonPressed) {
    delay(200);  // Debounce delay
    buttonPressed = true;
    if (!screenState) {
      // Change screen to "Synchro" if "Click Start" was pressed
      screenState = true;
      displaySynchroScreen();
    }
  } 
  else if (digitalRead(buttonPin) == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}

// Function to display "Click Start" screen
void displayStartScreen() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(90, 180);  // Adjust position for portrait mode
  tft.print("Click Start");
}

// Function to display "Synchro" screen
void displaySynchroScreen() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(110, 110);  // Adjust position for portrait mode
  tft.print("Synchro");
}
