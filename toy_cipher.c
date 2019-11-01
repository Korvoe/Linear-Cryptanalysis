#include <stdio.h>
#include <stdlib.h>

int s_box(int input);
int* encryption_round(int *input, int *subkey);

void main()
{
  int* Key = malloc(20*sizeof(*Key));
  int* Plaintext = malloc(4*sizeof(*Plaintext));
  int* Ciphertext = malloc(4*sizeof(*Ciphertext));
  int* K1 = malloc(4*sizeof(*K1));
  int* K2 = malloc(4*sizeof(*K2));
  int* K3 = malloc(4*sizeof(*K3));
  int* K4 = malloc(4*sizeof(*K4));
  int* K5 = malloc(4*sizeof(*K5));

  for (int i = 0; i < 0x14; i++)
  {
    Key[i] = i%0x10;
    if (i < 0x4)
      K1[i%4] = i%0x10;
    else if (i >= 0x4 && i < 0x8)
      K2[i%4] = Key[i];
    else if (i >= 0x8 && i < 0xc)
      K3[i%4] = Key[i];
    else if (i >= 0xc && i < 0x10)
      K4[i%4] = Key[i];
    else if (i >= 0x10)
      K5[i%4] = Key[i];
  }
  //Encryption
  Ciphertext = encryption_round(Plaintext, K1);
  Ciphertext = encryption_round(Plaintext, K2);
  Ciphertext = encryption_round(Plaintext, K3);
  Ciphertext = encryption_round(Plaintext, K4);
  for (int i = 0; i < 4; i++)
    Ciphertext[i] = Ciphertext[i]^K5[i];
  for (int i = 0; i < 4; i++)
    printf("%d ", Ciphertext[i]);
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



int* encryption_round(int *input, int *subkey)
{
  int* output = malloc(4*sizeof(output));
  for (int i = 0; i < 4; i++)
    output[i] = input[i]^subkey[i];

  for (int i =0; i < 4; i++)
    output[i] = s_box(output[i]);

  return output;
}
