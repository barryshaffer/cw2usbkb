#include "Keyboard.h"
#include<string>
#define PADDLE1_PIN   6 // Pin 6, Dot,J, tip, Single Key
#define PADDLE2_PIN   7 // Pin 7, Dash, L, ring, Paddle

// *****************************************
char Program_ID[] = "CW_Gadget_MultiMode";
char VerNo[] = "3.0";
char ProgDate[] = "7-6-21";
char Copywrite[] = "Copyright (c) 2021, CWG, All Rights Reserved.";
char morsestring[10];
char morseletter[1];
// *****************************************
// Program Description:
// This version of the program, 3.0, is the version.
//
// Fixed the pin 6-7 issue
//
// has working:
// Paddle-Simple
// Paddle-Normal
// StraightKey-Timings
// Straightkey-Normal
//
// Needs:
// Set WPM
// 
//
// *****************************************
int choicevar = 0;
int speedvar = 10;
int keypad = 0;
int game_mode = 0;       // game_mode 1 = J,L - game_mode 2 = characters - game mode 3 = timings.
int key_mode = 0;        // key_mode 1 = Straight Key, key mode 2 = paddle
int wordspace = 1;
int startpressed = 0;    // the moment the key was pressed
int endpressed = 0;      // the moment the key was released
int holdtime = 0;        // how long the button was hold
int idletime = 0;        // how long the button was idle
int laststatchange = 0;  // when key state changed (pressed/released)
int initialtime = 0;     //time the program starts
// *****************************************
float dv1 = 60000;
float dv2 = 50000;
float wpm = 0;
float ditlen = 0;
float dashlen = 0;
float intrael = 0;
float intraword = 0;
// *****************************************
unsigned long interval_timestart;
unsigned long interval_timecheck;
unsigned long interval_timer;
// *****************************************

static uint8_t PADDLE1StateLast = 0;
static uint8_t PADDLE2StateLast = 0;
static uint8_t PADDLEState;

static void PADDLE1Down(void)
{
  Keyboard.press('j');
}
static void PADDLE2Down(void)
{
  Keyboard.press('l');
}

static void StraightKeyDown(void) {
  //send how long key was idle.
  Keyboard.write('S');
  Keyboard.print(idletime);
  Keyboard.write(176);
  //send how long the key was held down
  Keyboard.write('K');
  Keyboard.print(holdtime);
  Keyboard.write(176);
  holdtime = 0;
  idletime = 0;
}

void setup() {
  Keyboard.begin();
  // Set pin to input
  pinMode(PADDLE1_PIN, INPUT);
  // Enable pullup resistor
  digitalWrite(PADDLE1_PIN, HIGH);

  // Set pin to input
  pinMode(PADDLE2_PIN, INPUT);
  // Enable pullup resistor
  digitalWrite(PADDLE2_PIN, HIGH);


}

static void SETMODE_Gadget(void) {

  Keyboard.write(176);
  Keyboard.print("Carrier Waves Game Gadget Setup  ");
  Keyboard.print("   Version No: ");
  Keyboard.print(VerNo);
  Keyboard.write(176);
  Keyboard.print("   ");
  Keyboard.print(Copywrite);
  Keyboard.write(176);
  Keyboard.print("............................................................");
  Keyboard.write(176);
  // This section lets the use choose Single Key or Paddle....
  // This section should set the key_mode.


  Keyboard.print("     Please choose Key or Paddle....");
  Keyboard.write(176);
  Keyboard.write(176);
  Keyboard.print("     If you are using a Paddle, key in a single dash.");
  Keyboard.write(176);
  Keyboard.write(176);
  Keyboard.print("     If you are using a Straight Key, ");
  Keyboard.write(176);
  Keyboard.print("         send a single dit at 5wpm or faster.");
  Keyboard.write(176);

  while (key_mode == 0) {
    PADDLEState = digitalRead(PADDLE1_PIN);
    if (PADDLEState != PADDLE1StateLast) {
      PADDLE1StateLast = PADDLEState;
      if (PADDLEState == 0) {
        key_mode = 1;
        delay(100);
        Keyboard.releaseAll();
      }
    }
    PADDLEState = digitalRead(PADDLE2_PIN);
    if (PADDLEState != PADDLE2StateLast) {
      PADDLE2StateLast = PADDLEState;
      if (PADDLEState == 0) {
        key_mode = 2;
        delay(100);
        Keyboard.releaseAll();
      }
    }
  }
  if (key_mode == 1) {
    Keyboard.print("     Straight Key Mode is chosen. ");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("............................................................");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("     Which Straight Key Mode would you like?");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("          Normal or Timings for Analysis?");
    Keyboard.write(176);
    Keyboard.print("     At 5wpm send a dit or a dash.");
    Keyboard.write(176);
    Keyboard.print("        (dit) = Normal, (dash) for Timings.");
    Keyboard.write(176);
    Keyboard.write(176);
    while (game_mode == 0) {
      PADDLEState = digitalRead(PADDLE1_PIN);
      if (PADDLEState != PADDLE1StateLast) {
        PADDLE1StateLast = PADDLEState;
        if (PADDLEState == 0) {
          game_mode = 1;
          Keyboard.print("    Normal mode chosen.");
          Keyboard.write(176);
          delay(100);
          Keyboard.releaseAll();
        }
      }
      PADDLEState = digitalRead(PADDLE2_PIN);
      if (PADDLEState != PADDLE2StateLast) {
        PADDLE2StateLast = PADDLEState;
        if (PADDLEState == 0) {
          game_mode = 2;
          Keyboard.print("    Timings mode chosen.");
          Keyboard.write(176);
          delay(100);
          Keyboard.releaseAll();
        }
      }
    }
  }

  if (key_mode == 2 ) {
    Keyboard.print("    Paddle Mode is chosen. ");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("............................................................");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("     Which Paddle Mode would you like?");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("     Normal or Simple?");
    Keyboard.write(176);
    Keyboard.print("     (dit) = Normal, (dash) for Simple.");
    Keyboard.write(176);
    Keyboard.write(176);

    while (game_mode == 0) {
      PADDLEState = digitalRead(PADDLE1_PIN);
      if (PADDLEState != PADDLE1StateLast) {
        PADDLE1StateLast = PADDLEState;
        if (PADDLEState == 0) {
          game_mode = 1;
          Keyboard.print("  Normal mode chosen.");
          Keyboard.write(176);
          delay(100);
          Keyboard.releaseAll();
        }
      }
      PADDLEState = digitalRead(PADDLE2_PIN);
      if (PADDLEState != PADDLE2StateLast) {
        PADDLE2StateLast = PADDLEState;
        if (PADDLEState == 0) {
          game_mode = 2;
          Keyboard.print("  Simple mode chosen.");
          Keyboard.write(176);
          delay(100);
          Keyboard.releaseAll();
        }
      }
    }
  }
  if (game_mode == 1) {
    Keyboard.print("............................................................");
    Keyboard.write(176);
    Keyboard.print("     Please Set Words per Minute for the Gadget.");
    Keyboard.write(176);
    Keyboard.print("     Enter a number between 5 and 20 using 5wpm morse code.");
    Keyboard.write(176);
    if (key_mode == 2 ) {
      // this is where the logic to set WPM  with a paddle, goes,

    }
    if (key_mode == 1 ) {
      // this is where the logic to set WPM  with a straight key, goes,

    }

    speedvar = 10;// temporarily set to 10wpm

    Keyboard.print("     Speed ");
    Keyboard.print(speedvar);
    Keyboard.print(" words per minute, is now set.");
    Keyboard.write(176);
    Keyboard.write(176);

    wpm = speedvar;
    ditlen = (dv2 * wpm);
    ditlen = (dv1 / ditlen);
    ditlen = (ditlen * 1000);
    dashlen = (3 * ditlen);
    intrael = (ditlen);
    intraword = (ditlen * 7);

    Keyboard.print("  Settings in Milliseconds:");
    Keyboard.write(176);
    Keyboard.print("     dit length = ");
    Keyboard.print(int(ditlen));
    Keyboard.write(176);
    Keyboard.print("     dash length = ");
    Keyboard.print(int(dashlen));
    Keyboard.write(176);
    Keyboard.print("     intra element = ");
    Keyboard.print(int(intrael));
    Keyboard.write(176);
    Keyboard.print("     intra word = ");
    Keyboard.print(int(intraword));
    Keyboard.write(176);
  }


  if (game_mode == 1)  {
    Keyboard.print("............................................................");
    Keyboard.write(176);
    Keyboard.write(176);
    Keyboard.print("  Remember... ");
    Keyboard.write(176);
    Keyboard.print("     AA  .-.-  = Carriage Return (enter).");
    Keyboard.write(176);
    Keyboard.print("     8 dits ........ = backspace (delete).");
    Keyboard.write(176);
    Keyboard.write(176);
  }
  if ((game_mode == 2) && (key_mode == 1)) {
    // Single Key, Timings mode
    Keyboard.write(176);
    Keyboard.print("     Your straight key will now report timings.");
    Keyboard.write(176);
    Keyboard.write(176);
  }

  if ((game_mode == 2) && (key_mode == 2)) {
    // Single Key, JL mode
    Keyboard.write(176);
    Keyboard.print("     Your paddle is set to simple mode.");
    Keyboard.write(176);
    Keyboard.print("     The left paddle sends a J.");
    Keyboard.write(176);
    Keyboard.print("     The right paddle sends a L.");
    Keyboard.write(176);
    Keyboard.print("     How fast or slow you send does not matter.");
    Keyboard.write(176);
    Keyboard.write(176);
  }

  Keyboard.print("............................................................");
  Keyboard.write(176);
  Keyboard.write(176);
  Keyboard.print("     To choose different settings, unplug the Gadget from ");
  Keyboard.write(176);
  Keyboard.print("  your usb port and then re-plug it in.");
  Keyboard.write(176);
  Keyboard.print("............................................................");
  Keyboard.write(176);
//  Keyboard.write(176);
//  Keyboard.print("game_mode: ");
//  Keyboard.print(game_mode);
//  Keyboard.print(" ");
//  Keyboard.print("key_mode: ");
//  Keyboard.print(key_mode);
//  Keyboard.write(176);
  Keyboard.write(176);
  Keyboard.print("Begin...");
  Keyboard.write(176);
  Keyboard.write(176);

}// End of SETMODE_Gadget

void loop() {
  if (choicevar < 5) {
    delay(1000);
  }
  if (choicevar == 2) {
    SETMODE_Gadget();
  }
  if (choicevar < 5) {
    choicevar = choicevar + 1;
  }
  // this section for Paddle-Simple(J/L)
  if ((game_mode == 2) && (key_mode == 2)) {
    PADDLEState = digitalRead(PADDLE1_PIN);
    if (PADDLEState != PADDLE1StateLast) {
      PADDLE1StateLast = PADDLEState;

      if (PADDLEState == 0) {
        PADDLE1Down();
        delay(100);
        Keyboard.releaseAll();
      }
    }

    PADDLEState = digitalRead(PADDLE2_PIN);
    if (PADDLEState != PADDLE2StateLast) {
      PADDLE2StateLast = PADDLEState;

      if (PADDLEState == 0) {
        PADDLE2Down();
        delay(100);
        Keyboard.releaseAll();
      }
    }
    delay(50);
  }// End of section Paddle-Simple(J/L)

  if ((game_mode == 1) && (key_mode == 2)) {// Begin of  of section Paddle-Normal
    interval_timer = millis();
    interval_timecheck = (interval_timer - interval_timestart);
    if (digitalRead(PADDLE1_PIN) == LOW) {
      delay(250);// debounce delay
      wordspace = 0;
      strcat(morsestring, ".");
      interval_timestart = millis();
      interval_timer = millis();
      interval_timecheck = 0;
    }
    if (digitalRead(PADDLE2_PIN) == LOW) {
      delay(250);// debounce delay
      wordspace = 0;
      strcat(morsestring, "-");
      interval_timestart = millis();
      interval_timer = millis();
      interval_timecheck = 0;
    }
    if ((morsestring != "") && (interval_timecheck >= (1.5 * intrael))) {
      MorseDecoderRing();  // Go decode the morsestring
      //     Keyboard.print(interval_timecheck);
    }
    if ((wordspace == 0) && (interval_timecheck >= intraword)) {
      wordspace = 1;
      Keyboard.print(" ");
      strcpy(morseletter, "");
      strcpy(morsestring, "");
    }
  }// End of  of section Paddle-Normal

  if ((game_mode == 2) && (key_mode == 1)) {// Begin of  of section Straight Key-Timings
    PADDLEState = digitalRead(PADDLE1_PIN);
    if (PADDLEState != PADDLE1StateLast) {
      PADDLE1StateLast = PADDLEState;

      if (PADDLEState == 0) {
        startpressed = millis();
        idletime = startpressed - initialtime;
      } else {
        endpressed = millis();
        initialtime = endpressed;
        holdtime = endpressed - startpressed;
        StraightKeyDown();
      }
    }


    delay(5);
  }// End of  of section Straight Key-Timings

  if ((game_mode == 1) && (key_mode == 1)) {// Begin of  of section Straight Key-Normal
    PADDLEState = digitalRead(PADDLE1_PIN);
    if (PADDLEState != PADDLE1StateLast) {
      PADDLE1StateLast = PADDLEState;
      if (PADDLEState == 0) {
        //Key pressed
        delay(25);
        startpressed = millis();
        if (endpressed == 0) {
        endpressed = startpressed - 100;
      }
      idletime = startpressed - endpressed;

    } else {

      // Key released
     delay(25);
        endpressed = millis();
        holdtime = endpressed - startpressed;
        if (holdtime <= ditlen) {
          wordspace = 0;
          strcat(morsestring, ".");
//          Keyboard.write(176);
//          Keyboard.print(". ");
//          Keyboard.print(holdtime);
//          Keyboard.write(176);
        }
        if (holdtime > ditlen) {
          wordspace = 0;
          strcat(morsestring, "-");
//          Keyboard.write(176);
//          Keyboard.print("- ");
//          Keyboard.print(holdtime);
//          Keyboard.write(176);
        }
      }
    }
    if ((morsestring != "") && (interval_timecheck >= (3 * intrael))) {
      MorseDecoderRing();  // Go decode the morsestring
      strcpy(morsestring, "");
      strcpy(morseletter, "");
    }
    if ((wordspace == 0) && (interval_timecheck >=(1.5* intraword))) {
      wordspace = 1;
      Keyboard.print(" ");
      strcpy(morseletter, "");
      strcpy(morsestring, "");
    }
    interval_timer = millis();
    if (startpressed == 0){
    startpressed = interval_timer - 100;
  }
  interval_timecheck = ((interval_timer) - (startpressed));
  }// End of  of section Straight Key-Normal

} //  End of the Main Loop

void MorseDecoderRing() {
  strcpy(morseletter, "");
  // Letters ____________________________________________________________
  if (strcmp(morsestring, ".-") == 0)      {
    strcpy(morseletter, "A");  // A
  }
  if (strcmp(morsestring, "-...") == 0)    {
    strcpy(morseletter, "B");  // B
  }
  if (strcmp(morsestring, "-.-.") == 0)    {
    strcpy(morseletter, "C");  // C
  }
  if (strcmp(morsestring, "-..") == 0)     {
    strcpy(morseletter, "D");  // D
  }
  if (strcmp(morsestring, ".") == 0)       {
    strcpy(morseletter, "E");  // E
  }
  if (strcmp(morsestring, "..-.") == 0)    {
    strcpy(morseletter, "F");  // F
  }
  if (strcmp(morsestring, "--.") == 0)     {
    strcpy(morseletter, "G");  // G
  }
  if (strcmp(morsestring, "....") == 0)    {
    strcpy(morseletter, "H");  // H
  }
  if (strcmp(morsestring, "..") == 0)      {
    strcpy(morseletter, "I");  // I
  }
  if (strcmp(morsestring, ".---") == 0)    {
    strcpy(morseletter, "J");  // J
  }
  if (strcmp(morsestring, "-.-") == 0)     {
    strcpy(morseletter, "K");  // K
  }
  if (strcmp(morsestring, ".-..") == 0)    {
    strcpy(morseletter, "L");  // L
  }
  if (strcmp(morsestring, "--") == 0)      {
    strcpy(morseletter, "M");  // M
  }
  if (strcmp(morsestring, "-.") == 0)      {
    strcpy(morseletter, "N");  // N
  }
  if (strcmp(morsestring, "---") == 0)     {
    strcpy(morseletter, "O");  // O
  }
  if (strcmp(morsestring, ".--.") == 0)    {
    strcpy(morseletter, "P");  // P
  }
  if (strcmp(morsestring, "--.-") == 0)    {
    strcpy(morseletter, "Q");  // Q
  }
  if (strcmp(morsestring, ".-.") == 0)     {
    strcpy(morseletter, "R");  // R
  }
  if (strcmp(morsestring, "...") == 0)     {
    strcpy(morseletter, "S");  // S
  }
  if (strcmp(morsestring, "-") == 0)       {
    strcpy(morseletter, "T");  // T
  }
  if (strcmp(morsestring, "..-") == 0)     {
    strcpy(morseletter, "U");  // U
  }
  if (strcmp(morsestring, "...-") == 0)    {
    strcpy(morseletter, "V");  // V
  }
  if (strcmp(morsestring, ".--") == 0)     {
    strcpy(morseletter, "W");  // W
  }
  if (strcmp(morsestring, "-..-") == 0)    {
    strcpy(morseletter, "X");  // X
  }
  if (strcmp(morsestring, "-.--") == 0)    {
    strcpy(morseletter, "Y");  // Y
  }
  if (strcmp(morsestring, "--..") == 0)    {
    strcpy(morseletter, "Z");  // Z
  }
  // Numbers_____________________________________________________________
  if (strcmp(morsestring, ".----") == 0)   {
    strcpy(morseletter, "1");  // 1
  }
  if (strcmp(morsestring, "..---") == 0)   {
    strcpy(morseletter, "2");  // 2
  }
  if (strcmp(morsestring, "...--") == 0)   {
    strcpy(morseletter, "3");  // 3
  }
  if (strcmp(morsestring, "....-") == 0)   {
    strcpy(morseletter, "4");  // 4
  }
  if (strcmp(morsestring, ".....") == 0)   {
    strcpy(morseletter, "5");  // 5
  }
  if (strcmp(morsestring, "-....") == 0)   {
    strcpy(morseletter, "6");  // 6
  }
  if (strcmp(morsestring, "--...") == 0)   {
    strcpy(morseletter, "7");  // 7
  }
  if (strcmp(morsestring, "---..") == 0)   {
    strcpy(morseletter, "8");  // 8
  }
  if (strcmp(morsestring, "----.") == 0)   {
    strcpy(morseletter, "9");  // 9
  }
  if (strcmp(morsestring, "-----") == 0)   {
    strcpy(morseletter, "0");  // 0
  }
  // Punctuation ________________________________________________________
  if (strcmp(morsestring, "-.-.--") == 0)  {
    strcpy(morseletter, "!");  // !
  }
  if (strcmp(morsestring, "..--.") == 0)  {
    strcpy(morseletter, "!");  // !
  }
  //if (strcmp(morsestring,".-..-.") == 0)  { strcpy(morseletter,"""); }  // "
  if (strcmp(morsestring, "...-..-") == 0)  {
    strcpy(morseletter, "$");  // $
  }
  if (strcmp(morsestring, ".-...") == 0)   {
    strcpy(morseletter, "&");  // &
  }
  if (strcmp(morsestring, ".----.") == 0)  {
    strcpy(morseletter, ",");  // '
  }
  if (strcmp(morsestring, "-.--.") == 0)   {
    strcpy(morseletter, "(");  // (
  }
  if (strcmp(morsestring, "-.--.-") == 0)  {
    strcpy(morseletter, ")");  // )
  }
  if (strcmp(morsestring, ".-.-.") == 0)   {
    strcpy(morseletter, "+");  // +
  }
  if (strcmp(morsestring, "--..--") == 0)  {
    strcpy(morseletter, ",");  // ,
  }
  if (strcmp(morsestring, "-....-") == 0)  {
    strcpy(morseletter, "-");  // -
  }
  if (strcmp(morsestring, ".-.-.-") == 0)  {
    strcpy(morseletter, ".");  // .
  }
  if (strcmp(morsestring, "-..-.") == 0)   {
    strcpy(morseletter, "/");  // /
  }
  if (strcmp(morsestring, "---...") == 0)  {
    strcpy(morseletter, ":");  // :
  }
  if (strcmp(morsestring, "-.-.-.") == 0)  {
    strcpy(morseletter, ";");  // ;
  }
  //if (strcmp(morsestring,"-...-") == 0)   { strcpy(morseletter,"A"); }  // = (Pause, BT)
  if (strcmp(morsestring, "..--..") == 0)  {
    strcpy(morseletter, "?");  // ?
  }
  if (strcmp(morsestring, ".--.-.") == 0)  {
    strcpy(morseletter, "@");  // @
  }
  if (strcmp(morsestring, "..--.-") == 0)  {
    strcpy(morseletter, "_");  // _
  }
  // Prosigns ____________________________________________________________________________________________________________
  //  if (strcmp(morsestring,".-.-") == 0)    { decodeProSign = "AA"; }   //            (New Line)
  //  if (strcmp(morsestring,".-.-.") == 0)   { decodeProSign = "AR"; }   //            (End of Message)
  //  if (strcmp(morsestring,".-...") == 0)   { decodeProSign = "AS"; }   //            (Wait)
  //  if (strcmp(morsestring,"-...-.-") == 0) { decodeProSign = "BK"; }   //            (Break)
  //  if (strcmp(morsestring,"-...-") == 0)   { decodeProSign = "BT"; }   //            (Pause, New Paragraph)
  //  if (strcmp(morsestring,"-.-..-..") == 0){ decodeProSign = "CL"; }   //            (Clear, Going Off Air)
  //  if (strcmp(morsestring,"-.-.-") == 0)   { decodeProSign = "CT"; }   //            (Start Copying)
  //  if (strcmp(morsestring,"-.--.") == 0)   { decodeProSign = "KN"; }   //            (Invite Specific Station Only)
  //  if (strcmp(morsestring,"...-.-") == 0)  { decodeProSign = "SK"; }   //            (End of Transmission)
  //  if (strcmp(morsestring,"...-.") == 0)   { decodeProSign = "SN"; }   //            (Understood)
  //  if (strcmp(morsestring,"...---...") == 0){ decodeProSign = "SOS"; } //            (SOS)
  //  if (strcmp(morsestring,"-.-.--.-") == 0) { decodeProSign = "CQ"); } //            (Invitation)
  //  if (strcmp(morsestring,"........") == 0) { decodeProSign = "err"; } //            (Error)
  //_____________________________________________________________________________
  // output morseletter
  if (strcmp(morsestring, ".-.-") == 0)    {
    Keyboard.write(176);
    strcpy(morsestring, "");
    wordspace = 1;
    interval_timestart = millis();
  }   // (New Line)

  if (strcmp(morsestring, "........") == 0) {
    Keyboard.write(178);
    strcpy(morsestring, "");
    wordspace = 1;
    interval_timestart = millis();
  } //   8 dits   (Error)

  if (strlen(morseletter) > 0) {
    Keyboard.print(morseletter);
    strcpy(morseletter, "");
    strcpy(morsestring, "");
  }//  Actually print letter
  //interval_timestart = millis();
  holdtime = 0;
} // return from DecoderRing
