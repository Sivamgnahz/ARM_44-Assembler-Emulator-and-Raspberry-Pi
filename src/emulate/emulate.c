#include "bit_field.h"
#include "emulate.h"

uint8_t memory[65536] = {0};  //initialise to 0, store instructions and data
uint32_t reg[17] = {0};  //0-12, 15, 16 are useful, store registers
CPSR_type CPSR; 
uint32_t shift_carry_out = 0;
uint32_t add_carry_out = 0;
int PIN = 0;

int main(int argc, char * argv[]) {
  FILE * file = fopen(argv[1], "rb"); //rb means read binary
  uint8_t * buffer = malloc(sizeof(uint8_t));
  int counter = 0;
  CPSR.CPSR_32bits = 0x00000000;
  while (fread(buffer, sizeof(uint8_t), 1, file) == 1) {
    memory[counter++] = * buffer; //store instructions into the memory
  }

  while (1) {
    instruction instr;
    instr = readInstruction();
    if (!instr.instr_32bits) {
      break;
    }
    check_instr_set(instr);
    reg[15] += 4;
  }
  reg[15] += 8;
  printf("Registers:\n");
  for (int x = 0; x < 13; x++) {
    printf("$%-3d", x);
    printf(": %10d (0x%08x)\n", reg[x], reg[x]);
  }
  printf("PC  : %10d (0x%08x)\n", reg[15], reg[15]);
  printf("CPSR: %10d (0x%08x)\n", CPSR.CPSR_32bits, CPSR.CPSR_32bits);
  free(buffer); //delete buffer
  fclose(file); //close file
  fprintf(stdout, "Non-zero memory:\n");
  for (int i = 0; i != 65536 / 4; i++) {
    if ((memory[4 * i] != 0) || (memory[4 * i + 1] != 0) || (memory[4 * i + 2] != 0) || (memory[4 * i + 3] != 0))
      fprintf(stdout, "0x%08x: 0x%02x%02x%02x%02x\n", i * 4, memory[4 * i], memory[4 * i + 1], memory[4 * i + 2], memory[4 * i + 3]);
  }
  return 0;
}

instruction readInstruction() {
  instruction instr;
  uint32_t pc = reg[15];
  instr.instr_Byt.Byt0 = memory[pc];
  instr.instr_Byt.Byt1 = memory[pc + 1];
  instr.instr_Byt.Byt2 = memory[pc + 2];
  instr.instr_Byt.Byt3 = memory[pc + 3];
  return instr;
}
void check_instr_set(instruction instr) { //check which type of instructions it is
  if (instr.instr_set.bit2726 == 0) {
    if ((instr.instr_set.bit2524 == 00) && (instr.instr_set.bit23 == 0) && (instr.instr_set.bit22 == 0) && (instr.instr_set.bit74 == 9)) {
      multiply(instr);
    } else {
      data_processing(instr);
    }
  } else if (instr.instr_set.bit2726 == 1) {
    if ((instr.instr_set.bit22 == 0) && (instr.instr_set.bit21 == 0)) {
      single_data_transfer(instr);
    } else {
      report_err_instruction();
    }
  } else if (instr.instr_set.bit2726 == 2) {
    if (instr.instr_set.bit2524 == 2) {
      branch(instr);
    } else {
      report_err_instruction();
    }
  } else {
    report_err_instruction();
  }
}

void data_processing(instruction instr) {
  uint32_t operand1 = read_register(instr.data_pro.Rn);
  uint32_t operand2 = get_operand2(instr);
  uint32_t result;
  uint32_t C;
  uint32_t Z;
  uint32_t V;
  if (check_cond(instr.instr_set.Cond)) {
    switch (instr.data_pro.OpCode) {
    case 0:
      result = operand1 & operand2;
      write_register(instr.data_pro.Rd, result);
      break;
    case 1:
      result = operand1 ^ operand2; //eor??
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
      if (result < operand1 || result < operand2) {
        add_carry_out = 1;
      } else {
        add_carry_out = 0;
      }
      write_register(instr.data_pro.Rd, result);
      break;
    case 8:
      result = operand1 & operand2;
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
      CPSR.CPSR_Bits.Z = 1;
    } else {
      CPSR.CPSR_Bits.Z = 0;
    }
    CPSR.CPSR_Bits.N = build_mask(result, 31, 1);
    set_C(result, instr.data_pro.OpCode);
  }
}


uint32_t get_operand2(instruction instr) {
  uint32_t value;
  if (instr.data_pro.I == 1) {
    value = instr.data_pro_imm.Imm;
    if (instr.data_pro_imm.Rotate != 0) {
      value <<= (32 - (2 * instr.data_pro_imm.Rotate));
    }
  } else {
    uint32_t value_Rm = read_register(instr.data_pro_reg.Rm);
    if (instr.data_pro_reg.bit4 == 0) {
      value = shift(instr.data_pro_reg.Shift_type, instr.data_pro_reg.Integer, value_Rm, instr);
    } else {
      if (instr.data_pro_reg.Integer % 2 != 0) {
        value = read_register(instr.data_pro_reg.Rm);
      } else {
        uint32_t value_Rs = read_register(instr.data_pro_reg.Integer / 2);
        value = shift(instr.data_pro_reg.Shift_type, value_Rs, value_Rm, instr);
      }
    }
  }
  return value;
}


void set_C(int result, int opCode) { //data processing helper function 
  switch (opCode) {
  case 0:
  case 1:
  case 8:
  case 9:
  case 12:
  case 13:
    CPSR.CPSR_Bits.C = shift_carry_out;
  case 4:
    CPSR.CPSR_Bits.C = add_carry_out;
    break;
  case 2:
  case 3:
  case 10:
    if (result >= 0) {
      CPSR.CPSR_Bits.C = 1;
    } else {
      CPSR.CPSR_Bits.C = 0;
    }
    break;
    defalt:
      CPSR.CPSR_Bits.C = 0;
  }
}


uint32_t shift(uint32_t shift_type, uint32_t shift_amount, uint32_t value, instruction instr) { //data processing helper functnion
  uint32_t bits;
  uint32_t bit_sign;
  uint32_t bits_low;
  uint32_t bits_high;
  switch (shift_type) {
  case 0:
    bits = build_mask(value, 0, DATA_LENGTH - shift_amount);
    if (instr.data_pro.S == 1) {
      if (shift_amount == 0) {
        return bits;
      }
      shift_carry_out = build_mask(value, DATA_LENGTH - shift_amount, 1);
    }
    return bits << shift_amount;
  case 1:
    bits = build_mask(value, shift_amount, DATA_LENGTH - shift_amount);
    if (instr.data_pro.S == 1) {
      shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits;
  case 2:
    bits = build_mask(value, shift_amount, DATA_LENGTH - shift_amount - 1);
    bit_sign = build_mask(value, 31, 1);
    bit_sign <<= 31;
    if (instr.data_pro.S == 1) {
      shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits ^ bit_sign;
  case 3:
    bits_high = build_mask(value, shift_amount, DATA_LENGTH - shift_amount);
    bits_low = build_mask(value, 0, shift_amount);
    bits_low <<= (DATA_LENGTH - shift_amount);
    if (instr.data_pro.S == 1) {
      shift_carry_out = build_mask(value, shift_amount - 1, 1);
    }
    return bits_high ^ bits_low;
  }
}


void multiply(instruction instr) { //multiply instruction
  uint32_t content; 
  if (check_cond(instr.Multiply.Cond)) {
    if(instr.Multiply.A){
      content = read_register(instr.Multiply.Rm) * read_register(instr.Multiply.Rs)
	      +read_register(instr.Multiply.Rn);
    } else {
      content = read_register(instr.Multiply.Rm) * read_register(instr.Multiply.Rs);
    }
      write_register(instr.Multiply.Rd, content);
    }
}

uint32_t read_register(uint32_t r){ //read the content of the register, r is 4-bit binary number
  return reg[r];
}

void write_register(uint32_t r, uint32_t content){ //write the content of the register, r is the 4-bit binary number
  reg[r] = content;
}

uint32_t check_cond(uint32_t cond) {  //check the initial condition for every instruction
  uint32_t n, z, c, v;
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


void branch(instruction instr) {
  signed int offset_shift = instr.Branch.Offset;
  if (check_cond(instr.Branch.Cond)){
    if (offset_shift & (1 << 23)) {
      offset_shift |= ((1 << 9) - 1) << 23;
    }
    offset_shift <<= 2;
    if ((reg[15] + offset_shift + 8 - 4) < 65536) {
      reg[15] = reg[15] + offset_shift + 8 - 4;
    } 
  }
}


void single_data_transfer(instruction instr) { //single data transfer instruction
  if (check_cond(instr.SingleDataTransfer.Cond)) {
    uint32_t r = add_sub_offset(instr);
    if (instr.SingleDataTransfer.L) { //load  
      if (instr.SingleDataTransfer.P) { //add before  
        single_data_load(r, instr);
      } else {
        single_data_load(read_register(instr.SingleDataTransfer.Rn), instr);
        write_register(instr.SingleDataTransfer.Rn, r);
      }
    } else { //store     
      if (instr.SingleDataTransfer.P) { //add/sub before
        single_data_store(r, instr);
      } else { //add/sub after 
        single_data_store(read_register(instr.SingleDataTransfer.Rn), instr);
        write_register(instr.SingleDataTransfer.Rn, r);
      }
    }
  }
}


void single_data_load(uint32_t rn, instruction instr) {
  if (rn < 0x10000) { //memory[65536]
    if (instr.SingleDataTransfer.Rn == 15) {
      write_register(instr.SingleDataTransfer.Rd, get_4bit_memory(rn + 8));
    } else {
      write_register(instr.SingleDataTransfer.Rd, get_4bit_memory(rn));
    }
  } else {
    if (if_in_pin_area(rn)) {
      write_register(instr.SingleDataTransfer.Rd, rn);
    }
  }
}

void single_data_store(uint32_t rn, instruction instr) {
  uint32_t result = read_register(instr.SingleDataTransfer.Rd);
  if (rn < 0x10000) { //memory[65536]
    if (instr.SingleDataTransfer.Rn == 15) {
      rn += 8;
    }
    for (int i = 0; i < 4; i++) {
      memory[rn + i] = build_mask(result, i * 8, 8);
    }
  }
}
uint32_t get_offset(instruction instr) {
  uint32_t value = 0;
  if (!instr.SingleDataTransfer.I) {
    value = instr.SingleDataTransfer.Offset;
  } else {
    uint32_t value_Rm = read_register(instr.data_pro_reg.Rm);
    if (instr.data_pro_reg.bit4 == 0) {
      value = shift(instr.data_pro_reg.Shift_type, instr.data_pro_reg.Integer, value_Rm, instr);
    } else {
      if (instr.data_pro_reg.Integer % 2 == 0) {
        value = read_register(instr.data_pro_reg.Rm);
      } else {
        uint32_t value_Rs = read_register(instr.data_pro_reg.Integer / 2);
        value = shift(instr.data_pro_reg.Shift_type, value_Rs, value_Rm, instr);
      }
    }
  }
  return value;
}

int if_in_pin_area(uint32_t memory_address) {
  if (memory_address == 0x20200008 ||
    memory_address == 0x20200004 ||
    memory_address == 0x20200000) {
    return 1;
  } else {
    return 0;
  }
}

int if_in_control_area(uint32_t memory_address) {
  if (memory_address == 0x20200028 ||
    memory_address == 0x2020001c) {
    return 1;
  } else {
    return 0;
  }
}

void clear_pin() {
  PIN = 0;
}

uint32_t add_sub_offset(instruction instr) {
  uint32_t result = read_register(instr.SingleDataTransfer.Rn); //result is the memory address in register
  //int offset = get_offset(instr)*4;
  uint32_t offset = get_offset(instr);

  if (instr.SingleDataTransfer.U) {
    result += offset;
  } else {
    result -= offset;
  }
  if (result > 0x10000) {

    if (if_in_pin_area(result)) {
      if (result == 0x20200008) {
        printf("One GPIO pin from 20 to 29 has been accessed\n");
      } else if (result == 0x20200004) {
        printf("One GPIO pin from 10 to 19 has been accessed\n");
      } else if (result == 0x20200000) {
        printf("One GPIO pin from 0 to 9 has been accessed\n");
      } else {
        return 0;
      }

    } else if (if_in_control_area(result)) {
      if (result == 0x20200028) {
        clear_pin();
        printf("PIN OFF\n");
      } else if (result == 0x2020001c) {
        clear_pin();
        printf("PIN ON\n");
        PIN = 1;
      } else {
        return 0;
      }
    } else {
      printf("Error: Out of bounds memory access at address 0x%08x\n", result);
    }
  }
  return result;
}

uint32_t get_4bit_memory(uint32_t start_point) {
  uint32_t res = 0;
  for (int i = 0; i < 4; i++) {
    if (i >= 65536) {
      break;
    }
    res += memory[start_point + i] << (8 * i);
  }
  return res;
}

void report_err_instruction() {
  printf("wrong instruction!");
}

uint32_t build_mask(uint32_t value_masked, uint32_t start_point, uint32_t length) { //startpoint starts from 0
  value_masked = value_masked >> start_point;
  uint32_t mask = 0;
  for (int x = 0; x < length; x++) {
    mask += (int) pow((double) 2, (double) x);
  }
  return mask & value_masked;
}


















 
