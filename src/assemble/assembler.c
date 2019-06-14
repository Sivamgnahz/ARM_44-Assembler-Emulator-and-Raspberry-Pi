#include "assembler.h"

FILE * fpw;
FILE * fp;

int instr_table[TOTAL_LABEL_LINE_NUM_BUFFER][11];  //first pass table
char read_1_key, read_2_key, read_3_key, read_4_key, read_w_key;  //chars used in read file
int registers_in_no3_paragraph_but_not_in_bracket = 0;
int lines = 0;
int labels = 0;
int data_transfer_ldr = 0;
int expr_num = 0;

int main(int argc, char * argv[]) {
  int i;  //row
  int j;  //column
  
  instruction *instr = malloc(sizeof(instruction));  
  if (instr == NULL) {
    printf("null pointer!!\n");
    return 0;
  }
  printf("****************success**********************\n");


  fpw = fopen(argv[2], "wb");
  for (i = 0; i < TOTAL_LABEL_LINE_NUM_BUFFER; i++)
    for (j = 0; j < 11; j++)
      instr_table[i][j] = INITIAL_VALUE;

  lines = Read_from_file(argv[1]);

  data_transfer_ldr = INITIAL_VALUE;
  for (j = 0; j < lines; j++) {
    switch (instr_table[j][1]) {
    case INSTRUTYPE_DATA_PROCESSING:
      printf(" INSTRUTYPE_DATA_PROCESSING...1      ");
      data_processing(j, instr);
      break;
    case INSTRUTYPE_MULTIPLY:
      printf(" INSTRUTYPE_MULTIPLY.....2          ");
      multiply(j, instr);
      break;
    case INSTRUTYPE_SINGLE_DATA_TRANSFER:
      printf(" INSTRUTYPE_SINGLE_DATA_TRANSFER....3");
      single_data_transfer(j, instr);
      break;
    case INSTRUTYPE_BRANCH:
      branch(j, instr);
      printf(" INSTRUTYPE_BRANCH.....4             ");
      break;
    case INSTRUTYPE_SPECIAL:
      printf(" INSTRUTYPE_SPECIAL.........5        ");
      if (instr_table[j][2] == INSTRU_ANDEQ) {
        instr->instr_32bits = ZERO_VALUE;
      } else if (instr_table[j][2] == INSTRU_LSL)
        data_processing(j, instr);
      break;
    case INSTRUTYPE_LABEL:
      printf(" INSTRUTYPE_LABEL..........6\n");
      break;
    default:
      printf("? unknown instrutype   !\n");
      break;
    }
    if (instr_table[j][1] != INSTRUTYPE_LABEL) {
      printf(" will write 0x%08x to file fpw   !\n", instr->instr_32bits);
      fwrite(instr, 4, 1, fpw);
    }
  }


  for (int i = 0; i < lines; i++) { 
    if (instr_table[i][2] == 48 && instr_table[i][4] == 2) {
      instr->instr_32bits = instr_table[i][5];
      printf(" will write 0x%08x to file fpw last line for data_transfer_ldr  !\n", instr->instr_32bits);
      fwrite(instr, 4, 1, fpw);
    }
  }
  fclose(fp);
  fclose(fpw);
  return 0;
}

int Read_from_file(char argv[]) {
  int j = 0;
  int i = 0;
  int lines_file = 0;

  if ((fp = fopen(argv, "r")) == NULL) {
    printf(" cannot open file \n");
    return 1;
  }
  
  L_first:
    while ((read_1_key = getc(fp)) != EOF) {
      if ((read_1_key == NEW_LINE_CHAR) || (read_1_key == ' '))
        goto L_first;
      read_2_key = getc(fp);
      if (read_2_key == NEW_LINE_CHAR)
        goto L_first;
      read_3_key = getc(fp);
      if (read_3_key == NEW_LINE_CHAR)
        goto L_first;
      read_4_key = getc(fp);
      if (read_4_key == NEW_LINE_CHAR)
        goto L_first;

      instr_table[i][0] = i;
      if (read_2_key == ' ') //b foo
      {
        analysis_b_label(i); //if branch has Read NEW_LINE_CHAR!  
        goto exit_would_not_while;
      } else if (read_4_key == ' ') //xxx' ' 
      {
        j = analysis_first_word(i); // only here will continue......
        if (j) //if branch has Read NEW_LINE_CHAR!  
          goto exit_would_not_while;
      } else if (read_4_key == 'e') //INSTRUTYPE_SPECIAL INSTRU_ANDEQ
      {
        if ((read_1_key == 'a') && (read_2_key == 'n') && (read_3_key == 'd')) {
          instr_table[i][1] = INSTRUTYPE_SPECIAL;
          instr_table[i][2] = INSTRU_ANDEQ;
        }
        goto exit_willdowhile;
      } else // label
      {
        if (read_4_key == ':') {
          instr_table[i][4] = read_1_key;
          instr_table[i][5] = read_2_key;
          instr_table[i][6] = read_3_key;
          instr_table[i][7] = read_4_key;
        } else
          analysis_last_label(i);
        labels++;
        instr_table[i][1] = INSTRUTYPE_LABEL;
        instr_table[i][2] = i + 1;
        instr_table[i][4] = read_1_key;
        instr_table[i][5] = read_2_key;
        instr_table[i][6] = read_3_key;
        instr_table[i][7] = read_4_key;
        goto exit_willdowhile;
      }
      /////////////////////
      analysis_second_word(i); //r1,
      /////////////////////
      analysis_third_word_and_after(i); //# r [ = -
      goto exit_would_not_while;
      exit_willdowhile:
        //while((read_1_key = getc(fp)) != NEW_LINE_CHAR);//enter ���з���
        read_1_key = getc(fp);
      if (read_1_key == NEW_LINE_CHAR)
        goto exit_would_not_while;
      else if (read_1_key == EOF)
        break;
      else
        goto exit_willdowhile;
      exit_would_not_while:
        lines_file++;
      i++;
      j = 0;

    }

  printf("Line0    Inst_ty1 ins_cMd2 Rn3      typ_Num4 Number5  Rd6      Rs7      Rm8      Lsr9     registers_in_no3_paragraph_but_not_in_bracket\n");
  for (i = 0; i < TOTAL_LABEL_LINE_NUM_BUFFER; i++) {
    for (j = 0; j < 11; j++)
      printf("%08x ", instr_table[i][j]);
    printf("\n");
  }
  return lines_file;
}

int analysis_first_word(int line) {
  int j = 0;
  if ((read_1_key == 'a') && (read_2_key == 'n') && (read_3_key == 'd')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = AND;
  } else if ((read_1_key == 'e') && (read_2_key == 'o') && (read_3_key == 'r')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = EOR;
  } else if ((read_1_key == 's') && (read_2_key == 'u') && (read_3_key == 'b')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = SUB;
  } else if ((read_1_key == 'r') && (read_2_key == 's') && (read_3_key == 'b')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = RSB;
  } else if ((read_1_key == 'a') && (read_2_key == 'd') && (read_3_key == 'd')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = ADD;
  } else if ((read_1_key == 't') && (read_2_key == 's') && (read_3_key == 't')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = TST;
  } else if ((read_1_key == 't') && (read_2_key == 'e') && (read_3_key == 'q')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = TEQ;
  } else if ((read_1_key == 'c') && (read_2_key == 'm') && (read_3_key == 'p')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = CMP;
  } else if ((read_1_key == 'o') && (read_2_key == 'r') && (read_3_key == 'r')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = ORR;
  } else if ((read_1_key == 'm') && (read_2_key == 'o') && (read_3_key == 'v')) {
    instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
    instr_table[line][2] = MOV;
  } //INSTRUTYPE_DATA_PROCESSING
  else if ((read_1_key == 'm') && (read_2_key == 'u') && (read_3_key == 'l')) {
    instr_table[line][1] = INSTRUTYPE_MULTIPLY;
    instr_table[line][2] = INSTRU_MUL;
  } else if ((read_1_key == 'm') && (read_2_key == 'l') && (read_3_key == 'a')) {
    instr_table[line][1] = INSTRUTYPE_MULTIPLY;
    instr_table[line][2] = INSTRU_MLA;
  } //INSTRUTYPE_MULTIPLY
  else if ((read_1_key == 'l') && (read_2_key == 'd') && (read_3_key == 'r')) {
    instr_table[line][1] = INSTRUTYPE_SINGLE_DATA_TRANSFER;
    instr_table[line][2] = INSTRU_LDR;
  } //!!!  = MOV  #	
  else if ((read_1_key == 's') && (read_2_key == 't') && (read_3_key == 'r')) {
    instr_table[line][1] = INSTRUTYPE_SINGLE_DATA_TRANSFER;
    instr_table[line][2] = INSTRU_STR;
  } //INSTRUTYPE_SINGLE_DATA_TRANSFER
  else if ((read_1_key == 'b') && (read_2_key == 'e') && (read_3_key == 'q')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BEQ;
    return j;
  } else if ((read_1_key == 'b') && (read_2_key == 'n') && (read_3_key == 'e')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BNE;
    return j;
  } else if ((read_1_key == 'b') && (read_2_key == 'g') && (read_3_key == 'e')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BGE;
    return j;
  } else if ((read_1_key == 'b') && (read_2_key == 'l') && (read_3_key == 't')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BLT;
    return j;
  } else if ((read_1_key == 'b') && (read_2_key == 'g') && (read_3_key == 't')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BGT;
    return j;
  } else if ((read_1_key == 'b') && (read_2_key == 'l') && (read_3_key == 'e')) {
    j = 1;
    analysis_bxx_label(line);
    instr_table[line][2] = INSTRU_BLE;
    return j;
  } //INSTRUTYPE_BRANCH	
  else if ((read_1_key == 'l') && (read_2_key == 's') && (read_3_key == 'l')) {
    instr_table[line][1] = INSTRUTYPE_SPECIAL;
    instr_table[line][2] = INSTRU_LSL;
  } else
    printf("unknown instruction_cMd\n");

  return j;
}

void analysis_b_label(int line) //b foo:
{
  int j = 0;
  instr_table[line][4] = read_3_key;
  instr_table[line][5] = read_4_key;
  read_w_key = getc(fp);
  while (read_w_key != NEW_LINE_CHAR) {
    instr_table[line][6 + j] = read_w_key;
    read_w_key = getc(fp);
    j++;
  }
  instr_table[line][1] = INSTRUTYPE_BRANCH;
  instr_table[line][2] = INSTRU_B;
  return;
}

void analysis_bxx_label(int line) //b foo:
{
  int j = 0;
  read_w_key = getc(fp);
  while (read_w_key != NEW_LINE_CHAR) {
    instr_table[line][4 + j] = read_w_key;
    read_w_key = getc(fp);
    j++;
  }
  instr_table[line][1] = INSTRUTYPE_BRANCH;
  return;
}

void analysis_last_label(int line) {
  int j = 0;
  read_w_key = getc(fp);
  instr_table[line][8] = read_w_key;
  while (read_w_key != ':') {
    read_w_key = getc(fp);
    instr_table[line][9 + j] = read_w_key;
  }
  return;
}

void analysis_second_word(int line) {
  read_w_key = getc(fp);
  if (read_w_key == ' ') {
    read_w_key = getc(fp); 
  }
  if (read_w_key != 'r')
    printf("unknown No2_paragraph  r!\n");

  read_w_key = getc(fp);
  instr_table[line][3] = read_w_key - CHANGE_TO_INT; //char --> int

  read_w_key = getc(fp);
  if (read_w_key != ',')
    printf("unknown No2_paragraph   line: %d  ,!\n", line);

  return;
}

void analysis_third_word_and_after(int line) //# r [ = ' '   will do while((read_1_key = getc(fp)) != NEW_LINE_CHAR)
{
  int j = 0;
  read_w_key = getc(fp);
  registers_in_no3_paragraph_but_not_in_bracket = 0;
  instr_table[line][10] = registers_in_no3_paragraph_but_not_in_bracket;
  while (read_w_key == ' ') // get off ' '
    read_w_key = getc(fp);

  if (read_w_key == '#') //read out #
  {
    analysis_third_word_as_number(line); //#2  #-2   

  } else if (read_w_key == '=') // only ldr
  {
    read_w_key = getc(fp); //read out  0
    read_w_key = getc(fp); //read out  x
    read_w_key = getc(fp); //read out  first 16hexnumbe
    j = analysis_16_based_help_func(line);
    instr_table[line][4] = NUM_EQ;
    if ((instr_table[line][5] <= 0xff) && (instr_table[line][1] == INSTRUTYPE_SINGLE_DATA_TRANSFER) && (instr_table[line][2] == INSTRU_LDR)) {
      instr_table[line][1] = INSTRUTYPE_DATA_PROCESSING;
      instr_table[line][2] = MOV; // spec P15
    }
  } else if (read_w_key == '[') {
    read_w_key = getc(fp); // read out r

    read_w_key = getc(fp);
    instr_table[line][6] = read_w_key - CHANGE_TO_INT; //read out  n char --> int   
    instr_table[line][4] = NUM_BRACKET_R; //[r
    restart_in_bracket:
      read_w_key = getc(fp);
    if (read_w_key == ']') {
      read_w_key = getc(fp);
      if (read_w_key == NEW_LINE_CHAR) // end or ','
        return;
      else if ((read_w_key = getc(fp)) == 'r') {

        instr_table[line][4] = NUM_BRACKET_R; //[ r  r
        read_w_key = getc(fp); //read out n delete
        instr_table[line][7] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
        registers_in_no3_paragraph_but_not_in_bracket++;
        instr_table[line][10] = registers_in_no3_paragraph_but_not_in_bracket;
        read_w_key = getc(fp); //read out NEW_LINE_CHAR
      } else if (read_w_key == '#') {
        analysis_third_word_as_number(line);
        instr_table[line][4] = NUM_BRACKET_X;
      }
    } else if (read_w_key == ' ')
      goto restart_in_bracket; //get off ' '
    else if (read_w_key == ',')
      goto restart_in_bracket; //get off ','
    else if (read_w_key == '#') {
      analysis_third_word_as_number(line);
      instr_table[line][4] += 5; // [#2  or [#-2
    } //NUM_BRACKET_POUND = 5,No3_paragraph fist char is  [# 
    else if (read_w_key == 'r') {
      instr_table[line][4] = NUM_BRACKET_R; //[r
      read_w_key = getc(fp);
      instr_table[line][7] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
      goto restart_in_bracket;
    } else if (read_w_key == 'l') //lsr r2
    {
      read_w_key = getc(fp); //s
      read_w_key = getc(fp); //r 
      read_w_key = getc(fp); //' '
      read_w_key = getc(fp); //r or '#'
      instr_table[line][9] = HAS_BRACKET_LSR;
      if (read_w_key == '#') {
        read_w_key = getc(fp);
        instr_table[line][5] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
        instr_table[line][4] = NUM_BRACKET_LSR_POUND; // [ lsr #
      } else {
        read_w_key = getc(fp);
        instr_table[line][8] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
        instr_table[line][4] = NUM_BRACKET_LSR_R; //[ lsr r
      }
      goto restart_in_bracket;
    } else
      printf("unknown No3_paragraph_Rs\n");
  } else if (read_w_key == 'r') {
    registers_in_no3_paragraph_but_not_in_bracket++;
    instr_table[line][10] = registers_in_no3_paragraph_but_not_in_bracket;
    read_w_key = getc(fp);
    instr_table[line][6] = read_w_key - CHANGE_TO_INT; //read out  n char --> int   
    instr_table[line][4] = NUM_R; // r
    restart:
      read_w_key = getc(fp);
    if (read_w_key == NEW_LINE_CHAR) // end
      return;
    else if (read_w_key == ' ')
      goto restart; //get off ' '
    else if (read_w_key == ',')
      goto restart; //get off ','
    else if (read_w_key == '#')
      analysis_third_word_as_number(line);
    else if (read_w_key == 'r') {
      registers_in_no3_paragraph_but_not_in_bracket++;
      instr_table[line][10] = registers_in_no3_paragraph_but_not_in_bracket;
      read_w_key = getc(fp);
      instr_table[line][7 + j] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
      j++;
      goto restart;
    } else if (read_w_key == 'l') //lsr r2  
    {
      read_w_key = getc(fp); //s
      read_w_key = getc(fp); //r 
      read_w_key = getc(fp); //' '
      read_w_key = getc(fp); //r or '#'
      instr_table[line][9] = HAS_LSR;
      if (read_w_key == 'r') {
        registers_in_no3_paragraph_but_not_in_bracket++;
        instr_table[line][10] = registers_in_no3_paragraph_but_not_in_bracket;
        read_w_key = getc(fp);
        instr_table[line][8] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 
        instr_table[line][4] = NUM_R_LSR_R; // r lsr r
      } else {
        read_w_key = getc(fp);
        instr_table[line][5] = read_w_key - CHANGE_TO_INT; //read out  n char --> int 				
        instr_table[line][4] = NUM_R_LSR_POUND; // r lsr #
      }
      goto restart;
    } else
      printf("unknown No3_paragraph_Rs\n");
  } else
    printf("unknown No3_paragraph\n");
}

void analysis_third_word_as_number(int line) //read out #       (#1 #0x3F0000 #-0x4 #0x0F)
{
  int j = 0;

  instr_table[line][4] = NUM_POUND;
  read_w_key = getc(fp); //read out  first 10hexnumbe
  if (read_w_key == '0') //16hexnumbe
  {
    j = analysis_number_10_or_16_based(line);
    if (j == 0xaa) //'x'
    {
      read_w_key = getc(fp); //read out  first 16hexnumbe
      j = analysis_16_based_help_func(line);
    }
  } else if (read_w_key == '-') //#-0x4 
  {
    instr_table[line][4] = NUM_POUND_NEGATIVE;
    read_1_key = getc(fp); //read out 0
    read_2_key = getc(fp); //read out  x
    read_w_key = getc(fp); //read out  first 16hexnumbe
    j = analysis_16_based_help_func(line);
  } else if ((read_w_key >= '0') && (read_w_key <= '9')) //#1  //#57954613//read out first 10hexnumber
  {
    j = analysis_number_10_or_16_based(line);

  } else
    printf("unknown No3_pound\n");
}

int analysis_number_10_or_16_based(int line) //#1
{
  int j = 0;
  int Tmp = 0;

  Tmp = read_w_key - CHANGE_TO_INT;
  read_w_key = getc(fp); //read out next 10hexnumber
  if (read_w_key == 'x')
    return 0xaa;
  while ((read_w_key != NEW_LINE_CHAR)) {
    if ((read_w_key >= '0') && (read_w_key <= '9')) {
      Tmp = Tmp * 10;
      Tmp += read_w_key - CHANGE_TO_INT;
    } else if (read_w_key == NEW_LINE_CHAR)
      break;
    else if (read_w_key == ']')
    ;
    else if ((read_w_key != ',') && (read_w_key != ' ')) {
      printf("unknown No3_10hexnumber !last char!\n");
      return j;
    } else // == ','or' '
    {
      j++;
      break;
    }
    read_w_key = getc(fp); //read out next 10hexnumber
  }
  instr_table[line][5] = Tmp;
  return j;
}

int analysis_16_based_help_func(int line) //#0x
{
  int j = 0;
  int Tmp = 0;

  if ((read_w_key >= '0') && (read_w_key <= '9')) {
    Tmp = read_w_key - CHANGE_TO_INT;
  } else if ((read_w_key >= 'A') && (read_w_key <= 'F')) // 'A'---'F'
  {
    Tmp = read_w_key - HEX_HELP;
  } else if ((read_w_key >= 'a') && (read_w_key <= 'f')) // 'a'--'f'
  {
    Tmp = read_w_key - HEX_HELP_2;
  } else {
    printf("unknown No3_11hexnumber \n");
    return j;
  }

  read_w_key = getc(fp); //read out next 16hexnumber
  while ((read_w_key != NEW_LINE_CHAR)) {
    if ((read_w_key >= '0') && (read_w_key <= '9')) {
      Tmp = Tmp * 16;
      Tmp += read_w_key - CHANGE_TO_INT;
    } else if ((read_w_key >= 'A') && (read_w_key <= 'F')) // 'A'---'F'
    {
      Tmp = Tmp * 16;
      Tmp += read_w_key - HEX_HELP;
    } else if ((read_w_key >= 'a') && (read_w_key <= 'f')) // 'a'--'f'
    {
      Tmp = Tmp * 16;
      Tmp += read_w_key - HEX_HELP_2;
    } else if (read_w_key == NEW_LINE_CHAR)
      break;
    else if (read_w_key == ']')
    ;
    else if ((read_w_key != ',') && (read_w_key != ' ')) {
      printf("unknown No3_11hexnumber \n");
      return j;
    } else // == ','or' '
    {
      j++;
      break;
    }
    read_w_key = getc(fp); //read out next 10hexnumber
  }
  instr_table[line][5] = Tmp;
  return j;
}

void single_data_transfer(int line, instruction *instr) { //second pass single data transfer, store as the 32bit instruction
  int i, j, k, TMP, TMP1;

  instr->SingleDataTransfer.Cond = ALWAYS;
  instr->SingleDataTransfer.bit2726 = 1;

  instr->SingleDataTransfer.I = 0;

  if (instr_table[line][4] == NUM_BRACKET_X) {
    instr->SingleDataTransfer.P = 0;
  }

  if ((instr_table[line][4] == NUM_EQ) || (instr_table[line][4] == NUM_BRACKET_R) || (instr_table[line][4] == NUM_BRACKET_POUND) || (instr_table[line][4] == NUM_BRACKET_POUND_NEGATIVE) || (instr_table[line][4] == NUM_BRACKET_LSR_POUND) || (instr_table[line][4] == NUM_BRACKET_LSR_R)) { //p15  <=expression> (ldr only). [Rn], [Rn,<#expression>]
    instr->SingleDataTransfer.P = 1;
  } else //[Rn],<#expression>
    instr->SingleDataTransfer.P = 0;

  instr->SingleDataTransfer.U = 1;
  instr->SingleDataTransfer.bit2221 = 0;
  if (instr_table[line][2] == INSTRU_LDR)
    instr->SingleDataTransfer.L = 1;
  else if (instr_table[line][2] == INSTRU_STR)
    instr->SingleDataTransfer.L = 0;

  if (instr_table[line][7] != INITIAL_VALUE) {
    instr->SingleDataTransfer.Offset = instr_table[line][7];
    instr->SingleDataTransfer.I = 1;
  } else {
    instr->SingleDataTransfer.Offset = 0;
  }
  if (instr_table[line][10] == 1)
    instr->SingleDataTransfer.P = 0;

  if ((instr_table[line][2] == INSTRU_LDR) && (instr_table[line][4] == NUM_EQ)) { //P15 <=expression> (ldr only).
    instr->SingleDataTransfer.Rn = 15; //pc

    instr->SingleDataTransfer.Offset = (expr_num + lines - labels - line - 2) * 4;
    expr_num++;

  } else {
    if (instr_table[line][4] == NUM_BRACKET_R) {
      instr->SingleDataTransfer.Rn = instr_table[line][6];
    } else if (instr_table[line][4] == NUM_BRACKET_POUND || instr_table[line][4] == NUM_BRACKET_X) {
      instr->SingleDataTransfer.Offset = instr_table[line][5] & BIT_MASK_OFFSET_ONE; // bit11-0

    } else if (instr_table[line][4] == NUM_BRACKET_POUND_NEGATIVE) { 
      instr->SingleDataTransfer.Offset = instr_table[line][5] & BIT_MASK_OFFSET_ONE;
      instr->SingleDataTransfer.U = 0;
    }
  }
  if (instr_table[line][9] != INITIAL_VALUE) {
    instr->SingleDataTransfer.I = 1;
    instr->data_pro_reg.Shift_type = instr_table[line][9] - 1;
    if (instr_table[line][8] != INITIAL_VALUE) {
      instr->data_pro_reg.Integer = instr_table[line][7] << 1;
      instr->data_pro_reg.bit4 = 1;
    } else {
      instr->data_pro_reg.Integer = instr_table[line][5];
      instr->data_pro_reg.bit4 = 0;
    }
  }
  instr->SingleDataTransfer.Rd = instr_table[line][3];
  instr->SingleDataTransfer.Rn = instr_table[line][6];

}
void branch(int line, instruction *instr) { //second pass branch instruction, store as the 32bit instruction
  int i = 0;
  int j = 0;
  int k = 0;
  int label_line = 0;
  int NumOfOtherLabel_between_BranchLineANDLabelLine = 0;
  int offset = 0;
  int is_equal = 1; 

  instr->Branch.bit2726 = 2;
  instr->Branch.bit2524 = 2;
  instr->Branch.Cond = instr_table[line][2];

  for (i = 0; i < lines; i++) //search label_line
  {
    is_equal = 1; 
    if (instr_table[i][1] == INSTRUTYPE_LABEL) 
    {
      for (int j = 0; j < SEARCH_LABEL_LENGTH_OF_CHAR; j++) {
        if (instr_table[i][j + 4] == COLON && instr_table[line][j + 4] == INITIAL_VALUE && is_equal) {
          label_line = i;
        }
        if (instr_table[i][j + 4] != COLON && instr_table[i][j + 4] != instr_table[line][j + 4]) {
          is_equal = 0;
        }
      }
    }
  }
  ////// srh othr Label between BranchLine AND LabelLine
  if (line < label_line) //jump down
  {
    for (i = line + 1; i < label_line; i++)
      if (instr_table[i][1] == INSTRUTYPE_LABEL)
        NumOfOtherLabel_between_BranchLineANDLabelLine++;
    offset = label_line - line - 2 - NumOfOtherLabel_between_BranchLineANDLabelLine;
  } else //jump up
  {
    for (i = label_line + 1; i < line; i++)
      if (instr_table[i][1] == INSTRUTYPE_LABEL)
        NumOfOtherLabel_between_BranchLineANDLabelLine++;
    offset = label_line + 1 - line - 2 + NumOfOtherLabel_between_BranchLineANDLabelLine; //change - to +
  }
  instr->Branch.Sign = (offset >> 31) & 1;
  instr->Branch.Offset = offset & BIT_MASK_OFFSET_TWO; //bit22--0
}

void data_processing(int line, instruction *instr) {//second pass data process, store as the 32bit instruction
  instr->data_pro.Cond = ALWAYS; //Cond
  instr->data_pro.bit2726 = 0; //bit2726
  if (instr_table[line][4] <= 1) //I
  {
    instr->data_pro.I = 1;
    instr->data_pro.Operand2 = instr_table[line][5];
  } else {
    instr->data_pro.I = 0;

  }
  if (instr_table[line][2] == INSTRU_LSL)
    instr->data_pro.I = 0;
  instr->data_pro.OpCode = instr_table[line][2]; //OpCode
  if (instr_table[line][2] == INSTRU_LSL) //P16  lsl Rn, <#expression>  === MOV Rn,Rn, lsl <#expression>.
    instr->data_pro.OpCode = MOV;

  if ((instr_table[line][2] == TST) || (instr_table[line][2] == TEQ) || (instr_table[line][2] == CMP)) //S
    instr->data_pro.S = 1;
  else
    instr->data_pro.S = 0;

  ///// set  Rn,Rd,Operand2
  if (instr_table[line][10] == 0) //P13
  {
    instr->data_pro.Operand2 = instr_table[line][5];
    if ((instr_table[line][2] == TST) || (instr_table[line][2] == TEQ) || (instr_table[line][2] == CMP)) { //TST, TEQ, CMP.<opcode> Rn, <Operand2>   ie.CMP r1,#2
      instr->data_pro.Rn = instr_table[line][3];
      instr->data_pro.Rd = 0;
    } else if (instr_table[line][2] == MOV) // MOV Rd, <Operand2>
    {
      instr->data_pro.Rd = instr_table[line][3];
      instr->data_pro.Rn = 0;
      instr->data_pro.I = 1; 
      int imm_value = instr_table[line][5];
      int count = 0;
      int rotate;

      if (imm_value < 0xff) {
        instr->data_pro.Operand2 = imm_value;
      } else {
        while (imm_value % 2 == 0) {
          if (imm_value % 4 != 0 && count % 2 == 0) {
            break;
          }
          imm_value >>= 1;
          count++;
        }
        rotate = 32 - count;
        rotate >>= 1;
        instr->data_pro_imm.Rotate = rotate;
        instr->data_pro_imm.Imm = imm_value; 

      }
    } else if (instr_table[line][2] == INSTRU_LSL) //P16  lsl Rn, <#expression>  === MOV Rn,Rn, lsl <#expression>.
    {

      instr->data_pro_reg.Rm = instr_table[line][3];
      instr->data_pro_reg.bit4 = 0;
      instr->data_pro_reg.Shift_type = 0;
      instr->data_pro_reg.Integer = instr_table[line][5]; 

    }

  } else if (instr_table[line][10] == 1) {
    if ((instr_table[line][2] == TST) || (instr_table[line][2] == TEQ) || (instr_table[line][2] == CMP)) { // TST, TEQ, CMP. <opcode> Rn, <Operand2>   ie.CMP r1,r2
      instr->data_pro.Rn = instr_table[line][3];
      instr->data_pro.Rd = 0;
      instr->data_pro.Operand2 = instr_table[line][6]; //TST, TEQ, CMP. <opcode> Rn, <Operand2>; <Operand2>= r2, I must is 0
    } else if ((instr_table[line][2] == AND) || (instr_table[line][2] == EOR) || (instr_table[line][2] == SUB) || (instr_table[line][2] == RSB) || (instr_table[line][2] == ADD) || (instr_table[line][2] == ORR)) {
      instr->data_pro.Rd = instr_table[line][3];
      instr->data_pro.Rn = instr_table[line][6];
      instr->data_pro.Operand2 = instr_table[line][5]; //P13  <opcode> Rd, Rn, <Operand2>
    } else if (instr_table[line][2] == MOV) {
      instr->data_pro.Rd = instr_table[line][3];
      instr->data_pro.Rn = 0;
      instr->data_pro.I = 0;
      instr->data_pro.Operand2 = instr_table[line][6];

    }
  } else if (instr_table[line][10] == 2) {
    instr->data_pro.Rd = instr_table[line][3];
    instr->data_pro.Rn = instr_table[line][6];
    instr->data_pro.Operand2 = instr_table[line][7];

  } else if (instr_table[line][10] == 3) {
    instr->data_pro.Rd = instr_table[line][3];
    instr->data_pro.Rn = instr_table[line][6];
    instr->data_pro_reg.Rm = instr_table[line][7];
    instr->data_pro_reg.bit4 = 1;
    instr->data_pro_reg.Shift_type = instr_table[line][9];
    instr->data_pro_reg.Integer = instr_table[line][8] << 1;

  } else
    printf(" unknown registers_in_no3_paragraph_but_not_in_bracket   !\n");
}

void multiply(int line, instruction *instr) { //second pass multiply process, store as the 32bit instruction
  instr->Multiply.bit2726 = 0;
  instr->Multiply.bit2522 = 0;
  instr->Multiply.bit74 = 9;
  instr->Multiply.A = 0;
  instr->Multiply.S = 1;
  for (int i = 0; i < line; i++) {
    if (instr_table[i][1] == 1) {
      instr->Multiply.Cond = 14;
      instr->Multiply.S = 0;
      break;
    }
  }
  instr->Multiply.Rd = instr_table[line][3];
  instr->Multiply.Rm = instr_table[line][6];
  instr->Multiply.Rs = instr_table[line][7];

  if (instr_table[line][10] == 3) {
    instr->Multiply.Rn = instr_table[line][8];
    instr->Multiply.A = 1;
  } else {
    instr->Multiply.Rn = 0;
  }

}




































