#ifndef FUNC_H
#define FUNC_H

#include <stdbool.h>

typedef struct {
  unsigned int num_rows;
  unsigned int num_cols;
  float **data;
  int is_square;
} Ndarray; 

Ndarray* readCSV(const char* filename, int maxRecords);
void ndarray_free(Ndarray* matrix);
int get_n_success(Ndarray* crabs);
int get_n_failed(Ndarray* crabs);
double sigmoid(double *X, int num_cols, double b0, double *b1);
Ndarray* calculate_sigmoid_list(Ndarray* matrix, double b0, double *b1);

#endif  