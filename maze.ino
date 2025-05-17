// no ledPin "selected"
const int NO_LED = -1;
// length in milliseconds before game is reset when button held
const int RESET_GAME_TIME = 5000;
// debounce delay
const int DEBOUNCE_DELAY = 250;
// array of all available pins
const int ledPins[] = {13, 12, 11, 10, 9, 8, 7, 6, 5};
// number of pins
const int ledPinsArrayLength = sizeof(ledPins) / sizeof(ledPins[0]);
// main game button
const int greenButtonPin = 2;

int selectedLedPin = NO_LED;
int greenButtonPinLastState = LOW;

bool isGreenButtonPressed = false;

unsigned long greenButtonElapsedPressTime;
unsigned long greenButtonLastPressedTime;

void setup()
{
  // start serial communication at 9600 baud
  Serial.begin(9600);

  // button connected to ground when pressed
  pinMode(greenButtonPin, INPUT_PULLUP);

  initializeLeds();
}

void loop()
{
  int greenButtonCurrentState = digitalRead(greenButtonPin);

  greenButtonElapsedPressTime = millis() - greenButtonLastPressedTime;

  // button released
  if (greenButtonPinLastState == HIGH && greenButtonCurrentState == LOW)
  {

    isGreenButtonPressed = false;

    if (greenButtonElapsedPressTime > RESET_GAME_TIME)
    {
      resetGame();

      return;
    }

    delay(DEBOUNCE_DELAY);
  }

  // button pressed
  if (greenButtonPinLastState == LOW && greenButtonCurrentState == HIGH)
  {
    isGreenButtonPressed = true;

    greenButtonLastPressedTime = millis();

    cycleToNextLed();
  }

  greenButtonPinLastState = greenButtonCurrentState;
}

void cycleToNextLed()
{
  // set current LED to LOW
  digitalWrite(ledPins[selectedLedPin], LOW);

  // increment current LED
  selectedLedPin++;
  if (selectedLedPin == ledPinsArrayLength)
  {
    selectedLedPin = NO_LED;
  }

  // set current LED to HIGH if greater than, "NO_LED"
  if (selectedLedPin > NO_LED)
  {
    digitalWrite(ledPins[selectedLedPin], HIGH);
  }
}

void initializeLeds()
{
  for (int i = 0; i < ledPinsArrayLength; i++)
  {
    pinMode(ledPins[i], OUTPUT);

    digitalWrite(ledPins[i], LOW);
  }
}

void resetGame()
{
  initializeLeds();

  selectedLedPin = NO_LED;
  greenButtonLastPressedTime = millis();
}
