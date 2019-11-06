#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
  // Read 10000 plaintext-ciphertext pairs from 2 files, which store them respectively.
  static int Plaintext[10000][4] = {0};
  static int Ciphertext[10000][4]  = {0};
  int counter1 = 0, counter2 = 0;
  char ch1, ch2;
  FILE *fp1, *fp2;
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

}
