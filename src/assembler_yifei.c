#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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


  
  //first pass

  fp = fopen(argv[1],"r");
  fpw = fopen(argv[2], "w");
  while (fgets(line, sizeof(line),fp) != NULL)
    {
      token = strtok(line, "\n\t\r ");
      if (strcmp(token,"add")==0
	  ||strcmp(token,"sub")==0
	  ||strcmp(token,"rsb")==0
	  ||strcmp(token,"and")==0
	  ||strcmp(token,"eor")==0
	  ||strcmp(token,"oor")==0
	  ){
		char *pro1 = data_pro1(line);
	  	fprintf(fpw, "%s\n", pro1);
		printf("%s\n", pro1);
		free(pro1);
      }else if (strcmp(token,"mov")==0){
		char *pro2 = data_pro2(line);
      		fprintf(fpw, "%s\n", pro2);
		printf("%s\n", pro2);
		free(pro2);
      }else if (strcmp(token,"tst")==0
		||strcmp(token,"teq")==0
		||strcmp(token,"cmp")==0){
		char *pro3 = data_pro3(line);
      		fprintf(fpw, "%s\n", pro3);
		printf("%s\n", pro3);
		free(pro3);
      }else if (strcmp(token,"mul")==0
		||strcmp(token,"mla")==0){
      	fprintf(fpw, "%s\n", multiply(line));
      }else if (strcmp(token,"ldr")==0
		||strcmp(token,"str")==0){
      	fprintf(fpw, "%s\n", data_transfer(line));
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
    }
  return 0;
}

// Process and, eor, sub, rsb, add, orr
char * data_pro1(char *instruction){

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

	// Convert opcode to binary string
	char binOpcode[4] = "";

	if (strcmp(opcode,"and")==0) {
		char *and = toBits(AND);
		strcpy(binOpcode, &(and[28]));
		free(and);
	}
	if (strcmp(opcode,"eor")==0) {
		char *eor = toBits(EOR);
		strcpy(binOpcode, &(eor[28]));
		free(eor);
	}
	if (strcmp(opcode,"sub")==0) {
		char *sub = toBits(SUB);
		strcpy(binOpcode, &(sub[28]));
		free(sub);
	}
	if (strcmp(opcode,"rsb")==0) {
		char *rsb = toBits(RSB);
		strcpy(binOpcode, &(rsb[28]));
		free(rsb);
	}
	if (strcmp(opcode,"add")==0) {
		char *add = toBits(ADD);
		strcpy(binOpcode, &(add[28]));
		free(add);
	}
	if (strcmp(opcode,"orr")==0) {
		char *orr = toBits(ORR);
		strcpy(binOpcode, &(orr[28]));
		free(orr);
	}

	// Convert rd to binary string
	char binRd[4] = "";
	char *reg1 = regtoBits(rd);
	strcpy(binRd, reg1);
	free(reg1);

	// Convert rn to binary string
	char binRn[4] = "";
	char *reg2 = regtoBits(rn);
	strcpy(binRn, reg2);
	free(reg2);

	// Convert operand2 to binary string
	char binOperand2[12] = "";
	char *imm = immtoBits(operand2);
	strcpy(binOperand2, imm);
	free(imm);

	// Concatenate into single binary string
	char *bits = malloc(32 * sizeof(char));
	strcpy(bits, "1110001");
	strcat(bits, binOpcode);
	strcat(bits, "0");
	strcat(bits, binRn);
	strcat(bits, binRd);
	strcat(bits, binOperand2);

	return bits;
}

// Process mov
char * data_pro2(char *instruction) {

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

	// Convert rd to binary string
	char binRd[4] = "";
	char *reg1 = regtoBits(rd);
	strcpy(binRd, reg1);
	free(reg1);

	// Convert operand2 to binary string
	char binOperand2[12] = "";
	char *imm = immtoBits(operand2);
	strcpy(binOperand2, imm);
	free(imm);

	// Concatenate into single binary string
	char *bits = malloc(32 * sizeof(char));
	strcpy(bits, "111000111010");
	strcat(bits, binRd);
	strcat(bits, binOperand2);

	return bits;
}

// Process tst, teq, cmp
char * data_pro3(char *instruction) {

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

	// Convert opcode to binary string
	char binOpcode[4] = "";

	if (strcmp(opcode,"tst")==0) {
		char *tst = toBits(TST);
		strcpy(binOpcode, &(tst[28]));
		free(tst);
	}
	if (strcmp(opcode,"teq")==0) {
		char *teq = toBits(TEQ);
		strcpy(binOpcode, &(teq[28]));
		free(teq);
	}
	if (strcmp(opcode,"cmp")==0) {
		char *cmp = toBits(CMP);
		strcpy(binOpcode, &(cmp[28]));
		free(cmp);
	}

	// Convert rn to binary string
	char binRn[4] = "";
	char *reg2 = regtoBits(rn);
	strcpy(binRn, reg2);
	free(reg2);

	// Convert operand2 to binary string
	char binOperand2[12] = "";
	char *imm = immtoBits(operand2);
	strcpy(binOperand2, imm);
	free(imm);

	// Concatenate into single binary string
	char *bits = malloc(32 * sizeof(char));
	strcpy(bits, "1110001");
	strcat(bits, binOpcode);
	strcat(bits, "1");
	strcat(bits, binRn);
	strcat(bits, "0000");
	strcat(bits, binOperand2);

	return bits;
}

// Convert register to binary string
char * regtoBits(char *reg) {
	char *binReg = malloc(4 * sizeof(char));
	char *bits = toBits(atoi(strcpy(reg, &reg[1])));
	strcpy(binReg, &(bits[28]));
	free(bits);
	return binReg;
}

// Convert immediate value to binary string
char * immtoBits(char *operand2) {
	char *binOperand2 = malloc(12 * sizeof(char));
	strcpy(binOperand2, "0000");
	if (operand2[1] == '0' && operand2[2] == 'x') {
		char *bits1 = hextoBits(strcpy(operand2, &operand2[3]));
		strcat(binOperand2, bits1);
		free(bits1);
	} else {
		char to8Bits[8] = "";
		char *bits2 = toBits(atoi(strcpy(operand2, &operand2[1])));
		strcpy(to8Bits, &(bits2[24]));
		free(bits2);
		strcat(binOperand2, to8Bits);
	}
	return binOperand2;
}

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

//char * multiply() {
//    char *p;
//	char *instr;
//	char rd[3] = "";
//	char rn[3] = "";
//	char rs[3] = "";
//	char rm[3] = "";
//	char operand2[5] = "";
//	char bits[32] = "1110";
//	char bina;
//	char bins = "0";
//
//
//	p = strtok(instruction, "\n");
//	if (p) {
//		strcat(instr, p);
//	}
//
//
//	p = strtok(NULL, "\n");
//	if (p) {
//		strcat(rd, p);
//	}
//	p = strtok(NULL, "\n");
//	if (p) {
//		strcat(rm, p);
//	}
//	p = strtok(NULL, "\n");
//	if (p) {
//		strcat(rs, p);
//	}
//
//	if (strcmp(instr,"mul")==0) {
//		strcpy(bina, "0");
//	}
//	if (strcmp(instr,"mla")==0) {
//        p = strtok(NULL, "\n");
//        if (p) {
//            strcat(rn, p);
//        }
//		strcpy(bina, "1");
//	}
//
//	char binRd[4] = "";
//	strcpy(binRd, &(toBits(atoi(strcpy(rd, &rd[1])))[28]));
//
//	char binRn[4] = "";
//	strcpy(binRn, &(toBits(atoi(strcpy(rn, &rn[1])))[28]));
//
//	char binRs[4] = "";
//	strcpy(binRs, &(toBits(atoi(strcpy(rs, &rs[1])))[28]));
//
//	char binRm[4] = "";
//	strcpy(binRm, &(toBits(atoi(strcpy(rm, &rm[1])))[28]));
//
//	strcat(bits, bina);
//	strcat(bits, bins);
//	strcat(bits, binRd);
//	strcat(bits, binRn);
//	strcat(bits, binRs);
//	strcat(bits, "1001");
//	strcat(bits, binRm);
//	return bits;
//
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
