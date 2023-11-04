#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_NUMBER_OF_RESTRICTIONS 100
#define MAX_NUMBER_OF_VARIABLES 100

void dbg(const char *msg) { fprintf(stdout, "%s", msg); }

void err_n_die(const char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

/* Assume que já existe uma base viável canônica na matriz.
Assume-se que essa base já tem identidade e que b >= 0 */
double tableau(double matrix[MAX_NUMBER_OF_RESTRICTIONS][MAX_NUMBER_OF_RESTRICTIONS], int number_of_restrictions, int number_of_variables)
{

  char base[number_of_variables] = {0};
  matrix[0][number_of_variables] = 0; // posição em que resultado sera armazenado. Garantios que começa com 0

  // reverte sinais de ct
  for (int i = 0; i < number_of_variables; i++)
  {
    matrix[0][i] *= -1;
  }

  for (;;)
  {
    int col = -1;
    for (int i = 0; i < number_of_variables; i++)
    {
      if (matrix[0][i] < 0)
      {
        col = i;
        break;
      }
    }

    if (col == -1)
    {
      fprintf(stdout, "acabou-se\n");
      return;
    }

    // tenho q achar linha do melhor ratio
    double ratio = -1;
    for (int i = 0; i < number_of_restrictions; i++)
    {
      ratio = matrix[i][col] <= 0 ? ratio : min(ratio, matrix[i][number_of_variables + 1] / matrix[i][col]);
    }

    if (ratio == -1)
    {
      fprintf(stdout, "inviavel\n");
      return;
    }


  }
}

int main(int argc, char **argv)
{

  if (argc != 2)
  {
    fprintf(stdout, "usage: simplex [input_file_path]");
    exit(EXIT_FAILURE);
  }

  /*   A coluna B SEMPRE será a coluna 'number_of_variables' da matriz. As
   * outras colunas após isso são lixo
   */
  int number_of_restrictions, number_of_variables;
  double matrix[MAX_NUMBER_OF_RESTRICTIONS][MAX_NUMBER_OF_VARIABLES + 1];

  // Parse input

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    err_n_die("Error opening input file.\n");
  }

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  if (fgets(buffer, sizeof(buffer), fp) == NULL)
  {
    err_n_die("Error using fread on first line on input.\n");
  }

  number_of_restrictions = atoi(strtok(buffer, " "));
  number_of_variables = atoi(strtok(NULL, " "));

  memset(buffer, 0, BUFFER_SIZE);
  if (fgets(buffer, sizeof(buffer), fp) == NULL)
  {
    err_n_die("Error using fread on second line on input.\n");
  }

  matrix[0][0] = (double)atoi(strtok(buffer, " "));
  for (int i = 1; i < number_of_variables; i++)
  {
    matrix[0][i] = (double)atoi(strtok(NULL, " "));
  }

  for (int i = 1; i < number_of_restrictions + 1; i++)
  {
    memset(buffer, 0, BUFFER_SIZE);
    if (fgets(buffer, sizeof(buffer), fp) == NULL)
    {
      fprintf(stdout, "%d\n", i);
      err_n_die("Error using fread on line of matrix.\n");
    }

    matrix[i][0] = (double)atoi(strtok(buffer, " "));
    for (int j = 1; j < number_of_variables + 1; j++)
    {
      matrix[i][j] = (double)atoi(strtok(NULL, " "));
    }
  }

  fclose(fp);

  // End input parsing

  // Print input for debug

  for (int i = 0; i < number_of_restrictions + 1; i++)
  {
    for (int j = 0; j < number_of_variables + 1; j++)
    {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }

  return EXIT_SUCCESS;
}
