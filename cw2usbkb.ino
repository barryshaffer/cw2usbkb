#include "Keyboard.h"
#include <string>
#define PADDLE1_PIN 6 // Pin 6, tip, Single Key
#define PADDLE2_PIN 7 // Pin 7, Dash, ring, Paddle
//*****************************************************************
//
// CW2USBKB
//  by KK7JXG
//  2021-03-20
//  version 1.0
//
//  This is a simple program to connect a straight key or paddle
//  to a phone via a USB cable.  The program is currently configured
//  for Morse Mainia.
//
//*****************************************************************

char MorseCode[100] = "";
char ConvertString[100] = "";

static uint8_t ProgramState = 0; // Inital State is 0, 1 is Setup Mode, 2 is Straight Key Mode, 3 is Paddle Mode
static uint8_t PADDLE1StateLast;
static uint8_t PADDLE2StateLast;
static uint8_t PADDLE1State;
static uint8_t PADDLE2State;

void blink_led_morse(char *morse_code)
{
    int i = 0;
    while (morse_code[i] != '\0')
    {
        if (morse_code[i] == '.')
        {
            digitalWrite(LED_BUILTIN, LOW);
            delay(70);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(70);
        }
        else if (morse_code[i] == '-')
        {
            digitalWrite(LED_BUILTIN, LOW);
            delay(210);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(70);
        }
        else if (morse_code[i] == ' ')
        {
            delay(210);
        }
        i++;
    }
}

void Convert_char_to_Morse(char *input, char *output)
{
    int i = 0;
    output[0] = '\0'; // clear the output string
    while (input[i] != '\0')
    {
        if (input[i] == 'a' || input[i] == 'A')
        {
            strcat(output, ".- ");
        }
        else if (input[i] == 'b' || input[i] == 'B')
        {
            strcat(output, "-... ");
        }
        else if (input[i] == 'c' || input[i] == 'C')
        {
            strcat(output, "-.-. ");
        }
        else if (input[i] == 'd' || input[i] == 'D')
        {
            strcat(output, "-.. ");
        }
        else if (input[i] == 'e' || input[i] == 'E')
        {
            strcat(output, ". ");
        }
        else if (input[i] == 'f' || input[i] == 'F')
        {
            strcat(output, "..-. ");
        }
        else if (input[i] == 'g' || input[i] == 'G')
        {
            strcat(output, "--. ");
        }
        else if (input[i] == 'h' || input[i] == 'H')
        {
            strcat(output, ".... ");
        }
        else if (input[i] == 'i' || input[i] == 'I')
        {
            strcat(output, ".. ");
        }
        else if (input[i] == 'j' || input[i] == 'J')
        {
            strcat(output, ".--- ");
        }
        else if (input[i] == 'k' || input[i] == 'K')
        {
            strcat(output, "-.- ");
        }
        else if (input[i] == 'l' || input[i] == 'L')
        {
            strcat(output, ".-.. ");
        }
        else if (input[i] == 'm' || input[i] == 'M')
        {
            strcat(output, "-- ");
        }
        else if (input[i] == 'n' || input[i] == 'N')
        {
            strcat(output, "-. ");
        }
        else if (input[i] == 'o' || input[i] == 'O')
        {
            strcat(output, "--- ");
        }
        else if (input[i] == 'p' || input[i] == 'P')
        {
            strcat(output, ".--. ");
        }
        else if (input[i] == 'q' || input[i] == 'Q')
        {
            strcat(output, "--.- ");
        }
        else if (input[i] == 'r' || input[i] == 'R')
        {
            strcat(output, ".-. ");
        }
        else if (input[i] == 's' || input[i] == 'S')
        {
            strcat(output, "... ");
        }
        else if (input[i] == 't' || input[i] == 'T')
        {
            strcat(output, "- ");
        }
        else if (input[i] == 'u' || input[i] == 'U')
        {
            strcat(output, "..- ");
        }
        else if (input[i] == 'v' || input[i] == 'V')
        {
            strcat(output, "...- ");
        }
        else if (input[i] == 'w' || input[i] == 'W')
        {
            strcat(output, ".-- ");
        }
        else if (input[i] == 'x' || input[i] == 'X')
        {
            strcat(output, "-..- ");
        }
        else if (input[i] == 'y' || input[i] == 'Y')
        {
            strcat(output, "-.-- ");
        }
        else if (input[i] == 'z' || input[i] == 'Z')
        {
            strcat(output, "--.. ");
        }
        else if (input[i] == '1')
        {
            strcat(output, ".---- ");
        }
        else if (input[i] == '2')
        {
            strcat(output, "..--- ");
        }
        else if (input[i] == '3')
        {
            strcat(output, "...-- ");
        }
        else if (input[i] == '4')
        {
            strcat(output, "....- ");
        }
        else if (input[i] == '5')
        {
            strcat(output, "..... ");
        }
        else if (input[i] == '6')
        {
            strcat(output, "-.... ");
        }
        else if (input[i] == '7')
        {
            strcat(output, "--... ");
        }
        else if (input[i] == '8')
        {
            strcat(output, "---.. ");
        }
        else if (input[i] == '9')
        {
            strcat(output, "----. ");
        }
        else if (input[i] == '0')
        {
            strcat(output, "----- ");
        }
        else if (input[i] == ' ')
        {
            strcat(output, "  ");
        }
        else if (input[i] == '.')
        {
            strcat(output, ".-.-.- ");
        }
        else if (input[i] == ',')
        {
            strcat(output, "--..-- ");
        }
        else if (input[i] == '?')
        {
            strcat(output, "..--.. ");
        }
        else if (input[i] == '\'')
        {
            strcat(output, ".----. ");
        }
        else if (input[i] == '!')
        {
            strcat(output, "-.-.-- ");
        }
        else if (input[i] == '/')
        {
            strcat(output, "-..-. ");
        }
        else if (input[i] == '(')
        {
            strcat(output, "-.--. ");
        }
        else if (input[i] == ')')
        {
            strcat(output, "-.--.- ");
        }
        else if (input[i] == '&')
        {
            strcat(output, ".-... ");
        }
        else if (input[i] == ':')
        {
            strcat(output, "---... ");
        }
        else if (input[i] == ';')
        {
            strcat(output, "-.-.-. ");
        }
        else if (input[i] == '=')
        {
            strcat(output, "-...- ");
        }
        else if (input[i] == '+')
        {
            strcat(output, ".-.-. ");
        }
        else if (input[i] == '-')
        {
            strcat(output, "-....- ");
        }
        else if (input[i] == '_')
        {
            strcat(output, "..--.- ");
        }
        else if (input[i] == '"')
        {
            strcat(output, ".-..-. ");
        }
        else if (input[i] == '$')
        {
            strcat(output, "...-..- ");
        }
        else if (input[i] == '@')
        {
            strcat(output, ".--.-. ");
        }
        else if (input[i] == '\n')
        {
            strcat(output, "\n");
        }
        else
        {
            strcat(output, " ");
        }
        i++;
    }
}

void setup()
{
    // Initialize the Keyboard library
    Keyboard.begin();
    // Set pin to input
    pinMode(PADDLE1_PIN, INPUT);
    // Enable pullup resistor
    digitalWrite(PADDLE1_PIN, HIGH);

    // Set pin to input
    pinMode(PADDLE2_PIN, INPUT);
    // Enable pullup resistor
    digitalWrite(PADDLE2_PIN, HIGH);

    // setup LED for for output
    pinMode(LED_BUILTIN, OUTPUT);

    PADDLE1State = PADDLE1StateLast = digitalRead(PADDLE1_PIN);
    PADDLE2State = PADDLE2StateLast = digitalRead(PADDLE2_PIN);
}

void setup_mode()
{

    // Choose between Straight Key or Paddle Mode
    while (ProgramState == 1)
    {
        PADDLE1State = digitalRead(PADDLE1_PIN);
        PADDLE2State = digitalRead(PADDLE2_PIN);

        if (PADDLE1State != PADDLE1StateLast)
        {
            PADDLE1StateLast = PADDLE1State;

            if (PADDLE1State == 0)
            {
                ProgramState = 2;
                strcpy(ConvertString, "sk");
                Convert_char_to_Morse(ConvertString, MorseCode);
                blink_led_morse(MorseCode);
            }
        }

        if (PADDLE2State != PADDLE2StateLast)
        {
            PADDLE2StateLast = PADDLE2State;

            if (PADDLE2State == 0)
            {
                ProgramState = 3;
                strcpy(ConvertString, "p");
                Convert_char_to_Morse(ConvertString, MorseCode);
                blink_led_morse(MorseCode);
            }
        }
    }

    PADDLE1State = PADDLE1StateLast = digitalRead(PADDLE1_PIN);
    PADDLE2State = PADDLE2StateLast = digitalRead(PADDLE2_PIN);
}

void loop()
{
    if (ProgramState == 0)
    {
        delay(1000);

        // Use the built in LED to show the program is running

        strcpy(ConvertString, "77");
        Convert_char_to_Morse(ConvertString, MorseCode);
        blink_led_morse(MorseCode);
        ProgramState = 1;
        /*
        Keyboard.println("Initialized");
        Keyboard.print("Program State: ");
        Keyboard.println(ProgramState);
        Keyboard.print("PADDLE1State: ");
        Keyboard.println(PADDLE1State);
        Keyboard.print("PADDLE1StateLast: ");
        Keyboard.println(PADDLE1StateLast);
        Keyboard.print("PADDLE2State: ");
        Keyboard.println(PADDLE2State);
        Keyboard.print("PADDLE2StateLast: ");
        Keyboard.println(PADDLE2StateLast);
        */
    }

    if (ProgramState == 1)
    {
        delay(1000);
        // Use the built in LED to show the program is in Setup Mode
        strcpy(ConvertString, "s or p");
        Convert_char_to_Morse(ConvertString, MorseCode);
        blink_led_morse(MorseCode);
        setup_mode();
    }

    PADDLE1State = digitalRead(PADDLE1_PIN);
    PADDLE2State = digitalRead(PADDLE2_PIN);

    if (PADDLE1State != PADDLE1StateLast)
    {
        PADDLE1StateLast = PADDLE1State;

        if (PADDLE1State == 0 && ProgramState == 2)
        {
            Keyboard.press(' ');
        }
        else
        {
            Keyboard.release(' ');
        }

        if (PADDLE1State == 0 && ProgramState == 3)
        {
            Keyboard.press('a');
        }
        else
        {
            Keyboard.release('a');
        }
    }

    if (PADDLE2State != PADDLE2StateLast)
    {
        PADDLE2StateLast = PADDLE2State;

        if (PADDLE2State == 0)
        {
            Keyboard.press('s');
        }
        else
        {
            Keyboard.release('s');
        }
    }

    // this was needed to debounce the paddle
    delay(20);

} //  End of the Main Loop
