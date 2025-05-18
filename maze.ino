// no ledPin "selected"
const int NO_LED = -1;
// length in milliseconds before game is reset when button held
const int RESET_GAME_TIME = 5000;
// debounce delay
const int DEBOUNCE_DELAY = 50;
// array of all available pins
const int ledPins[] = {13, 12, 11, 10, 9, 8, 7, 6, 5};
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

  const int pinsArrayLength = sizeof(ledPins) / sizeof(ledPins[0]);
  initializeLeds(ledPins, pinsArrayLength);
}

void loop()
{
  int greenButtonCurrentState = digitalRead(greenButtonPin);

  // if the switch changed, due to noise or pressing, etc
  if (greenButtonCurrentState != greenButtonPinLastState)
  {
    // reset the debouncing timer
    greenButtonLastPressedTime = millis();
  }

  greenButtonElapsedPressTime = millis() - greenButtonLastPressedTime;

  // reset the game if the green button has been held for longer than the specified time
  if (isGreenButtonPressed && greenButtonElapsedPressTime > RESET_GAME_TIME)
  {
    const int pinsArrayLength = sizeof(ledPins) / sizeof(ledPins[0]);

    resetGame(ledPins, pinsArrayLength);

    return;
  }

  // if the debounce delay has been exceeded
  if ((greenButtonElapsedPressTime) > DEBOUNCE_DELAY)
  {
    // if the button state has changed:
    if (isGreenButtonPressed != greenButtonCurrentState)
    {
      isGreenButtonPressed = greenButtonCurrentState;

      // only cycle to the next LED if the new button state is HIGH
      if (isGreenButtonPressed == HIGH)
      {
        const int ledPinsArrayLength = sizeof(ledPins) / sizeof(ledPins[0]);
        selectedLedPin = cycleToNextLed(ledPins, ledPinsArrayLength, selectedLedPin);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the greenButtonPinLastState:
  greenButtonPinLastState = greenButtonCurrentState;
}

int cycleToNextLed(
    const int pins[],
    const int pinsArrayLength,
    int selectedPin)
{
  if (selectedPin >= 0 && selectedPin < pinsArrayLength)
  {
    digitalWrite(pins[selectedPin], LOW);
  }

  selectedPin++;
  if (selectedPin >= pinsArrayLength)
  {
    selectedPin = NO_LED;
  }

  if (selectedPin >= 0 && selectedPin < pinsArrayLength)
  {
    digitalWrite(pins[selectedPin], HIGH);
  }

  return selectedPin;
}

void initializeLeds(const int pins[], int pinsArrayLength)
{
  for (int i = 0; i < pinsArrayLength; i++)
  {
    pinMode(pins[i], OUTPUT);

    digitalWrite(pins[i], LOW);
  }
}

void resetGame(int pins[], int pinsArrayLength)
{
  initializeLeds(pins, pinsArrayLength);

  selectedLedPin = NO_LED;
  greenButtonLastPressedTime = millis();
}
