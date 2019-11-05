#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
  char ch1, ch2;
  FILE *fp1, *fp2;
  static int Plaintext[10000][4] = {0};
  static int Ciphertext[10000][4]  = {0};
  int counter1 = 0, counter2 = 0;

  fp1 = fopen("plaintext.txt", "rb");
  fp2 = fopen("ciphertext.txt", "rb");
  while( ((ch1 = fgetc(fp1)) != EOF) &&
         ((ch2 = fgetc(fp2)) != EOF)){
    Plaintext[counter1][counter2] = (int)ch1;
    Ciphertext[counter1][counter2] = (int)ch2;
    counter2++;
    if (counter2 == 4){
      counter2 = 0;
      counter1++;
    }
  }

  for (int i = 0; i < 10000; i++)
    for (int j = 0; j < 4; j++)
      printf("%X-%X ", Plaintext[i][j], Ciphertext[i][j]);
}
