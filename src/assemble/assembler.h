#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bit_field.h"

int main();
int Read_from_file(char argv[]); //read every character from the file and transfer its register, value... into the table
int analysis_first_word(int line); //analyse the first word in the instruction, i.e. mov, add, sub,...
//because normally every instruction begin with 3 characters
void analysis_last_label(int line);
//label case

void analysis_b_label(int line);//b foo:
//label begin with b 

void analysis_bxx_label(int line);
//label with the form of bxx

void analysis_second_word(int line);
//analysis the second  'word' in a normal instruction: it must be a register 

void analysis_third_word_and_after(int line);
//consider all the word after the second word ...(many different cases)

void analysis_third_word_as_number(int line);//#
//store a number(if the word is a number)

int analysis_number_10_or_16_based(int line);//#1
//transfer a number into the table(if its a ten based number)

int analysis_16_based_help_func(int line);//#0x
//transfer a number into the table(if its a hex based number)

void data_processing(int line, instruction *instr);
//store an instruction as an instruct from the table
//by extract every useful block for this instruction 

void branch(int line, instruction *instr);
//same as above

void single_data_transfer(int line, instruction *instr);
//same as above

void multiply(int line, instruction *instr);
//same as above

//instrution table
//0:the number of instructions
//1:type of instructions
//2:specific type of the data processing instruction
//3:Rn
//4:type of the number
//5:absolute value of the number
//6:Rd
//7:Rs
//8:Rm
//9:Lsr
//10:total number of registers except the first one

//first pass second column ---instruction type

enum instruction_type {
    INSTRUTYPE_DATA_PROCESSING = 1,
    INSTRUTYPE_MULTIPLY = 2,
    INSTRUTYPE_SINGLE_DATA_TRANSFER = 3,
    INSTRUTYPE_BRANCH = 4, 
    INSTRUTYPE_SPECIAL = 5,
    INSTRUTYPE_LABEL = 6,
};

//first pass third coloumn ---specific type when it is data processing or multiply
enum instruction_cMd {//Instr_Table[line][2]
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8, 
    TEQ = 9, 
    CMP = 10,
    ORR = 12,
    MOV = 13, // type 1

    INSTRU_MUL = 0x20,
    INSTRU_MLA = 0X21,//type 2

    INSTRU_LDR = 0x30,
    INSTRU_STR = 0x31,//type 3

    INSTRU_BEQ = 0,
    INSTRU_BNE = 1,
    INSTRU_BGE = 10,
    INSTRU_BLT = 11,
    INSTRU_BGT = 12,
    INSTRU_BLE = 13,
    INSTRU_B = 14,//type 4

    INSTRU_ANDEQ = 0x50,//type 5
    INSTRU_LSL = 0x51,
};

//first pass fifth column
enum typ_Number { 
    NUM_POUND = 0, 
    NUM_POUND_NEGATIVE = 1,
    NUM_EQ = 2,

    NUM_R = 3,

    NUM_BRACKET_R = 4,
    NUM_BRACKET_POUND = 5,
    NUM_BRACKET_POUND_NEGATIVE = 6,
    NUM_BRACKET_LSR_POUND = 7,
    NUM_BRACKET_LSR_R = 8,
    NUM_BRACKET_END_POUND = 9,
    NUM_BRACKET_END_POUND_NEGATIVE = 10,
    NUM_BRACKET_X = 11,

    NUM_R_LSR_R = 0x10,
    NUM_R_LSR_POUND = 0x11,

    HAS_LSR = 1,
    HAS_BRACKET_LSR = 2,
};

//the 4-bit 'cond', as mentioned in the spec(a 4 bit number)
enum instr_Cond {
    BEQ = 0,
    BNE = 1,
    BGE = 10,
    BLT = 11,
    BGT = 12,
    BLE = 13,
    ALWAYS = 14, //type 4

};