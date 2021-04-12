#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

/**
SETUP FOR SETTING SOUND TIME USING ALARM-CLOCK-LIKE BUTTONS USING WIRING PI

todo: fix sleep in loop, auto start
Testing some stuff in C
before this works, use the folling commands in Raspbian
gpio mode 0 in
gpio mode 0 up

**/


static int min = 20, hour = 10, timeset = 0;
static int* curtimeunit = &min; static int i=0;


int main(int argc, char *argv[]) {

	puts("go");
	//setup, returns -1 if failed
	if (wiringPiSetup() == -1) {
		return -1; //failed to init
	}
	pinMode(12, INPUT);//mosi pin, all out of pins, switch min and hour
	pinMode(13, INPUT);//miso pin, all out of pins, set time, re use for deleting cron jobs
	pinMode(7, INPUT);//GPIO7
	pinMode(1, INPUT);//GPIO1
	while (1) {

			//char in = getchar();
		if (digitalRead(1)==1){
			if (timeset<2){
				makeCron(hour, min, "Geluid1", 30);
				timeset++;
				//system("echo last press $(date) > /home/pi/templog");
			//	puts("time %s set", timeset);
				}
			else if (timeset >2){
				puts("times removed");
				system("crontab -r");
				timeset=0;
			}			
			
		}
		if (digitalRead(12)==1){
			*curtimeunit = changetime(curtimeunit, +1);	
			}
		if (digitalRead(7)==1){
			*curtimeunit = changetime(curtimeunit, -1);	
			}
		if (digitalRead(13)==1)	
			curtimeunit = switchtimeunit(curtimeunit);
		delay(100);	
	}
	return 0;
}

int changetime(int* timeunit, int change) {
	*timeunit += change;
	if (timeunit == &min && *timeunit  > 59)
		*timeunit-=60;
	else if (timeunit == &min && *timeunit < 0)
		*timeunit+=60;
	if (timeunit == &hour && *timeunit  > 23)
		*timeunit-=24;
	else if (timeunit == &hour && *timeunit < 0)
		*timeunit+=24;
	printf("target: %d value: %d\n", timeunit, *timeunit);
	return *timeunit;
	
}
int switchtimeunit(curtimeunit){
	if (curtimeunit == &min)
		return &hour;
	else
		return &min;

}
int makeCron(int _hour, int _min, char _filename[20], int _repeat) {
	int i;
	int min = _min;
	int hour = _hour;

	FILE *file = fopen("/home/pi/temp2", "w");
	if (file == NULL) {
		printf("file not found\n");
		return 0;
	}
	for (i = 0; i < _repeat; i++) {
		min++;
		if (min >= 60) {
			min = min - 60;
			hour++;
			if (hour >= 24)
				hour = hour - 24;
		}
		char cronstring[60];
		snprintf(cronstring, sizeof(cronstring),
				"%d %d * * *  aplay /home/pi/%s.wav", min, hour, _filename);
		fprintf(file, " %s\n", cronstring);
	}
	fclose(file);
	system("crontab /home/pi/temp2");
	return 1;
}

