#include <stdio.h> 
#include <math.h> 

void read_input_to(int origin[][8])
{
  int i,j;
  for(i=0; i < 8; i++)
      for(j=0; j < 8; j++)
	scanf("%d",&origin[i][j]);
}
void quantize(int transformed[][8], int quantized[][8])
{
  int i,j;
  FILE *fp = fopen("Quantization-table.txt","r");
  int val;
  for(i=0; i < 8; i++)
    for(j=0; j < 8; j++)
      {
	fscanf(fp,"%d", &val);
	quantized[i][j]=myround(((double)transformed[i][j])/val);
      }
  fclose(fp);
}

void shift(int origin[][8], int shifted[][8])
{
 int i,j;
  for(i=0; i < 8; i++)
    for(j=0; j < 8; j++)
      shifted[i][j]=origin[i][j]-128;
}

int myround(double val)
{
  int result = val;
  if((val - result)>=0.5)
    result++;
  else if((val - result)<=-0.5)
    result--;
  return result;
}

void fdct(int shifted[][8], int transformed[][8])
{
  int apply_formula(int i,int j,int p[][8])
  {
    double pi=3.141592654;
    //    double ci = i==0 ? 1.0:(1.0/sqrt(2));
    //    double cj = j==0 ? 1.0:(1.0/sqrt(2));
    double ci = i==0 ? (1.0/sqrt(8)):(0.5);
    double cj = j==0 ? (1.0/sqrt(8)):(0.5);
    int x, y;
    double tot = 0;
    for(x=0; x < 8; x++)
      for(y=0; y < 8; y++)
	tot += p[x][y] * cos(((2*x+1)*i*pi)/16) * cos(((2*y+1)*j*pi)/16);
    return myround(ci*cj*tot);
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
void printArray(int a[64])
{
  int i;
  for(i=0; i < 64; i++)
    printf("%d ",a[i]);
  printf("\n");
}
void zigzag(int quantized[][8], int zz[64])
{
  int i=1, j=1;
  int k;
  for(k=0;k<64;k++)
    {
      zz[k]=quantized[i-1][j-1];
      if((i+j)%2==0)
	{
	  if(j<8)
	    j++;
	  else
	    i+=2;
	  if(i>1)
	    i--;
	}
      else
	{
	  if(i<8)
	    i++;
	  else
	    j+=2;
	  if(j>1)
	    j--;
	}
    }
}

int main()
{
  int original[8][8];
  int shifted[8][8];
  int transformed[8][8];
  int quantized[8][8];
  int zz[64];
  read_input_to(original);
  printf("\nORIGINAL\n");
  printMatrix(original);
  shift(original,shifted);
  printf("\nSHIFTED\n");
  printMatrix(shifted);
  fdct(shifted, transformed);
  printf("\nTRANSFORMED\n");
  printMatrix(transformed);
  quantize(transformed,quantized);
  printf("\nQUANTIZED\n");
  printMatrix(quantized);
  zigzag(quantized,zz);
  printf("\nZIGZAGED\n");
  printArray(zz);
  return 0;
  
}
