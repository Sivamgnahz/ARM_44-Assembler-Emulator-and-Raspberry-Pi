#ifndef _TYPE_H
#define _TYPE_H
#define u32 unsigned int

typedef struct instruction{
  char *mnemonic;
  u32 operamd2;
  u32 rd;
  u32 rn;
  u32 rs;
  u32 rm;
  u32 imm;
  u32 expression;
} instruct;

typedef enum {
  AND = 0,
  EOR = 1,
  SUB = 2,
  RSB = 3,
  ADD = 4,
  TST = 8,
  TEQ = 9,
  CMP = 10,
  ORR = 12,
  MOV = 13
} OPCODE;

enum {
  eq = 0,
  ne = 1,
  ge = 10,
  lt = 11,
  gt = 12,
  le = 13,
  al = 14

};

struct label
  {
    int location;
    char *label;
   
  };




#endif
