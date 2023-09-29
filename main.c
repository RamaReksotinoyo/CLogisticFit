#include "headers.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main() {
  const char* filename = "hs.csv";
  int maxRecords = 173;
  int n_s, n_f;
  double pi;

  Ndarray* crabs = readCSV(filename, maxRecords);
  if (crabs == NULL) {
    printf("Error reading CSV.\n");
    return 1;
  }

  for (int i = 0; i < crabs->num_rows; i++) {
    printf("%f, %f\n", crabs->data[i][0], crabs->data[i][1]);
  }

  n_s = get_n_success(crabs);
  n_f = get_n_failed(crabs);
  printf("n_succes: %d, n_failed: %d \n", n_s, n_f);

  double b1[] = {0.0};
  Ndarray* sigmoid_list = calculate_sigmoid_list(crabs, 0.0, b1);
  
  for (int i = 0; i < sigmoid_list->num_rows; i++) {
      printf("%f\n", sigmoid_list->data[i][0]);
  }

  ndarray_free(crabs);
  ndarray_free(sigmoid_list);

  return 0;
}
