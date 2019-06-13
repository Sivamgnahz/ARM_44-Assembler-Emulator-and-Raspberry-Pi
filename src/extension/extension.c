#include "extension.h"
#define MAX_LENGTH 1000

int main(int argc, char *argv[]){
    char answer;
    char answer2;
    char sentence[MAX_LENGTH];
    char str[] = "SOS";

    // wiringPiSetup();
	// pinMode(LED, OUTPUT);

    SOS_wrong: //whether SOS emergency
    printf("================\n");
    printf("SOS? y/n\n");
    printf("================\n");    
    scanf("%c",&answer);
    if(answer == 'y'){
        strcpy(sentence, str);
        goto SOS;
    } else if(answer != 'n'){
        // printf("Wrong message, please try again\n");
        goto SOS_wrong;
    }

    SKIP_wrong:  //whether skip the introduction
    printf("Skip Introduction? y/n\n");
    scanf("%c",&answer2);
    if(answer2 == 'y'){
        goto Play_again;
    } else if(answer2 != 'n'){
        // printf("Wrong message:%c,please try again\n",answer2);
        goto SKIP_wrong;
    }

    Rules: //introduction rules
    printf("\n=====Welcome!=====\n\n");
    printf("This is a tool that will convert the sentence into the morse code.\n");
    printf("You can enter the sentence(s) you want to convert.\n");
    printf("Once you finished, press the enter button.\n");
    printf("\n================\n\n");
    printf("Please note that you are only allowed to use English letters, numbers, and symbols below: \n");
    printf("' '; ','; '!'; '.'; '?'\n");
    printf("Any symbol or character other than these will not been showed by light.\n"); 
    printf("\n================\n\n");  
    Play_again:
    printf("Please enter the sentence below: \n");

    fgets(sentence, MAX_LENGTH, stdin);
    fgets(sentence, MAX_LENGTH, stdin);

    // gets(sentence);
    // gets(sentence);

    printf("Your sentence is: %s\n", sentence);
 

    SOS:
    read_sentence(sentence); //main function to be run

    CONVERT_wrong: //play again or not
    printf("Want to convert another sentence? y/n \n");
    scanf("%c",&answer);
    if(answer == 'y'){
    } else if(answer == 'n'){
        goto End;
    } else {
        printf("Wrong message, please try again\n");
        goto CONVERT_wrong;
    }

    REVIEW_wrong: //need to read the rule or not
    printf("Want to see the rules again? y/n\n");
    scanf("%c",&answer);
    if(answer == 'y'){
        printf("\n\n\n");
        goto Rules;
    } else if(answer == 'n'){
        printf("\n\n\n");
        goto Play_again;
    } else {
        // printf("Wrong message, please try again\n");
        goto REVIEW_wrong;
    }

    End:
    printf("Thanks for your using.\n");

    return 0;
}

void read_sentence(char* sentence){
    int length = strlen(sentence);
    char previous;
    for(int i=0; i<length; i++){
        char ch = sentence[i];
        int type = filter(ch);
        int morse_code = 0;
        if(type==LOWER_LETTER){
            ch = toupper(ch);
            morse_code = letter_to_morse(ch);
        } else if(type==UPPER_LETTER){
            morse_code = letter_to_morse(ch);            
        } else if(type==NUMBER){
            morse_code = number_to_morse(ch);
        } else if(type==NORMAL_SYMBOL){
            if(filter(previous)!= NORMAL_SYMBOL){
                morse_code = symbol_to_morse(ch);
            }
        } else if(type==OTHERS){
            printf("The character:%c is not in the list. It will be ignored.\n", ch);
        }
        previous = ch;
        morse_to_light(morse_code);
    }
}

int filter(char letter){
    if(isalpha(letter) > 0){
        if(islower(letter) >0){
            return LOWER_LETTER;
        } else {
            return UPPER_LETTER;
        }
    }
    if(isdigit(letter) > 0){
        return NUMBER;
    }
    if(letter == ' '
    || letter == ','
    || letter == '!'
    || letter == '.'
    || letter == '?'){
        return NORMAL_SYMBOL;
    }
return OTHERS;
}

int letter_to_morse(char letter){  // 1-short 2-long 3-pause
    switch(letter){
        case 'A': return 12;
        case 'B': return 2111;
        case 'C': return 2121;
        case 'D': return 211;
        case 'E': return 1;
        case 'F': return 1121;
        case 'G': return 221;
        case 'H': return 1111;
        case 'I': return 11;
        case 'J': return 1222;
        case 'K': return 212;
        case 'L': return 1211;
        case 'M': return 22;
        case 'N': return 21;
        case 'O': return 222;
        case 'P': return 1221;
        case 'Q': return 2212;
        case 'R': return 121;
        case 'S': return 111;
        case 'T': return 2;
        case 'U': return 112;
        case 'V': return 1112;
        case 'W': return 122;
        case 'X': return 2112;
        case 'Y': return 2122;
        case 'Z': return 2211;
        default: 
        printf("error on letter_to_morse.");
        return 0;
    }
    return 0;
}

int number_to_morse(char number){
    switch(number){
        case '0': return 22222;
        case '1': return 12222;
        case '2': return 11222;
        case '3': return 11122;
        case '4': return 11112;
        case '5': return 11111;
        case '6': return 21111;
        case '7': return 22111;
        case '8': return 22211;
        case '9': return 22221;
        default: 
        printf("error on number_to_morse.");
        return 0;
    }
    return 0;
}

int symbol_to_morse(char symbol){

    switch(symbol){
        case '?': return 112211;
        case '.': return 121212;
        default:
        return 3;
    }

    return 0;
}

int invert(int morse){
    int res = 0;
    while (morse>0){
        res = res*10+morse%10;
        morse = (int)(morse/10);
    }
    return res;
}

void morse_to_light(int morse){
    morse = invert(morse);

    while(morse > 0){
        if(morse%10 == 1){
            dit();
        } else if(morse%10 == 2){
            dah();
        } else if(morse%10 == 3){
            pause_medium();
        }
        pause_short();
        morse /= 10;
    }
}

void dit(){
    // printf("dit\n");
	// digitalWrite(LED, HIGH);
	// delay(200);
}
void dah(){
    // printf("dah\n");
	// digitalWrite(LED, HIGH);
	// delay(600);

}
void pause_short() {
    //     printf("short\n");
	
	// digitalWrite(LED, LOW);
	// delay(200);
}

void pause_medium() {
    //         printf("medium\n");
	
	// digitalWrite(LED, LOW);
	// delay(600);
}

