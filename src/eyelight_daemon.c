#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>


#define STEP_TEMP 50
#define	STEP_DELAY_MS 50
#define CONFIG /.config/eyelight/temperatures.txt

struct passwd *pw;

typedef struct temps{
	int day_temp;
	int transition_temp;
	int night_temp;
};

void transition(int current_temp, int resultant_temp){
	int step = (current_temp > resultant_temp)? -STEP : STEP;
	for (int t = current_temp; (step > 0) ? t <=resultant_temp : t >= resultant_temp; t += step){
		char command[11];
		snprintf(command, sizeof(command), "xsct %d", t);
		system(command);
		usleep(STEP_DELAY_MS*1000);
	}
	
}
	 
int fetch_temps(){
	struct passwd *pw = getpwuid(getuid());
	printf("Home directory is: %s\n", pw->pw_dir);
	
}

int fetch_config_location(){
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL){
		perror("Error in finding config's location");
		exit(EXIT_FAILURE);
		return 0;
	}
}

int main(){
	fetch_config_location();
	
	daemonize();
	while(1){
		time_t now = time(NULL);
		struct tm *tm_info = localtime(&now);
		if(tm_info->tm_hour==10 && tm_info->tm_min==0){
			update_temp();
			if(
		}
		sleep(60);
		
	}
	
}



/*void daemonize() {
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
*/
