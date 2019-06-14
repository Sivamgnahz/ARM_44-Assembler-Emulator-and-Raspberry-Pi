#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#define LED 1

int main(int argc, char *argv[]);
int filter(char letter);//extract letter and number only
                        //change the string to capital word(if the char is a letter)

int letter_to_morse(char letter); //change the word to the moss code
int number_to_morse(char letter);
int symbol_to_morse(char letter);

void morse_to_light(int morse);//change the morse code to light
void read_sentence(char *sentence);

int invert(int morse);

void dit();
void dah();
void pause_short();
void pause_medium();

enum char_type{
    LOWER_LETTER = 0,
    UPPER_LETTER = 1,
    NUMBER = 2,//0, 1, 2,..9
    NORMAL_SYMBOL = 3, //' ', ',', '!', '.', '?'
    OTHERS = 4,
};

