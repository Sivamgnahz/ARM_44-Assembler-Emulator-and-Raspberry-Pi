#include <stdlib.h>
#include "assembler_types.h"
#include "assembler.h"

int main(int argc, char *argv) {
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
	  	fprintf(fpw, "%s\n", data_pro1(strtok(line, "\n")));
      }else if (strcmp(token,"mov")==0){
      	fprintf(fpw, "%s\n", data_pro2(strtok(line, "\n")));
      }else if (strcmp(token,"tst")==0
		||strcmp(token,"teq")==0
		||strcmp(token,"cmp")==0){
      	fprintf(fpw, "%s\n", data_pro3(strtok(line, "\n")));
      }else if (strcmp(token,"mul")==0
		||strcmp(token,"mla")==0){
      	fprintf(fpw, "%s\n", multiply(strtok(line, "\n")));
      }else if (strcmp(token,"ldr")==0
		||strcmp(token,"str")==0){
      	fprintf(fpw, "%s\n", data_transfer(strtok(line, "\n"));
      }else if (strcmp(token,"beq")==0
		||strcmp(token,"bne")==0
		||strcmp(token,"bge")==0
		||strcmp(token,"blt")==0
		||strcmp(token,"bgt")==0
		||strcmp(token,"ble")==0
		||strcmp(token,"b")==0){
		fprintf(fpw, "%s\n", branch(strtok(line, "\n"));
      }else{
      	fprintf(fpw, "%s\n", special(strtok(line, "\n"));
      }
      fclose(fp);
      fclose(fpw);
      fflush(fpw);
    }
  return EXIT_SUCCESS;
}

char * data_pro1(char *instruction){

	char *p;
	char opcode[3] = "";
	char rd[3] = "";
	char rn[3] = "";
	char operand2[5] = "";

	p = strtok(instruction, " ");
	if (p) {
		strcat(opcode, p);
	}
	p = strtok(NULL, ",");
	if (p) {
		strcat(rd, p);
	}
	p = strtok(NULL, ",");
	if (p) {
		strcat(rn, p);
	}
	p = strtok(NULL, ",");
	if (p) {
		strcat(operand2, p);
	}

	char binOpcode[4] = "";

	if (strcmp(opcode,"and")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.AND)[28]));
	}
	if (strcmp(opcode,"eor")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.EOR)[28]));
	}
	if (strcmp(opcode,"sub")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.SUB)[28]));
	}
	if (strcmp(opcode,"rsb")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.RSB)[28]));
	}
	if (strcmp(opcode,"add")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.ADD)[28]));
	}
	if (strcmp(opcode,"orr")==0) {
		strcpy(binOpcode, &(toBits(assembler_types.OPCODE.ORR)[28]));
	}

	char binRd[4] = "";
	strcpy(binRd, &(toBits(atoi(strcpy(rd, &rd[1])))[28]));

	char binRn[4] = "";
	strcpy(binRn, &(toBits(atoi(strcpy(rn, &rn[1])))[28]));

	char binOperand2[12] = "0000";
	if (operand2[1] == '0' && operand2[2] == 'x') {
		strcat(binOperand2, hextoBits(strcpy(operand2, &operand2[3])));
	} else {
		char to8Bits[8] = "";
		strcpy(to8Bits, &(toBits(atoi(strcpy(operand2, &operand2[1])))[24]));
		strcat(binOperand2, to8Bits);
	}

	char bits[32] = "1110001";
	strcat(bits, binOpcode);
	strcat(bits, "0");
	strcat(bits, binRn);
	strcat(bits, binRd);
	strcat(bits, binOperand2);

	return bits;
}

char * toBits(unsigned int x) {
	char bits[32] = "";
	int i; 
	uint32_t mask = 1 << 31;
	for(i=0; i<32; ++i) { 
		if((x & mask) == 0){ 
			strcat(bits, "0");
		} else { 
			strcat(bits, "1"); 
		} 
		x = x << 1; 
	} 
	strcat(bits, "\n");
	return bits;
}

char * hextoBits(char *hex) {
	char bin[8] = "";
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