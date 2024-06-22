#include <stdio.h>
#include <stdlib.h>
#include "order-srt.h"
#include <time.h>
#include <string.h>
#include <limits.h>

#define MAX_SUBTITLES 50000000
#define MAX_TIME_LEN 13 // Format "HH:MM:SS,mmm"    
#define MAX_TEXT_LEN 200

typedef struct {
    char initial_time[MAX_TIME_LEN];
    char final_time[MAX_TIME_LEN];
    char text[MAX_TEXT_LEN];
} Subtitle;

void swap(Subtitle* a, Subtitle* b){
  Subtitle t = *a;
  *a = *b;
  *b = t;
}

int partition(Subtitle * vetor, int low, int high){
  char * pivot = vetor[high].initial_time;
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++){
    if (isLower(vetor[j].initial_time, pivot) == 1){
      i++;
      swap(&vetor[i], &vetor[j]);
    }
  }
  swap(&vetor[i + 1], &vetor[high]);
  return (i + 1);
}

void quicksort(Subtitle vetor[], int low, int high) {
  if (low < high) {
    int pivot = partition(vetor, low, high);
    quicksort(vetor, low, pivot - 1);
    quicksort(vetor, pivot + 1, high);
  }
}

int main(int argc, char **argv){
    Subtitle * subtitles = (Subtitle*) malloc(sizeof(Subtitle) * MAX_SUBTITLES);
    int subtitle_count = 0;

    FILE *file = fopen("generated_subtitles.srt", "r");
    if (!file) {
        perror("Could not open file");
        return 1;
    }

    while (subtitle_count < MAX_SUBTITLES) {
        char line[256];

        // Read index line (and ignore it)
        if (!fgets(line, sizeof(line), file)) break;

        // Read time range line
        if (!fgets(line, sizeof(line), file)) break;
        sscanf(line, "%12s --> %12s", subtitles[subtitle_count].initial_time, subtitles[subtitle_count].final_time);

        // Read text lines
        char text[MAX_TEXT_LEN] = "";
        while (fgets(line, sizeof(line), file) && line[0] != '\n') {
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
        strncpy(subtitles[subtitle_count].text, text, MAX_TEXT_LEN - 1);

        subtitle_count++;
    }

    fclose(file);

    quicksort(subtitles, 0, subtitle_count - 1);

    // Print parsed subtitles

    printf("Resolved \n");
    
    /*
    for (int i = 0; i < subtitle_count; i++) {
        printf("Subtitle %d:\n", i + 1);
        printf("Initial time: %s\n", subtitles[i].initial_time);
        printf("Final time: %s\n", subtitles[i].final_time);
        printf("Text: %s\n", subtitles[i].text);
        printf("\n");
    }

    */

    return 0;
}