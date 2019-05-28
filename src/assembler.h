int main();

char * data_pro1(char *instruction);
char * data_pro2(char *instruction);
char * data_pro3(char *instruction);

char * regtoBits(char *reg);
char * immtoBits(char *operand2);
char * toBits(unsigned int x);
char * hextoBits(char *hex);

char * multiply();
char * data_transfer();
char * branch();
char * special();