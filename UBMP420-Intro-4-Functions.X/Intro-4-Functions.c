/*==============================================================================
 Project: Intro-4-Functions             Activity: mirobo.tech/ubmp4-intro-4
 Date:    May 16, 2023
 
 This introductory programming activity for the mirobo.tech UBMP4 demonstrates
 the use of functions and the ability to pass variables between the program's
 main code and the function code.
 
 Additional program analysis and programming activities examine the location of
 function code within the program, introduce function prototypes, and reinforce
 the concepts of global and local variables and making sounds using loops.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Button constant definitions
#define NOBUTTON    0
#define UP          1
#define DOWN        2

// Program variable definitions
unsigned char LED4Brightness = 125;
unsigned char button;

unsigned char read_button(void)
{
    if(SW4 == 0)
    {
        return(UP);
    }
    else if(SW3 == 0)
    {
        return(DOWN);
    }
    else
    {
        return(NOBUTTON);
    }
}

void pwm_LED4(unsigned char pwmValue)
{
    for(unsigned char t = 255; t != 0; t --)
    {
        if(pwmValue == t)
        {
            LED4 = 1;
        }
        __delay_us(20);
    }
    // End the pulse if pwmValue < 255
    if(pwmValue < 255)
    {
        LED4 = 0;
    }
}

int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure I/O for on-board UBMP4 devices
	
    while(1)
	{
        // Read up/down buttons and adjust LED4 brightness
        button = read_button();
        
        if(button == UP && LED4Brightness < 255)
        {
            LED4Brightness += 1;
        }

        if(button == DOWN && LED4Brightness > 0)
        {
            LED4Brightness -= 1;
        }

        // PWM LED4 with current brightness
        pwm_LED4(LED4Brightness);
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

// Move the function code to here in Program Analysis step 6.


/* Learn More -- Program Analysis Activities
 * 
 * 1.   There are three functions in this program. Which function will run
 *      first? How do you know?
 * 
 * 2.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the read_button() function? Is it used by the function to receive a
 *      variable from the main code, or to return a variable to the main code?
 * 
 * 3.   How does the function call statement 'button = read_button();' in the
 *      main code support your answer in 2, above?
 * 
 * 4.   What value will the button variable be set to when no button is pressed?
 *      Where does this value originate?
 * 
 * 5.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the pwm_LED4() function? Where does the value of the variable come from?
 *      Which variable in the function stores this value?
 * 
 * 6.   C language compilers typically read through an entire program file in a
 *      single pass, converting the C code into machine code. The two functions
 *      read_button() and pwm_LED4() are located above the main() function so
 *      that their memory locations and variables will be defined as they are
 *      compiled, before the rest of the program is compiled. When the compiler
 *      later encounters a function call statement to one of the functions, it
 *      already knows that the functions exist, and where to access their code.
 * 
 *      Try moving the read_button() and pwm_LED4() functions to the end of the
 *      program, below the closing brace of the main() function, and build the
 *      program in MPLAB. What happens?
 * 
 *      The compiler should have reported an error since it had not encountered
 *      the functions before trying to compile the function call statements in
 *      the main code. This error can be eliminated by adding function prototype
 *      statements above the main() function in the code.
 * 
 *      A function prototype is an 'empty' copy the function declaration (the
 *      first line of the function). The purpose served by a function prototype
 *      is to inform the compiler about both the name of a function that the
 *      compiler will encounter, as well as the number and type of variables
 *      that the function will use. A C language linker program will take care
 *      of sorting out the names and memory locations of all of the functions
 *      and variables used in a program, and arranges all of them in the proper
 *      order in the microcontroller's memory following the compilation step.
 * 
 *      Let's explore how this works. Leave the functions in their new location,
 *      below the main() function, and add the two function prototype
 *      statements, below, above the main() function and all the program code.
 *      In other words, add these two lines to the original location of the
 *      functions in the program, right below the variable definitions:

unsigned char read_button(void);

void pwm_LED4(unsigned char);

 *      The function prototype for the pwm_LED4() function is slightly different
 *      than the actual function declaration statement which is now in its new
 *      location, after the while loop in the program code. Can you identify
 *      the difference and, thinking about the linear compilation process
 *      described earlier, provide a reason why the difference is not a real
 *      concern during the compilation?
 * 
 *      Building the program now, with the added function prototypes, should
 *      work without generating errors just as it did in the original program.
 *      Try it!
 * 
 * 7.   In the C language functions may be located either above the program code
 *      that calls them, below their function call statements if function
 *      prototypes are supplied before the function call (as seen in 6, above),
 *      or even in completely separate files known as function libraries.
 * 
 *      Function libraries are separate source '.c' files which are typically
 *      joined to your project's main '.c' file using 'include' statements
 *      specifying a '.h' header file. The header files contain both the
 *      variable definitions and the function prototypes for the associated .c
 *      files containing the library function source code, and an typically also
 *      contain comments and descriptions relating to the purpose and use of the
 *      included functions.
 * 
 *      The statement '#include "UBMP420.h"' near the top of this program (and
 *      all of the other Introductory programs), adds external functions into
 *      the main program. The 'UBMP420.h' file contains function prototypes and
 *      other details about the functions located in the 'UBMP420.c' file, along
 *      with various symbolic constants used by both the main program in this
 *      source code file, as well as the program code in the UBMP420.c file.
 * 
 *      Open the UBMP420.c file in the code editor and you will find two
 *      config() functions called from this program's main() function. Are any
 *      values passed between this code and the two config() functions? How do
 *      you know?
 * 
 * 8.   The 'button' variable is a global variable because it was assigned
 *      at the beginning of this program, outside of any functions. Global
 *      variables are available to all functions. How does the 'button'
 *      variable get assigned a value? In which function does this occur?
 * 
 * 9.   Which variable does the value of LED4Brightness get transferred to in
 *      the pwm_LED4() function? Is this variable global, or local to the LED
 *      function? Could the pwm_LED4() function use the LED4Brightness variable
 *      directly, instead of transferring its value to another variable first?
 *      What would happen to the value of the LED4Brightness variable if it was
 *      used in the pwm_LED4() function?
 * 
 * Programming Activities
 * 
 * 1.   It might be useful to have a button that instantly turns LED D4 fully
 *      on or off instead of waiting for it to brighten and dim while the UP
 *      and DOWN buttons are held to change the PWM duty cycle. But, PWM dimming
 *      dimming capability is also a useful feature that should be retained.
 * 
 *      Modify the read_button() and main() functions to use SW2 as an
 *      instant-on button, and SW5 as an instant-off button. Pressing either of
 *      these buttons should over-write the current LED4Brightness value with
 *      either 255 or 0, while still allowing SW3 and SW4 to adjust the
 *      brightness smoothly and in smaller increments when pressed.
 *
 * 2.   Create a new program that uses functions to read the pushbuttons and to
 *      control the LEDs. One LED will light corresponding to each pushbutton
 *      while the button is being pressed, and all of the LEDs should turn off
 *      when the buttons are released.
 * 
 *      Start by creating a function that will return a number (from 1-4)
 *      corresponding to which of the SW2 to SW5 pushbuttons is being pressed,
 *      or that returns a value of 0 if no buttons are pressed. Then, create a
 *      second function that will accept a number from 0 to 4 which will turn
 *      off all of the LEDs in response to a 0 input, or light the appropriate
 *      LED for each of the input numbers from 1-4.
  * 
 * 3.   Create a four note music player using two functions, one to read the
 *      pushbuttons (similar to Activity 2, above), and a second to play a
 *      corresponding sound on the piezo beeper using a loop.
 * 
 *      Start by creating a sound function that will receive a parameter
 *      representing a tone's period or pitch. Then, modify the pushbutton
 *      function to return a different period or pitch in response to each
 *      button being pressed. You could even modify your pushbutton function to
 *      respond to multiple, simultaneous button presses so that your program
 *      is able to play more than four notes while using just four buttons!
 * 
 * 4.   A function that converts an 8-bit binary value into its equivalent
 *      3-digit decimal number might be useful for helping us to debug our
 *      programs (as you will see in the next activity). Create a function that
 *      converts an 8-bit binary number into three decimal character variables
 *      representing the hundreds, tens, and ones digits of a (binary) number
 *      passed to it. For example, passing the function the value of 142 will
 *      result in the hundreds variable containing the value 1, the tens
 *      variable containing 4, and the ones variable 2. How could you test this
 *      function to verify that it works? Try it! (Hint: the BCD number system
 *      represents the decimal digits 0-9 using only 4 bits. Can you display
 *      the equivalent BCD digits on the LEDs?)
 */
