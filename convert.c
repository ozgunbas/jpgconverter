#include <stdio.h>

void read_input_to(int origin[][8])
{
  int i,j;
  for(i=0; i < 8; i++)
      for(j=0; j < 8; j++)
	scanf("%d",&origin[i][j]);
}

void shift(int origin[][8], int shifted[][8])
{
 int i,j;
  for(i=0; i < 8; i++)
    for(j=0; j < 8; j++)
      shifted[i][j]=origin[i][j]-128;
}

void fdct(int shifted[][8], int transformed[][8])
{
  int i,j;
  double pi=3.141592654;
  for(i=0; i < 8; i++)
    for(j=0; j < 8; j++)
      shifted[i][j]=origin[i][j]-128;
}

void printMatrix(int a[][8])
{
  int i,j;
  for(i=0; i < 8; i++)
    {
      for(j=0; j < 8; j++)
	printf("%d ",a[i][j]);
      printf("\n");
    }
}

int main()
{
  int original[8][8];
  int shifted[8][8];
  int transformed[8][8];
  int quantized[8][8];
  read_input_to(original);
  printf("\nORIGINAL\n");
  printMatrix(original);
  shift(original,shifted);
  printf("\nSHIFTED\n");
  printMatrix(shifted);
  fdct(shifted, transformed);
  //  quantize(transformed,quantized);
  return 0;
  
}
