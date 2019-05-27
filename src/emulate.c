#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdint.h"
#define int8 uint8_t
//#define Tstbug
#include "emulate_struct.h"
#include "emulate.h"

//0xa0
//0b1011
int8 memory[65536] = {0};  //initialise to 0, store instructions and data
int32 reg[17] = {0};  //0-12, 15, 16 are useful, store registers
CPSR_type CPSR; 
instruction instr;
int data_length = 32; //data have 32 bits
int shift_carry_out = 0;
int add_carry_out = 0;
FILE* fplog = NULL;//xj
int instruction_is_branch =0;  


int main(int argc, char *argv[]) {	
	
  FILE *file = fopen(argv[1], "rb");  //rb means read binary
  int8 *buffer = malloc(sizeof(int8));  
  int counter = 0;
  int Fetched0Byt =0;//Xj pipeline
  int Fetched1Byt =0;
  int Fetched2Byt =0;
  int Fetched3Byt =0;
  CPSR.CPSR_32bits = 0x00000000;
  int32 PC = 0x00000000;

  
  fplog = fopen("output.out", "w+");
  if(file == NULL)
  	fprintf(fplog,"can not opn file\n");
  while(fread(buffer, sizeof(int8), 1, file) == 1){  
    memory[counter++] = *buffer;  //store instructions into the memory
  }
  /*
  Fetched0Byt = memory[reg[15]-4];
  Fetched1Byt = memory[reg[15]-3];
  Fetched2Byt = memory[reg[15]-2];
  Fetched3Byt = memory[reg[15]-1];
  reg[15]+= 4;//Xj pipeline
	

  for(int i=0;i < counter;i++)  //memory: 8 bit per block, 4 block per instruction
  {
	instr.instr_Byt.Byt0 = Fetched0Byt;
	instr.instr_Byt.Byt1 = Fetched1Byt;
	instr.instr_Byt.Byt2 = Fetched2Byt;
	instr.instr_Byt.Byt3 = Fetched3Byt;
*/
#ifdef Tstbug	
	fprintf(stdout,"  instruction is 0x%08x\n",instr.instr_32bits);
#endif	
	if(instr.instr_32bits!=0)
		check_instr_set();
#ifdef Tstbug
	fprintf(stdout,"  instruction is 0x%08x\n",instr.instr_32bits);
	fprintf(stdout,"  reg[15]  pc is 0x%08x\n",reg[15]);
#endif
/*
	if(instruction_is_branch!=1){
	  	Fetched0Byt = memory[reg[15]-4];
	  	Fetched1Byt = memory[reg[15]-3];
  		Fetched2Byt = memory[reg[15]-2];
		Fetched3Byt = memory[reg[15]-1];
  		reg[15]+= 4;//Xj pipeline
		}
	else{
		Fetched0Byt = memory[reg[15]];
		Fetched1Byt = memory[reg[15]];
		Fetched2Byt = memory[reg[15]];
		Fetched3Byt = memory[reg[15]];
	 	reg[15]+= 12;//Xj pipeline
		}
  }
  */
while (1) {
  readInstruction();
  if (!instr.instr_32bits) {
    break;
  }
  check_instr_set();
  reg[15] += 4;
}
reg[15] += 8;
  
  
  fprintf(stdout, "Registers:\n");
#ifdef Tstbug
  for(int x = 0; x < 16; x++) {
#else
  for(int x = 0; x < 13; x++) {
#endif  	
    printf("$%-3d", x);
    printf(": %10d (0x%08x)\n", reg[x], reg[x]);
  }
  fprintf(stdout, "PC  : %10d (0x%08x)\n", reg[15], reg[15]);
  fprintf(stdout, "CPSR: %10d (0x%08x)\n", CPSR.CPSR_32bits, CPSR.CPSR_32bits);
  free(buffer);  //delete buffer
  fclose(file);  //close file
#ifdef Tstbug	
	printf("counter is  %d \n", counter);
#endif
  fprintf(stdout,"Non-zero memory:\n");
  for(int i=0;i!= 65536/4;i++)
  {
  	if((memory[4*i]!=0)||(memory[4*i+1]!=0)||(memory[4*i+2]!=0)||(memory[4*i+3]!=0))
  		fprintf(stdout,"0x%08x: 0x%02x%02x%02x%02x\n",i*4,memory[4*i],memory[4*i+1],memory[4*i+2],memory[4*i+3]);
   }
  fclose(fplog);
  return 0;
}

void readInstruction() { 
  int32 pc = reg[15];
  instr.instr_Byt.Byt0 = memory[pc];
	instr.instr_Byt.Byt1 = memory[pc + 1];
	instr.instr_Byt.Byt2 = memory[pc + 2];
	instr.instr_Byt.Byt3 = memory[pc + 3];
}
void check_instr_set() { //check which type of instructions it is
	instruction_is_branch = 0;
	if(instr.instr_set.bit2726 == 0){
    	if((instr.instr_set.bit2524 == 00)&&(instr.instr_set.bit23 == 0)&&(instr.instr_set.bit22 == 0)&&(instr.instr_set.bit74 == 9)) {
      		multiply();
    	}else {
      		data_processing();
    	}
  	}else if(instr.instr_set.bit2726 == 1){
		if((instr.instr_set.bit22 == 0)&&(instr.instr_set.bit21 == 0)) {
			single_data_transfer();	
      	}else{
			report_err_instruction();
      	}
    }else if(instr.instr_set.bit2726 == 2){
		if(instr.instr_set.bit2524 == 2) {
			branch();
		}else{
			report_err_instruction();
		}
    }
    else
		report_err_instruction();
}

//check_cond() ---wrong
void data_processing(){

    int operand1 = read_register(instr.data_pro.Rn);
    int operand2 = get_operand2();
    int result;
    int C;
    int Z;
    int V;

#ifdef Tstbug    
  fprintf(fplog,"  data_processing....\n");//xj
#endif
  if(check_cond(instr.instr_set.Cond)){
   
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
      if((result < operand1 || result < operand2) &&
      operand1 > 0){
	      add_carry_out = 1;
      }else {
	      add_carry_out = 0;
      }
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
      result = operand1 | operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 13:
      result = operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    }
  }

  if (instr.data_pro.S) {
	    if (result == 0) {
	    	CPSR.CPSR_Bits.Z =1;
	    } else {
        CPSR.CPSR_Bits.Z =0;
      }

      if(result < 0){
        CPSR.CPSR_Bits.N = 1;
      } else {
        CPSR.CPSR_Bits.N = 0;
      }
    	
	    set_C(result);
  }
}


int get_operand2(){

  int value;
  if (instr.data_pro.I == 1) {
    value = instr.data_pro_imm.Imm;
    if (instr.data_pro_imm.Rotate != 0) {
      value <<= (32 - (2 * instr.data_pro_imm.Rotate)); 
    }
  }else {
    int32 value_Rm = read_register(instr.data_pro_reg.Rm);
    if (instr.data_pro_reg.bit4 == 0) {
      value = shift(instr.data_pro_reg.Shift_type, instr.data_pro_reg.Integer, value_Rm);
    }else {
      if (instr.data_pro_reg.Integer % 2 == 0) {
	value = read_register(instr.data_pro_reg.Rm);
      }else {
        int32 value_Rs = read_register(instr.data_pro_reg.Integer/2);
        value = shift(instr.data_pro_reg.Shift_type, value_Rs, value_Rm);
      }
    }
  }
  return value;
}


void set_C(int result) { //data processing helper function 
#ifdef Tstbug
  printf("C value:%d \n", CPSR.CPSR_Bits.C);
  printf("result:%8x \n", result);
#endif
  switch(instr.data_pro.OpCode) {

  case 0:
  case 1:
  case 8:
  case 9:
  case 12:
  case 13:
    CPSR.CPSR_Bits.C = shift_carry_out; break;
  case 4:
    CPSR.CPSR_Bits.C = add_carry_out; break;
  case 2:
  case 3:
  case 10:
    if (result >= 0) { 
      CPSR.CPSR_Bits.C = 1;
    }else {
      CPSR.CPSR_Bits.C = 0;
    }
    break;
  defalt:
    CPSR.CPSR_Bits.C = 0;
  }
  shift_carry_out = 0;
  add_carry_out = 0;
#ifdef Tstbug
  printf("C value:%d \n", CPSR.CPSR_Bits.C);
  printf("%8x", instr.instr_32bits);
#endif

}


int32 shift(int shift_type, int shift_amount, int value) { //data processing helper functnion

  int bits;
  int bit_sign;
  int bits_low;
  int bits_high;

  switch(shift_type) {
  case 0:
    bits = build_mask(value, 0, data_length - shift_amount);
    if (instr.data_pro.S == 1) {
    shift_carry_out = build_mask(value, data_length - shift_amount, 1);
    }
    return bits << shift_amount;
  case 1:
    bits = build_mask(value, shift_amount, data_length - shift_amount);
    if (instr.data_pro.S == 1) {
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits;
  case 2:
    bits = build_mask(value, shift_amount, data_length - shift_amount - 1);
    bit_sign = build_mask(value, 31, 1);
    bit_sign <<= 31; //all should be sign
    if (instr.data_pro.S == 1) {
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits ^ bit_sign;
  case 3:
    bits_high = build_mask(value, shift_amount, data_length - shift_amount);
    bits_low = build_mask(value, 0, shift_amount);
    bits_low <<= (data_length - shift_amount);  //add =
    if (instr.data_pro.S == 1) {
    shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits_high ^ bits_low;
  }
}

void multiply() { 
  int content;
#ifdef Tstbug  
  fprintf(fplog,"  multiply....\n");//xj
  printf("multiply....\n");
#endif    
  if(check_cond(instr.Multiply.Cond)){
    
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
  
}

void change_1_bit_content(int reg, int position, int content){ 
  int n = (int)pow((double)10,(double) position);
  int32 result = read_register(reg);
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


int32 read_register(int r){ //read the content of the register, r is 4-bit binary number
  return reg[r];
}

void write_register(int r, int content){ //write the content of the register, r is the 4-bit binary number
  reg[r] = content;
}

int check_cond(int cond) {  //check the initial condition for every instruction
  int n, z, c, v;
  n = CPSR.CPSR_Bits.N;	//n = build_mask(reg[16], 31, 1);
  z = CPSR.CPSR_Bits.Z;	//z = build_mask(reg[16], 30, 1);
  c = CPSR.CPSR_Bits.C;	//c = build_mask(reg[16], 29, 1);
  v = CPSR.CPSR_Bits.V;	//v = build_mask(reg[16], 28, 1);

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
    return 1;
  defalt:
    return 0;
  }
  return 0;
 }


void branch(){
#ifdef Tstbug
  fprintf(fplog,"  branch....\n");//xj
  printf("branch....\n");
#endif
int32 offset_shift = instr.Branch.Offset;
instruction_is_branch = 1;
    if (check_cond(instr.Branch.Cond)){
      if (offset_shift & (1 << 23)) {
        offset_shift |= (1 << 9 - 1) << 23;
      }
      offset_shift <<= 2;
      reg[15] = reg[15] + offset_shift + 8 - 4;
#ifdef Tstbug
    printf("shift:%8x \n", offset_shift);
    printf("shift:%8d \n", offset_shift);
#endif
reg[15] = reg[15] + offset_shift;
#ifdef Tstbug
    printf("reg[15]:%8x \n", reg[15] + offset_shift);
    printf("shift:%8d \n", reg[15] + offset_shift);
#endif
    }
}


//build mask from start bpoint 
int32 build_mask(int32 instr, int start_point, int length) { //startpoint starts from 0
  instr = instr >> start_point; 
  int32 mask = 0;
  for(int x = 0; x < length; x++) {
    mask = 1 << (length + 1) - 1;
  }
  return mask&instr;
}



void single_data_transfer() { //single data transfer instruction

  if(check_cond(instr.SingleDataTransfer.Cond)){

    if(instr.SingleDataTransfer.L){  //load  
      if(instr.SingleDataTransfer.P){  //add before  
      add_sub_offset(instr);
      single_data_load(instr);
    } else {
	    single_data_load(instr);
	   add_sub_offset(instr);
      }
    } else { //store     
      if(instr.SingleDataTransfer.P){
    	add_sub_offset(instr);  //add/sub before
    	single_data_store(instr);
      } else {  //add/sub after 
    	single_data_store(instr);
	    add_sub_offset(instr);
      }
    }
  }
}


void single_data_load(){
  int32 result;	
  int32 rn = read_register(instr.SingleDataTransfer.Rn);
  if(rn<0x10000){//memory[65536]
  	result = get_4bit_memory(rn);
	write_register(instr.SingleDataTransfer.Rd, result);
	} else {
    printf("Error: Out of bounds memory access at address 0x%08x\n", rn);
  }
}

void single_data_store(){
	
  int32 rn = read_register(instr.SingleDataTransfer.Rn);
  int32 result = read_register(instr.SingleDataTransfer.Rd);
  if(rn < 0x10000)//memory[65536]
    for(int i = 0; i<4;i++){
        	memory[rn+i] = build_mask(result, i*8,8);
    }

}


int32 get_offset(){
  int value = 0;
  if (!instr.SingleDataTransfer.I) {
    value = instr.SingleDataTransfer.Offset;
  }else {
    int32 value_Rm = read_register(instr.data_pro_reg.Rm);
    if (instr.data_pro_reg.bit4 == 0) {
      value = shift(instr.data_pro_reg.Shift_type, instr.data_pro_reg.Integer, value_Rm);
    }else {
      if (instr.data_pro_reg.Integer % 2 == 0) {
	value = read_register(instr.data_pro_reg.Rm);
      }else {
        int32 value_Rs = read_register(instr.data_pro_reg.Integer/2);
        value = shift(instr.data_pro_reg.Shift_type, value_Rs, value_Rm);
      }
    }
  }    
  return value;
}

void add_sub_offset(){
  int32 result = read_register(instr.SingleDataTransfer.Rn);
  //int offset = get_offset(instr)*4;
  int offset = get_offset();
 
  if(instr.SingleDataTransfer.U){
    result += offset;
  } else {
    result -= offset;
  }

  if(result<0x10000)//xj //memory[65536]	
  	write_register(instr.SingleDataTransfer.Rn, result);  
  else 
  	fprintf(fplog,"Error: Out of bounds memory access at address 0x%08x\n",result);
}


int32 get_4bit_memory(int start_point){

  int res = 0;

  for(int i = 0; i < 4; i++) {
    if (i >= 65536) {
      break;
      }
    res |= memory[start_point + i] << (i * 8);
  }
  return res;
}  

void report_err_instruction() {

  printf("wrong instruction!");

}













 
