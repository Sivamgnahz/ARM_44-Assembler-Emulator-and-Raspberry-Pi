#include <stdint.h>

typedef union {
  uint32_t CPSR_32bits;
  struct {
    uint32_t reserved: 28; // Bit[0:27]
    uint32_t V: 1; // Bit[29] 
    uint32_t C: 1; // Bit[29] 
    uint32_t Z: 1; // Bit[30]
    uint32_t N: 1; // Bit[31]
  }
  CPSR_Bits;
}
CPSR_type;

typedef union {
  uint32_t instr_32bits;
  struct {
    uint32_t Byt0: 8; // Bit[0:7]
    uint32_t Byt1: 8; // Bit[8:15] 
    uint32_t Byt2: 8; // Bit[16:23]
    uint32_t Byt3: 8; // Bit[24:31]
  }
  instr_Byt;
  struct {
    uint32_t reserved2: 4; // Bit[0:3]
    uint32_t bit74: 4; // Bit[4:7] 
    uint32_t reserved1: 13; // Bit[8:20]
    uint32_t bit21: 1; // Bit[21]
    uint32_t bit22: 1; // Bit[22] 
    uint32_t bit23: 1; // Bit[23]	
    uint32_t bit2524: 2; // Bit[24:25]
    uint32_t bit2726: 2; // Bit[26:27]		
    uint32_t Cond: 4; // Bit[28:31]
  }
  instr_set;
  struct {
    uint32_t Operand2: 12; // Bit[0:11]
    uint32_t Rd: 4; // Bit[12:15]
    uint32_t Rn: 4; // Bit[16:19]
    uint32_t S: 1; // Bit[20];
    uint32_t OpCode: 4; // Bit[21:24]
    uint32_t I: 1; // Bit[25];
    uint32_t bit2726: 2; // Bit[26:27]	00
    uint32_t Cond: 4; // Bit[28:31]
  }
  data_pro;

  struct {
    uint32_t Imm: 8; //Bit[0:7]
    uint32_t Rotate: 4; //Bit[8:11]
    uint32_t Rd: 4; // Bit[12:15]
    uint32_t Rn: 4; // Bit[16:19]
    uint32_t S: 1; // Bit[20];
    uint32_t OpCode: 4; // Bit[21:24]
    uint32_t I: 1; // Bit[25];
    uint32_t bit2726: 2; // Bit[26:27]	00
    uint32_t Cond: 4; // Bit[28:31]
  }
  data_pro_imm;

  struct {

    uint32_t Rm: 4; //Bit[0:3]
    uint32_t bit4: 1; //Bit[4]
    uint32_t Shift_type: 2; // Bit[5:6]
    uint32_t Integer: 5; //Bit[7:11]
    uint32_t Rd: 4; // Bit[12:15]
    uint32_t Rn: 4; // Bit[16:19]
    uint32_t S: 1; // Bit[20];
    uint32_t OpCode: 4; // Bit[21:24]
    uint32_t I: 1; // Bit[25];
    uint32_t bit2726: 2; // Bit[26:27]	00
    uint32_t Cond: 4; // Bit[28:31]

  }
  data_pro_reg;

  struct {
    uint32_t Rm: 4; // Bit[3:0]
    uint32_t bit74: 4; // Bit[7:4] 	1001
    uint32_t Rs: 4; // Bit[11:8]
    uint32_t Rn: 4; // Bit[15:12]
    uint32_t Rd: 4; // Bit[19:16]
    uint32_t S: 1; // Bit[20];
    uint32_t A: 1; // Bit[21]
    uint32_t bit2522: 4; // Bit[25:22] 	0000
    uint32_t bit2726: 2; // Bit[27:26]	00
    uint32_t Cond: 4; // Bit[31:28]
  }
  Multiply;
  struct {
    uint32_t Offset: 12; // Bit[11:0]
    uint32_t Rd: 4; // Bit[15:12]
    uint32_t Rn: 4; // Bit[19:16]
    uint32_t L: 1; // Bit[20];
    uint32_t bit2221: 2; // Bit[22:21] 	00
    uint32_t U: 1; // Bit[23];
    uint32_t P: 1; // Bit[24];
    uint32_t I: 1; // Bit[25];
    uint32_t bit2726: 2; // Bit[27:26]	01
    uint32_t Cond: 4; // Bit[31:28]
  }
  SingleDataTransfer;

  struct {
    signed int Offset: 23; // Bit[22:0]
    uint32_t Sign: 1; // Bit[23]
    uint32_t bit2524: 2; // Bit[25:24]
    uint32_t bit2726: 2; // Bit[27:26]    01
    uint32_t Cond: 4; // Bit[31:28]
  }
  Branch;

}
instruction;