#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define STEP_TEMP 50
#define	STEP_DELAY_MS 50
#define CONFIG "/.config/eyelight/temperatures.txt"

//global variables
char path_to_config[73];	//supports upto 32 char usersnames
char *home_directory;
struct alltemperatures{
	int day_temp;
	int transition_temp;
	int night_temp;
}temps;
int current_temp=0;

void load_current_temp(){
	//printf("load_current_temp function launched\n");
	FILE *fp;
	char xsct_output[40];
	fp = popen("xsct","r");
	if(fp == NULL) perror("Failed to run xsct\n");
	
	if (fgets(xsct_output, sizeof(xsct_output), fp) != NULL){
		// Example output: "Screen 0: temperature ~ 5000 0.997068"
        if (sscanf(xsct_output, "Screen 0: temperature ~ %d", &current_temp) != 1) {
		printf("Failed to parse temperature.\n");
		} else {
		//printf("Extracted temperature: %d\n", current_temp);
		}
	}	
}

void smooth_transition(int current_temp, int resultant_temp){
	//printf("Smooth transition func launched\n");
	int step = (current_temp > resultant_temp)? -STEP_TEMP : STEP_TEMP;
	for (int t = current_temp; (step > 0) ? t <=resultant_temp : t >= resultant_temp; t += step){
		char command[11];
		snprintf(command, sizeof(command), "xsct %d", t);
		system(command);
		usleep(STEP_DELAY_MS*1000);
	}	
}

void daemonize() {
    //printf("Daemonize function launced\n");
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
	//printf("fetch_temps function launched\n");
	fscanf(config_file, "%d\n%d\n%d", &temps.day_temp, &temps.transition_temp, &temps.night_temp);
}

void generate_config(){
	//printf("generate config function laucnhed\n");
	FILE *config_file = fopen(path_to_config, "w");
	if (!config_file){
		perror("Could not create file, check permissions\n");
		exit(EXIT_FAILURE);
	}
	fprintf(config_file, "6500\n5000\n4000");
	fclose(config_file);
}

int fetch_config_location(){
	//printf("fetch_config_location function laucnhed\n");
	home_directory = getenv("HOME");
	    if (!home_directory){
		perror("Error in finding config's location");
		exit(EXIT_FAILURE);
	}
	snprintf(path_to_config, sizeof(path_to_config), "%s%s", home_directory, CONFIG);
}

void time_check_and_change_temperature(FILE *config_file){
	//printf("time_check_and_change_temperature function launched\n");
	load_current_temp();
	fetch_temps(config_file);
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);
	int hour = tm_info->tm_hour;
	//Time checking begins according to hours:
	// asterisk = not included
	//19-5 o clock night
	//6-8 transition
	//8-16 day
	//17-18 transition
	if (hour >= 19 || hour <= 5) {
		//printf("Night time: %02d:00\n", hour);
		smooth_transition(current_temp, temps.night_temp);
	} else if ((hour >= 6 && hour <= 7) || (hour >= 17 && hour <= 18)) {
		//printf("Transition time: %02d:00\n", hour);
		smooth_transition(current_temp, temps.transition_temp);
	} else if (hour >= 8 && hour <= 16) {
		//printf("Day time: %02d:00\n", hour);
		smooth_transition(current_temp, temps.day_temp);
	} else {
		printf("Unknown time: %02d:00\n", hour);  // safety fallback
	}
}

int main(){
	fetch_config_location();
//	open_config();
	FILE *config_file = fopen(path_to_config, "r");
	if(!config_file){
		generate_config();
		config_file = fopen(path_to_config, "r");
		if(!config_file){
			perror("Error opening file\n");exit(EXIT_FAILURE);
		}
	}
	
	//fetch_temps(config_file);
//	daemonize();
	
	while (1) {
		time_check_and_change_temperature(config_file);
		sleep(60); // Check every minute
    }
}
