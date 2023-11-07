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

/* Assume que já existe uma base viável canônica na matriz.
Assume-se que essa base já tem identidade e que b >= 0 */
void tableau(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES + 1],
    int number_of_restrictions, int number_of_variables) {

  // reverte sinais de ct
  for (int i = MAX_NUMBER_OF_RESTRICTIONS;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables; i++) {
    matrix[0][i] *= -1;
  }

  for (;;) {
    int col = -1;
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables; i++) {
      if (matrix[0][i] < 0) {
        col = i;
        break;
      }
    }

    if (col == -1) {
      fprintf(stdout, "acabou-se\n");
      return;
    }

    // tenho q achar linha do melhor ratio
    double ratio = -1;
    int min_ratio_line = -1;
    for (int i = 1; i < number_of_restrictions + 1; i++) {
      if (matrix[i][col] > 0) {
        double new_ratio = matrix[i][number_of_variables + 1] / matrix[i][col];
        ratio = ratio != -1 && ratio <= new_ratio ? ratio : new_ratio;
        min_ratio_line = i;
      }
    }

    if (ratio == -1) {
      fprintf(stdout, "ilimitada\n");
      return;
    }

    // pivoteia linha
    for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         i < MAX_NUMBER_OF_VARIABLES + number_of_variables + 1; i++) {
      matrix[min_ratio_line][i] /= matrix[min_ratio_line][col];
    }

    // zera resto da coluna
    for (int i = 0; i < number_of_restrictions + 1; i++) {
      if (i == min_ratio_line)
        continue;
      double curr_ratio = matrix[i][col] / matrix[min_ratio_line][col];

      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_VARIABLES + number_of_variables + 1; j++) {
        matrix[i][j] -= matrix[min_ratio_line][j] * curr_ratio;
      }
    }
  }
}

void auxiliar(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES + 1],
    int number_of_restrictions, int number_of_variables);

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stdout, "usage: simplex [input_file_path]");
    exit(EXIT_FAILURE);
  }

  /*   A coluna B SEMPRE será a coluna 'number_of_variables' da matriz. As
   * outras colunas após isso são lixo
   */
  int number_of_restrictions, number_of_variables;

  /* A matriz possui tamanho dobrado para armazenar o registro de operações à
   * esquerda */
  double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
               [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES + 1] = {0};

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

  /* Populating VERO with identity matrix. */

  for (int i = 1; i < number_of_restrictions + 1; i++) {
    matrix[i][MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions + i - 1] = 1;
  }

  memset(buffer, 0, BUFFER_SIZE);
  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    err_n_die("Error using fread on second line on input.\n");
  }

  // Reading cost array
  matrix[0][MAX_NUMBER_OF_RESTRICTIONS] = (double)atoi(strtok(buffer, " "));
  for (int i = 1; i < number_of_variables; i++) {
    matrix[0][MAX_NUMBER_OF_RESTRICTIONS + i] = (double)atoi(strtok(NULL, " "));
  }

  // Reading each restriction
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
      fprintf(stdout, "%d\n", i);
      err_n_die("Error using fread on line of matrix.\n");
    }

    matrix[i][MAX_NUMBER_OF_RESTRICTIONS] = (double)atoi(strtok(buffer, " "));
    for (int j = 1; j < number_of_variables + 1; j++) {
      matrix[i][MAX_NUMBER_OF_RESTRICTIONS + j] =
          (double)atoi(strtok(NULL, " "));
    }
  }

  fclose(fp);

  // End input parsing

  // Print input for debug

  for (int i = 0; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_VARIABLES + number_of_variables + 1; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }

  tableau(matrix, number_of_restrictions, number_of_variables);

  printf("---------------------------------------------------------------------"
         "--------\n");
  for (int i = 0; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_VARIABLES + number_of_variables + 1; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }

  return EXIT_SUCCESS;
}
