#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#define STEP 50
#define	STEP_DELAY_MS 50
#define CONFIG /.config/eyelight/temperatures.txt

struct passwd *pw;

typedef struct temps{
	int day_temp;
	int transition_temp;
	int night_temp;
};
 
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
