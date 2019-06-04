#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler_types.h"
#include "assembler.h"
#define Total_Label_Line_Num_buffr  30
FILE *fpw;
FILE *fp;

instruction instr;

int Instr_Table[40][11];
char read_1_key,read_2_key,read_3_key,read_4_key,read_w_key;
int	TotalRegNum_inNo3_paragraph_butnot_in_bracket=0;	

int main(int argc, char *argv[]) 
{
	char *p;
	char p1[20] = "";
	char p2[20] = "";
	char p3[20] = "";
	char p4[20] = "";
	int onntr_num =0;
	int j; int i;	
	int Line_Num=0;
	
	//fp = fopen(argv[1],"r");
	fpw = fopen(argv[2], "wb");
	for(i=0;i<Total_Label_Line_Num_buffr;i++)
		for(j=0;j<11;j++)
			Instr_Table[i][j] = 0xffffffff;
		
	Line_Num = Read_from_file(argv[1]);

	
for(j=0;j<Line_Num;j++)
{
	switch(Instr_Table[j][1])
	{
		case instrutype_data_processing:
			printf(" instrutype_data_processing...\n");
			break;
		case instrutype_multiply:	 
			printf(" instrutype_multiply.....   \n");	
			break;
		case instrutype_single_data_transfer:	 
			printf(" instrutype_single_data_transfer....\n");	
			break;
		case instrutype_branch:	 
			printf(" instrutype_branch.....\n");	
			break;
		case instrutype_special:	 
			printf(" instrutype_special.........\n");	
			break;
		case instrutype_label:	 
			printf(" instrutype_label..........\n");	
			break;
		default: 
			printf(" unknown instrutype   !\n");
			break;
	}
}
	fclose(fp);
	fclose(fpw);
	return 0;
}


int Read_from_file(char argv[])
{
int j; int i;	
int	Total_Label_Num=0;
int Line_Num_File=0;

	if((fp = fopen(argv,"r")) == NULL)
	{
		printf(" cannot open file \n");
		return 1;
	}
	i=0;j=0;
	while((read_1_key = getc(fp)) != EOF)
	{
		read_2_key =getc(fp);
		if((read_2_key == EOF)||(read_2_key == 0x0a))
			break;
		read_3_key =getc(fp);
		if((read_3_key == EOF)||(read_3_key == 0x0a))
			break;
		read_4_key =getc(fp);
		if((read_4_key == EOF)||(read_4_key == 0x0a))	
			break;
		
		
		Instr_Table[i][0] = i;
		if(read_2_key == ' ')//b foo
		{
			analysis_b_label(i);//if branch has Read 0x0a!  
			goto exit_willdonotwhile;
		}
		else if(read_4_key == ' ') //xxx' ' 
		{
			j = analysis_First_paragraph(i);// only here will continue......
			if(j)			//if branch has Read 0x0a!  
				goto exit_willdonotwhile; 	
		}

		else if(read_4_key == 'e')//instrutype_special instru_ANDEQ
		{
			if((read_1_key == 'a')&&(read_2_key == 'n')&&(read_3_key == 'd'))
				{Instr_Table[i][1] = instrutype_special;Instr_Table[i][2] = instru_ANDEQ;}
			goto exit_willdonotwhile;
		}
		else// label
		{
			if(read_4_key == ':')
				{Instr_Table[i][4] = read_1_key;Instr_Table[i][5] = read_2_key;Instr_Table[i][6] = read_3_key;Instr_Table[i][7] = read_4_key;}		
			else 
				analysis_last_label(i);
			Total_Label_Num++;
			Instr_Table[i][1] = instrutype_label;Instr_Table[i][2] = Total_Label_Num;Instr_Table[i][2] = i+1;
			Instr_Table[i][4] = read_1_key;Instr_Table[i][5] = read_2_key;Instr_Table[i][6] = read_3_key;Instr_Table[i][7] = read_4_key;			
			goto exit_willdowhile;
		}
		/////////////////////
		analysis_No2_paragraph(i);//r1,
		/////////////////////
		analysis_No3_paragraph(i);//# r [ = -
		goto exit_willdonotwhile;
exit_willdowhile:
		read_1_key = getc(fp);
		while(read_1_key != 0x0a);//enter »»ĞĞ·ûºÅ
exit_willdonotwhile:		
		Line_Num_File++;
		Total_Label_Num = 0;
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
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bne;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bge;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_blt;return j;}	
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q'))
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_bgt;return j;}
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q')) 
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_ble;return j;}//instrutype_branch	
	else if((read_1_key == 'b')&&(read_2_key == 'e')&&(read_3_key == 'q')) 
		{j=1;analysis_bxx_label(line);Instr_Table[line][2] = instru_b;return j;}	
		
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
	if(read_w_key != 'r')
		printf("unknown No2_paragraph  r!\n");
	
	read_w_key = getc(fp);
	Instr_Table[line][3] = read_w_key-0x30;//char --> int

	read_w_key = getc(fp);
	if(read_w_key != ',')
		printf("unknown No2_paragraph   ,!\n");

	return ;	
}

void analysis_No3_paragraph(int line)//# r [ = ' '   will do while((read_1_key = getc(fp)) != 0x0a)
{
int j;
	read_w_key = getc(fp);
	TotalRegNum_inNo3_paragraph_butnot_in_bracket = 0;
	while(read_w_key == ' ')// get off ' '
		read_w_key = getc(fp);
	
	if(read_w_key == '#')//read out #
	{
		analysis_No3_pound(line);
		
	}
	else if(read_w_key == '=')// only ldr
	{
		read_w_key = getc(fp);//read out  0
		read_w_key = getc(fp);//read out  x
		read_w_key = getc(fp);//read out  first 16hexnumbe
		j = analysis_No3_16hexnumber(line);
		Instr_Table[line][4] = Num_EQ;
		if((Instr_Table[line][5]<=0x100)&&(Instr_Table[line][1]==instrutype_single_data_transfer)&&(Instr_Table[line][2]==instru_ldr))
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
		
restart_in_bracket:
		read_w_key = getc(fp);
		if(read_w_key == ']')
		{
			read_w_key = getc(fp);
			if(read_w_key == 0x0a)// end or ','
				return;
			else 
			{
				read_w_key = getc(fp);//read out r
				read_w_key = getc(fp);//read out n
				Instr_Table[line][7] = read_w_key-0x30;//read out  n char --> int 
				TotalRegNum_inNo3_paragraph_butnot_in_bracket++;
				Instr_Table[line][10] = TotalRegNum_inNo3_paragraph_butnot_in_bracket;
				read_w_key = getc(fp);//read out 0x0a
			}
		}
		else if(read_w_key == ' ')
			goto restart_in_bracket; //get off ' '
		else if(read_w_key == ',')
			goto restart_in_bracket; //get off ','
		else if(read_w_key == '#')
			analysis_No3_pound(line);
		else if(read_w_key == 'r')
		{
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
			Instr_Table[line][9] = Has_lsr;
			if(read_w_key == '#')
			{
				read_w_key = getc(fp);
				Instr_Table[line][5] = read_w_key-0x30;//read out  n char --> int 
				Instr_Table[line][4] = Num_pound;
			}
			else 
			{
				read_w_key = getc(fp);
				Instr_Table[line][8] = read_w_key-0x30;//read out  n char --> int 
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
			}
			else
			{
				read_w_key = getc(fp);
				Instr_Table[line][5] = read_w_key-0x30;//read out  n char --> int 				
				Instr_Table[line][4] = Num_pound;
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
		read_1_key = getc(fp);//read out  x
		read_w_key = getc(fp);//read out  first 16hexnumbe
		j = analysis_No3_16hexnumber(line);

	}
	else if(read_w_key == '-')//#-0x4 
	{
		Instr_Table[line][4] = Num_negative;
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





































