#include <stdlib.h>
#include "assembler_types.h"
#include "assembler.h"

int main(int argc, char **argv) {
  FILE *fp;
  char line[100];
  char *token = NULL;
  char *op1, *op2,*op3, lable;
  int program[1000];
  int cnt = 0; // holds the address of the machine code instruction


  
  //first pass

  fp = fopen(argv[1],"r");
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
	data_pro1();
      }else if (strcmp(token,"mov")==0){
	data_pro2();

      }else if (strcmp(token,"tst")==0
		||strcmp(token,"teq")==0
		||strcmp(token,"cmp")==0){
	data_pro3();
      }else if (strcmp(token,"mul")==0
		||strcmp(token,"mla")==0){
	multiply();
      }else if (strcmp(token,"ldr")==0
		||strcmp(token,"str")==0){
	data_transfer();

      }else if (strcmp(token,"beq")==0
		||strcmp(token,"bne")==0
		||strcmp(token,"bge")==0
		||strcmp(token,"blt")==0
		||strcmp(token,"bgt")==0
		||strcmp(token,"ble")==0
		||strcmp(token,"b")==0){
	branch();
      }else{
	special();
      }


    }
  return EXIT_SUCCESS;
}
