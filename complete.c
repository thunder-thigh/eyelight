#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

//global variables
int curtime, day, night, trans;
char day_string, night_string, trans_string; 
/* char cmdDay[15] = "sct ";
char cmdNight[15] = "sct ";
char cmdTrans[15] = "sct ";

char dayStr[10];
char nightStr[10];
char transStr[10];
*/

//Functions declaration
// Reader functions
int r_trans(){          //Transition state reader
	FILE *fptr; 
	fptr = fopen("/etc/.EyeLight/transition.txt", "r");
	//int num;
	fscanf(fptr, "%d", &trans);
	//printf("%d\n", trans);         
	fclose(fptr);
	return trans;
}

int r_night(){          //Night state reader
	FILE *fptr;
	fptr = fopen("/etc/.EyeLight/night.txt", "r");
	//int num;
	fscanf(fptr, "%d", &night);  
	// printf("%d\n", night);
	fclose(fptr);
	return night;
}

int r_day(){            //Day state reader
	FILE *fptr;
	fptr = fopen("/etc/.EyeLight/day.txt", "r");
	//int num;
	fscanf(fptr, "%d", &day);
	// printf("%d\n", day);
	fclose(fptr);
	return day;
}

//Time checking function
int timecheck(){
	// int currentHour;
        time_t now = time(NULL);
	struct tm *localTime = localtime(&now);
	//currentHour = localTime->tm_hour;
	//printf("%d is the current hour", currentHour);
	curtime = localTime->tm_hour;
	printf("%d is the current hour", curtime);

        return 0;
}

void execTemp(int y){
	char cmd[10], Str[10];
//	sprintf(Str, "%d", y);
	sprintf(cmd, "sct %d", y);
	system(cmd);
}


/* int inttostr(){
	char cmdDay[15] = "sct ";
	char cmdNight[15] = "sct ";
	char cmdTrans[15] = "sct ";

	char dayStr[10];
	char nightStr[10];
	char transStr[10];

	sprintf(dayStr, "%d", day); 	
	sprintf(transStr, "%d", trans);
	sprintf(nightStr, "%d", night);
	printf("%d\n", day);
	strcat(cmdDay, dayStr);
	strcat(cmdTrans, transStr);
	strcat(cmdNight, nightStr);

	return 0;
}
*/

int main2(){
/*	day = r_day();
	night = r_night();
	trans = r_trans();
*/
//	printf("%d\n", day);
//	cmdMaker();
	if((curtime>4 && curtime<=6) || (curtime>17 && curtime<=18)){
		execTemp(trans);
	}
	else if((curtime<5 && 0<=curtime) || (curtime>17 && 18<=curtime)){
		execTemp(night);}
	else {execTemp(day);}
//	cmdDay[15] = "sct ";
//	cmdNight[15] = "sct ";
//	cmdTrans[15] = "sct ";
}

int main() {
    int fd, wd;
    char buffer[BUF_LEN];
		day = r_day();
		night = r_night();
		trans = r_trans();
    main2();
    // Specify the file to watch
    const char *folder_to_watch = "/etc/.EyeLight";  // Change to the file you want to monitor
    //const char *command_to_execute = "sct 4000"; // Change to the command you want to run

    // Create an inotify instance
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Watch the specified file for modifications (IN_MODIFY)
    wd = inotify_add_watch(fd, folder_to_watch, IN_MODIFY);
    if (wd == -1) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Monitoring changes to %s. Press Ctrl+C to stop.\n", folder_to_watch);

    // Monitor the file for events
    while (1) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Process all the events in the buffer
        for (int i = 0; i < length;) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->mask & IN_MODIFY) {
                // If the file is modified, execute the command
		day = r_day();
		night = r_night();
		trans = r_trans();
		main2();
		main2();
                // execute_command(command_to_execute);
            }
            i += EVENT_SIZE + event->len;
        }
    }

    // Clean up
    close(fd);
    return 0;
}

