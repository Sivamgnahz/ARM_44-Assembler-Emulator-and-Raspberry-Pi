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





enum instruction_type{
instrutype_data_processing = 1,
instrutype_multiply = 2,
instrutype_single_data_transfer  = 3,
instrutype_branch= 4,
instrutype_special = 5,
instrutype_label= 6,
};	

enum instruction_cMd {
and = 0,// spec P14
eor = 1,
sub = 2,
rsb = 3,
add = 4,
tst = 8,
teq = 9,
cmp = 10,
orr = 12,
mov = 13,// 1 
 
instru_mul = 0x20,	
instru_mla = 0x21,// 2
	
instru_ldr = 0x30,
instru_str  = 0x31, // 3
	
instru_beq = 0,// spec P16
instru_bne = 1,	
instru_bge = 10,
instru_blt = 11,
instru_bgt = 12,
instru_ble = 13,
instru_b  = 14,// 4

instru_ANDEQ  = 0x50// 5
};

enum typ_Number {
  Num_null = 0,// null ,also no R
  Num_pound = 1,//#
  Num_r = 2,// only has R
  Num_L_bracket = 3,// [
  Num_EQ = 4,// =
  Num_negative = 5,// #-

  HasNot_lsr = 0,
  Has_lsr = 1,
};


