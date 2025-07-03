#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char output[128];
    int temperature = 0;

    // Open the command for reading.
    fp = popen("xsct", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return 1;
    }

    // Read the output line.
    if (fgets(output, sizeof(output), fp) != NULL) {
        // Example output: "Screen 0: temperature ~ 5000 0.997068"
        // We extract the number after '~'
        if (sscanf(output, "Screen 0: temperature ~ %d", &temperature) == 1) {
            printf("Extracted temperature: %d\n", temperature);
        } else {
            printf("Failed to parse temperature.\n");
        }
    }

    // Close the command stream.
    pclose(fp);

    return 0;
}
