#include <math.h>
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
      printf("%+06.2lf ", matrix[i][j]);
    }
    printf("\n");
  }
}

void err_n_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

/* Salvo ela globalmente caso seja ilimitada no final das contas */
double solucao_viavel[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS] = {
    0};

/* Assume que já existe uma base viável canônica na matriz.
Assume-se que essa base já tem identidade e que b >= 0 */
void tableau(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                  MAX_NUMBER_OF_RESTRICTIONS + 1],
    int number_of_restrictions, int number_of_variables,
    int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
             MAX_NUMBER_OF_RESTRICTIONS]) {

  /* // reverte sinais de ct
  for (int i = MAX_NUMBER_OF_RESTRICTIONS;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions;
       i++) {
    matrix[0][i] *= -1;
  } */

  for (;;) {

    print_matrix(matrix, number_of_restrictions, number_of_variables);
    printf("-------------------------------------------------------------------"
           "-------------------\n");

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

    /* Achamos o ótimo */
    if (col == -1) {

      fprintf(stdout, "otima\n");
      fprintf(stdout, "%lf\n",
              matrix[0][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                        number_of_restrictions]);
      double solucao[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS] = {
          0};
      for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
        if (base[i] == 1) {
          int linha_com_1;
          for (int j = 1; j < number_of_restrictions + 1; j++) {
            if (matrix[j][MAX_NUMBER_OF_RESTRICTIONS + i] == 1) {
              linha_com_1 = j;
              break;
            }
          }

          solucao[i] =
              matrix[linha_com_1][MAX_NUMBER_OF_RESTRICTIONS +
                                  number_of_variables + number_of_restrictions];
        }
      }
      for (int i = 0; i < number_of_variables; i++) {
        if (i == number_of_variables - 1) {
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
        double new_ratio =
            matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                      number_of_restrictions] /
            matrix[i][col];
        if (ratio != -1 && ratio <= new_ratio) {
          continue;
        } else {
          ratio = new_ratio;
          min_ratio_line = i;
        }
      }
    }

    if (ratio == -1) {
      fprintf(stdout, "ilimitada\n");

      double certificado_ilimitada[MAX_NUMBER_OF_VARIABLES +
                                   MAX_NUMBER_OF_RESTRICTIONS] = {0};

      certificado_ilimitada[col - MAX_NUMBER_OF_RESTRICTIONS] = 1;

      for (int i = 0; i < number_of_variables; i++) {
        if (base[i] == 1) {
          int linha_com_1 = -1;
          ;
          for (int j = 1; j < number_of_restrictions + 1; j++) {
            if (fabs(matrix[j][MAX_NUMBER_OF_RESTRICTIONS + i] - 1) <= 10e-4) {
              linha_com_1 = j;
              break;
            }
          }
          if (linha_com_1 == -1) {
            printf(
                "Erro ao pegar certificado de ilimitada. linha_com_1 == -1\n");
            exit(0);
          }
          certificado_ilimitada[i] = -1 * matrix[linha_com_1][col];

          solucao_viavel[i] =
              matrix[linha_com_1][MAX_NUMBER_OF_RESTRICTIONS +
                                  number_of_restrictions + number_of_variables];
        }
      }
      for (int i = 0; i < number_of_variables; i++) {
        if (i == number_of_variables - 1) {
          fprintf(stdout, "%lf\n", solucao_viavel[i]);
        } else {
          fprintf(stdout, "%lf ", solucao_viavel[i]);
        }
      }

      for (int i = 0; i < number_of_variables; i++) {
        if (i == number_of_variables - 1) {
          fprintf(stdout, "%lf\n", certificado_ilimitada[i]);
        } else {
          fprintf(stdout, "%lf ", certificado_ilimitada[i]);
        }
      }

      return;
    }

    /* Atualiza a base */
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions;
         i++) {
      if (base[i - MAX_NUMBER_OF_RESTRICTIONS] == 1 &&
          matrix[min_ratio_line][i] == 1.0 && col != i) {
        base[i - MAX_NUMBER_OF_RESTRICTIONS] = 0;
        break;
      }
    }
    base[col - MAX_NUMBER_OF_RESTRICTIONS] = 1;

    // pivoteia linha
    double ratio_to_divide = matrix[min_ratio_line][col];
    for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         i < MAX_NUMBER_OF_VARIABLES + number_of_variables +
                 number_of_restrictions + 1;
         i++) {
      matrix[min_ratio_line][i] /= ratio_to_divide;
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

int auxiliar(
    double matrix[MAX_NUMBER_OF_RESTRICTIONS + 1]
                 [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                  MAX_NUMBER_OF_RESTRICTIONS + 1],
    int number_of_restrictions, int number_of_variables,
    int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
             MAX_NUMBER_OF_RESTRICTIONS]) {

  double matrix_auxiliar[MAX_NUMBER_OF_RESTRICTIONS + 1]
                        [MAX_NUMBER_OF_RESTRICTIONS + MAX_NUMBER_OF_VARIABLES +
                         MAX_NUMBER_OF_RESTRICTIONS +
                         MAX_NUMBER_OF_RESTRICTIONS + 1] = {0};
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                 number_of_variables;
         j++) {
      matrix_auxiliar[i][j] = matrix[i][j];
    }
  }

  // PREENCHE IDENTIDADE DA BASE NOVA
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    matrix_auxiliar[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                       number_of_restrictions + i - 1] = 1;
  }

  /* preenche coluna de b */
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    matrix_auxiliar[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                       number_of_restrictions + number_of_restrictions] =
        matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                  number_of_restrictions];
  }

  // popula ct com valores da auxiliar ja invertidos pro tableau
  for (int i = MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions + number_of_restrictions;
       i++) {
    matrix_auxiliar[0][i] = 1;
  }

  // onde b é neg multiplica linha na auxiliar
  for (int i = 1; i < number_of_restrictions + 1; i++) {

    if (matrix_auxiliar[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                           number_of_restrictions + number_of_restrictions] <
        0) {
      int neg_b_line = i;
      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                   number_of_restrictions;
           j++) {
        matrix_auxiliar[neg_b_line][j] *= -1;
      }
      matrix_auxiliar[neg_b_line][MAX_NUMBER_OF_RESTRICTIONS +
                                  number_of_variables + number_of_restrictions +
                                  number_of_restrictions] *= -1;
    }
  }

  // torna base viavel zerando em c
  for (int i = 1; i < number_of_restrictions + 1; i++) {
    for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + number_of_restrictions + 1;
         j++) {
      matrix_auxiliar[0][j] -= matrix_auxiliar[i][j];
    }
  }

  for (;;) {

    for (int i = 0; i < number_of_restrictions + 1; i++) {
      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                   2 * number_of_restrictions + 1;
           j++) {
        printf("%+06.2lf ", matrix_auxiliar[i][j]);
      }
      printf("\n");
    }
    printf("-------------------------------------------------------------------"
           "---------------------------\n");

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

    if (col == -1) { // auxiliar achou otimo, checar dois casos
      /* Aqui eu dou round pq erro numerico pode atrapalhar a checagem */
      if (round(
              matrix_auxiliar[0][MAX_NUMBER_OF_RESTRICTIONS +
                                 number_of_variables + number_of_restrictions +
                                 number_of_restrictions]) == 0.0) {

        for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
          if (base[i] == 1) {
            int linha_com_1 = -1;
            for (int j = 1; j < number_of_restrictions + 1; j++) {
              if (matrix_auxiliar[j][MAX_NUMBER_OF_RESTRICTIONS + i] == 1) {
                linha_com_1 = j;
                break;
              }
            }
            if (linha_com_1 == -1) {
              printf("Erro ao achar linha_com_1 na hora de tornar base em "
                     "viavel\n");
              exit(0);
            }
          }
        }

        /* Se achamos base viável, precisamos pivotear a matrix original para
         * que a nova base tenha custos zero e elas sejam identidade */

        for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
          printf("%d ", base[i]);
        }
        printf("\n");

        double result[MAX_NUMBER_OF_RESTRICTIONS][MAX_NUMBER_OF_RESTRICTIONS +
                                                  MAX_NUMBER_OF_VARIABLES + 1] =
            {0};

        for (int i = 1; i < number_of_restrictions + 1; i++) {
          for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
               j < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                       number_of_variables + 1;
               j++) {
            for (int k = 1; k < number_of_restrictions + 1; k++) {
              result[i][j] +=
                  matrix_auxiliar[i][k - 1 + MAX_NUMBER_OF_RESTRICTIONS -
                                     number_of_restrictions] *
                  matrix[k][j];
            }
          }
        }

        for (int i = 1; i < number_of_restrictions + 1; i++) {
          for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
               j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                       number_of_restrictions + 1;
               j++) {
            matrix[i][j] = result[i][j];
          }
        }

        for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
          if (base[i] == 1) {
            int linha_com_1 = -1;

            for (int j = 1; j < number_of_restrictions + 1; j++) {
              if (fabs(matrix[j][i + MAX_NUMBER_OF_RESTRICTIONS] - 1) <=
                  10e-4) {
                linha_com_1 = j;
                break;
              }
            }
            if (linha_com_1 == -1) {
              printf(
                  "linha_com_1 igual a -1, nao achei a base do jeito certo\n");
              exit(0);
            }

            double value_to_multiply =
                matrix[0][i + MAX_NUMBER_OF_RESTRICTIONS];
            for (int k = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
                 k < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                         number_of_variables + 1;
                 k++) {
              matrix[0][k] =
                  matrix[0][k] - value_to_multiply * matrix[linha_com_1][k];
            }
          }
        }

        return 1;
      } else {

        fprintf(stdout, "inviavel\n");
        for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
             i < MAX_NUMBER_OF_RESTRICTIONS; i++) {
          if (i == MAX_NUMBER_OF_RESTRICTIONS - 1) {
            fprintf(stdout, "%lf\n", matrix_auxiliar[0][i]);
          } else {
            fprintf(stdout, "%lf ", matrix_auxiliar[0][i]);
          }
        }
        return 0;
      }
    }

    // tenho q achar linha do melhor ratio
    double ratio = -1;
    int min_ratio_line = -1;
    for (int i = 1; i < number_of_restrictions + 1; i++) {
      if (matrix_auxiliar[i][col] > 0) {
        double new_ratio =
            matrix_auxiliar[i]
                           [MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                            number_of_restrictions + number_of_restrictions] /
            matrix_auxiliar[i][col];
        if (ratio != -1 && ratio <= new_ratio) {
          continue;
        } else {
          ratio = new_ratio;
          min_ratio_line = i;
        }
      }
    }

    if (ratio == -1) { // nunca vai cair pq auxiliar é limitada
      fprintf(stdout, "Auxiliar deu ilimitada, algo esta errado\n");

      // logica de multiplciar plea inversa e zerar ct nas bases repetida do
      // caso anterior

      double result[MAX_NUMBER_OF_RESTRICTIONS][MAX_NUMBER_OF_RESTRICTIONS +
                                                MAX_NUMBER_OF_VARIABLES + 1] = {
          0};

      for (int i = 1; i < number_of_restrictions + 1; i++) {
        for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
             j < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                     number_of_variables + 1;
             j++) {
          for (int k = 1; k < number_of_restrictions + 1; k++) {
            result[i][j] +=
                matrix_auxiliar[i][k - 1 + MAX_NUMBER_OF_RESTRICTIONS -
                                   number_of_restrictions] *
                matrix[k][j];
          }
        }
      }

      for (int i = 1; i < number_of_restrictions + 1; i++) {
        for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
             j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                     number_of_restrictions + 1;
             j++) {
          matrix[i][j] = result[i][j];
        }
      }

      for (int i = 0; i < number_of_variables + number_of_restrictions; i++) {
        if (base[i] == 1) {
          int linha_com_1 = -1;

          for (int j = 1; j < number_of_restrictions + 1; j++) {
            if (fabs(matrix[j][i + MAX_NUMBER_OF_RESTRICTIONS] - 1) <= 10e-4) {
              linha_com_1 = j;
              break;
            }
          }
          if (linha_com_1 == -1) {
            printf("linha_com_1 igual a -1, nao achei a base do jeito certo\n");
            exit(0);
          }

          double value_to_multiply = matrix[0][i + MAX_NUMBER_OF_RESTRICTIONS];
          for (int k = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
               k < MAX_NUMBER_OF_RESTRICTIONS + number_of_restrictions +
                       number_of_variables + 1;
               k++) {
            matrix[0][k] =
                matrix[0][k] - value_to_multiply * matrix[linha_com_1][k];
          }
        }
      }

      return 1; // retorno 1 pq nao vai importat mt, em tese so cai aqui cm erro
                // numerico desconsideravel em c ~ 0
      exit(EXIT_SUCCESS);
    }

    /* Atualiza a base */
    for (int i = MAX_NUMBER_OF_RESTRICTIONS;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + number_of_restrictions;
         i++) {
      if (base[i - MAX_NUMBER_OF_RESTRICTIONS] == 1 &&
          matrix_auxiliar[min_ratio_line][i] == 1.0 && col != i) {
        base[i - MAX_NUMBER_OF_RESTRICTIONS] = 0;
        break;
      }
    }
    base[col - MAX_NUMBER_OF_RESTRICTIONS] = 1;

    double ratio_to_divide = matrix_auxiliar[min_ratio_line][col];
    // pivoteia linha
    for (int i = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
         i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                 number_of_restrictions + number_of_restrictions + 1;
         i++) {

      matrix_auxiliar[min_ratio_line][i] /= ratio_to_divide;
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

  /* Checa se alguma entrada no vetor b é negativa.Se sim,
      devemos colocar as folgas,
      montar a PL auxiliar para descobrir uma base viável. */

  // reverte sinais de ct ja
  for (int i = MAX_NUMBER_OF_RESTRICTIONS;
       i < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
               number_of_restrictions;
       i++) {
    matrix[0][i] *= -1;
  }

  int base[MAX_NUMBER_OF_VARIABLES + MAX_NUMBER_OF_RESTRICTIONS +
           MAX_NUMBER_OF_RESTRICTIONS] = {0};

  int neg_b = 0;
  for (int i = 1; i < number_of_restrictions + 1; i++) {

    if (matrix[i][MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                  number_of_restrictions] < 0) {
      neg_b++;
      /* neg_b_line = i;
      for (int j = MAX_NUMBER_OF_RESTRICTIONS - number_of_restrictions;
           j < MAX_NUMBER_OF_RESTRICTIONS + number_of_variables +
                   number_of_restrictions + 1;
           j++) {
        matrix[neg_b_line][j] *= -1;
      } */
    }
  }

  int aux_res = -1;

  if (neg_b == 0) {
    for (int i = number_of_variables;
         i < number_of_variables + number_of_restrictions; i++) {
      base[i] = 1;
    }
  } else {
    for (int i = number_of_variables + number_of_restrictions;
         i <
         number_of_variables + number_of_restrictions + number_of_restrictions;
         i++) {
      base[i] = 1;
    }

    // multiplica por -1
    aux_res =
        auxiliar(matrix, number_of_restrictions, number_of_variables, base);
  }

  if (aux_res != 0) {
    tableau(matrix, number_of_restrictions, number_of_variables, base);
  }

  return EXIT_SUCCESS;
}
