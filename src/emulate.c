#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define int8      unsigned short
#define int32      unsigned int
#include "emulate_struct.h"
#include "emulate.h"

//0xa0
//0b1011


int memory[65536] = {0};  //initialise to 0
int reg[17] = {0};  //0-12, 15, 16 are useful
int state =0; //not used yet
int data_length = 32; //data
int shift_carry_out;



int main(int argc, char *argv[]) {	// e3 a0 10 01
  FILE *file = fopen(argv[1], "rb");  //rb means read binary
  int32 *buffer = malloc(sizeof(int32));  //put the buffer into the heap
  int counter = 0;
  int pc = 0;
  instruction instr;
  FILE* fplog = NULL;
  
  fplog = fopen("output.out", "w+");

  
   
  
  while(fread(buffer, sizeof(int32), 1, file) == 1){  //read file
    instr.instr_32bits = *buffer;  //store instructions into the memory
    fprintf(fplog, "instruction is 0x%8x,****0x%8x\n",instr.instr_32bits,instr.instr_32bits);// e3a01001
    check_instr_set(instr);
    reg[15] += 4;
  }

  
  /*
  while(fread(buffer, sizeof(int8), 1, file) == 1){  //read file
    memory[counter++] = *buffer;  //store instructions into the memory

  }

  printf("%d \n", counter);
  for(int i=0;i++;i<= counter/4)
  {
	instr.instr_Byt.Byt0 = memory[4*i];
	instr.instr_Byt.Byt1 = memory[4*i+1];
	instr.instr_Byt.Byt2 = memory[4*i+2];
	instr.instr_Byt.Byt3 = memory[4*i+3];
	fprintf(fplog," instruction is 0x%8x,****0x%8x\n",instr.instr_32bits,instr.instr_32bits);// e3a01001
	check_instr_set(instr);
	reg[15] += 4;
 
  }
  */
  

  fprintf(fplog, "Register: \n");

  reg[15] += 8;

  for(int x = 0; x < 18; x++) {

    fprintf(fplog, "$%d :  %d(0x%x) \n", x, read_register(x), read_register(x));

  }

  free(buffer);  //delete buffer
  fclose(file);  //close file
  fclose(fplog);

  return 0;
}



void check_instr_set(instruction instr) { //check which type of instructions it is

  if(instr.instr_set.bit2726 == 0){
     
    if((instr.instr_set.bit2524 == 00)&&(instr.instr_set.bit23 == 0)&&(instr.instr_set.bit22 == 0)&&(instr.instr_set.bit74 == 9)) {
      
      printf("%d", instr.instr_set.bit23);
      printf("%d", instr.instr_set.bit74);
      multiply(instr);
      
    }else {
      
      data_processing(instr);
      
    }
  }else if(instr.instr_set.bit2726 == 1){
      
      if((instr.instr_set.bit23 == 0)&&(instr.instr_set.bit22 == 0)) {
	
	single_data_transfer(instr);
	
      }else{
	
	report_err_instruction();
	
      }
    }else if(instr.instr_set.bit2726 == 2){
	
	if(instr.instr_set.bit2524 == 2) {
	  
	  branch(instr);
	}else{
	  
	  report_err_instruction();
	  
	}
    }
	
}
//check_cond() ---wrong
void data_processing(instruction instr){

    int operand1 = read_register(instr.data_pro.Rn);
    int operand2 = get_operand2(instr);
    int result;
    int C;
    int Z;
    int V;

  if(!check_cond(instr.instr_set.Cond)){
   
    switch(instr.data_pro.OpCode) {
    case 0:
      result = operand1&operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 1:
      result = operand1 ^ operand2;  //eor??
      write_register(instr.data_pro.Rd, result);
      break;
    case 2:
      result = operand1 - operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 3:
      result = operand2 - operand1;
      write_register(instr.data_pro.Rd, result);
      break;
    case 4:
      result = operand1 + operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 8:
      result = operand1&operand2;
      break;
    case 9:
      result = operand1 ^ operand2;
      break;
    case 10:
      result = operand1 - operand2;
      break;
    case 12:
      result = operand1 ^ operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 13:
      result = operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    }

    if (result == 0) {
      Z = 1;
    }
    V = build_mask(result, 31, 1);
    C = get_C(instr, result);

    if (instr.data_pro.S == 1) {
      if (Z == 1) {
change_1_bit_content(16, 30, 1);
      }
      change_1_bit_content(16, 29, C);
      change_1_bit_content(16, 28, V);
    }

  }

}


int get_operand2(instruction instr){

  int value;
  if (instr.data_pro.I == 1) {
    value = instr.data_pro_imm.Imm;
    value << 24;
    value >> instr.data_pro_imm.Rotate;
  }else {
    int value_Rm = read_register(instr.data_pro_reg.Rm);
    if (instr.data_pro_reg.bit4 == 0) {
      value = shift(instr.data_pro_reg.Shift_type, instr.data_pro_reg.Integer, value_Rm);
    }else {
      if (instr.data_pro_reg.Integer % 2 == 0) {
	value = read_register(instr.data_pro_reg.Rm);
      }else {
        int value_Rs = read_register(instr.data_pro_reg.Integer/2);
        value = shift(instr.data_pro_reg.Shift_type, value_Rs, value_Rm);
      }
    }
  }
  return value;
}


 int get_C(instruction instr, int result) {

  int C;

  switch(instr.data_pro.OpCode) {

  case 0:
  case 1:
  case 8:
  case 9:
  case 12:
  case 13:
    C = shift_carry_out; break;
  case 2:
  case 3:
  case 4:
  case 10:
    C = result >> 31; break;
  defalt:
    C = 0;
  }

  return C;

}



 
int shift(int shift_type, int shift_amount, int value) {

  int bits;
  int bit_sign;
  int bits_low;
  int bits_high;

  switch(shift_type) {
  case 0:
    bits = build_mask(value, 0, data_length - shift_amount);
    shift_carry_out = build_mask(value, data_length - shift_amount, 1);
    return bits << shift_amount;
  case 1:
    bits = build_mask(value, shift_amount, data_length - shift_amount);
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    return bits;
  case 2:
    bits = build_mask(value, shift_amount, data_length - shift_amount - 1);
    bit_sign = build_mask(value, 31, 1);
    bit_sign << 31;
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    return bits ^ bit_sign;
  case 3:
    bits_high = build_mask(value, shift_amount, data_length - shift_amount);
    bits_low = build_mask(value, 0, shift_amount);
    bits_low << (data_length - shift_amount);
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    return bits_high ^ bits_low;
  }
}

void multiply(instruction instr) {
  int content;
  if(!check_cond(instr.Multiply.Cond)){
    if(instr.Multiply.A){
      content = read_register(instr.Multiply.Rm)*read_register(instr.Multiply.Rs)
	+read_register(instr.Multiply.Rn);
     
    } else {
      content = read_register(instr.Multiply.Rm)*read_register(instr.Multiply.Rs);
    }
    write_register(instr.Multiply.Rd, content);
    }
  if(instr.Multiply.S){
    if(read_register(instr.Multiply.Rd)<0){
      change_1_bit_content(instr.Multiply.Rd,31,1);
    } else {
      change_1_bit_content(instr.Multiply.Rd,31,0);
    }

    if(read_register(instr.Multiply.Rd)==0){
     change_1_bit_content(instr.Multiply.Rd,31,1); 
    } else {
     change_1_bit_content(instr.Multiply.Rd,31,1);
    }
  }
  
  printf("multiply");

}


void single_data_transfer(instruction instr) {

  printf("single data trandfer");

}




void report_err_instruction() {

  printf("wrong instruction!");

}

void change_1_bit_content(int reg, int position, int content){
  int n = (int)pow((double)10,(double) position);
  int result = read_register(reg);
  if(build_mask(read_register(reg), position, 1)){
    if(!content){
      result -= n;
    } 
  } else {
    if(content){
      result += n;
    }
  }
  write_register(reg, result);
}


int read_register(int r){ //read the content of the register, r is 4-bit binary number

  if(r == 14){
  return reg[15];
  } else if (r == 15){
    return reg[16];
  }
  return reg[r];
}

void write_register(int r, int content){ //write the content of the register, r is the 4-bit binary number
  if(r == 14){
  reg[15] = content;
  } else if (r == 15){
    reg[16] = content;
  }
  reg[r] = content;
}

//wrong!!!!!!
int check_cond(int cond) {  //check the initial condition for every instruction
  int n, z, c, v;
  n = build_mask(reg[16], 31, 1);
  z = build_mask(reg[16], 30, 1);
  c = build_mask(reg[16], 29, 1);
  v = build_mask(reg[16], 28, 1);

  switch(cond) {
  case 0:
    if (z){ return 1;}
    else {return 0;}
  case 1:
    if (!z){return 1;}
    else {return 0;}
  case 10:
    if (n==v){return 1;}
    else {return 0;}
  case 11:
    if (n!=v){return 1;}
    else {return 0;}
  case 12:
    if (!z && (n == v)){return 1;}
    else {return 0; }
  case 13:
    if (z || (n!=v)){return 1;}
    else {return 0;}
  case 14:
    return 0;
  defalt:
    return 0;
  }
  return 0;
 }

void branch(instruction instr){
}
//not ture !!!!
/*
void branch(int instr){
  int sign, offset;
  if (check_cond(build_mask(instr, 28, 4))){
    sign = build_mask(instr, 23, 1);
    offset = build_mask(instr, 0, 24)*100;
    int result = change_bi_to_dec(offset, 23);
    if(sign){
      result = result * -1;
    }
    result += change_bi_to_dec(reg[15]);
    reg[15] =  change_dec_to_bi(result, 32);
  }
}
*/

//very useful, build mask from start bpoint 
int build_mask(int instr, int start_point, int length) { //startpoint starts from 0

  instr = instr >> start_point; 
  int mask = 0;
  for(int x = 0; x < length; x++) {
    mask += (int)pow((double)2, (double)x);
  }
  return mask&instr;
}














 
