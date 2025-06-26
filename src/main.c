#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

#define TEMP_FILE "/tmp/screen_temp.txt"
#define STEP 100
#define STEP_DELAY_MS 100
#define DAY_TEMP 6500
#define NIGHT_TEMP 3000

// Write current temperature to file
void save_temp(int temp) {
    FILE *f = fopen(TEMP_FILE, "w");
    if (f) {
        fprintf(f, "%d", temp);
        fclose(f);
    }
}

// Read last temperature from file
int load_temp() {
    int temp = DAY_TEMP;
    FILE *f = fopen(TEMP_FILE, "r");
    if (f) {
        fscanf(f, "%d", &temp);
        fclose(f);
    }
    return temp;
}

// Gradually change temperature
void smooth_transition(int from, int to) {
    int step = (to > from) ? STEP : -STEP;
    for (int t = from; (step > 0) ? t <= to : t >= to; t += step) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "xsct %d", t);
        system(cmd);
        usleep(STEP_DELAY_MS * 1000);
    }
    save_temp(to);
}

// Make the process a daemon
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

int main() {
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
