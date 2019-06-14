#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bit_field.h"

#define DATA_LENGTH 32

int main(int argc, char *argv[]);

//check instruction type and its conditions
void check_instr_set(instruction instr);
uint32_t check_cond(uint32_t cond);
void report_err_instruction();

//data processing and its helper functions 
void data_processing(instruction instr);

uint32_t get_operand2(instruction instr);
uint32_t shift(uint32_t shift_type, uint32_t shift_amount, uint32_t value, instruction instr);
void set_C(int result, int opCode);

//multiply instruction
void multiply(instruction instr);

//single_data_transferand and its helper funcitons
void single_data_transfer(instruction instr);

void single_data_load(uint32_t rn, instruction instr);
void single_data_store(uint32_t rn, instruction instr);
uint32_t get_offset(instruction instr);
uint32_t add_sub_offset(instruction instr);

//branch 
void branch(instruction instr);

//registers
uint32_t read_register(uint32_t r);
void write_register(uint32_t r, uint32_t content);
void change_1_bit_content(uint32_t reg, uint32_t position, uint32_t content);

//other helper functinons
uint32_t get_4bit_memory(uint32_t start_point);
uint32_t build_mask(uint32_t instr, uint32_t start_point, uint32_t length);
instruction readInstruction();

//PIN
int if_in_pin_area(uint32_t memory_address);
int if_in_control_area(uint32_t memory_address);
void clear_pin();





 
