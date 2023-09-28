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

// int get_n_success(Ndarray* crabs)
// {
//   int n_success = 0;

//   for (int i = 0; i < crabs->num_rows; i++)
//   {
//     if (crabs->data[i][0] == 1.0)  // Perubahan pada baris ini
//     {
//       n_success += 1;
//     }
//   }
  
//   return n_success;
// }

// int get_n_failed(HorseshoeCrab* crabs, int records)
// {
//   int n_failed = 0;

//   for (int i = 0; i < records; i++)
//   {
//     if (crabs[i].satellite == 0)
//     {
//       n_failed += 1;
//     }
//   }
  
//   return n_failed;
// }