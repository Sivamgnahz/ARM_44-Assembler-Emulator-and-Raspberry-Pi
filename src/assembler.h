#define u_int32_t unsigned int

int main();

u_int32_t data_pro1(char *instruction);
u_int32_t data_pro2(char *instruction);
u_int32_t data_pro3(char *instruction);

u_int32_t regtoBits(char *reg);
u_int32_t immtoBits(char *operand2);
u_int32_t concat(int cond, int i, int opcode, int s, int rn, int rd, int operand2);
//char * toBits(unsigned int x);
//char * hextoBits(char *hex);

char * multiply();
char * data_transfer();
char * branch();
char * special();
