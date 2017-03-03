/* Technology Operations Toner Inventory System (TOTIS)
 * CPS 210
 * Qi Liao
 * 2/2/2017 //change later//
 * Final Project
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

//DEFINE CONSTANTS BELOW:
const int HI_THRESHOLD_QUANTITY = 2;	//toners like 80x and 81x that we use too often to risk only having 1 left before ordering (High Importance)
const int LI_THRESHOLD_QUANTITY = 1;	//toners that are used less often than 80x and 81x but still need to have in stock (Low Importance)
int TONER_NUM = 32; //equal to the line numbers in toners.txt, and therefore the number of toners we have. Not a true constant, but rather a "pseudo-constant" that is treated as constant unless otherwise necessary (adding more toner that wasn't previously on the list//struct String strings[];



//define the struct for the general toner
struct Toner{
	char name[20];
	char barcode[30];
	int quantity;
	int threshold;
};

struct String{
	char value[30];
};

struct Toner tonerlist[];

void deployToner(int index);
void addToner(struct Toner toner);


void mainMenu(){
	/*
	 * This will be a main menu of sorts
	 * so that TOTIS can come back here
	 * as opposed to simply terminating after
	 * a quantity change occurs
	 *
	 * Since not much will be visually displayed
	 * there is no need to actually printf
	 * eventually I will add LCD I2C 16x2 support
	 * but for now nothing to be displayed
	 */

	//1.) toner deployment
	//2.) toner delivery

	int choice;

	printf("TOTIS Main Menu\n");
	printf("1. Toner Deployment\n");
	printf("2. Toner Delivery\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);

	while(choice != 1 && choice != 2){
		printf("Error, invalid choice. Try again.\n");
		mainMenu();
	}

	if(choice == 1){
		//toner deployment
		printf("Enter the barcode: ");
		char tempBarcode[30];
		scanf("%s", tempBarcode);
		int i = 0;
		while(strcmp(tempBarcode, tonerlist[i].barcode) != 0){
			i += 1;
		}
		//deploy toner here
		deployToner(i);
	} else if(choice == 2){
		//toner delivery
	}
}


//begin basic inventory methods below
//remove toner will be called after a button press is detected by GPIO button
void deployToner(int index){
	if(tonerlist[index].quantity > 0){
		tonerlist[index].quantity -= 1;
		printf("%d\n", tonerlist[index].quantity);
	} else {
		printf("error, quantity already 0."); //change this to LED output later, also LCD output of an error code (yet to be defined)
		mainMenu();
	}
	if(tonerlist[index].quantity <= tonerlist[index].threshold){
		//log here
		//notify here
		mainMenu();
	} else {
		//just log here
		//logToFile(f, getTime());
		mainMenu();
	}
}

//add toner will be called after a button press is detected by GPIO button
void addToner(int index){
	if(tonerlist[index].barcode[strlen(tonerlist[index].barcode)] == 'D'){
		//this means it is a dual pack, so we should add 2 to the toner quantity
		tonerlist[index].quantity += 2;
		//log here in the future

	} else {
		//this means only a single pack, therefore we should only add 1 to toner quantity
		//log here in the future
		tonerlist[index].quantity += 1;

	}
}

void logToFile(char * str, FILE *file){
	fprintf(file, "%s\n", str);
	//this prints a new line to the log file
	//the str will be the message and can be determined in each
	//necessary instance
}

const char * getTime(){
	time_t rawtime;
	struct tm *info;

	time(&rawtime);

	info = localtime(&rawtime);

	char *temp = asctime(info);
	return temp;
	//This method returns the current (LOCAL) date and time in this format:
	//Day Month Date Hour:Minute:Second Year
	//For example:
	//Mon Feb 20 10:55:40 2017
	//This is dependent on the time set on the Pi of course
}

void sendEmail(){
	//not working as of yet (obviously)
}


//begin main method below
int main(void){
	/*
	 * TOTIS will be ran with a 16x2 I2C LCD display connected to GPIO pins on the Pi
	 * sit will be held in our server room (which is where we store toner as well)
	 * and will mainly be the buttons, LEDs, the barcode scanner, along with the display
	 * LEDs will be
	 * to do this I will take advantage of different colors, flashing, and solid lights
	 */
	FILE *toners;
	toners = fopen("toners.txt", "r+");

	struct String barcodes[TONER_NUM];
	struct String names[TONER_NUM];
	int quantities[TONER_NUM];



	//rewind(toners);
	for(int i = 0; i < TONER_NUM; i++){
		struct Toner tempToner;
		fscanf(toners, "%s %d %s", &tempToner.barcode, &tempToner.quantity, &tempToner.name);
		tonerlist[i] = tempToner;
	}

	//tonerlist now contains the information from toners.txt
	//so we can close toners file below
	fclose(toners);


	//creating the log file and opening it in append mode
	FILE *logfile;
	logfile = fopen("logfile.txt", "a"); //opened in append mode, "a+", since typically will just append information to this file

	mainMenu();

	//closing log file
	fclose(logfile);
	return 0;
}
