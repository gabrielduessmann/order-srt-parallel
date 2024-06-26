#include <stdio.h>
#include <stdlib.h>
#include "order-srt.h"
#include <time.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

#define MAX_SUBTITLES 50000000 // 50M
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
    #pragma omp task
    {
        quicksort(vetor, low, pivot - 1);
    }
    #pragma omp task
    {
       quicksort(vetor, pivot + 1, high); 
    }
    
    
  }
}

int main(int argc, char **argv){
  // Check if file is specified
  if (argc != 2) {
      fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
      return 1;
  }

    // timer
    double start, end;
    start = omp_get_wtime();

    double start_read, end_read;
    double start_process, end_process;
    double start_write, end_write;
    double cpu_time_used;

    Subtitle * subtitles = (Subtitle*) malloc(sizeof(Subtitle) * MAX_SUBTITLES);
    int subtitle_count = 0;

    start_read = omp_get_wtime();

    char *file_name = argv[1];
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("File not found\n");
        return 1;
    }

    while (subtitle_count < MAX_SUBTITLES) {
        char line[256];
        if (!fgets(line, sizeof(line), file)) break;

        if (!fgets(line, sizeof(line), file)) break;
        sscanf(line, "%12s --> %12s", subtitles[subtitle_count].initial_time, subtitles[subtitle_count].final_time);

        char text[MAX_TEXT_LEN] = "";
        while (fgets(line, sizeof(line), file) && line[0] != '\n') {
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
        strncpy(subtitles[subtitle_count].text, text, MAX_TEXT_LEN - 1);

        subtitle_count++;
    }

    fclose(file);


    end_read = omp_get_wtime();
    cpu_time_used = ((double) (end_read - start_read));
    printf("Time to read file: %f seconds\n", cpu_time_used);

    start_process = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            quicksort(subtitles, 0, subtitle_count - 1);
        }

    }
    end_process = omp_get_wtime();
    cpu_time_used = ((double) (end_process - start_process));
    printf("Time to process quick-sort: %f seconds\n", cpu_time_used);

    start_write = omp_get_wtime();

    FILE *file_output = fopen("subtitles_result.srt", "w");
    if (!file_output) {
      perror("Could not open file");
      return 1;
    }

    for (int i = 0; i < subtitle_count; i++) {
        fprintf(file_output, "%d\n", i + 1);
        fprintf(file_output, "%s --> %s\n", subtitles[i].initial_time, subtitles[i].final_time);
        fprintf(file_output, "%s\n", subtitles[i].text);
    }
    fclose(file_output);

   end_write = omp_get_wtime();

   cpu_time_used = ((double) (end_write - start_write));
   printf("Time to write file: %f seconds \n", cpu_time_used);

   end = omp_get_wtime();

    cpu_time_used = ((double) (end - start));
    printf("Time to run everything: %f seconds\n", cpu_time_used);

    return 0;
}