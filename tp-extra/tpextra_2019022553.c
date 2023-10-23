#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_NUMBER_OF_RESTRICTIONS 100
#define MAX_NUMBER_OF_VARIABLES 100

void dbg(const char *msg) { fprintf(stdout, "%s", msg); }

void err_n_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stdout, "usage: simplex [input_file_path]");
    exit(EXIT_FAILURE);
  }

  /*   A coluna B SEMPRE será a coluna 'number_of_variables' da matriz. As
   * outras colunas após isso são lixo
   */
  int number_of_restrictions, number_of_variables;
  int cost_array[MAX_NUMBER_OF_VARIABLES];
  int matrix[MAX_NUMBER_OF_RESTRICTIONS][MAX_NUMBER_OF_VARIABLES + 1];

  // Parse input

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    err_n_die("Error opening input file.\n");
  }

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    err_n_die("Error using fread on first line on input.\n");
  }

  number_of_restrictions = atoi(strtok(buffer, " "));
  number_of_variables = atoi(strtok(NULL, " "));

  memset(buffer, 0, BUFFER_SIZE);
  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    err_n_die("Error using fread on second line on input.\n");
  }

  cost_array[0] = atoi(strtok(buffer, " "));
  for (int i = 1; i < number_of_variables; i++) {
    cost_array[i] = atoi(strtok(NULL, " "));
  }

  for (int i = 0; i < number_of_restrictions; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
      fprintf(stdout, "%d\n", i);
      err_n_die("Error using fread on line of matrix.\n");
    }

    matrix[i][0] = atoi(strtok(buffer, " "));
    for (int j = 1; j < number_of_variables + 1; j++) {
      matrix[i][j] = atoi(strtok(NULL, " "));
    }
  }

  fclose(fp);

  // End input parsing

  // Print input for debug
  /* printf("%d %d\n", number_of_restrictions, number_of_variables);
  for (int i = 0; i < number_of_variables; i++) {
    printf("%d ", cost_array[i]);
  }
  printf("\n");

  for (int i = 0; i < number_of_restrictions; i++) {
    for (int j = 0; j < number_of_variables + 1; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  } */

  




  return EXIT_SUCCESS;
}
