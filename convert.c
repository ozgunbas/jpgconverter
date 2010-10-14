#include <stdio.h> 
#include <math.h> 

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
  int apply_formula(int i,int j,int p[][8])
  {
    double pi=3.141592654;
    double ci = i==0 ? 1:(1/sqrt(2));
    double cj = j==0 ? 1:(1/sqrt(2));
    int x, y;
    double tot = 0;
    for(x=0; x < 8; x++)
      for(y=0; y < 8; y++)
	tot += p[x][y] * cos(((2*x+1)*i*pi)/16) * cos(((2*y+1)*j*pi)/16);
    return round(ci*cj*tot/4);
  }
  int ii,jj;
  for(ii=0; ii < 8; ii++)
    for(jj=0; jj < 8; jj++)
      transformed[ii][jj]=apply_formula(ii,jj,shifted);
  
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
  printf("\nTRANSFORMED\n");
  printMatrix(transformed);
  //  quantize(transformed,quantized);
  return 0;
  
}
