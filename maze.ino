// no ledPin "selected"
const int NO_LED = -1;
// length in milliseconds before game is reset when button held
const int RESET_GAME_TIME = 5000;
// debounce delay
const int DEBOUNCE_DELAY = 50;
// pointer to current mode pins
const int *ledPins;
int ledPinsLength;

// define pins for each mode
const int easyModePins[] = {13, 12, 11, 10, 9};
const int hardModePins[] = {13, 12, 11, 10, 9, 8, 7, 6, 5};

// define modes
enum Mode
{
  EASY_MODE,
  HARD_MODE
};

// set mode prior to compile time
const Mode mode = HARD_MODE;

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

  // set ledPins and length based on mode
  if (mode == EASY_MODE)
  {
    ledPins = easyModePins;
    ledPinsLength = sizeof(easyModePins) / sizeof(easyModePins[0]);

    Serial.println("Mode: EASY_MODE");
  }

  if (mode == HARD_MODE)
  {
    ledPins = hardModePins;
    ledPinsLength = sizeof(hardModePins) / sizeof(hardModePins[0]);

    Serial.println("Mode: HARD_MODE");
  }

  initializeLeds(ledPins, ledPinsLength);
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
    resetGame((int *)ledPins, ledPinsLength);
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
        selectedLedPin = cycleToNextLed(ledPins, ledPinsLength, selectedLedPin);
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
