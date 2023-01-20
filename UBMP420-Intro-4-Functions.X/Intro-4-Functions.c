/*==============================================================================
 Project: Intro-4-Functions             Activity: mirobo.tech/ubmp4-intro-4
 Date:    January 20, 2023
 
 This program demonstrates the use of functions, and variable passing between
 the main and function code.
 
 Additional program analysis and programming activities examine function code
 location, function prototypes, and reinforce the concepts of global and local
 variables.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Button constant definitions
const char noButton = 0;
const char UP = 1;
const char DOWN = 2;

// Program variable definitions
unsigned char LED4Brightness = 125;
unsigned char button;

unsigned char read_button(void)
{
    if(SW4 == 0)
    {
        return(UP);
    }
    else if(SW5 == 0)
    {
        return(DOWN);
    }
    else
    {
        return(noButton);
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
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
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

// Move the function code to here in Program Analysis step 5.


/* Learn More -- Program Analysis Activities
 * 
 * 1.   Which function in this program will run first? How do you know?
 * 
 * 2.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the read_button() function? Is it used by the function to receive a
 *      variable from, or return a variable to the main code?
 * 
 * 3.   How does the function call statement 'button = read_button();' in the
 *      main code support your answer in 2, above?
 * 
 * 4.   What value will the button variable be set to when no button is pressed?
 *      Where does this value originate?
 * 
 * 5.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the pwm_LED4() function? Where does the value of the variable come from?
 *      Which variable in the function does this value get stored in?
 * 
 * 6.   C language compilers typically read through an entire program file in a
 *      single pass, converting C code into machine code. The two functions,
 *      read_button() and pwm_LED4(), are located above the main() function
 *      so that the memory locations and variables will be defined as they are
 *      compiled, and before the rest of the program is compiled. When the
 *      compiler later encounters a function call statement to one of the
 *      functions, it knows how to access them.
 * 
 *      Try moving the read_button() and pwm_LED4() functions to the end of the
 *      program, below the closing brace of the main() function, and build the
 *      code. What happens?
 * 
 *      The compiler should have reported an error since it had not encountered
 *      the functions before trying to compiler the function call statements.
 *      We can eliminate this error by adding function prototype statements
 *      above the main() function in the code.
 * 
 *      The function prototype is almost a copy the function declaration (the
 *      first line of the function), and informs the compiler about both the
 *      function's name as well as the number and type of variables that the
 *      function will use. A C language 'linker' program will take care of
 *      sorting out the names and memory locations of all of the functions and
 *      variables in a program, and arranging them into the microcontroller's
 *      memory following the compilation step.
 * 
 *      Let's try this out. Leave the functions in their new location, below
 *      the main() function, and add the two function prototype statements, 
 *      below, above the code in the main() function. In other words, add these
 *      in the functions' original location, above the main() function:

unsigned char read_button(void);

void pwm_LED4(unsigned char);

 *      What is the difference between the function prototype for pwm_LED4()
 *      and the actual pwm_LED4() function declaration statement later in the
 *      code?
 * 
 *      Building the program now, with the added function prototypes, should
 *      work without generating errors just as it did in the original program.
 * 
 * 7.   In C, functions may be located before the code that calls them, after
 *      their function calls if a function prototype is supplied before the
 *      function call, or even in completely separate files known as function
 *      libraries.
 * 
 *      Function libraries are typically joined to your project's main '.c' 
 *      file using an 'include' statement specifying a '.h' header file. The
 *      header file contains both the variable definitions and the function
 *      prototypes for an associated .c file, which contains the additional
 *      library function code.
 * 
 *      The '#include "UBMP420.h"' statement near the top of this program is an
 *      example of a statement that does exactly this, adding external
 *      functions into our program. The 'UBMP420.h' file contains function
 *      prototypes for the functions located in the 'UBMP420.c' file, along
 *      with various symbolic constants used by both the main program in this
 *      source code file, as well as and the program code in the UBMP420.c file.
 * 
 *      Open the UBMP420.c file to find the OSC_config() and UBMP4_config()
 *      functions called from the main() function in this program. Are any
 *      values passed between this code and the two setup functions? How do
 *      you know?
 * 
 * 8.   The 'button' variable is a global variable because it was assigned
 *      at the beginning of the program, outside of any functions. Global
 *      variables are available to all functions. How does the 'button'
 *      variable get assigned a value? In which function does this occur?
 * 
 * 9.   Which variable does the value of LED4Brightness get transferred to in
 *      the pwm_LED4() function? Is this variable global, or local to the LED
 *      function? Could the pwm_LED4() function use the LED4Brightness variable
 *      directly, instead of transferring its value to another variable?
 * 
 * Programming Activities
 * 
 * 1.   It might be useful to have a button that instantly turns LED D4 fully
 *      on or off instead of waiting for it to brighten and dim while the UP
 *      and DOWN buttons are held to change the PWM duty cycle. But, PWM dimming
 *      dimming capability is still a useful feature that should be retained.
 * 
 *      Modify the read_button() and main() functions to use SW3 as an
 *      instant-on button, and SW2 as an instant-off button. Pressing either of
 *      these buttons should over-write the current LED4Brightness value with
 *      either 255 or 0, while still allowing SW4 and SW5 to adjust the
 *      brightness in smaller increments when pressed.
 *
 * 2.   Create a new program with a function that will return a number from 1-4
 *      corresponding to which of the SW2 to SW5 switches is pressed, or return
 *      0 if no switches are pressed. Then, create a second function that will
 *      accept a number from 1 to 4 that lights the corresponding LED beside
 *      each button. A value of 0 should turn off all of the lights. 
 * 
 * 3.   Create a sound function that receives a parameter representing a tone's
 *      period. Modify your button function, above, to return a variable that
 *      will be passed to the sound function to make four different tones.
 * 
 * 4.   A function that converts an 8-bit binary value into its 3-digit decimal
 *      equivalent might be useful for helping us to debug our programs. Create
 *      a function that converts an 8-bit binary number into three decimal
 *      character variables representing the hundreds, tens, and ones digits
 *      of a binary number passed to it. For example, passing the function
 *      a value of 142 will result in the hundreds variable containing the
 *      value 1, the tens variable containing 4, and the ones variable 2. How
 *      could you test this function to verify that it works? Try it! (Hint:
 *      the BCD number system can represent the decimal digits 0-9 using only
 *      4 bits. Can you display the BCD digits on the LEDs?)
 */
