#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int i = 0;
	int min = 00;
	int hour = 11;
	char ast[4] = "\*";
	int repeat = 5; //5 minutes
	char filename[20] = "testsound";
	char cronstring[30];
	/** if setup fails, exit**/
	if (wiringPiSetup() == -1) {
		system("echo $(date): setup failed >> /home/pi/log");
		return -1;
	}
	pinMode(4, INPUT); //bcm23
	pinMode(2, OUTPUT);
	while (1) {
		/* making cronjob */
		if (digitalRead(4) == 1) {
			/* making cronjob */
			FILE *file = fopen("temp", "w");
			if (file == NULL) {
				printf("file not found\n");
				exit(1);
			}
			for (i = 0; i < repeat; i++) {
				min++;
				snprintf(cronstring, sizeof(cronstring),
						"%d %s %s %s %s  aplay /home/pi/%s.wav", min, hour,
						ast, ast, ast, filename);
				const char *text = cronstring;
				fprintf(file, " %s\n", text);
			}
			fclose(file);

			system("crontab temp");

		}

	}
}

