#ifndef FUNC_H
#define FUNC_H

#include <stdbool.h>

typedef struct {
  unsigned int num_rows;
  unsigned int num_cols;
  float **data;
  int is_square;
} Ndarray; // Definisikan struct Ndarray secara lengkap di sini

Ndarray* readCSV(const char* filename, int maxRecords);
int get_n_success(Ndarray* crabs);
int get_n_failed(Ndarray* crabs);

#endif  