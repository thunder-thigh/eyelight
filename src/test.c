#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONFIG "/.config/eyelight/temperatures.txt"
char path_to_config[72];

typedef struct alltemps{
	int day_temp;
	int transition_temp;
	int night_temp;
}temps;

int fetch_temps(){
	fscanf(config_file, "%d\n%d\n%d" temps->day_temp, temps->transition_temp, temps->night_temp);
}

int main(){
	const char *home_directory = getenv("HOME");
	printf("Home directory is: %s\n", home_directory);
	printf("%s\n",CONFIG);
	printf("length is: %d\n", strlen(home_directory));
	snprintf(path_to_config, strlen(home_directory)+strlen(CONFIG), "%s%s", home_directory, CONFIG);
	printf("Config located at %s", path_to_config);
	FILE *config_file = fopen(path_to_config, "r");
	fetch_temps();
	printf("%d%d%d\n", temps.day_temp, temps.transition_temp, temps.night_temp);
}
