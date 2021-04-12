#include <stdio.h>
#include<stdlib.h>
#include<string.h>
/*
 atatatatatta
 TODO:
 * dynamic memory
 * finish arg validation
 * debug output
 * catch errors
 * volume string
 **/
int validateArgs(char** argv, int argc);
int makeCron(int _hour, int _min, char _filename[20], int _repeat);
int printtofile(char* filename, char* print);

int main(int argc, char **argv) {
	if (argc < 5) {
		puts("invalid params input, using debug param\n");
		argv[1] = "something";
		argv[2] = "100";
		argv[3] = "10:33"; //time1
		argv[4] = "20:30"; //time2, 2 times each day can be set
		argv[5] = "4";
	}
	//validateArgs(argv, argc);
	printtofile("text", "dingen");
	int strlen = sizeof(argv[1]);
	char *filename;

	//getting commandline argss
	filename = (char*) malloc(strlen * sizeof(char));
	strcpy(filename, argv[1]);
	char volumestring[30];
	//very cheesy solution, should research if can be done more direct
	system(snprintf(volumestring, sizeof(volumestring),"amixer set \"Master d\" %s; amixer set PCM 100 unmute",argv[2]));

	//time to min and hour
	char hour[3];
	memcpy(hour, &argv[3][0], 2);
	hour[2] = '\0';
	char min[3];
	memcpy(min, &argv[3][3], 2);
	min[2] = '\0';
	int repeat = atoi(argv[5]); //how long sound should play

	makeCron(atoi(hour), atoi(min), filename, repeat);

	//TODO: free properly free stuff
	return 1;
}

int makeCron(int _hour, int _min, char _filename[20], int _repeat) {
	int i;
	int min = _min;
	int hour = _hour;

	FILE *file = fopen("temp", "w");
	if (file == NULL) {
		printf("couldn't open file\n");
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
	system("crontab temp");
	return 1;
}

int printtofile(char* filename, char* print) {
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		printf("file not found\n");
		return 0;
	}
	fprintf(file, print);
	return 1;
}

validateArgs(char **argv, int argc) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s - \n", argv[i]);
	}
}

