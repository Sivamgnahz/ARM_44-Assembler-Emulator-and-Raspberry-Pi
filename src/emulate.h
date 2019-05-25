int main(int argc, char *argv[]);
void check_instr_set(instruction instr);
void data_processing(instruction instr);
int get_operand2(instruction instr);
int shift(int shift_type, int shift_amount, int value);
int get_C(instruction instr, int result);
void multiply(instruction instr);
void single_data_transfer(instruction instr);
void branch(instruction instr);
void report_err_instruction();
int read_register(int r);
void write_register(int r, int content);
int check_cond(int cond);
void change_1_bit_content(int reg, int position, int content);
int build_mask(int instr, int start_point, int length);

