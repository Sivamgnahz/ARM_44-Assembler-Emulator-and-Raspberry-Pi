#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler_types.h"
#include "assembler.h"

FILE *fpw;
FILE *fp;

instruction instr;

int	Instr_Table[Total_Label_Line_Num_buffr][11];
char	read_1_key,read_2_key,read_3_key,read_4_key,read_w_key;
int	TotalRegNum_inNo3_paragraph_butnot_in_bracket=0;	
int	Line_Num=0;
int	data_transfer_ldr =0;
int	Total_Label_Num=0;
int expr_num =0;

	
int main(int argc, char *argv[]) 
{
	char *p;
	char p1[20] = "";
	char p2[20] = "";
	char p3[20] = "";
	char p4[20] = "";
	int onntr_num =0;
	int j; int i;	
	
	//fp = fopen(argv[1],"r");
	fpw = fopen(argv[2], "wb");
	for(i=0;i<Total_Label_Line_Num_buffr;i++)
		for(j=0;j<11;j++)
			Instr_Table[i][j] = 0xffffffff;
		
	Line_Num = Read_from_file(argv[1]);

	data_transfer_ldr =0xffffffff;
	for(j=0;j<Line_Num;j++) 
	{
		switch(Instr_Table[j][1])
		{
			case instrutype_data_processing:
				printf(" instrutype_data_processing...1      ");
				data_processing(j);
				break;
			case instrutype_multiply:	 
				printf(" instrutype_multiply.....2          ");	
				multiply(j);
				break;
			case instrutype_single_data_transfer:	 
				printf(" instrutype_single_data_transfer....3");
				printf("aaaaacccccccccccaaaa ");
				single_data_transfer(j);
				break;
			case instrutype_branch:
				branch(j);
				printf(" instrutype_branch.....4             ");	
				break;
			case instrutype_special:
				printf(" instrutype_special.........5        ");
				if(Instr_Table[j][2] == instru_ANDEQ)
				{
					instr.instr_32bits = 0x00000000;
				}
				else if(Instr_Table[j][2] == instru_lsl)
					data_processing(j);
 				break;
			case instrutype_label:	 
				printf(" instrutype_label..........6\n");	
				break;
			default: 
				printf("? unknown instrutype   !\n");
				break;
		}
		if(Instr_Table[j][1] != instrutype_label)
		{
			printf(" will write 0x%08x to file fpw   !\n",instr.instr_32bits);
			fwrite(&instr,4,1,fpw); //write instruction
		}	
	}

	for(int i = 0; i <Line_Num; i++){ //hzz
		if(Instr_Table[i][2] == 48 && Instr_Table[i][4] == 2){
		instr.instr_32bits = Instr_Table[i][5];
		printf(" will write 0x%08x to file fpw last line for data_transfer_ldr  !\n",instr.instr_32bits);
		fwrite(&instr,4,1,fpw);
		}
	}
	fclose(fp);
	fclose(fpw);
	return 0;
}
void single_data_transfer(int line)
{
	printf("aaaaacccccccccccaaaa :%d", line);
int i,j,k,TMP,TMP1;

	instr.SingleDataTransfer.Cond = always;
	instr.SingleDataTransfer.bit2726 = 1;



	instr.SingleDataTransfer.I = 0;

	if(Instr_Table[line][4] == Num_bracket_x){
		instr.SingleDataTransfer.P = 0;
	}

	if((Instr_Table[line][4] == Num_EQ)||(Instr_Table[line][4] == Num_bracket_r)||(Instr_Table[line][4] == Num_bracket_pound)||(Instr_Table[line][4] == Num_bracket_pound_negative)||(Instr_Table[line][4] == Num_bracket_lsr_pound)||(Instr_Table[line][4] == Num_bracket_lsr_r))
	{//p15  <=expression> (ldr only). [Rn], [Rn,<#expression>]
		instr.SingleDataTransfer.P = 1;
	}
	else //[Rn],<#expression>
		instr.SingleDataTransfer.P = 0;
	
	instr.SingleDataTransfer.U = 1;
	instr.SingleDataTransfer.bit2221 = 0;
	if(Instr_Table[line][2] == instru_ldr)
		instr.SingleDataTransfer.L = 1;
	else if(Instr_Table[line][2] == instru_str)
		instr.SingleDataTransfer.L = 0;

	if (Instr_Table[line][7] != 0xffffffff)
	{
		instr.SingleDataTransfer.Offset = Instr_Table[line][7];
		instr.SingleDataTransfer.I = 1;	
	}
	else {
		instr.SingleDataTransfer.Offset = 0;
	}
	if (Instr_Table[line][10] == 1) 
		instr.SingleDataTransfer.P = 0;

	if((Instr_Table[line][2] == instru_ldr)&&(Instr_Table[line][4] == Num_EQ))
	{//P15 <=expression> (ldr only).
		instr.SingleDataTransfer.Rn = 15;//pc	
		//data_transfer_ldr = Instr_Table[line][5];// the assembler should put the value of <expression> in four bytes at the end of the assembled program
		instr.SingleDataTransfer.Offset = (expr_num + Line_Num -Total_Label_Num - line -2)*4;
		expr_num++;//xq
	}
	else
	{
		if (Instr_Table[line][4] == Num_bracket_r) {
			instr.SingleDataTransfer.Rn = Instr_Table[line][6];
		}
		else if(Instr_Table[line][4] == Num_bracket_pound || Instr_Table[line][4] == Num_bracket_x) {
			instr.SingleDataTransfer.Offset = Instr_Table[line][5]&0x00000fff;// bit11-0		
		}
		else if(Instr_Table[line][4] == Num_bracket_pound_negative){  //hzz
			instr.SingleDataTransfer.Offset = Instr_Table[line][5]&0x00000fff;
				instr.SingleDataTransfer.U = 0;
		}
	}
	if (Instr_Table[line][9] != 0xffffffff) {
		instr.SingleDataTransfer.I = 1;
		instr.data_pro_reg.Shift_type = Instr_Table[line][9] - 1;
		if (Instr_Table[line][8] != 0xffffffff) {
			instr.data_pro_reg.Integer = Instr_Table[line][7] << 1;
			instr.data_pro_reg.bit4 = 1;
		}
		else {
			instr.data_pro_reg.Integer = Instr_Table[line][5];
			instr.data_pro_reg.bit4 = 0;
		}
	}
	instr.SingleDataTransfer.Rd = Instr_Table[line][3];
	instr.SingleDataTransfer.Rn = Instr_Table[line][6];


}
void branch(int line)
{
int i = 0;
int j = 0;
int k = 0;
int label_line = 0;
int NumOfOtherLabel_between_BranchLineAndLabelLine = 0;
int offset= 0;
int is_equal = 1; //hzz
	
	instr.Branch.bit2726 = 2;
	instr.Branch.bit2524 = 2;
	instr.Branch.Cond = Instr_Table[line][2];
 		
	for(i=0;i<Line_Num;i++)//search label_line
	{
		if(Instr_Table[i][1] == instrutype_label) //hzz
		{
			for(int j = 0; j<search_label_lenthofchar;j++){
				if(Instr_Table[i][j+4]== 0x3a && Instr_Table[line][j+4] == 0xffffffff && is_equal){
					label_line = i;
				} 
				if(Instr_Table[i][j+4] != 0x3a && Instr_Table[i][j+4] != Instr_Table[line][j+4]){
					is_equal = 0;
				}
			}
		}
	}
	if(line < label_line)//jump down
	{
		for(i=line+1;i<label_line;i++)
			if(Instr_Table[i][1] == instrutype_label)
				NumOfOtherLabel_between_BranchLineAndLabelLine++;
		offset = label_line - line - 2 - NumOfOtherLabel_between_BranchLineAndLabelLine;
	}
	else//jump up
	{
		for(i=label_line+1;i<line;i++)
			if(Instr_Table[i][1] == instrutype_label)
				NumOfOtherLabel_between_BranchLineAndLabelLine++;
		offset =  label_line+1-line-2+NumOfOtherLabel_between_BranchLineAndLabelLine; //hzz, change - to +
	}
			
	instr.Branch.Sign = (offset&0x80000000)>>31;//bit31-->bit23	negative bit23
	instr.Branch.Offset = offset&0x007fffff;//bit22--0
}
void data_processing(int line)
{
	instr.data_pro.Cond = always;//Cond
	instr.data_pro.bit2726 = 0;//bit2726
	if (Instr_Table[line][4] <= 1)//I
	{
		instr.data_pro.I = 1;
		instr.data_pro.Operand2 = Instr_Table[line][5];
	}
	else
	{
		instr.data_pro.I = 0;
		
	}
	if(Instr_Table[line][2] == instru_lsl)
		instr.data_pro.I = 0;
	instr.data_pro.OpCode = Instr_Table[line][2];//OpCode
	if(Instr_Table[line][2] == instru_lsl)//P16  lsl Rn, <#expression>  === mov Rn,Rn, lsl <#expression>.
		instr.data_pro.OpCode = mov;
		
	if((Instr_Table[line][2] == tst )||(Instr_Table[line][2] == teq )||(Instr_Table[line][2] == cmp ))//S
		instr.data_pro.S = 1;
	else
		instr.data_pro.S = 0;
	
	///// set  Rn,Rd,Operand2
	if(Instr_Table[line][10] == 0)//P13
	{
		instr.data_pro.Operand2 = Instr_Table[line][5];
		if((Instr_Table[line][2] == tst )||(Instr_Table[line][2] == teq )||(Instr_Table[line][2] == cmp ))
		{//tst, teq, cmp.<opcode> Rn, <Operand2>   ie.cmp r1,#2
			instr.data_pro.Rn = Instr_Table[line][3];
			instr.data_pro.Rd = 0;
		}
		else if(Instr_Table[line][2] == mov)// mov Rd, <Operand2>
		{
			instr.data_pro.Rd = Instr_Table[line][3];
			instr.data_pro.Rn = 0;
			instr.data_pro.I = 1;//xq
			int imm_value = Instr_Table[line][5];
			int count = 0;
			int rotate;

			if (imm_value < 0xff) {
				instr.data_pro.Operand2 = imm_value;
			} else {
				while(imm_value % 2 == 0) {
					if (imm_value % 4 != 0 && count % 2 == 0) {
						break;
					}
					imm_value >>= 1;
					count++;
				}
				rotate = 32 - count;
				rotate >>= 1;
				instr.data_pro_imm.Rotate = rotate;
				instr.data_pro_imm.Imm = imm_value;//xq
		
			}
		}
		else if(Instr_Table[line][2] == instru_lsl)//P16  lsl Rn, <#expression>  === mov Rn,Rn, lsl <#expression>.
		{

			instr.data_pro_reg.Rm = Instr_Table[line][3];
			instr.data_pro_reg.bit4 = 0;
			instr.data_pro_reg.Shift_type = 0;
			instr.data_pro_reg.Integer = Instr_Table[line][5];//xq

		}
		
	}
	else if(Instr_Table[line][10] == 1)
	{
		if((Instr_Table[line][2] == tst)||(Instr_Table[line][2] == teq)||(Instr_Table[line][2] == cmp))
		{// tst, teq, cmp. <opcode> Rn, <Operand2>   ie.cmp r1,r2
			instr.data_pro.Rn = Instr_Table[line][3];
			instr.data_pro.Rd = 0;
			instr.data_pro.Operand2 = Instr_Table[line][6];//tst, teq, cmp. <opcode> Rn, <Operand2>; <Operand2>= r2, I must is 0
		}
		else if((Instr_Table[line][2] == and)||(Instr_Table[line][2] == eor)||(Instr_Table[line][2] == sub)||(Instr_Table[line][2] == rsb)||(Instr_Table[line][2] == add)||(Instr_Table[line][2] == orr))
		{
			instr.data_pro.Rd = Instr_Table[line][3];
			instr.data_pro.Rn = Instr_Table[line][6];
			instr.data_pro.Operand2 = Instr_Table[line][5];//P13  <opcode> Rd, Rn, <Operand2>
		}
		else if(Instr_Table[line][2] == mov) {
			instr.data_pro.Rd = Instr_Table[line][3];
			instr.data_pro.Rn = 0;
			instr.data_pro.I = 0;
			instr.data_pro.Operand2 = Instr_Table[line][6];

		}
	}
	else if(Instr_Table[line][10] == 2)
	{
		instr.data_pro.Rd = Instr_Table[line][3];
		instr.data_pro.Rn = Instr_Table[line][6];
		instr.data_pro.Operand2 = Instr_Table[line][7];

	}
	else if(Instr_Table[line][10] == 3)
	{
		instr.data_pro.Rd = Instr_Table[line][3];
		instr.data_pro.Rn = Instr_Table[line][6];
		instr.data_pro_reg.Rm = Instr_Table[line][7];
		instr.data_pro_reg.bit4 = 1;
		instr.data_pro_reg.Shift_type = Instr_Table[line][9];
		instr.data_pro_reg.Integer = Instr_Table[line][8] << 1;

	}
	else 
		printf(" unknown TotalRegNum_inNo3_paragraph_butnot_in_bracket   !\n");

	
}

void multiply(int line){
	instr.Multiply.bit2726 = 0;
	instr.Multiply.bit2522 = 0;
    instr.Multiply.bit74 = 9;
	instr.Multiply.A = 0;
	instr.Multiply.S = 1;
	for(int i = 0; i < line; i++){
		if(Instr_Table[i][1] == 1){
			instr.Multiply.Cond = 14;
			instr.Multiply.S = 0;
			break;
		}
	}
	instr.Multiply.Rd = Instr_Table[line][3];
	instr.Multiply.Rm = Instr_Table[line][6];
	instr.Multiply.Rs = Instr_Table[line][7];

	if(Instr_Table[line][10] == 3){
		instr.Multiply.Rn = Instr_Table[line][8];
		instr.Multiply.A = 1;
	} else {
		instr.Multiply.Rn = 0;
	}
}

//split assembly and make a table
int Read_from_file(char argv[])
{
int j; int i;	
int Line_Num_File=0;
int Line_realinstruNum=0;

	if((fp = fopen(argv,"r")) == NULL)
	{
		printf(" cannot open file \n");
		return 1;
	}
	i=0;j=0;
L_first:	
	while((read_1_key = getc(fp)) != EOF)
	{
		if((read_1_key == 0x0a)||(read_1_key == ' '))
			goto L_first;
		read_2_key =getc(fp);
		if(read_2_key == 0x0a)
			goto L_first;
		read_3_key =getc(fp);
		if(read_3_key == 0x0a)
			goto L_first;
		read_4_key =getc(fp);
		if(read_4_key == 0x0a)	
			goto L_first;
		
		
		Instr_Table[i][0] = i;
		if(read_2_key == ' ')//b foo
		{
			analysis_b_label(i);//if branch has Read 0x0a!  
			goto exit_would_not_while;
		}
		else if(read_4_key == ' ') //xxx' ' 
		{
			j = analysis_First_paragraph(i);// only here will continue......
			if(j)			//if branch has Read 0x0a!  
				goto exit_would_not_while; 	
		}

		else if(read_4_key == 'e')//instrutype_special instru_ANDEQ
		{
			if((read_1_key == 'a')&&(read_2_key == 'n')&&(read_3_key == 'd'))
				{Instr_Table[i][1] = instrutype_special;Instr_Table[i][2] = instru_ANDEQ;}
			goto exit_willdowhile;
		}
		else// label
		{
			if(read_4_key == ':')
				{Instr_Table[i][4] = read_1_key;Instr_Table[i][5] = read_2_key;Instr_Table[i][6] = read_3_key;Instr_Table[i][7] = read_4_key;}		
			else 
				analysis_last_label(i);
			Total_Label_Num++;
			Instr_Table[i][1] = instrutype_label;Instr_Table[i][2] = i+1;
			Instr_Table[i][4] = read_1_key;Instr_Table[i][5] = read_2_key;Instr_Table[i][6] = read_3_key;Instr_Table[i][7] = read_4_key;			
			goto exit_willdowhile;
		}
		/////////////////////
		analysis_No2_paragraph(i);//r1,
		/////////////////////
		analysis_No3_paragraph(i);//# r [ = -
		goto exit_would_not_while;
exit_willdowhile:
		//while((read_1_key = getc(fp)) != 0x0a);//enter ���з���
		read_1_key = getc(fp);
		if(read_1_key == 0x0a)
			goto exit_would_not_while; 
		else if(read_1_key == EOF)
			break;
		else
			goto exit_willdowhile;
exit_would_not_while:		
		Line_Num_File++;
		i++;
		j=0;
		
			
	}
	
	printf("Line0    Inst_ty1 ins_cMd2 Rn3      typ_Num4 Number5  Rd6      Rs7      Rm8      Lsr9     TotalRegNum_inNo3_paragraph_butnot_in_bracket\n");
	for(i=0;i<Total_Label_Line_Num_buffr;i++)
	{
		for(j=0;j<11;j++)
			printf("%08x ",Instr_Table[i][j]);
		printf("\n");
	}
	return Line_Num_File;
}


int analysis_First_paragraph(int line) 
{
	int j=0;
	if((read_1_key == 'a')&&(read_2_key == 'n')&&(read_3_key == 'd'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = and;}
 	else if((read_1_key == 'e')&&(read_2_key == 'o')&&(read_3_key == 'r'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = eor;}
	else if((read_1_key == 's')&&(read_2_key == 'u')&&(read_3_key == 'b'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = sub;}	
	else if((read_1_key == 'r')&&(read_2_key == 's')&&(read_3_key == 'b'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = rsb;}	
	else if((read_1_key == 'a')&&(read_2_key == 'd')&&(read_3_key == 'd'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = add;}	
	else if((read_1_key == 't')&&(read_2_key == 's')&&(read_3_key == 't'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = tst;}
	else if((read_1_key == 't')&&(read_2_key == 'e')&&(read_3_key == 'q')) 
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = teq;}	
	else if((read_1_key == 'c')&&(read_2_key == 'm')&&(read_3_key == 'p')) 
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = cmp;}	
	else if((read_1_key == 'o')&&(read_2_key == 'r')&&(read_3_key == 'r'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = orr;}	
	else if((read_1_key == 'm')&&(read_2_key == 'o')&&(read_3_key == 'v'))
		{Instr_Table[line][1] = instrutype_data_processing;Instr_Table[line][2] = mov;}//instrutype_data_processing

	else if((read_1_key == 'm')&&(read_2_key == 'u')&&(read_3_key == 'l'))
		{Instr_Table[line][1] = instrutype_multiply;Instr_Table[line][2] = instru_mul;}	
	else if((read_1_key == 'm')&&(read_2_key == 'l')&&(read_3_key == 'a'))
		{Instr_Table[line][1] = instrutype_multiply;Instr_Table[line][2] = instru_mla;}//instrutype_multiply

	else if((read_1_key == 'l')&&(read_2_key == 'd')&&(read_3_key == 'r'))
		{Instr_Table[line][1] = instrutype_single_data_transfer;Instr_Table[line][2] = instru_ldr;}//!!!  = mov  #	
	else if((read_1_key == 's')&&(read_2_key == 't')&&(read_3_key == 'r'))
		{Instr_Table[line][1] = instrutype_single_data_transfer;Instr_Table[line][2] = instru_str;}//instrutype_single_data_transfer

 	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_beq;return j;}
	else if((read_1_key == 'b')&&(read_2_key == 'n')&&(read_3_key == 'e'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bne;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'g')&&(read_3_key == 'e'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bge;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'l')&&(read_3_key == 't'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_blt;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'g')&&(read_3_key == 't'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bgt;return j;}
	else if((read_1_key == 'b')&&(read_2_key == 'l')&&(read_3_key == 'e')) 
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_ble;return j;}//instrutype_branch	

	
	else if((read_1_key == 'l')&&(read_2_key == 's')&&(read_3_key == 'l')) 
		{Instr_Table[line][1] = instrutype_special;Instr_Table[line][2] = instru_lsl;}	
	
		
	else 
		printf("unknown instruction_cMd\n");
	
	return j;
}

void analysis_b_label(int line)//b foo:
{
	int j=0;
	Instr_Table[line][4] = read_3_key;Instr_Table[line][5] = read_4_key;
	read_w_key = getc(fp);
//	Instr_Table[line][6] = read_w_key;
	while(read_w_key != 0x0a)
	{
		Instr_Table[line][6+j] = read_w_key;
		read_w_key = getc(fp);
		j++;
	}
	Instr_Table[line][1] = instrutype_branch;Instr_Table[line][2] = instru_b;	
	return ;
}

void analysis_bxx_label(int line)//b foo:
{
	int j=0;
	read_w_key = getc(fp);
	while(read_w_key != 0x0a)
	{
		Instr_Table[line][4+j] = read_w_key;
		read_w_key = getc(fp);
		j++;
	}
	Instr_Table[line][1] = instrutype_branch;
	return ;
}

void analysis_last_label(int line)
{
	int j=0;
	read_w_key = getc(fp);
	Instr_Table[line][8] = read_w_key;
	while(read_w_key != ':')
	{
		read_w_key = getc(fp);
		Instr_Table[line][9+j] = read_w_key;
	}
	return ;
}


void analysis_No2_paragraph(int line)
{
	read_w_key = getc(fp);
	if (read_w_key == ' ') {
		read_w_key = getc(fp);//xq
	}
	if(read_w_key != 'r')
		printf("unknown No2_paragraph  r!\n");
	
	read_w_key = getc(fp);
	Instr_Table[line][3] = read_w_key-0x30;//char --> int

	read_w_key = getc(fp);
	if(read_w_key != ',')
		printf("unknown No2_paragraph   line: %d  ,!\n", line);

	return ;	
}

void analysis_No3_paragraph(int line)//# r [ = ' '   will do while((read_1_key = getc(fp)) != 0x0a)
{
int j=0;
	read_w_key = getc(fp);
	TotalRegNum_inNo3_paragraph_butnot_in_bracket = 0;
	Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
	while(read_w_key == ' ')// get off ' '
		read_w_key = getc(fp);
	
	if(read_w_key == '#')//read out #
	{
		analysis_No3_pound(line);//#2  #-2   
		
	}
	else if(read_w_key == '=')// only ldr
	{
		read_w_key = getc(fp);//read out  0
		read_w_key = getc(fp);//read out  x
		read_w_key = getc(fp);//read out  first 16hexnumbe
		j = analysis_No3_16hexnumber(line);
		Instr_Table[line][4] = Num_EQ;
		if((Instr_Table[line][5]<=0xff)&&(Instr_Table[line][1]==instrutype_single_data_transfer)&&(Instr_Table[line][2]==instru_ldr))
		{
			Instr_Table[line][1] = instrutype_data_processing;
			Instr_Table[line][2] = mov;// spec P15
		}
	}
	else if(read_w_key == '[')
	{
		read_w_key = getc(fp);// read out r
		
		read_w_key = getc(fp);
		Instr_Table[line][6] = read_w_key-0x30;//read out  n char --> int   
		Instr_Table[line][4] = Num_bracket_r; //[r
restart_in_bracket:
		read_w_key = getc(fp);
		if(read_w_key == ']')
		{
			read_w_key = getc(fp);
			if(read_w_key == 0x0a) // end or ','
				return;
			else if((read_w_key = getc(fp)) == 'r')
			{
				
				Instr_Table[line][4] = Num_bracket_r; //[ r  r
				read_w_key = getc(fp);//read out n//xq delete
				Instr_Table[line][7] = read_w_key-0x30;//read out  n char --> int 
				TotalRegNum_inNo3_paragraph_butnot_in_bracket++;
				Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
				read_w_key = getc(fp);//read out 0x0a
			}
			else if(read_w_key == '#')
			{
				analysis_No3_pound(line);
				Instr_Table[line][4] = Num_bracket_x;
			}
		}
		else if(read_w_key == ' ')
			goto restart_in_bracket; //get off ' '
		else if(read_w_key == ',')
			goto restart_in_bracket; //get off ','
		else if(read_w_key == '#')
		{
			analysis_No3_pound(line);
			Instr_Table[line][4] +=5;// [#2  or [#-2
		}//Num_bracket_pound = 5,No3_paragraph fist char is  [# 
		else if(read_w_key == 'r')
		{
			Instr_Table[line][4] = Num_bracket_r; //[r
			read_w_key = getc(fp);
			Instr_Table[line][7] = read_w_key-0x30;//read out  n char --> int 
			goto restart_in_bracket;
		}
		else if(read_w_key == 'l')//lsr r2
		{
			read_w_key = getc(fp);//s
			read_w_key = getc(fp);//r 
			read_w_key = getc(fp);//' '
			read_w_key = getc(fp);//r or '#'
			Instr_Table[line][9] = Has_bracket_lsr;
			if(read_w_key == '#')
			{
				read_w_key = getc(fp);
				Instr_Table[line][5] = read_w_key-0x30;//read out  n char --> int 
				Instr_Table[line][4] = Num_bracket_lsr_pound;// [ lsr #
			}
			else 
			{
				read_w_key = getc(fp);
				Instr_Table[line][8] = read_w_key-0x30;//read out  n char --> int 
				Instr_Table[line][4] = Num_bracket_lsr_r;//[ lsr r
			}
			goto restart_in_bracket;
		}		
		else 
			printf("unknown No3_paragraph_Rs\n");
	}
	else if(read_w_key == 'r')
	{
		TotalRegNum_inNo3_paragraph_butnot_in_bracket++;Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
		read_w_key = getc(fp);
		Instr_Table[line][6] = read_w_key-0x30;//read out  n char --> int   
		Instr_Table[line][4] = Num_r;	// r
restart:
		read_w_key = getc(fp);
		if(read_w_key == 0x0a)// end
			return;
		else if(read_w_key == ' ')
			goto restart; //get off ' '
		else if(read_w_key == ',')
			goto restart; //get off ','
		else if(read_w_key == '#')
			analysis_No3_pound(line);
		else if(read_w_key == 'r')
		{
			TotalRegNum_inNo3_paragraph_butnot_in_bracket++;Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
			read_w_key = getc(fp);
			Instr_Table[line][7+j] = read_w_key-0x30;//read out  n char --> int 
			j++;
			goto restart;
		}
		else if(read_w_key == 'l')//lsr r2  
		{
			read_w_key = getc(fp);//s
			read_w_key = getc(fp);//r 
			read_w_key = getc(fp);//' '
			read_w_key = getc(fp);//r or '#'
			Instr_Table[line][9] = Has_lsr;
			if(read_w_key == 'r')
			{
				TotalRegNum_inNo3_paragraph_butnot_in_bracket++;
				Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
				read_w_key = getc(fp);
				Instr_Table[line][8] = read_w_key-0x30;//read out  n char --> int 
				Instr_Table[line][4] = Num_r_lsr_r;// r lsr r
			}
			else
			{
				read_w_key = getc(fp);
				Instr_Table[line][5] = read_w_key-0x30;//read out  n char --> int 				
				Instr_Table[line][4] = Num_r_lsr_pound;// r lsr #
			}
			goto restart;
		}		
		else 
			printf("unknown No3_paragraph_Rs\n");
	}
	else 
		printf("unknown No3_paragraph\n");	
}


void analysis_No3_pound(int line)//read out #       (#1 #0x3F0000 #-0x4 #0x0F)
{
	int j=0;

	Instr_Table[line][4] = Num_pound;
	read_w_key = getc(fp);//read out  first 10hexnumbe
	if(read_w_key == '0')//16hexnumbe
	{
		j = analysis_No3_10hexnumber(line);
		if(j == 0xaa)	//'x'
		{
			read_w_key = getc(fp);//read out  first 16hexnumbe
			j = analysis_No3_16hexnumber(line);
		}
	}
	else if(read_w_key == '-')//#-0x4 
	{
		Instr_Table[line][4] = Num_pound_negative;
		read_1_key = getc(fp);//read out 0
		read_2_key = getc(fp);//read out  x
		read_w_key = getc(fp);//read out  first 16hexnumbe
		j = analysis_No3_16hexnumber(line);	
	}
	else if((read_w_key>='0')&&(read_w_key<='9'))//#1  //#57954613//read out first 10hexnumber
	{
		j = analysis_No3_10hexnumber(line);

	}
	else 
		printf("unknown No3_pound\n");
}

int analysis_No3_10hexnumber(int line)//#1
{
	int j=0;
	int Tmp=0;
	
	Tmp = read_w_key - 0x30;
	read_w_key = getc(fp); //read out next 10hexnumber
	if(read_w_key == 'x')
		return 0xaa;	
	while((read_w_key != 0x0a))
	{
		if((read_w_key>='0')&&(read_w_key<='9'))
		{
			Tmp =Tmp * 10;
			Tmp += read_w_key - 0x30;
		}
		else if(read_w_key == 0x0a)
			 break;
		else if(read_w_key == ']')
			 ;			 
		else if((read_w_key != ',')&&(read_w_key != ' '))
			{printf("unknown No3_10hexnumber !last char!\n");return j;}		
		else // == ','or' '
		{
			j++;
			break;
		}
		read_w_key = getc(fp); //read out next 10hexnumber
	}
	Instr_Table[line][5] = Tmp;
	return j;	
}

int analysis_No3_16hexnumber(int line)//#0x
{
	int j=0;
	int Tmp=0;

	if((read_w_key>='0')&&(read_w_key<='9'))
	{
		Tmp = read_w_key - 0x30;
	}
	else if((read_w_key>='A')&&(read_w_key<='F'))// 'A'---'F'
	{
		Tmp = read_w_key - 0x37;
	}
	else if((read_w_key>='a')&&(read_w_key<='f'))// 'a'--'f'
	{
		Tmp = read_w_key - 0x57;
	}				
	else 
		{printf("unknown No3_11hexnumber \n");return j;}	
	
	read_w_key = getc(fp); //read out next 16hexnumber
	while((read_w_key != 0x0a))
	{
		if((read_w_key>='0')&&(read_w_key<='9'))
		{
			Tmp =Tmp * 16;
			Tmp += read_w_key - 0x30;
		}
		else if((read_w_key>='A')&&(read_w_key<='F'))// 'A'---'F'
		{
			Tmp =Tmp * 16;
			Tmp += read_w_key - 0x37;
		}
		else if((read_w_key>='a')&&(read_w_key<='f'))// 'a'--'f'
		{
			Tmp =Tmp * 16;
			Tmp += read_w_key - 0x57;
		}			
		else if(read_w_key == 0x0a)
			 break;
		else if(read_w_key ==  ']')
			 ;			 
		else if((read_w_key != ',')&&(read_w_key != ' '))
			{printf("unknown No3_11hexnumber \n");return j;}		
		else // == ','or' '
		{
			j++;
			break;
		}
		read_w_key = getc(fp); //read out next 10hexnumber
	}
	Instr_Table[line][5] = Tmp;
	return j;
}





































