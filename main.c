#include "headers.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main() {
  const char* filename = "hs.csv";
  int maxRecords = 173;
  int n_s, n_f;
  double log_loss, grad0;
  double b0 = 0.0;

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

  double b11[] = {0.0}; // Ubah menjadi double b1[1] = {0.0};
  Ndarray* sigmoid_list = calculate_sigmoid_list(crabs, 0.0, b11);
  
  for (int i = 0; i < sigmoid_list->num_rows; i++) {
      printf("%f\n", sigmoid_list->data[i][0]);
  }

  log_loss = cost_function(crabs, (double*)sigmoid_list->data);
  printf("Log Loss: %f\n", log_loss);

  // grad0 = gradient_b0(crabs, (double*)sigmoid_list->data);
  // printf("grad of b0: %f\n", grad0);

  // double* pi = (double*)sigmoid_list->data;
  // double* y = (double*)malloc(crabs->num_rows * sizeof(double));

  // for (int i = 0; i < crabs->num_rows; i++) {
  //     y[i] = crabs->data[i][1];
  // }

  // double* grad_b1 = gradient_b1(crabs, (double*)sigmoid_list->data, y, b0, b1);
  // printf("Gradient of b1: ");
  // for (int j = 0; j < crabs->num_cols; j++) {
  //     printf("%f ", grad_b1[j]);
  // }
  printf("\n");

  // printf("num of rows: %d \n", crabs->num_rows);

  // Ndarray* std = standarize(crabs, crabs->num_rows);
  
  // for (int i = 0; i < std->num_rows; i++) {
  //     printf("%f\n", std->data[i][0]);
  // }

  // // Pemanggilan gradient_descent
  // int iterations_list[100];
  // double log_loss_list[100];
  // double b1_opt[1]; 

  // gradient_descent((double*)crabs->data, y, crabs->num_rows, crabs->num_cols, 0.01, 1e-4, &b0, b1_opt, iterations_list, log_loss_list);


  // // Menampilkan hasil
  // printf("Optimum b0: %f\n", b0);
  // printf("Optimum b1: %f\n", b1_opt[0]); 

  // Gradient Descent
  int num_rows = crabs->num_rows;
  int num_cols = crabs->num_cols - 1;
  double eta = 0.01;
  double tol = 0.0001;
  int iterations;
  double *log_loss_list = malloc(100 * sizeof(double));
  // double *b1 = malloc(num_cols * sizeof(double)); // Tambahkan alokasi memori untuk b1

  srand(time(NULL));
  double b1[2] = {((double)rand() / RAND_MAX) * 0.01, ((double)rand() / RAND_MAX) * 0.01};

  // Panggil fungsi gradient_descent()
  gradient_descent((double*)crabs->data, (double*)sigmoid_list->data, num_rows, num_cols, eta, tol, &b0, b1, &iterations, log_loss_list);


  // Bebaskan memori
  free(log_loss_list);
  free(crabs->data);
  free(crabs);
  free(sigmoid_list->data);
  free(sigmoid_list);
  // free(b1); // Hapus baris ini

}
