#define int32 unsigned int
int main(int argc, char *argv[]);

//check instruction type and its conditions
void check_instr_set();
int check_cond(int cond);
void report_err_instruction();

void readInstruction();

//data processing and its helper functions 
void data_processing();

int get_operand2();
int32 shift(int shift_type, int shift_amount, int value);
void set_C(int result);

//multiply instruction
void multiply();

//single_data_transferand and its helper funcitons
void single_data_transfer();

void single_data_load();
void single_data_store();
int32 get_offset();
void add_sub_offset();

//branch 
void branch();

//registers
int32 read_register(int r);
void write_register(int r, int content);
void change_1_bit_content(int reg, int position, int content);

//other helper functinons
int32 get_4bit_memory(int start_point);
int32 build_mask(int32 instr, int start_point, int length);



