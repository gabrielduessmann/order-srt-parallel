#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    char label;
    int length;
    Point * points;
} Group;

void on_error() {
    printf("Invalid input file.\n");
    exit(1);
}

int parse_number_of_groups() {
    int n;
    if (scanf(" n_groups=%d ", &n) != 1) on_error();

    return n;
}

Point parse_point() {
    float x, y;
    if (scanf(" (%f ,%f) ", &x, &y) != 2)  on_error();

    Point point;
    point.x = x;
    point.y = y;

    return point;
}

Group parse_next_group() {
    char label; 
    int length;

    if (scanf(" label=%c ", &label) != 1) on_error();
    if (scanf(" length=%d ", &length) != 1) on_error();

    Group group;
    group.label = label;
    group.length = length;
    group.points = (Point *) malloc(sizeof(Point) * length);

    for (int i = 0; i < length; i++) {
        group.points[i] = parse_point();
    }

    return group;
}

int parse_k() {
    int k;
    if (scanf(" k=%d ", &k) != 1) on_error();

    return k;
}

char* read_time() {
    char* initial_time;
    char* final_time;
    if (scanf("%s -->", initial_time) != 1) on_error();

    return initial_time;
}

float euclidean_distance_no_sqrt (Point a, Point b) {
    return ((b.x - a.x) * ((b.x - a.x))) + ((b.y - a.y) * (b.y - a.y));
}

int compare_for_sort(const void *a, const void *b) {
  return *(char*)a - *(char*)b;
}

// -------------------------------

// Function to parse the time string and return a time_t object
time_t parse_time(const char *time_str) {
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));
    strptime(time_str, "%H:%M:%S", &tm_time);
     strftime(time_str, 26, "%H:%M:%S", &tm_time);
     //puts(time_str);
    return mktime(&tm_time);
}

// Function to format time_t to string in "HH:MM:SS,mmm" format
void format_time(time_t t, char *buffer) {
    struct tm *tm_time = localtime(&t);
    strftime(buffer, 13, "%H:%M:%S,%L", tm_time);
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