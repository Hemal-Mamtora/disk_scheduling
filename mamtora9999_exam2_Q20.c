#include <stdio.h>
// https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
#include <time.h>
#include <stdlib.h>

#define NUM_CYL 1000
#define MAX 5000

int cylinders[NUM_CYL];

void generate_random_cylinders(void){
  int val;
  for (int i=0; i<NUM_CYL; i++){
    val = rand() % MAX; // value between 0 and 4999, inclusive
    cylinders[i] = val;
  }
}

int FIFO(int start_position){
  int movement = 0;
  int current_position = start_position;
  for (int i = 0; i < NUM_CYL; i++){
    movement += abs(cylinders[i] - current_position);
    current_position = cylinders[i];
  }
  return movement;
}

// https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int SCAN(int start_position){
  // note:  assume that head moves towards zero first
  int movement = 0;

  qsort(cylinders, NUM_CYL, sizeof(int), cmpfunc);

  int index = NUM_CYL - 1;

  // find the greatest index smaller than start_position
  for(int i = 0; i < NUM_CYL; i++){
    if (cylinders[i] > start_position) {
      index = i-1;
      break;
    }
  }

  // printf("%d, %d, %d\n", cylinders[0], cylinders[NUM_CYL - 1], cylinders[index+1]);

  int current_position = start_position;

  // disk arm: serves all left side requests first (all less than current start_position)
  for(int i = index; i >= 0; i--){
    movement += abs(cylinders[i] - current_position);
    current_position = cylinders[i];
  }

  movement += abs(current_position - 0); // disk arm: touches zero
  current_position = 0;

  // disk arm: goes other direction
  for(int i = index + 1; i < NUM_CYL; i++){
    movement += abs(cylinders[i] - current_position);
    current_position = cylinders[i];
  }

  return movement;
}

int C_SCAN(int start_position){
  // note:  assume that head moves towards zero first
  int movement = 0;

  qsort(cylinders, NUM_CYL, sizeof(int), cmpfunc);

  int index = NUM_CYL - 1;

  // find the greatest index smaller than start_position
  for(int i = 0; i < NUM_CYL; i++){
    if (cylinders[i] > start_position) {
      index = i-1;
      break;
    }
  }

  // printf("%d, %d, %d\n", cylinders[0], cylinders[NUM_CYL - 1], cylinders[index+1]);

  int current_position = start_position;

  // disk arm: serves all left side requests first (all less than current start_position)
  for(int i = index; i >= 0; i--){
    movement += abs(cylinders[i] - current_position);
    current_position = cylinders[i];
  }

  movement += abs(current_position - 0); // disk arm: touches zero
  current_position = MAX - 1; // 4999 , circular

  // disk arm: serves remaining side
  for(int i = NUM_CYL - 1; i > index; i--){
    movement += abs(cylinders[i] - current_position);
    current_position = cylinders[i];
  }

  return movement;
}

int main( int argc, char *argv[] )  {
  if (argc < 2){
    printf("\nPlease provide start_position of disk head:\n./filename.exe <start_position>\n\n");
    return 0;
  }

  int start_position = atoi(argv[1]);

  if (start_position > 5000 || start_position < 0){
    printf("\nPlease provide start_position betweeen 0 and 4999, inclusive. \n\n");
    return 0;
  }

  printf("start position: %d\n", start_position);
  int movement;
  srand(time(NULL));
  generate_random_cylinders();
  movement = FIFO(start_position);
  printf("Total movement in FIFO: %d\n", movement);

  movement = SCAN(start_position);
  printf("Assuming the head moves towards 0 first.\n");
  printf("Total movement in SCAN: %d\n", movement);

  movement = C_SCAN(start_position);
  printf("Assuming the head moves towards 0 first.\n");
  printf("Total movement in C-SCAN: %d\n", movement);

  return 0;
}