#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// One round of encryption
int* encryption_round(int *input, int *subkey, int *output);
int s_box(int input);
int* encryption(int *input, int **subkeys, int *output);

void main()
{
  FILE *fp1 = fopen("plaintext.txt", "w+");
  FILE *fp2 = fopen("ciphertext.txt", "w+");
  char value = 0;
  char str[5];
  // Key and subkey memory allocation.
  int* Key = malloc(20*sizeof(*Key));
  int **Subkeys = (int**)malloc(5*sizeof(*Subkeys));
    for (int i = 0; i < 5; i++)
      Subkeys[i] = (int*)malloc(4*sizeof(*Subkeys));

  // Plaintext and Ciphertext 2D arrays'(10000x4) memory allocation.
  int **Plaintext = (int**)malloc(10000*sizeof(*Plaintext));
    for (int i = 0; i < 10000; i++)
      Plaintext[i] = (int*)malloc(4*sizeof(*Plaintext));
  int **Ciphertext = (int**)malloc(10000*sizeof(*Ciphertext));
    for (int i = 0; i < 10000; i++)
      Ciphertext[i] = (int*)malloc(4*sizeof(*Ciphertext));

  // Filling the Plaintext with random numbers from 0 to 15.
  srand(time(NULL));
    for (int i = 0; i < 10000;  i++)
      for (int j = 0; j < 4; j++)
        Plaintext[i][j] = rand()%16;

  // Key and subkey generation.
  for (int i = 0; i < 0x14; i++)
  {
    Key[i] = i%0x10;
    if (i < 0x4)
      Subkeys[0][i%4] = i%0x10;
    else if (i >= 0x4 && i < 0x8)
      Subkeys[1][i%4] = Key[i];
    else if (i >= 0x8 && i < 0xc)
      Subkeys[2][i%4] = Key[i];
    else if (i >= 0xc && i < 0x10)
      Subkeys[3][i%4] = Key[i];
    else if (i >= 0x10)
      Subkeys[4][i%4] = Key[i];
  }

  // Encryption
  for (int i = 0; i < 10000; i++)
    Ciphertext[i] = encryption(Plaintext[i],  Subkeys, Ciphertext[i]);

  for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 4; j++){
        value = Plaintext[i][j];
        fwrite(&value, sizeof(value), 1, fp1);
        value = Ciphertext[i][j];
        fwrite(&value, sizeof(value), 1, fp2);
        printf("%X-%X ",Plaintext[i][j], Ciphertext[i][j]);
      }

  fclose(fp1);
  fclose(fp2);
}

int* encryption_round(int *input, int *subkey, int *output)
{
  // XOR
  for (int i = 0; i < 4; i++)
    output[i] = input[i]^subkey[i];

  // S-box
  for (int i =0; i < 4; i++)
    output[i] = s_box(output[i]);

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

int* encryption(int *input, int **subkeys, int *output)
{
  output = encryption_round(input, subkeys[0], output);
  output = encryption_round(output, subkeys[1], output);
  output = encryption_round(output, subkeys[2], output);
  output = encryption_round(output, subkeys[3], output);
  // Last XOR
  for (int i = 0; i < 4; i++)
    output[i] = output[i]^subkeys[4][i];

  return output;
}
