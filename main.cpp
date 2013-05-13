#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <sys/types.h>
#include "tools_eigen.h"

int main(int argc, char **argv){
  double *a;
  double *b;
  double temp;
  int initID = 0;
  int n=0;
  //int res;
  char *name;
  clock_t t;
  double time=0.;
  FILE *input;
    
  if (argc==2){
    if ( !(n = atoi(argv[1]))){
      printf("This is not a moon!\nI need numberz!\n");
      return -4;
    }
    //n = atoi(argv[1]);
    //m = atoi(argv[2]);
    a = new double[n*n];
    if (a==NULL){
      printf("Failed to allocate memory for matrix A\n");
      return -2;
    }
    defineMatrixWithFunction(a, n);
    initID=1;
  }
  else if (argc==3){
    if ( !(n = atoi(argv[2]))){
      printf("This is not a moon!\n");
      return -4;
    }
    name = argv[1];
    //n = atoi(argv[2]);
    //m = atoi(argv[3]);
    int size = n*n;
    a = new double[size];
    if (a==NULL){
      printf("Failed to allocate memory for matrix A\n");
      return -2;
    }
    input = fopen(name, "r");
    readMatrixFromFile(input, a, n);
    fclose(input);
    initID=2;
  }
  else{
    printf("Usage:\n %s n\n %s file_name n\n", argv[0], argv[0]);
    return -1;
  }
  if (initID==0){
    printf("Failed to initialize matrix\n");
    delete[] a;
    return -1;
  }
  b = new double[n];
  //printMatrix(a, n, n);

  //temp = someInvariant_ii(a, n);
  //printf("Sum before step 1:%.2lf\n", temp);
  t = clock();
  qrAlmostUpperTriangle(a, n);
  //printMatrix(a, n, n);
  temp = traceMatrix(a, n);
  printf("Trace after step 1:%.2lf\n", temp);
  //temp = someInvariant_ii(a, n);
  //printf("Sum after step 1:%.2lf\n", temp);
  searchEigenValues(a, b, n);
  printMatrix(a, n, n);
  temp = traceMatrix(a, n);
  printf("Trace after step 2:%.2lf\n", temp);
  time = (clock()-t)/(double)(CLOCKS_PER_SEC);
  printf("Total time:\n%.2lf sec\n", time);
  for (int i=0; i<n; i++){
    printf("Value %d: %.4lf\n", i, b[i]);
  }
  delete[] a;
  delete[] b;
  return 0;
}
