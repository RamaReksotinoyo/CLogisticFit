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

  Ndarray* crabs = readCSV(filename, maxRecords);
  if (crabs == NULL) {
    printf("Error reading CSV.\n");
    return 1;
  }

  for (int i = 0; i < crabs->num_rows; i++) {
    printf("%f, %f\n", crabs->data[i][0], crabs->data[i][1]);
  }

  for (int i = 0; i < crabs->num_rows; i++) {
    free(crabs->data[i]);
  }
  free(crabs->data);
  free(crabs);

  return 0;
}
