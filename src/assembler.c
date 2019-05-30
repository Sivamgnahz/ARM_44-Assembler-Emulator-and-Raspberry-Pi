#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "assembler_types.h"
#include "assembler.h"

int main(int argc, char *argv[]) {
  FILE *fp;
  FILE *fpw;
  char line[100];
  char *token = NULL;
  char *op1, *op2,*op3, label;
  int program[1000];
  int cnt = 0; // holds the address of the machine code instruction
  instruct ins;


  
  //first pass

  fp = fopen(argv[1],"r");
  fpw = fopen(argv[2], "wb");
  int count = 0;
  u_int32_t *pro;
  while (fgets(line, sizeof(line),fp) != NULL)
    {
      token = strtok(line, "\n\t\r ");
      ins.mnemonic = token;
      if (strcmp(token,"add")==0
	  ||strcmp(token,"sub")==0
	  ||strcmp(token,"rsb")==0
	  ||strcmp(token,"and")==0
	  ||strcmp(token,"eor")==0
	  ||strcmp(token,"oor")==0
	  ){
		pro[count] = data_pro1(line);
      }else if (strcmp(token,"mov")==0){
		pro[count] = data_pro2(line);
      }else if (strcmp(token,"tst")==0
		||strcmp(token,"teq")==0
		||strcmp(token,"cmp")==0){
		pro[count] = data_pro3(line);
      }else if (strcmp(token,"mul")==0
		||strcmp(token,"mla")==0){
          ins.rd = regtoBits(strtok(NULL, "\n"));
          ins.rm = regtoBits(strtok(NULL, "\n"));
          ins.rs = regtoBits(strtok(NULL, "\n"));
          ins.mnemonic = "mul";
          if (token[1] == 'l'){
              ins.rn = regtoBits(strtok(NULL, "\n"));
              ins.mnemonic = "mla";
          }
      	fprintf(fpw, "%s\n", multiply(ins));
      }else if (strcmp(token,"ldr")==0
		||strcmp(token,"str")==0){
          ins.rd = regtoBits(strtok(NULL, "\n"));
          ins.operand2 = immtoBits(strtok(NULL, "\n"))
          if (token[0] == 's'){
              ins.rm = ins.operand2;
          }
      	fprintf(fpw, "%s\n", data_transfer(ins));
      }else if (strcmp(token,"beq")==0
		||strcmp(token,"bne")==0
		||strcmp(token,"bge")==0
		||strcmp(token,"blt")==0
		||strcmp(token,"bgt")==0
		||strcmp(token,"ble")==0
		||strcmp(token,"b")==0){
		fprintf(fpw, "%s\n", branch(line));
      }else{
      	fprintf(fpw, "%s\n", special(line));
      }
      fclose(fp);
      fclose(fpw);
      fflush(fpw);

	  count++;
	  fwrite(pro, sizeof(pro), 1, fpw);
    }
  return 0;
}

// Process and, eor, sub, rsb, add, orr
u_int32_t data_pro1(char *instruction){

	char *p;
	char opcode[3] = "";
	char rd[3] = "";
	char rn[3] = "";
	char operand2[5] = "";

	// Split instruction into opcode, rd, rn, operand2
	p = strtok(instruction, " ");
	if (p) {
		strcat(opcode, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(rd, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(rn, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(operand2, p);
	}

	// Convert opcode to u_int32_t
	u_int32_t binOpcode = 0;

	if (strcmp(opcode,"and")==0) {
		binOpcode = AND;
	}
	if (strcmp(opcode,"eor")==0) {
		binOpcode = EOR;
	}
	if (strcmp(opcode,"sub")==0) {
		binOpcode = SUB;
	}
	if (strcmp(opcode,"rsb")==0) {
		binOpcode = RSB;
	}
	if (strcmp(opcode,"add")==0) {
		binOpcode = ADD;
	}
	if (strcmp(opcode,"orr")==0) {
		binOpcode = ORR;
	}

	// Convert rd to u_int32_t
	u_int32_t binRd = regtoBits(rd);

	// Convert rn to u_int32_t
	u_int32_t binRn = regtoBits(rn);

	// Convert operand2 to u_int32_t
	u_int32_t binOperand2 = immtoBits(operand2);

	// Concatenate into u_int32_t
	u_int32_t bits = concat(56, 1, binOpcode, 0, binRn, binRd, binOperand2);

	return bits;
}

// Process mov
u_int32_t data_pro2(char *instruction) {

	char *p;
	char rd[3] = "";
	char operand2[5] = "";

	// Split instruction into mov, rd, operand2
	p = strtok(instruction, " ");
	p = strtok(NULL, " ");
	if (p) {
		strcat(rd, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(operand2, p);
	}

	// Convert rd to u_int32_t
	u_int32_t binRd = regtoBits(rd);

	// Convert operand2 to u_int32_t
	u_int32_t binOperand2 = immtoBits(operand2);

	// Concatenate into u_int32_t
	u_int32_t bits = concat(56, 1, 13, 0, 0, binRd, binOperand2);
	
	return bits;
}

// Process tst, teq, cmp
u_int32_t data_pro3(char *instruction) {

	char *p;
	char opcode[3] = "";
	char rn[3] = "";
	char operand2[5] = "";

	// Split instruction into opcode, rn, operand2
	p = strtok(instruction, " ");
	if (p) {
		strcat(opcode, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(rn, p);
	}
	p = strtok(NULL, " ");
	if (p) {
		strcat(operand2, p);
	}

	// Convert opcode to u_int32_t
	u_int32_t binOpcode = 0;

	if (strcmp(opcode,"tst")==0) {
		binOpcode = TST;
	}
	if (strcmp(opcode,"teq")==0) {
		binOpcode = TEQ;
	}
	if (strcmp(opcode,"cmp")==0) {
		binOpcode = CMP;
	}

	// Convert rn to u_int32_t
	u_int32_t binRn = regtoBits(rn);

	// Convert operand2 to u_int32_t
	u_int32_t binOperand2 = immtoBits(operand2);

	// Concatenate into u_int32_t
	u_int32_t bits = concat(56, 1, binOpcode, 1, binRn, 0, binOperand2);

	return bits;
}

// Convert register to u_int32_t
u_int32_t regtoBits(char *reg) {
	return atoi(strcpy(reg, &reg[1]));
}

// Convert immediate value to u_int32_t
u_int32_t immtoBits(char *operand2) {
	char *binOperand2 = "";
	strcpy(binOperand2, &operand2[1]);
	if (binOperand2[0] == '0' && binOperand2[1] == 'x') {
		return (int)strtol(binOperand2, NULL, 0);
	}
	return atoi(binOperand2);
}

// Concatenate into u_int32_t
u_int32_t concat(int cond, int i, int opcode, int s, int rn, int rd, int operand2) {
	cond = cond << 26;
	i = i << 25;
	opcode = opcode << 21;
	s = s << 20;
	rn = rn << 16;
	rd = rd << 12;
	return cond | i | opcode | s | rn | rd | operand2;
}

/*
// Convert int to binary string
char * toBits(unsigned int x) {
	char *bits = malloc(32 * sizeof(char));
	int i; 
	unsigned int mask = 1 << 31;
	for(i=0; i<32; ++i) { 
		if((x & mask) == 0){ 
			bits[i] = 0;
		} else { 
			bits[i] = 1;
		} 
		x = x << 1; 
	} 
	return bits;
}

// Convert hex to binary string
char * hextoBits(char *hex) {
	char *bin = malloc(8 * sizeof(char));
	bin[0] = '\0';
	int i;
	for(i=0; hex[i]!='\0'; i++)
    {
        switch(hex[i])
        {
            case '0':
                strcat(bin, "0000");
                break;
            case '1':
                strcat(bin, "0001");
                break;
            case '2':
                strcat(bin, "0010");
                break;
            case '3':
                strcat(bin, "0011");
                break;
            case '4':
                strcat(bin, "0100");
                break;
            case '5':
                strcat(bin, "0101");
                break;
            case '6':
                strcat(bin, "0110");
                break;
            case '7':
                strcat(bin, "0111");
                break;
            case '8':
                strcat(bin, "1000");
                break;
            case '9':
                strcat(bin, "1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                break;
            default:
                printf("Invalid hexadecimal input.");
        }
    }
    return bin;
}
*/

//char * multiply() {
//	return NULL;
//}
//
//char * data_transfer() {
//	return NULL;
//}
u_int32_t multiply(instruct ins){
    u_int32_t cond  = 1110;
    u_int32_t rd  = ins.rd;
    u_int32_t rn = ins.rn;
    u_int32_t rs = ins.rs;
    u_int32_t rm = ins.rm;
    u_int32_t a = 0;
    u_int32_t s = 0;
    if (ins.mnemonic[1] == 'l'){
        u_int32_t a = 1;
    }
    cond <<= 28;
    a <<= 21;
    s <<= 20;
    rn <<= 12;
    rd <<= 16;
    rs <<= 8;

    return cond | a | s | rd | rn | rs | 9 <<= 4 | rm;
}

u_int32_t data_transfer(instruct ins) {
    u_int32_t rd, rn,l,p, offset;
    p = 1;
    u = 1;
    if (ins.mnemonic[0] == 'l'){
        if (ins.operand2 < 0xff){ // when the size of operand2 is within 4 bytes, it acts like mov
            ins.mnemonic = strdup("mov");
            data_pro2(ins);
            breaks;
        } else {
            rn = 15;// PC = 15; And rn = PC;
            l = 1;
            rd = ins.rd;
            offset = 8;// size of instruction * 4 - 8; temperately can't figure out the size of instr
        }
        rn <<= 16;
        p <<= 24;
        u <<= 23;

    }
    return  1 <<= 26 | i | p | u |  l | rn | rd | offset;
}




char * branch() {
	return NULL;
}

char * special() {
	return NULL;
}

