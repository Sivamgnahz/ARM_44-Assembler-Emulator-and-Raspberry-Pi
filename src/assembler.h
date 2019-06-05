int main();
int Read_from_file(char argv[]);
int analysis_First_paragraph(int line);
void analysis_last_label(int line);
void analysis_b_label(int line);//b foo:
void analysis_bxx_label(int line);
void analysis_No2_paragraph(int line);
void analysis_No3_paragraph(int line);
void analysis_No3_pound(int line);//#
int analysis_No3_10hexnumber(int line);//#1
int analysis_No3_16hexnumber(int line);//#0x

void data_processing(int line);
void branch(int line);
void single_data_transfer(int line);

enum instruction_type{
instrutype_data_processing = 1,					//Instr_Table[line][1]
instrutype_multiply = 2,
instrutype_single_data_transfer  = 3,
instrutype_branch= 4,
instrutype_special = 5,
instrutype_label= 6,
};	

enum instruction_cMd {
and = 0,	// spec P14  						Instr_Table[line][2]
eor = 1,
sub = 2,
rsb = 3,
add = 4,
tst = 8,
teq = 9,
cmp = 10,
orr = 12,
mov = 13,	//type 1 
 
instru_mul = 0x20,	
instru_mla = 0x21,	//type 2
	
instru_ldr = 0x30,
instru_str  = 0x31,	//type 3
	
instru_beq = 0,	// spec P16
instru_bne = 1,	
instru_bge = 10,
instru_blt = 11,
instru_bgt = 12,
instru_ble = 13,
instru_b  = 14,	//type 4

instru_ANDEQ  = 0x50,//type 5
instru_lsl =0x51,
};

enum typ_Number {
Num_pound = 0,						//No3_paragraph fist char is # 	Instr_Table[line][4]
Num_pound_negative = 1,					//No3_paragraph fist char is  #-
Num_EQ = 2,							//No3_paragraph fist char is  =

Num_r = 3,							//No3_paragraph fist char is  R

Num_bracket_r = 4,					//No3_paragraph fist char is [Rn],
Num_bracket_pound = 5,				//No3_paragraph fist char is  [Rn,<#expression>], 
Num_bracket_pound_negative = 6,		//No3_paragraph fist char is  [Rn,<#expression>], 
Num_bracket_lsr_pound = 7,			//No3_paragraph fist char is  [ lsr #
Num_bracket_lsr_r = 8,			//No3_paragraph fist char is  [ lsr r
Num_bracket_end_pound = 9,				//No3_paragraph fist char is  [Rn],#expression,
Num_bracket_end_pound_negative = 10,		//No3_paragraph fist char is  [Rn],#-expression

Num_r_lsr_r = 0x10,							// r lsr r
Num_r_lsr_pound = 0x11,							// r lsr #

Has_lsr = 1,
Has_bracket_lsr = 2,		//No3_paragraph Has lsr  		Instr_Table[line][9]
};

enum instr_Cond{
beq = 0,	// spec P4
bne = 1,	
bge = 10,
blt = 11,
bgt = 12,
ble = 13,
always  = 14,	//type 4
};

