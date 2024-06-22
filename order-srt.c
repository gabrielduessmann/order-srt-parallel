#include <stdio.h>
#include <stdlib.h>
#include "order-srt.h"
#include <time.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
    FILE *file = fopen("subtitles.srt", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }

    char start_time[13];
    char end_time[13];
    int subtitle_count = 0;
    int temp;
    char start_time_str[13] = "99:59:59,999";
    char highest_time_str[13] = "00:00:00,000";
    
    while (fscanf(file, "%d\n", &temp) != EOF) {
        if (fscanf(file, "%12s --> %12s\n", start_time, end_time) == 2) {
            if (isLower(start_time, start_time_str) == 1) {
                strcpy(start_time_str, start_time); 

                printf("lowest: %s \n", start_time_str);
                
            }

            if (isHigher(start_time, highest_time_str)) {
                strcpy(highest_time_str, start_time); 
            }

            subtitle_count++;
        }
    }

    printf("*** closed file \n");
    fclose(file);

    printf("Total subtitles: %d\n", subtitle_count);
    printf("Lowest initial time: %s\n", start_time_str);
    printf("Highest initial time: %s\n", highest_time_str);

    return 0;
}