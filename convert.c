#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <string.h>

int myround(double val);
void read_input_to(int origin[][8]);
void quantize(int transformed[][8], int quantized[][8]);
void shift(int origin[][8], int shifted[][8]);
void fdct(int shifted[][8], int transformed[][8]);
void zigzag(int quantized[][8], int *zz);
int intermediate(int *zz, int *intsym);
void printMatrix(int a[][8],char*);
void printArray(int *a,int n,char*);
double log2(double x);

double log2(double x)
{
  return log(x)/log(2);
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


void fdct(int shifted[][8], int transformed[][8])
{
  int apply_formula(int i,int j,int p[][8])
  {
    double pi=3.141592654;
    double ci = i==0 ? (1.0/sqrt(2)):(1);
    double cj = j==0 ? (1.0/sqrt(2)):(1);
    int x, y;
    double tot = 0;
    for(x=0; x < 8; x++)
      for(y=0; y < 8; y++)
	tot += p[x][y] * cos(((2*x+1)*i*pi)/16) * cos(((2*y+1)*j*pi)/16);
    return myround(0.25*ci*cj*tot);
  }
  int ii,jj;
  for(ii=0; ii < 8; ii++)
    for(jj=0; jj < 8; jj++)
      transformed[ii][jj]=apply_formula(ii,jj,shifted);
  
}

void zigzag(int quantized[][8], int *zz)
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
int intermediate(int *zz, int *intsym)
{
  int i;
  int sss;
  int current = 2;
  int zeroes;
  //No 0 skip for the firs element
  intsym[0] = log2(abs(zz[0]))+1;
  intsym[1] = zz[0];
  for(i=1;i<64;i++)
    {
      zeroes = 0;
      while((zz[i]==0) && (i<64))
	{
	  zeroes++;i++;
	}
      if(i==64)
	break;
      sss=log2(abs(zz[i]))+1;
      intsym[current++]=zeroes;
      intsym[current++]=sss;
      intsym[current++]=zz[i];
    }
  //Adding 00 as the end of code sequence
  intsym[current++]=0;
  intsym[current++]=0;
  return current;
}
int encode(int *intsym, char fin[][19])
{
  char hufftab[11][10];
  char codetab[16][11][19];
  int codeLength = 2;
  int i = 2;
  void read_huff(char hufftab[11][10])
  {
    FILE *fp = fopen("Default_huffman_codewords.txt","r");
    int i,j;
    int foo;
    char fooc[10];
    for(i=0; i < 11; i++)
      {
	fscanf(fp,"%d%s",&foo,hufftab[i]);
      }
    fclose(fp);
    fp = fopen("Huffman.txt","r");
    for(i=0;i<16;i++)
      for(j=0;j<11;j++)
	{
	  if((j!=0) || (i==0) || (i==15))
	    {
	      fscanf(fp,"%s%s",fooc,codetab[i][j]);
	    }
	}
    fclose(fp);
  }
  void find_huff(int x, char *target)
  {
    strcpy(target,hufftab[x]);
  }
  
  void convert_to_bin(int x, int sss, char *target)
  {
    if(sss != 0)
      {
	convert_to_bin(x>>1, sss - 1, target);
	*(target+sss-1)=(char)(((int)'0')+(x&0x01));
      }
      
  }
  void to_bin(int x, int sss, char *target)
  {
    *(target+sss) = '\0';
    if(x>=0)
      convert_to_bin(x, sss, target);
    else
      {
	int mask = pow(2,sss)-1;
	x=abs(x);
	x=~x;
	x=x&mask;
	convert_to_bin(x , sss, target);
      }
  }
  void find_code(int skip,int sss, char *target)
  {
    strcpy(target,codetab[skip][sss]);
  }
  
  read_huff(hufftab);
  find_huff(intsym[0],fin[0]);
  to_bin(intsym[1], intsym[0], fin[1]);
  i=2;
  while(!((intsym[i] == 0)&&(intsym[i+1] == 0)))
    {
      //what if skip>15
      find_code(intsym[i],intsym[i+1],fin[codeLength]);
      to_bin(intsym[i+2], intsym[i+1], fin[codeLength+1]);
      if(intsym[i] > 15)
	{
	  intsym[i]-=15;
	}
      else
	{
	  i+=3;
	  
	}
      codeLength+=2;
    }
  strcpy(fin[codeLength++],"1010");
  return codeLength;
}
void printMatrix(int a[][8], char *filename)
{
  FILE *fp = fopen(filename,"w"); 
  int i,j;
  for(i=0; i < 8; i++)
    {
      for(j=0; j < 8; j++)
	{
	  printf("%d ",a[i][j]);
	  fprintf(fp,"%d\n",a[i][j]);
	}
      printf("\n");
    }
  fclose(fp);
}
void printArray(int *a,int n, char *filename)
{
  int i;
  FILE *fp = fopen(filename,"w"); 
  for(i=0; i < n; i++)
    {
      printf("%d ",a[i]);
      fprintf(fp,"%d\n",a[i]);
    }
  printf("\n");
  fclose(fp);
}

int main()
{
  int original[8][8];
  int shifted[8][8];
  int transformed[8][8];
  int quantized[8][8];
  int zz[64];
  //Maximum symbol size is 64*3+1
  int intsym[64*3+1];
  int symlength;
  char fin[64*3][19];
  int finlength;
  read_input_to(original);
  printf("\nORIGINAL\n");
  printMatrix(original,"original.txt");
  shift(original,shifted);
  printf("\nSHIFTED\n");
  printMatrix(shifted,"shifted.txt");
  fdct(shifted, transformed);
  printf("\nTRANSFORMED\n");
  printMatrix(transformed,"dct.txt");
  quantize(transformed,quantized);
  printf("\nQUANTIZED\n");
  printMatrix(quantized,"after_quantization.txt");
  zigzag(quantized,zz);
  printf("\nZIGZAGED\n");
  printArray(zz,64,"zigzag.txt");
  symlength=intermediate(zz,intsym);
  printf("\nINTERMEDIATE SYMBOL\n");
  printArray(intsym,symlength,"intermediate.txt");
  finlength=encode(intsym,fin);
  printf("\nFINAL\n");
  FILE *fp = fopen("final.txt","w"); 
  for(int i=0;i<finlength;i++)
    {
      printf("%s\n",fin[i]);
      fprintf(fp,"%s\r\n",fin[i]);
    }
  fclose(fp);
  return 0;
  
}
