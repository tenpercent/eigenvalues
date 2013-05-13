#include <cstdio>
#include <cstdlib>

void defineMatrixWithFunction(double *a, int n){
  /*for (int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      if (i==j)
	a[i*n+j]=1.;
      else
	a[i*n+j]=0.;
      if (i==(n-1)) a[i*n+j]=j+1;
      if (j==(n-1)) a[i*n+j]=i+1;
    }
    }*/
  for (int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      if (abs(i-j)==0) a[i*n+j]=1;
      else if (abs(i-j)==1) a[i*n+j]=2;
      else a[i*n+j]=0;
    }
  }
}

int readMatrixFromFile(FILE* in, double* a, int n){
  int res;
  int i,j;
  if (!in){
    printf("No file\n");
    return -1;
  }
  if (a==NULL){
    printf("No matrix\n");
    return -2;
  }
  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
      res = fscanf(in, "%lf", a+i*n+j);
      if (!res){
	printf("Failed to read from file!\n");
	return -3;
      }
    }
  }
  return 0;
}
