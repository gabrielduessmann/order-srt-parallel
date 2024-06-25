#include <stdio.h>
#include <stdlib.h>
#include "order-srt.h"
#include <time.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <mpi.h>

#define MAX_SUBTITLES 100//50000000 
#define MAX_TIME_LEN 13 // Format "HH:MM:SS,mmm"    
#define MAX_TEXT_LEN 200

typedef struct {
    char initial_time[MAX_TIME_LEN];
    char final_time[MAX_TIME_LEN];
    char text[MAX_TEXT_LEN];
} Subtitle;

typedef struct {
    int index_lowest;
    int index_highest;
    int index_current;
    int index;
} Current;

void swap(int* a, int* b){
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(long times[], int index[], int low, int high){
  long pivot = times[index[high]];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++){
    if (times[index[j]] < pivot){
      i++;
      swap(&index[i], &index[j]);
    }
  }
  swap(&index[i + 1], &index[high]);
  return (i + 1);
}

void quicksort(long times[], int index[], int low, int high) {
  if (low < high) {
    int pivot = partition(times, index, low, high);
    quicksort(times, index, low, pivot - 1);
    quicksort(times, index, pivot + 1, high); 
    
  }
}

int p_partition(long times[], int index[], Current state_index_core[], int low, int high){
  long pivot = times[index[state_index_core[high].index_current]];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++){
    if (times[index[state_index_core[j].index_current]] < pivot){
      i++;
      swap(&state_index_core[i], &state_index_core[j]);
    }
  }
  // The swap bellow has a problem
  swap(&state_index_core[i + 1], &state_index_core[high]);

  return (i + 1);
}

void p_quicksort(long times[], int index[], Current state_index_core[], int low, int high) {
  if (low < high) {
    
    int pivot = p_partition(times, index, state_index_core, low, high);
    #pragma omp task
    {
        p_quicksort(times, index, state_index_core, low, pivot - 1);
    }
    #pragma omp task
    {
       p_quicksort(times, index, state_index_core, pivot + 1, high); 
    }
    
  }
}

Current findNextIndex(long all_initial_time[], int all_index_subs[], Current state_index_core[], int size) {
  #pragma omp parallel num_threads(nt)
  {
    #pragma omp single
    {
      p_quicksort(all_initial_time, all_index_subs, state_index_core, 0, size - 1);
    }
  }
 
  return state_index_core[0];
}

int main(int argc, char **argv){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // timer
    clock_t start, end;
    double cpu_time_used;

    Subtitle * subtitles = NULL;
    int subtitle_count = 0;

    long * all_initial_time =  NULL;
    int * all_index_subs = NULL;
    int chunk_size = 0;
    int remainder = 0;

    if (rank == 0) {

      subtitles = (Subtitle*) malloc(sizeof(Subtitle) * MAX_SUBTITLES);
      all_initial_time = (long *) malloc(sizeof(long) * MAX_SUBTITLES);

      printf("Process open file \n");
      FILE *file = fopen("generated_subtitles_5M.srt", "r");
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
          all_initial_time[subtitle_count] = timeToMilliseconds(subtitles[subtitle_count].initial_time);

          // Read text lines
          char text[MAX_TEXT_LEN] = "";
          while (fgets(line, sizeof(line), file) && line[0] != '\n') {
              strncat(text, line, sizeof(text) - strlen(text) - 1);
          }
          strncpy(subtitles[subtitle_count].text, text, MAX_TEXT_LEN - 1);

          subtitle_count++;
      }

      fclose(file);

      printf("Read everything. Before qsort. \n");

      
      chunk_size = subtitle_count / size;
      remainder = subtitle_count % size;

      all_index_subs = (int *) malloc(sizeof(int) * subtitle_count);
    }

    
    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    long * chunk_initial_time = (long *) malloc(sizeof(long) * chunk_size);

    if (rank == 0) {
      printf("before scatter by %d\n", rank);
      printf("chunk is: %d \n", chunk_size);
      printf("subtitle count is: %d \n", subtitle_count);
    }
      
    
    MPI_Scatter(
    all_initial_time, //send data buffer
    chunk_size,
    MPI_LONG,
    chunk_initial_time, //receive data buffer
    chunk_size,
    MPI_LONG,
    0,
    MPI_COMM_WORLD);

    //start = clock();

    int chunk_index_subs[chunk_size];

    for(int i=0; i<chunk_size; i++) {
      chunk_index_subs[i] = (chunk_size * rank) + i;
    }
   
    
    quicksort(chunk_initial_time, chunk_index_subs, 0, chunk_size - 1);
    free(chunk_initial_time);

    MPI_Gather(
    chunk_index_subs, // send data buffer
    chunk_size,
    MPI_INT,
    all_index_subs, // receive data buffer;
    chunk_size,
    MPI_INT,
    0,
    MPI_COMM_WORLD);

    if (rank == 0) {
      subtitle_count = chunk_size * size;
      for (int i = 0; i<subtitle_count; i++) {
        printf("Index subs: %d\n", all_index_subs[i]);
        printf("Initial time: %s\n", subtitles[all_index_subs[i]].initial_time);
        printf("\n");
      }
    }
    

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


    if (rank == 0) {
      // TODO - Remaining values
      int final_index_subs[subtitle_count];
      int count = 0;

      int count_cores = size;
      Current * state_index_core = (Current*) malloc(sizeof(Current) * count_cores); 
      for(int i=0; i<count_cores; i++) {
        state_index_core[i].index_lowest = i * chunk_size;
        state_index_core[i].index_current = i * chunk_size;
        state_index_core[i].index_lowest = (i * chunk_size) + chunk_size - 1;
        state_index_core[i].index = i;
      }
      int go = 1;
      
      while(go == 1) {
        Current current = findNextIndex(all_initial_time, all_index_subs, state_index_core, count_cores);
        final_index_subs[count] = all_index_subs[current.index_current];
        printf("final_index_subs[%d]=%d \n", count, final_index_subs[count]);
        
        current.index_current = current.index_current + 1;
        state_index_core[current.index] = current;
        printf("current index is: %d \n", state_index_core[current.index]);
        if (current.index_current > current.index_highest) {
          // one of the core groups have gone through all index, so it can be excluded from comparison
          count_cores--;
          if (count_cores == 0) {
            break;
          }
          Current temp[count_cores];
          for(int i = 0; i < count_cores; i++) {
            temp[i] = state_index_core[i+1]; 
          }
          free(state_index_core);
          state_index_core = (Current*) malloc(sizeof(Current) * count_cores);
          for(int i = 0; i < count_cores; i++) {
            state_index_core[i] = temp[i]; 
          }
          free(temp);
        } 
        count++;

      }

    
      for (int i = 0; i < subtitle_count; i++) {
        printf("Subtitle %d:\n", i + 1);
        printf("Initial time: %s\n", subtitles[final_index_subs[i]].initial_time);
        printf("Final time: %s\n", subtitles[final_index_subs[i]].final_time);
        printf("Text: %s\n", subtitles[final_index_subs[i]].text);
        printf("\n");
      }
      
    }
    

    //printf("Time taken by example_function: %f seconds\n", cpu_time_used);

    // Print parsed subtitles

    //printf("Resolved by %d\n", rank);
    
    /*
    for (int i = 0; i < subtitle_count; i++) {
        printf("Subtitle %d:\n", i + 1);
        printf("Initial time: %s\n", subtitles[i].initial_time);
        printf("Final time: %s\n", subtitles[i].final_time);
        printf("Text: %s\n", subtitles[i].text);
        printf("\n");
    }

    */

   //free(subtitles);
   //free(all_initial_time);

   MPI_Finalize();

    return 0;
}