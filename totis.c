/* Technology Operations Toner Inventory System (TOTIS)
 * CPS 210
 * Qi Liao
 * 4/21/17
 * Final Project
 *
 */



#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>

//DEFINE CONSTANTS BELOW:
const int HI_THRESHOLD_QUANTITY = 2;	//toners like 80x and 81x that we use too often to risk only having 1 left before ordering (High Importance)
const int LI_THRESHOLD_QUANTITY = 1;	//toners that are used less often than 80x and 81x but still need to have in stock (Low Importance)
//int TONER_NUM = 32; //equal to the line numbers in toners.txt, and therefore the number of toners we have. Not a true constant, but rather a "pseudo-constant" that is treated as constant unless otherwise necessary (adding more toner that wasn't previously on the list//struct String strings[];
#define TONER_NUM 33 //equal to the line numbers in toners.txt, and therefore the number of toners we have.

//wiringPi constants:
const int DELIVER_BTN_PIN = 5; //gpio 24
const int DEPLOY_BTN_PIN = 6;  //gpio 25
const int BLUE_LED_PIN = 0; //gpio 17

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

struct Toner tonerlist[TONER_NUM]; //create an array of type struct Toner of size TONER_NUM which is the number of toners included for TOTIS
void deployToner(int index);
void addToner(int index);
void mainDisplay();
void logToFile(char* event);
void update();


void mainMenu(){
	/*
	 * This will be a main menu of sorts
	 * so that TOTIS can come back here
	 * as opposed to simply terminating after
	 * a quantity change occurs
	 */

	if(digitalRead(DEPLOY_BTN_PIN) != 1){ //if the deploy button was pressed
		//toner deployment
		digitalWrite(BLUE_LED_PIN, HIGH);
		//printf("Enter the barcode: ");
		char command[] = "python /home/pi/Desktop/Desktop/enterbarcode.py";
		system(command);
		char tempBarcode[30];
		scanf("%s", tempBarcode);
		digitalWrite(BLUE_LED_PIN, LOW);
		int i = 0;
		while(strcmp(tempBarcode, tonerlist[i].barcode) != 0){
			i += 1;
		}
		//deploy toner here
		if(i <= TONER_NUM)
			deployToner(i);
		else{ //barcode not found
			//printf("Barcode not found.\n");
			char tempcommand[] = "python /home/pi/Desktop/Desktop/barcodenotfound.py";
			system(tempcommand);
			mainMenu();
		}
			
	} else if(digitalRead(DELIVER_BTN_PIN) != 1){
		//toner delivery
		digitalWrite(BLUE_LED_PIN, HIGH);
		//printf("Enter the barcode: ");
		char command[] = "python /home/pi/Desktop/Desktop/enterbarcode.py";
		system(command);
		char tempBarcode[30];
		scanf("%s", tempBarcode);
		digitalWrite(BLUE_LED_PIN, LOW);
		int i = 0;
		while(strcmp(tempBarcode, tonerlist[i].barcode) != 0){
			i += 1;
		}
		if(i <= TONER_NUM)
			addToner(i);
		else{
			//printf("Barcode not found.\n");
			char tempcommand[] = "python /home/pi/Desktop/Desktop/barcodenotfound.py";
			system(tempcommand);
			mainMenu();
		}
	}
}

void mainDisplay(){
	//this method calls the python file to show the main display
	//to be ran at beginning of TOTIS and any other time necessary
	char tempCommand[50];
	strcpy(tempCommand, "python /home/pi/Desktop/Desktop/totis-main-display.py");
	system(tempCommand);
}


//begin basic inventory methods below
//remove toner will be called after a button press is detected by GPIO button
void deployToner(int index){
	if(tonerlist[index].quantity == 0){
		static char tempCommand[100];
        	strcpy(tempCommand, "python /home/pi/Desktop/Desktop/tonerat0.py");
		printf("yep\n");
                system(tempCommand);
	} else if(tonerlist[index].quantity > 0){
		tonerlist[index].quantity = tonerlist[index].quantity - 1;
		static char tempCommand[100];
		strcpy(tempCommand, "python /home/pi/Desktop/Desktop/totis_i2c_lcd.py -1 ");
		strcat(tempCommand, tonerlist[index].name);
		system(tempCommand);
		
		//log to file below
		char event[100]; //create a char array named event
		strcat(event, tonerlist[index].name); //append the name of the toner to event
		strcat(event, " removed: "); //append " removed: " to event
		logToFile(event); //pass event to logToFile'
		
		update();
		}
	if(tonerlist[index].quantity <= HI_THRESHOLD_QUANTITY && tonerlist[index].quantity > 0){ //above 0 but below threshold value
		static char tempCommand[100];
		strcpy(tempCommand, "python /home/pi/Desktop/Desktop/email_slack.py 1 ");
		strcat(tempCommand, tonerlist[index].name);
		system(tempCommand);
		mainMenu();
	} else{ 
		static char tempCommand[100];
		strcpy(tempCommand, "python /home/pi/Desktop/Desktop/email_slack.py 0 ");
		strcat(tempCommand, tonerlist[index].name);
		system(tempCommand);
		mainMenu();
	}
}

//add toner will be called after a button press is detected by GPIO button
void addToner(int index){
	
	if(tonerlist[index].barcode[strlen(tonerlist[index].barcode)-1] == 'D'){
		//this means it is a dual pack, so we should add 2 to the toner quantity
		tonerlist[index].quantity = tonerlist[index].quantity + 2;
		static char tempCommand[100];
		static char tempCommand2[100];
		strcpy(tempCommand, "python /home/pi/Desktop/Desktop/email_slack.py 2 ");
		strcat(tempCommand, tonerlist[index].name);
		system(tempCommand);
		
		strcpy(tempCommand2, "python /home/pi/Desktop/Desktop/totis_i2c_lcd.py 1 ");
		strcat(tempCommand2, tonerlist[index].name);
		system(tempCommand2);
		
		//begin log to file here
		char event[100]; //create a char array named event
		strcat(event, tonerlist[index].name); //append the name of the toner to event
		strcat(event, " added: "); //append ' added: ' to the end of event so event will be of form 'Two TONER added: ' after this line
		logToFile(event); //pass event to logToFile
		
		//update the toners.txt file
		update();
		
		mainMenu(); //return to main menu

	} else { //this means only a single pack, therefore we should only add 1 to toner quantity

		tonerlist[index].quantity = tonerlist[index].quantity + 1; //increment the quantity by 1
		static char tempCommand1[100]; //first command char array
		static char tempCommand2[100]; //second command char array
		strcpy(tempCommand1, "python /home/pi/Desktop/Desktop/email_slack.py 2 "); //copy the first part of the email_slack command to tempCommand1
		strcat(tempCommand1, tonerlist[index].name); //append the name of the toner to the end of tempCommand1 to finish the formatting needed to call that python script
		system(tempCommand1); //use a system call to call the python command with the command defined in the last 2 lines
		strcpy(tempCommand2, "python /home/pi/Desktop/Desktop/totis_i2c_lcd.py 1 "); //put the first part of the lcd command to tempCommand2
		strcat(tempCommand2, tonerlist[index].name); //append the name of the toner to the end of tempCommand2 to finish the formatting needed to call that python script
		system(tempCommand2); //use a system call to call the python command with the command defined in the last 2 lines
		
		//begin log to file here
		char event[100]; //create a char array named event
		strcat(event, tonerlist[index].name); //append the name of the toner to event
		strcat(event, " added: "); //append " added: " to the end of event so event will be of form 'One TONER added: ' after this line
		logToFile(event); //pass event to logToFile
		
		//update the toners.txt file
		update();
		
		mainMenu(); //return to main menu
	}
}

void logToFile(char* event){ //event is a string passed when necessary, for example "80x added: " or "81x removed: ". will be followed by time
	time_t rawtime; //rawtime variable of type time_t
	struct tm *info; //pointer of type tm
	char buffer[80]; //buffer will eventually hold the date/time string
	time(&rawtime); //here we casting rawtime's value to type time
	info = localtime(&rawtime); //now, set info to the return of localtime after passing the time type contents of rawtime
	strftime(buffer, 80, "%x - %I:%M%p", info); //simply formatting the string
	strcat(event, buffer); //appending buffer (date/time info) to the event string passed to the function
	FILE *fp; //file pointer
	fp = fopen("/var/www/html/totislog.txt", "a"); //make file pointer actually point to the log file, open in append mode
	fputs(event, fp); //print the string to the file
	fputs("\n", fp); //prints a new line at the end so that each entry is a new line for ease of reading the log file
	//fputs was used because the string was pre-formatted so we don't have to worry about using formatting so don't use fprintf
	fclose(fp); //close the file to preserve it
}

void update(){
	FILE *toners;
		toners = fopen("/var/www/html/toners.txt", "w+");
		for(int i = 0; i < TONER_NUM; i++){
			fprintf(toners, "%s %d %s\n", tonerlist[i].barcode, tonerlist[i].quantity, tonerlist[i].name);
		}
		fclose(toners);
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
	toners = fopen("/var/www/html/toners.txt", "r+");

	for(int i = 0; i < TONER_NUM; i++){
		struct Toner tempToner;
		fscanf(toners, "%s %d %s", &tempToner.barcode, &tempToner.quantity, &tempToner.name);
		tempToner.threshold = HI_THRESHOLD_QUANTITY;
		tonerlist[i] = tempToner;
	}

	//tonerlist now contains the information from toners.txt
	//so we can close toners file below
	fclose(toners);
	
	if(!freopen("/dev/tty", "r", stdin)){
		perror("/dev/tty");
		exit(1);
	}
	
	
	//begin wiringPi setup below
	wiringPiSetup();
	pinMode(DELIVER_BTN_PIN, INPUT); //the deliver button pin setup for input
	pinMode(DEPLOY_BTN_PIN, INPUT); //the deploy button pin setup for input
	pinMode(BLUE_LED_PIN, OUTPUT); //the red led pin setup for output
	pullUpDnControl(DELIVER_BTN_PIN, PUD_UP); //resistor on positive side of button
	pullUpDnControl(DEPLOY_BTN_PIN, PUD_UP); //resistor on positive side of button
	
	mainDisplay();
	
	while(1)
		mainMenu(); //calling this essentially starts the program in the sense of being able to take user input and such

	return 0;
}
