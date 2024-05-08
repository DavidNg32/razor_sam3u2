/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */
static SequenceGame game;
static u16 seed = 65535;
static bool gamestart = FALSE;
static u32 startTime = 0; // Initialize startTime
static bool beepFlag = FALSE; // Flag to control beep







/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void playSingleNote(u16 note) {
    // Define the duration of the note (in milliseconds)
    u16 duration = EN;  // You can adjust this to change the length of the beep

    // Set the PWM frequency to the provided note
    PWMAudioSetFrequency(BUZZER2, note);

    // Turn on the PWM audio
    PWMAudioOn(BUZZER2);

    // Wait for the duration of the note
    delay(duration);

    // Turn off the PWM audio
    PWMAudioOff(BUZZER2);
}
void playSwitchingSound(void) {
    // Define the notes for the switching sound
    u16 notes[] = {
        NOTE_E4, NOTE_G4
    };
    
    // Define the durations of each note (in milliseconds)
    u16 durations[] = {
        EN, EN
    };

    // Loop through the notes and play each one
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
        PWMAudioSetFrequency(BUZZER2, notes[i]);  // Set the PWM frequency
        PWMAudioOn(BUZZER2);                      // Turn on the PWM audio
        delay(durations[i]);                       // Wait for the duration of the note
        PWMAudioOff(BUZZER2);                     // Turn off the PWM audio
        delay(50);                                 // Add a small delay between notes
    }
}
void playSadTune(void) {
    // Define the notes of the melancholic tune
    u16 notes[] = {
        NOTE_A3, NOTE_G3, NOTE_F3, NOTE_D3, 
        NOTE_C3, NOTE_E3, NOTE_C3, NOTE_D3, 
        NOTE_F3, NOTE_A3, NOTE_G3, NOTE_C4
    };
    
    // Define the durations of each note (in milliseconds)
    u16 durations[] = {
        HN, EN, EN, QN, 
        EN, QN, EN, EN, 
        QN, QN, EN, FN
    };

    // Loop through the notes and play each one
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
        PWMAudioSetFrequency(BUZZER2, notes[i]);  // Set the PWM frequency
        PWMAudioOn(BUZZER2);                      // Turn on the PWM audio
        delay(durations[i]);                       // Wait for the duration of the note
        PWMAudioOff(BUZZER2);                     // Turn off the PWM audio
        delay(50);                                 // Add a small delay between notes
    }
}
void playFunTune(void) {
    // Define the notes of the fun tune
    u16 notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
    
    // Define the durations of each note (in milliseconds)
    u16 durations[] = {QN, QN, QN, QN, QN, QN, QN, QN};

    // Loop through the notes and play each one
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
        PWMAudioSetFrequency(BUZZER2, notes[i]);  // Set the PWM frequency
        PWMAudioOn(BUZZER2);                      // Turn on the PWM audio
        delay(durations[i]);                       // Wait for the duration of the note
        PWMAudioOff(BUZZER2);                     // Turn off the PWM audio
        delay(50);                                 // Add a small delay between notes
    }
}
void toggleGameMode(){
  if(game.gamemode == NORMAL){
    game.gamemode = BLIND;
    LcdClearChars(LINE1_START_ADDR, 20);
    LcdMessage(LINE1_START_ADDR + 4, "BLIND   MODE");
  }
  
  else if(game.gamemode == BLIND){
    game.gamemode = NORMAL;
    LcdClearChars(LINE1_START_ADDR, 20);
    LcdMessage(LINE1_START_ADDR + 4, "NORMAL  MODE");
  }
}
void displayScore(SequenceGame* game)
{
    u8 au8score[5];  // Assuming the score can be up to 9999
    intToString(game->score, au8score);
    LcdClearChars(LINE2_END_ADDR - 5, 5);  // Clear previous score
    LcdMessage(LINE2_END_ADDR - 5, au8score);  // Display new score
}

void gameOver(u8 u8Address_, u8 *u8Message){
    gamestart = FALSE;
    LcdCommand(LCD_CLEAR_CMD);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
    LcdMessage(u8Address_, u8Message);
    LcdMessage(LINE2_START_ADDR, "SCORE:");
    playSadTune();
    
}
void resetAllButtonStates(){
  ButtonAcknowledge(BUTTON0);
  ButtonAcknowledge(BUTTON1);
  ButtonAcknowledge(BUTTON2);
  ButtonAcknowledge(BUTTON3);
  LedOn(LCD_BLUE);
  LedOn(LCD_RED);
  LedOn(LCD_GREEN);
  PWMAudioOff(BUZZER2);
}

void displayInput(){
  LcdMessage(LINE2_START_ADDR, "A");
  LcdMessage(LINE2_START_ADDR + 6, "B");
  LcdMessage(LINE2_START_ADDR + 13, "C");
  LcdMessage(LINE2_END_ADDR, "D");
  
}

void delay(uint32_t milliseconds)
{
    uint32_t delay_count = milliseconds * 1000; // Convert milliseconds to microseconds
    while(delay_count--)
    {
        // Do nothing
    }
}

void intToString(int number, char *str) 
{
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (number < 0) 
    {
        isNegative = 1;
        number = -number;
    }

    // Handle the case of 0 separately
    if (number == 0) 
    {
        str[i++] = '0';
    }
    else 
    {
        // Extract digits from the number and store them in reverse order
        while (number != 0) 
        {
            int digit = number % 10;
            str[i++] = '0' + digit; // Convert digit to ASCII character
            number /= 10;
        }
    }

    // Add negative sign if necessary
    if (isNegative) 
    {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    int j = 0;
    int length = i;
    i--;
    while (j < i) 
    {
        char temp = str[j];
        str[j] = str[i];
        str[i] = temp;
        j++;
        i--;
    }
}

void InitializeGame(SequenceGame* game, bool displaySequence){
  
   const int maxLength = MAX_CODE_LENGTH;  // Maximum length of the sequence
   int sequenceLength = rand() % maxLength + 1;  // Random length between 1 and maxLength
   char* sequence = (char*)malloc(sequenceLength + 1);
    
    for(int i = 0; i < sequenceLength; i++){
        sequence[i] = 'A' + (rand() % 4);  // Randomly select 'A', 'B', 'C', or 'D'
    }
  sequence[sequenceLength] = '\0'; // Null terminator
  strcpy(game->sequence, sequence);
  game->current_index = 0;
  game->state = WAITING1;
  game->sequenceLength = sequenceLength;
  
  if(displaySequence){
        LcdClearChars(LINE1_START_ADDR, 14);
        LcdMessage(LINE1_START_ADDR, sequence);
    }
  
}
char* input(SequenceGame* game, char button){
  if(game->state != WAITING1){
     LcdMessage(LINE1_START_ADDR, "Game Over");
  }
  if(game->gamemode == NORMAL){
    LcdClearChars(LINE1_START_ADDR + game->current_index, 1);
  }
  else if(game->gamemode == BLIND){
    LcdClearChars(LINE1_START_ADDR, 14);
  }
  
  if(button == game->sequence[game->current_index]){
    game->current_index++;
    if(game->current_index == game->sequenceLength){
      game->state = COMPLETED1;
      game->score++;
    }
  }
  else{
    game->state = FAILURE1;
    gameOver(LINE1_START_ADDR + 1, "BOMB HAS EXPLODED!");
    displayScore(game);
  }
}

void resetGame(SequenceGame* game){
  game->current_index = 0;
  game->state = WAITING1;
}

void ADCCallBack(u16 result){
  seed = result;
}

void timer(){
  u32 currentTime = G_u32SystemTime1s;
  u32 elapsedTime = currentTime - startTime;
  static int previousTimeLeft = GAME_TIME;

  if(elapsedTime >= GAME_TIME){
    game.state = FAILURE1;
    gameOver(LINE1_START_ADDR + 5, "TIME IS UP!");
    displayScore(&game);
    return;
  }

  int timeLeft = GAME_TIME - elapsedTime;

  if(timeLeft != previousTimeLeft){
    u8 au8message[3];
    intToString(timeLeft, au8message);
    LcdMessage(LINE1_END_ADDR - 2, au8message);
    previousTimeLeft = timeLeft;
    beepFlag = TRUE;
  }

  // Update startTime if it's the first time entering the timer function
  if(startTime == 0){
    startTime = currentTime;
  }
  
  if(beepFlag){
    PWMAudioSetFrequency(BUZZER2, NOTE_G6_SHARP);
    PWMAudioOn(BUZZER1);
    LedOn(RED);
    delay(500); // Delay for beep duration
    PWMAudioOff(BUZZER1);
    LedOff(RED);
    beepFlag = FALSE;
  }

  
}


void UserApp1Initialize(void)
{
  Adc12AssignCallback(ADC12_BLADE_AN0, ADCCallBack);
  Adc12StartConversion(ADC12_BLADE_AN0);
  srand(50); //TODO: CHange magic number for real random see
  game.score = 0;
  game.gamemode = NORMAL;
  LcdMessage(LINE1_START_ADDR + 4, "NORMAL  MODE");
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  
  if(seed != 65535 && gamestart == FALSE){
    srand(seed); 
    if(WasButtonPressed(BUTTON0)){
      resetAllButtonStates();
      gamestart = TRUE;
      LcdCommand(LCD_CLEAR_CMD);
      displayInput();
      InitializeGame(&game, TRUE);
      playFunTune();
    } 
    
    if(WasButtonPressed(BUTTON3)){
      resetAllButtonStates();
      playSwitchingSound();
      toggleGameMode();
    }
  }
  else if (seed == 65535 && gamestart == FALSE){
    return;
  }
  char button = '\0';
  if(gamestart == TRUE){
    timer();
    if(WasButtonPressed(BUTTON0)){
      ButtonAcknowledge(BUTTON0);
      button =  'A';
      playSingleNote(NOTE_C4);
    }
    if(WasButtonPressed(BUTTON1)){
      ButtonAcknowledge(BUTTON1);
      button = 'B';
      playSingleNote(NOTE_E4);
    }
    if(WasButtonPressed(BUTTON2)){
      ButtonAcknowledge(BUTTON2);
      button = 'C'; 
      playSingleNote(NOTE_G4);
    }
    if(WasButtonPressed(BUTTON3)){
      ButtonAcknowledge(BUTTON3);
      button = 'D';
      playSingleNote(NOTE_C5);
    }
    if(game.state == WAITING1 && button != '\0'){
      input(&game, button);
      if(game.state == COMPLETED1){
        // Reset the game state and initialize a new game
        LedOff(LCD_BLUE);
        LedOff(LCD_RED);
        playFunTune();
        resetAllButtonStates();
        InitializeGame(&game, TRUE);
    }
  }
 
  }
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
