#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>


#define STEP 50
#define	STEP_DELAY_MS 50
#define CONFIG /.config/eyelight/temperatures.txt

struct passwd *pw;

typedef struct temps{
	int day_temp;
	int transition_temp;
	int night_temp;
};
 
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

int update_temp(){
//    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL){
		perror("get_userdetails");
		return 0;
	}
//    else FILE *file = fopen("
}


int main(){
	struct passwd *pw = getpwuid(getuid());
	update_temp();
	printf("Home directory is: %s\n", pw->pw_dir);
	
	
}
