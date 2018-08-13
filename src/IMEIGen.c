//Coded by sRBill1996
//Sites:netixzen.blogspot.com.ar
//      github.com/srbill1996 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FILE_DB_imeis "db_imeis.txt"
#define LONG_IMEI_NUM 15
#define LONG_IMEI_BASE 8

int   checkLuhn(char imeiNum[]);       	    //Luhn comprobation
void  randomizeImeiBase(char imeiBase[]);				//Fill imei X
void  genIMEI(char imeiNum[], int cantidad);	//Gen random imei
char *locationOfimei(char imeiNum[], int buffersize); //Search in db imeis
int   getInputStr(char imeiNum[], char *nameInput, int limit);
void  printLogo();
void  locatePersonalimei();
void  lowerArray(char array[]);
void  randomimeiFromDb();

char lastimei[100] = "000000";
int main(int argc, char *argv[])
{	
	srand(time(NULL));
	char imeiNum[15];
	long int amountGen = 10;
	while(1){
		system("cls");
		printLogo();
		printf("Insert opcion, input 'b' for back to the menu.\n\n");
		printf("1)Generate IMEI from imei base. (Firts 8 numbers or more)\n");
		printf("2)Check IMEI Luhn validation.\n");
		printf("3)Set amount of IMEIs to generate.\n");
		printf("4)Search IMEI in database.\n");
		printf("5)Search a IMEI by keywords.\n");
		printf("6)Drop Random IMEI BASE\n");
		printf("7)Generate IMEI from last in memory. (Actual:%s)\n",lastimei);
		printf("8)Exit");
		printf("\n\nOption>:");
		int opcion;
		switch((opcion = getchar())){
			case '1':
				while(getInputStr(imeiNum, "imei:", 1) != -1)
					genIMEI(imeiNum, amountGen);
				break;
			case '2':
				while(getInputStr(imeiNum, "IMEI:", LONG_IMEI_NUM) != -1){
					if(checkLuhn(imeiNum) == 1)
						printf("Is a valid IMEI.\n");
					else
						printf("Is a invalid IMEI.\n");
				}
				break;
			case '3'://imei SET AMOUNT
				printf("Amount:");
				scanf("%ld", &amountGen);
				printf("Done.\n");
				break;
			case '4':
				while(getInputStr(imeiNum, "imei:", LONG_IMEI_BASE) != -1){
					printf("Result:%s", locationOfimei(imeiNum, 100));
					puts("");
				}
				break;
			case '5'://SEARCH imei BY KEYWORDS
				while(getInputStr(imeiNum, "Keywords:",2) != -1)
					locatePersonalimei(imeiNum);
				break;
			case '6'://CHOICE RANDOM imei FROM DB
				printf("'c' to cancel.\n");
				while(getchar()!='c'){
					randomimeiFromDb();
				}
				break;
			case '7'://GENERATE imei FROM LAST IN MEMORY
				printf("HEY! 'c' to cancel and back to the main menu.\n\n");
				while(getchar()!='b'){
					genIMEI(lastimei, amountGen);
				}
				break;
			case '8'://EXIT
				exit(1);
			default:
				printf("Invalid option(?\n ");
				break;
		}
	}
	return 0;
}

void printLogo(){


puts("  ___ __  __ ___ ___               ");
puts(" |_ _|  \\/  | __|_ _|__ _ ___ _ _  ");
puts("  | || |\\/| | _| | |/ _` / -_) ' \\ ");
puts(" |___|_|  |_|___|___\\__, \\___|_||_|");
puts("                    |___/          ");
puts("-By:sRBill1996 https://github.com/srbill1996 ----\n");
}

void genIMEI(char imeiNum[], int amountToGen){
	
	printf("Generating..\n");
	if(strlen(imeiNum) > LONG_IMEI_NUM){
		printf("Max of %d digits exceded.\n", LONG_IMEI_NUM);
		return;
	}
	char tmpimei[19];
	int count = 0, failRounds = 0;
	while(amountToGen > count){
		strncpy(tmpimei, imeiNum, LONG_IMEI_BASE);
		tmpimei[LONG_IMEI_BASE+1] = '\0';
		randomizeImeiBase(tmpimei);
		if(checkLuhn(tmpimei) == 1){
			printf("%s\n", 	tmpimei);
			count++;
			strncpy(lastimei, tmpimei, LONG_IMEI_BASE);
			failRounds = 0;
		}else{
			failRounds++;
			if(failRounds > 10000){
				printf("Max rounds limit exceded, please insert other IMEI..\n");
				break;
			}
		}
	}
	printf("\n>IMEI DEVICE MODEL:%s\n\n", locationOfimei(imeiNum, 100));
}


void randomizeImeiBase(char imeiBase[]){
	int cnt;
	//generate 
	if(strlen(imeiBase) < LONG_IMEI_NUM){
		for(cnt=strlen(imeiBase); cnt < LONG_IMEI_NUM; cnt++){
			imeiBase[cnt] = 'x';
		}
		imeiBase[LONG_IMEI_NUM] = '\0';
		for(cnt=0; cnt<LONG_IMEI_NUM; cnt++){
			if(!isdigit(imeiBase[cnt])){
				imeiBase[cnt] = (rand() % 9) + '0';
			}
		}
	}else{
		for(cnt=0; cnt<strlen(imeiBase); cnt++){
			if(tolower(imeiBase[cnt]) == 'x'){
				imeiBase[cnt] = (rand() % 9) + '0';
			}
		}
	}
}

void randomimeiFromDb(){
	FILE *imeis;
	char buffer[100];
	int choicer = rand() % 604899;
	int choice = 0;
	if(!(imeis = fopen(FILE_DB_imeis, "r"))){
		printf("%s was not found :(.\n", FILE_DB_imeis);
	}else{
		while(choice++ != choicer){
			fgets(buffer, 100, imeis);
		}
		printf("%s",buffer);
		strncpy(lastimei, buffer, LONG_IMEI_BASE+1);
		return;
	}
}

void locatePersonalimei(char keywords[]){
	FILE *imeis;
	char buffer[100];
	char tmpkeywords[100];
	char *word;
	if(!(imeis = fopen(FILE_DB_imeis, "r"))){
		printf("%s was not found :(.\n", FILE_DB_imeis);
	}else{
		while(!feof(imeis)){
			fgets(buffer, 100, imeis);
			lowerArray(buffer);
			strcpy(tmpkeywords, keywords);
			lowerArray(tmpkeywords);
			word = strtok(tmpkeywords,",");
			do{
				if(!strstr(buffer, word))
					break;
			}while((word = strtok(NULL, ",")));
			if(word==NULL)
				printf("%s\n", buffer);
		}
		strncpy(lastimei, buffer, LONG_IMEI_BASE);
	}
}

int getInputStr(char imeiNum[], char *nameInput, int limit){
	imeiNum[0] = '\0';
	while(strlen(imeiNum) < limit){
		printf("%s", nameInput);
		scanf("%s", imeiNum);
		if(strlen(imeiNum) < limit)
			printf("Error in long limit of digits.\n");
		if(imeiNum[0] == 'b')//back to the menu
			return -1;
	}
	return 1;
}

void lowerArray(char array[]){
	int i;for(i=0;i < strlen(array);i++)
		array[i] = tolower(array[i]);
}

char *locationOfimei(char imei[], int buffersize){
	FILE *imeis;
	if(buffersize > 100)
		printf("Searching imei..\n");
	char buffer[buffersize];
	if(!(imeis = fopen(FILE_DB_imeis, "r")) ){
		printf("%s was not found :(.\n", FILE_DB_imeis);
		return;
	}else{
		while(!feof(imeis)){
			fgets(buffer, buffersize, imeis);
			if(!strncmp(buffer, imei, LONG_IMEI_BASE)){
				return strdup(buffer);
			}
		}
	}
	return "not found";
}

int checkLuhn(char *number)
{
    int i, sum, ch, num, twoup, len;
    len = strlen(number);
    sum = 0;
    twoup = 0;
    for (i = len - 1; i >= 0; --i) {
        ch = number[i];
        num = (ch >= '0' && ch <= '9') ? ch - '0' : 0;
        if (twoup) {
            num += num;
            if (num > 9) num = (num % 10) + 1;
        }
        sum += num;
        twoup = ++twoup & 1;
    }
    sum = 10 - (sum % 10);
    if (sum == 10) sum = 0;
    return (sum == 0) ? 1 : 0;
}

