#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_NUMBER_OF_RESTRICTIONS 100
#define MAX_NUMBER_OF_VARIABLES 100

void dbg(const char *msg) { fprintf(stdout, "%s", msg); }

void print_matrix(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                  MAX_NUMBER_OF_RESTRICTIONS + 1],
    int number_of_restrictions, int number_of_variables) {
  for (int i = 0; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + 1;
         j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
}

void err_n_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

/* Assume que já existe uma base viável canônica na matriz.
Assume-se que essa base já tem identidade e que b >= 0 */
void tableau(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                  MAX_NUMBER_OF_RESTRICTIONS + 1],
    int number_of_restrictions, int number_of_variables,
    int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
             MAX_NUMBER_OF_RESTRICTIONS]) {

  // reverte sinais de ct
  for (int i = MAX_NUMBER_OF_RESTRICTIONS;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions;
       i++) {
    matrix[0][i] *= -1;
  }

  for (;;) {
    int col = -1;
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions;
         i++) {
      if (matrix[0][i] < 0 && base[i] == 0) {
        col = i;
        break;
      }
    }

    for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
      printf("%d ", base[i]);
    }
    printf("\n");
    if (col == -1) {
      print_matrix(matrix, number_of_restrictions, number_of_variables);
      fprintf(stdout, "otimo\n");
      fprintf(stdout, "%lf\n",
              matrix[0][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                        number_of_restrictions]);
      double solucao[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS] = {
          0};
      for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
        if (base[i] == 1) {
          int linha_com_1;
          for (int j = 1; j < number_of_restrictions + 1; j++) {
            if (j == 1) {
              linha_com_1 = j;
              break;
            }
          }

          solucao[i] =
              matrix[linha_com_1][MAX_NUMBER_OF_RESTRICTIONS +
                                  number_of_variables + number_of_restrictions];
        }
      }
      for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
        if (i == number_of_variables + number_of_restrictions - 1) {
          fprintf(stdout, "%lf\n", solucao[i]);
        } else {
          fprintf(stdout, "%lf ", solucao[i]);
        }
      }

      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_RESTRICTIONS; j++) {
        if (j == MAX_NUMBER_OF_RESTRICTIONS - 1) {
          fprintf(stdout, "%lf\n", matrix[0][j]);
        } else {
          fprintf(stdout, "%lf ", matrix[0][j]);
        }
      }
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
      fprintf(stdout, "ILIMITADA\n");
      return;
    }

    /* Atualiza a base */
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions;
         i++) {
      if (base[i] == 1 && min_ratio_line == 1 && col != i) {
        base[i] = 0;
        break;
      }
    }
    base[col] = 1;

    // pivoteia linha
    for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         i < MAX_NUMBER_OF_VARIABLES + number_of_variables +
                 number_of_restrictions + 1;
         i++) {
      matrix[min_ratio_line][i] /= matrix[min_ratio_line][col];
    }

    // zera resto da coluna
    for (int i = 0; i < number_of_restrictions + 1; i++) {
      if (i == min_ratio_line)
        continue;
      double curr_ratio = matrix[i][col] / matrix[min_ratio_line][col];

      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_VARIABLES + number_of_variables +
                   number_of_restrictions + 1;
           j++) {
        matrix[i][j] -= matrix[min_ratio_line][j] * curr_ratio;
      }
    }
  }
}

void auxiliar(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                  MAX_NUMBER_OF_RESTRICTIONS + 1],
    int number_of_restrictions, int number_of_variables,
    int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
             MAX_NUMBER_OF_RESTRICTIONS]) {

  int matrix_auxiliar[MAX_NUMBER_OF_RESTRICTIONS + 1]
                     [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                      MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_RESTRICTIONS +
                      1] = {0};
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                 number_of_variables + number_of_restrictions;
         j++) {
      matrix_auxiliar[i][j] = matrix[i][j];
    }
  }

  // popula ct com valores da auxiliar ja invertidos pro tableau
  for (int i = MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions + number_of_restrictions;
       i++) {
    matrix_auxiliar[0][i] = -1;
  }

  for (;;) {
    int col = -1;
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + number_of_restrictions;
         i++) {
      if (matrix_auxiliar[0][i] < 0 && base[i] == 0) {
        col = i;
        break;
      }
    }

    if (col == -1) {
      fprintf(stdout, "ÓTIMO\n");
      return;
    }

    // tenho q achar linha do melhor ratio
    double ratio = -1;
    int min_ratio_line = -1;
    for (int i = 1; i < number_of_restrictions + 1; i++) {
      if (matrix_auxiliar[i][col] > 0) {
        double new_ratio = matrix_auxiliar[i][number_of_variables + 1] /
                           matrix_auxiliar[i][col];
        ratio = ratio != -1 && ratio <= new_ratio ? ratio : new_ratio;
        min_ratio_line = i;
      }
    }

    if (ratio == -1) {
      fprintf(stdout, "ILIMITADA\n");
      return;
    }

    /* Atualiza a base */
    base[col] = 1;
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + number_of_restrictions;
         i++) {
      if (base[i] == 1 && min_ratio_line == 1 && col != i) {
        base[i] = 0;
        break;
      }
    }

    // pivoteia linha
    for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         i < MAX_NUMBER_OF_VARIABLES + number_of_variables +
                 number_of_restrictions + number_of_restrictions + 1;
         i++) {
      matrix_auxiliar[min_ratio_line][i] /=
          matrix_auxiliar[min_ratio_line][col];
    }

    // zera resto da coluna
    for (int i = 0; i < number_of_restrictions + 1; i++) {
      if (i == min_ratio_line)
        continue;
      double curr_ratio =
          matrix_auxiliar[i][col] / matrix_auxiliar[min_ratio_line][col];

      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_VARIABLES + number_of_variables +
                   number_of_restrictions + number_of_restrictions + 1;
           j++) {
        matrix_auxiliar[i][j] -=
            matrix_auxiliar[min_ratio_line][j] * curr_ratio;
      }
    }
  }
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

  /* A matriz possui tamanho dobrado para armazenar o registro de operações à
   * esquerda */
  double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
               [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                MAX_NUMBER_OF_RESTRICTIONS + 1] = {0};

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

  // Reading each restriction, populating looseness variables and reading b
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
      fprintf(stdout, "%d\n", i);
      err_n_die("Error using fread on line of matrix.\n");
    }

    matrix[i][MAX_NUMBER_OF_RESTRICTIONS] = (double)atoi(strtok(buffer, " "));
    for (int j = 1; j < number_of_variables; j++) {
      matrix[i][MAX_NUMBER_OF_RESTRICTIONS + j] =
          (double)atoi(strtok(NULL, " "));
    }
    matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables + i - 1] =
        1; /* Setta 1 na variável de folga relativa */
    matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
              number_of_restrictions] =
        (double)atoi(strtok(NULL, " ")); /* Setta b */
  }

  fclose(fp);

  /* Programação linear lida do arquivo */

  print_matrix(matrix, number_of_restrictions, number_of_variables);

  /* Checa se alguma entrada no vetor b é negativa.Se sim,
      devemos colocar as folgas,
      montar a PL auxiliar para descobrir uma base viável. */

  int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
           MAX_NUMBER_OF_RESTRICTIONS] = {0};

  int neg_b = -1;
  for (int i = 1; i < MAX_NUMBER_OF_RESTRICTIONS + 1; i++) {
    if (matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                  number_of_restrictions + 1] < 0) {
      neg_b = i;
      break;
    }
  }

  if (neg_b == -1) {
    for (int i = number_of_variables;
         i < number_of_variables + number_of_restrictions; i++) {
      base[i] = 1;
    }
  } else {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + 1;
         j++) {
      matrix[neg_b][j] *= -1;
    }
    for (int i = number_of_variables + number_of_restrictions;
         i <
         number_of_variables + number_of_restrictions + number_of_restrictions;
         i++) {
      base[i] = 1;
    }
    auxiliar(matrix, number_of_restrictions, number_of_variables, base);
  }

  tableau(matrix, number_of_restrictions, number_of_variables, base);

  return EXIT_SUCCESS;
}
