#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// One round of encryption
int* encryption_round(int *input, int *subkey, int *output);
int s_box(int input);
int* permutation(int *input);
int* encryption(int *input, int **subkeys, int *output);
int* convert_to_binary(int number, int* output);

void main()
{
  FILE *fp1 = fopen("plaintext.hex", "w+");
  FILE *fp2 = fopen("ciphertext.hex", "w+");
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
  for (int i = 0; i < 4; i++)
    output[i] = s_box(output[i]);

  // Permutation
  output = permutation(output);
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

int* permutation(int *input)
{
  int binary[4] = {0};
  int block[16] = {0};
  int new_block[16] = {0};

  for (int i = 0; i < 4; i++){
    block[i]    = convert_to_binary(input[0], binary)[i];
    block[i+4]  = convert_to_binary(input[1], binary)[i];
    block[i+8]  = convert_to_binary(input[2], binary)[i];
    block[i+12] = convert_to_binary(input[3], binary)[i];
  }

  new_block[0]  = block[0];
  new_block[1]  = block[4];
  new_block[2]  = block[8];
  new_block[3]  = block[12];
  new_block[4]  = block[1];
  new_block[5]  = block[5];
  new_block[6]  = block[9];
  new_block[7]  = block[13];
  new_block[8]  = block[2];
  new_block[9]  = block[6];
  new_block[10] = block[10];
  new_block[11] = block[14];
  new_block[12] = block[3];
  new_block[13] = block[7];
  new_block[14] = block[11];
  new_block[15] = block[15];

  for (int i = 0; i < 4; i++)
    input[i] = new_block[i*4]*8 +
               new_block[i*4+1]*4 +
               new_block[i*4+2]*2 +
               new_block[i*4+3];

  return input;
}

int* encryption(int *input, int **subkeys, int *output)
{
  output = encryption_round(input, subkeys[0], output);
  output = encryption_round(output, subkeys[1], output);
  output = encryption_round(output, subkeys[2], output);
  // round 4
  // round 4 XOR
  for (int i = 0; i < 4; i++)
    output[i] = output[i]^subkeys[4][i];
  // round 4 Substitution
  for (int i = 0; i < 4; i++)
    output[i] = s_box(output[i]);
  // Last XOR
  for (int i = 0; i < 4; i++)
    output[i] = output[i]^subkeys[4][i];
  return output;
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
