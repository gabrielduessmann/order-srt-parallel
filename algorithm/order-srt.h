#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// -------------------------------

// Function to parse the time string and return a time_t object
time_t parse_time(const char *time_str) {
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));
    strptime(time_str, "%H:%M:%S", &tm_time);
     strftime(time_str, 26, "%H:%M:%S", &tm_time);
    return mktime(&tm_time);
}


int isLower(char * str1, char * str2) {
    for (int i=0; i < 12; i++) {
        if (str1[i] == str2[i]) {
            continue;
        }
        else if (str1[i] < str2[i]) {
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}

int isHigher(char * a, char * b) {
    for (int i=0; i < 12; i++) {
        if (a[i] == b[i]) {
            continue;
        }
        else if (a[i] > b[i]) {
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}

// Function to convert time string to milliseconds
long timeToMilliseconds(const char *time) {
    int hours, minutes, seconds, milliseconds;
    sscanf(time, "%d:%d:%d,%d", &hours, &minutes, &seconds, &milliseconds);
    return ((hours * 3600 + minutes * 60 + seconds) * 1000 + milliseconds);
}