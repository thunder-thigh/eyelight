#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define STEP_TEMP 50
#define	STEP_DELAY_MS 50
#define CONFIG /.config/eyelight/temperatures.txt

//global variables
char path_to_config[72];	//supports upto 32 char usersnames
char *home_directory;
typedef struct alltemperatures{
	int day_temp;
	int transition_temp;
	int night_temp;
}temps;

void transition(int current_temp, int resultant_temp){
	int step = (current_temp > resultant_temp)? -STEP_TEMP : STEP_TEMP;
	for (int t = current_temp; (step > 0) ? t <=resultant_temp : t >= resultant_temp; t += step){
		char command[11];
		snprintf(command, sizeof(command), "xsct %d", t);
		system(command);
		usleep(STEP_DELAY_MS*1000);
	}	
}
	 
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // Parent exits
    // Child continues
    umask(0);
    setsid();
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void fetch_temps(FILE *config_file){
	fscanf(config_file, "%d\n%d\n%d", &temps.day_temp, &temps.transition_temp, &temps.night_temp);
}

void generate_config(){
	FILE *config_file = fopen(path_to_config, "w");
	if (!config_file){
		perror("Could not create file, check permissions\n");
		exit(EXIT_FAILURE);
	}
	fprintf(config_file, "6500\n5000\n4000");
	fclose(config_file);
}

int fetch_config_location(){
	home_directory = getenv("HOME");
	    if (!home_directory){
		perror("Error in finding config's location");
		exit(EXIT_FAILURE);
		return 0;
	}
	snprintf(path_to_config, strlen(home_directory)+strlen(CONFIG), "%s%s", home_directory, CONFIG);
}

int main(){
	fetch_config_location();
//	open_config();
	FILE *config_file = fopen(path_to_config, "r");
	if(!config_file){
		fclose(config_file);
		generate_config();
	}
	FILE *config_file = fopen(path_to_config, "r");
	if(!config_file){perror("Error opening file\n");exit(EXIT_FAILURE);}
	fetch_temps(config_file);
	daemonize();
	
	while (1) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);

        int hour = tm_info->tm_hour;
        int min = tm_info->tm_min;

        if (hour == 10 && min == 0) {
            int current = load_temp();
            if (current != NIGHT_TEMP) {
                smooth_transition(current, NIGHT_TEMP);
            }
        } else if (hour == 6 && min == 0) {
            int current = load_temp();
            if (current != DAY_TEMP) {
                smooth_transition(current, DAY_TEMP);
            }
        }
        sleep(60); // Check every minute
    }
    return 0;
}
}
