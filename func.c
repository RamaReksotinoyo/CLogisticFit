#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


Ndarray* readCSV(const char* filename, int maxRecords)
{
  FILE* file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return NULL;
  }

  int records = 0;
  int read = 0;
  float** data = malloc(maxRecords * sizeof(float*));
  if (data == NULL) {
    printf("Error allocating memory.\n");
    fclose(file);
    return NULL;
  }

  while (records < maxRecords)
  {
    data[records] = malloc(2 * sizeof(float));
    if (data[records] == NULL) {
      printf("Error allocating memory.\n");
      for (int i = 0; i < records; i++) {
        free(data[i]);
      }
      free(data);
      fclose(file);
      return NULL;
    }

    read = fscanf(file, "%f,%f\n", &data[records][0], &data[records][1]);
  
    if (read == 2)
      records++;
    else if (read != 2 && !feof(file))
    {
      printf("File format incorrect.\n");
      for (int i = 0; i < records; i++) {
        free(data[i]);
      }
      free(data);
      fclose(file);
      return NULL;
    }
    
    if (feof(file))
      break;
  }

  fclose(file);

  Ndarray* crabs = malloc(sizeof(Ndarray));
  if (crabs == NULL) {
    printf("Error allocating memory.\n");
    for (int i = 0; i < records; i++) {
      free(data[i]);
    }
    free(data);
    return NULL;
  }

  crabs->num_rows = records;
  crabs->num_cols = 2;
  crabs->data = data;

  return crabs;
}

int get_n_success(Ndarray* crabs)
{
  int n_success = 0;

  for (int i = 0; i < crabs->num_rows; i++)
  {
    if (crabs->data[i][0] == 1.0)  // Perubahan pada baris ini
    {
      n_success += 1;
    }
  }
  
  return n_success;
}

int get_n_failed(Ndarray* crabs)
{
  int n_failed = 0;

  for (int i = 0; i < crabs->num_rows; i++)
  {
    if (crabs->data[i][0] == 0.0)
    {
      n_failed += 1;
    }
  }
  
  return n_failed;
}

void ndarray_free(Ndarray* matrix){
  for (int i = 0; i < matrix->num_rows; i++) {
    free(matrix->data[i]);
  }
  free(matrix->data);
  free(matrix);
}


double sigmoid(double *X, int num_cols, double b0, double *b1) {
    double logit = b0;
    double exp_val, pi;
    
    for (int i = 0; i < num_cols; i++) {
        logit += X[i] * b1[i];
    }
    
    exp_val = exp(logit);
    pi = exp_val / (1 + exp_val);
    
    return pi;
}

Ndarray* calculate_sigmoid_list(Ndarray* matrix, double b0, double *b1) {
    Ndarray* sigmoid_list = malloc(sizeof(Ndarray));
    sigmoid_list->num_rows = matrix->num_rows;
    sigmoid_list->num_cols = 1;
    sigmoid_list->data = malloc(sigmoid_list->num_rows * sizeof(double*));
    
    for (int i = 0; i < sigmoid_list->num_rows; i++) {
        sigmoid_list->data[i] = malloc(sigmoid_list->num_cols * sizeof(double));
        
        // Mengubah tipe data dari float* menjadi double*
        double* matrix_row = (double*) matrix->data[i];
        
        sigmoid_list->data[i][0] = sigmoid(matrix_row, matrix->num_cols, b0, b1);
    }
    
    return sigmoid_list;
}
