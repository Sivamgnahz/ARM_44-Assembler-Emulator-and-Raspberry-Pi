#define uint32_t unsigned int
#define SEARCH_LABEL_LENGTH_OF_CHAR 6
#define TOTAL_LABEL_LINE_NUM_BUFFER 90
#define INITIAL_VALUE 0xffffffff
#define ZERO_VALUE 0x00000000
#define CHANGE_TO_INT 0x30
#define HEX_HELP 0x37
#define HEX_HELP_2 0x57
#define BIT_MASK_OFFSET_ONE 0x00000fff //bit11-0
#define BIT_MASK_OFFSET_TWO 0x007fffff //bit22--0
#define NEW_LINE_CHAR 0x0a
#define COLON 0x3a

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
    uint32_t Offset: 23; // Bit[22:0]
    uint32_t Sign: 1; // Bit[23]
    uint32_t bit2524: 2; // Bit[25:24]
    uint32_t bit2726: 2; // Bit[27:26]    01
    uint32_t Cond: 4; // Bit[31:28]
  }
  Branch;
}
instruction;