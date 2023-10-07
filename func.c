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

        double* matrix_row = (double*) matrix->data[i];
        
        sigmoid_list->data[i][0] = sigmoid(matrix_row, matrix->num_cols, b0, b1);
    }
    
    return sigmoid_list;
}

double cost_function(Ndarray* matrix, double *pi) {
    double eps = 1e-10;
    double log_loss = 0.0;
    
    for (int i = 0; i < matrix->num_rows; i++) {
        double log_like_success = matrix->data[i][0] * log(pi[i] + eps);
        double log_like_failure = (1 - matrix->data[i][0]) * log(1 - pi[i] + eps);
        double log_like_total = log_like_success + log_like_failure;
        log_loss -= log_like_total;
    }
    
    return log_loss;
}

float gradient_b0(Ndarray* matrix, double *pi){
  float grad;
  for(int i=0; i<matrix->num_rows; i++){
    grad += (pi[i] - matrix->data[i][0]);
  }

  return grad;
}

double* gradient_b1(Ndarray* matrix, double* pi, double* y, double b0, double* b1) {
    int num_rows = matrix->num_rows;
    int num_cols = matrix->num_cols;
    double* grad_b1 = (double*)malloc(num_cols * sizeof(double));

    for (int j = 0; j < num_cols; j++) {
        grad_b1[j] = 0.0;
        for (int i = 0; i < num_rows; i++) {
            double* matrix_row = (double*)matrix->data[i];
            double sigmoid_val = sigmoid(matrix_row, num_cols, b0, b1);
            grad_b1[j] += matrix_row[j] * (sigmoid_val - y[i]);
        }
    }

    return grad_b1;
}

void gradient_descent(double *X, double *y, int num_rows, int num_cols, double eta, double tol, double *b0, double *b1, int *iterations, double *log_loss_list) {
    // Define initial b0 and b1
    double b0_initial = 0.0;
    double *b1_initial = malloc(num_cols * sizeof(double));
    for (int i = 0; i < num_cols; i++) {
        b1_initial[i] = 0.0;
    }
    
    // Make a criteria to run the iteration
    int continue_iteration = 1;
    
    // Running the iteration
    int i = 0;
    while (continue_iteration) {
        // Update i
        i++;
        
        // Calculate success probability (pi) from the current b0 and b1
        Ndarray matrix;
        matrix.num_rows = num_rows;
        matrix.num_cols = num_cols;
        matrix.data = malloc(matrix.num_rows * sizeof(double*));
        for (int j = 0; j < matrix.num_rows; j++) {
            matrix.data[j] = &X[j * matrix.num_cols];
        }
        Ndarray* sigmoid_list = calculate_sigmoid_list(&matrix, b0_initial, b1_initial);
        double *pi = malloc(num_rows * sizeof(double));
        for (int j = 0; j < num_rows; j++) {
            pi[j] = sigmoid_list->data[j][0];
        }
        
        // Calculate log loss from the current b0 and b1
        double log_loss = cost_function(&matrix, pi);
        log_loss_list[i - 1] = log_loss;

        // Calculate gradient of b0 and b1
        double grad_b0 = gradient_b0(&matrix, pi);
        double* grad_b1 = gradient_b1(&matrix, pi, y, b0_initial, b1_initial);

        // Update b0 and b1
        *b0 -= eta * grad_b0;
        for (int j = 0; j < num_cols; j++) {
            b1[j] -= eta * grad_b1[j];
        }

        // Check convergence
        if (i > 1 && fabs(log_loss_list[i - 1] - log_loss_list[i - 2]) < tol) {
            continue_iteration = 0;
        }

        // Free memory
        free(pi);
        free(grad_b1);
        free(sigmoid_list->data);
        free(sigmoid_list);
        free(matrix.data);
    }

    // Store the number of iterations
    *iterations = i;

    // Free memory for initial b1
    free(b1_initial);
}


Ndarray* standarize(Ndarray* matrix, int n) {
    int i, j;
    double sum = 0;
    double sum_std = 0;
    double mean, std;

    Ndarray* standarized_x = malloc(sizeof(Ndarray));
    standarized_x->num_rows = matrix->num_rows;
    standarized_x->num_cols = matrix->num_cols;
    standarized_x->data = malloc(standarized_x->num_rows * sizeof(double*));

    for (i = 0; i < matrix->num_rows; i++) {
        standarized_x->data[i] = malloc(standarized_x->num_cols * sizeof(double));
        sum += matrix->data[i][1];
    }
    mean = sum / n;

    for (i = 0; i < matrix->num_rows; i++) {
        sum_std += pow(matrix->data[i][1] - mean, 2);
    }
    std = sqrt(sum_std / n);

    for (i = 0; i < matrix->num_rows; i++) {
        standarized_x->data[i][0] = (matrix->data[i][1] - mean) / std;
    }

    return standarized_x;
}