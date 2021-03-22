#include <stdio.h>
//#define MIN(x,y) (x)<(y)?(x):(y)
//#define GET_MIN(x,y,z) (MIN(x, y))<(z)?(MIN(x, y)):(z)

int GET_MIN(int a, int b, int c) {
  if(a > b) {
    return (b > c ? c : b);
  } else {
    return (a > c ? c : a);
  }
}

int main()
{
    FILE *in = fopen("ballpark.inp","r");
    //FILE *out = fopen("ballpark.out","w");
    int N,M;
    fscanf(in,"%d",&N);
    fscanf(in,"%d",&M);
    int matrix[N][M];//세로크기N 가로크기M인 행렬
    for(int i=N-1;i>=0;i--)
    {
        for(int j=0;j<M;j++)
        {
            fscanf(in,"%1d",&matrix[i][j]);
            if(matrix[i][j]==0) matrix[i][j]=1;
            else matrix[i][j]=0;
        }
    }

    for(int i=0;i<N;i++)
        {
            for(int j=0;j<M;j++)
            {
                printf("%d",matrix[i][j]);
            }
            printf("  %d\n",i);
        }
      printf("\n\n");

    for(int i=N-2;i>=0;i--)
    {
        for(int j=M-2;j>=0;j--)
        {   
            if(matrix[i][j]!=0)
            {
                int min=GET_MIN(matrix[i+1][j],matrix[i][j+1],matrix[i+1][j+1])+1;
                matrix[i][j] = min;
            }
        }
    }

    for(int i=0;i<N;i++)
        {
            for(int j=0;j<M;j++)
            {
                printf("%d",matrix[i][j]);
            }
            printf("  %d\n",i);
        }

    fclose (in);


    return 0;
}