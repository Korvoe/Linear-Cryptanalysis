#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* convert_to_binary(int number, int *output);
int s_box(int input);

void main()
{
  // Read 10000 plaintext-ciphertext pairs from 2 files, which store them respectively.
  static int Plaintext[10000][4] = {0};
  static int Ciphertext[10000][4]  = {0};
  int counter1 = 0, counter2 = 0;
  char ch1, ch2;
  FILE *fp1, *fp2;
  fp1 = fopen("plaintext.hex", "rb");
  fp2 = fopen("ciphertext.hex", "rb");
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
  // linear cryptanalysis.
  int Subkey5[4] = {0};
  int result_subkey[4] = {0};
  int output[4] = {0};
  float count = 0;
  float bias = 0;
  for (int i = 0; i < 16; i++){
    for (int j = 0; j < 16; j++){
      for (int k = 0; k < 16; k++){
        for (int l = 0; l < 16; l++){
          for (int n = 0; n < 10000; n++){
            if((convert_to_binary(s_box(Subkey5[0]^Ciphertext[n][0]), output)[2] ^
                convert_to_binary(s_box(Subkey5[1]^Ciphertext[n][1]), output)[2] ^
                convert_to_binary(s_box(Subkey5[2]^Ciphertext[n][2]), output)[2] ^
                convert_to_binary(s_box(Subkey5[3]^Ciphertext[n][3]), output)[2] ^
                convert_to_binary(s_box(Subkey5[0]^Ciphertext[n][0]), output)[3] ^
                convert_to_binary(s_box(Subkey5[1]^Ciphertext[n][1]), output)[3] ^
                convert_to_binary(s_box(Subkey5[2]^Ciphertext[n][2]), output)[3] ^
                convert_to_binary(s_box(Subkey5[3]^Ciphertext[n][3]), output)[3] ^
                convert_to_binary(Plaintext[n][1], output)[0] ^
                convert_to_binary(Plaintext[n][1], output)[2] ^
                convert_to_binary(Plaintext[n][1], output)[3]) == 0)
               count++;

               if (bias != 0.5 && bias < (count - 5000)/10000){
                 bias = (count - 5000)/10000;
                 printf("%f ", bias);
                 result_subkey[0] = Subkey5[0];
                 result_subkey[1] = Subkey5[1];
                 result_subkey[2] = Subkey5[2];
                 result_subkey[3] = Subkey5[3];
                 if (bias < 0)
                   bias = -bias;
                 }
               }
          count = 0;
          Subkey5[3] = l;
        }
        Subkey5[2] = k;
      }
      Subkey5[1] = j;
    }
    Subkey5[0] = i;
  }
  for (int i = 0; i < 4; i++)
    printf("%X", result_subkey[i]); // Answer
}

int* convert_to_binary(int number, int *output)
{
  output[0] = number >= 8 ? 1 : 0;
  output[1] = (number >= 4 && number < 8) ||
                     (number >= 12) ? 1 : 0;
  output[2] = (4 > number && number >= 2) ||
              (8 > number && number >= 6) ||
              (12 > number && number >= 10) ||
              (number >= 14) ? 1 : 0;
  output[3] = number % 2 ? 1 : 0;
  return output;
}

int s_box(int input)
{
  switch(input)
  {
    case 0:
      return 0xe;
    case 1:
      return 0x4;
    case 2:
      return 0xd;
    case 3:
      return 1;
    case 4:
      return 2;
    case 5:
      return 0xf;
    case 6:
      return 0xb;
    case 7:
      return 8;
    case 8:
      return 3;
    case 9:
      return 0xa;
    case 0xa:
      return 6;
    case 0xb:
      return 0xc;
    case 0xc:
      return 5;
    case 0xd:
      return 9;
    case 0xe:
      return 0;
    case 0xf:
      return 7;
  }
}
