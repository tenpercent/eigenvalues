#include <cmath>
#include "tools_eigen.h"


double vectorNorm(double *x, int n){
  int i;
  double ans=0.;
  for (i=0; i<n; i++){
    ans+=x[i]*x[i];
  }
  return sqrt(ans);
}

void printMatrix(double* const a, int h, int l){
  int point=0;
  for (int j=0; j<2*l; j++){
    printf(" _");
  }
  printf("\n ");
  for (int i=0; i<h; i++){
    printf("|");
    for(int j=0; j<l; j++){
      printf("%8.4lf", a[point]);
      point++;
    }
    printf("|\n ");
  }
  for (int j=0; j<2*l; j++){
    printf("_ ");
  }
  printf("\n");
  //return 0;
}

double traceMatrix(double *a, int n){
  double ans=0.;
  for (int i=0; i<n; i++){
    ans+=a[i*n+i];
  }
  return ans;
}

double someInvariant_i(double *a, int n, int k){
  double ans=0.;
  int i;
  for (i=0; i<n; i++){
    ans+=a[i*n+k]*a[i*n+k];
  }
  return ans;
}

double someInvariant_ii(double *a, int n){
  double ans=0.;
  int i;
  for (i=0; i<n*n; i++){
    ans+=a[i]*a[i];
  }
  return ans;
}

int shiftMatrix(double *a, int n, double k){
  for (int i=0; i<n*n; i+=(n+1)){
    a[i]-=k;
  }
  return 0;
}

double lInfMatrixNorm(double *a, int n){
  double temp=0., ans=0.;
  int i,j;
  for (i=0; i<n; i++){
    temp=0.;
    for (j=0; j<n; j++){
      temp+=fabs(a[j*n+i]);
    }
    if (temp>ans){
      ans=temp;
    }
  }
  return ans;
}

int qrAlmostUpperTriangle(double *a, int n){//приведение к почти треугольному виду методом отражений
  int i, j1, j2, k1, k2, j, k;
  double s_k, temp;
  double eps = 1e-6;
  int dim_x;
  double *rotMatrix = new double[n*n];
  double *tempMatrix = new double[n*n];
  double *tempVector = new double[n];
  double *tempMatrix_i = new double[n*n];
  double *tempMatrix_ii = new double[n*n];
  for (i=0; i<(n-2); ++i){
    dim_x = n-i-1;
    s_k=0.;
    for (j=i+2; j<n; ++j){
      s_k+=a[j*n+i]*a[j*n+i];
    }
    if (s_k<eps){
      continue;
    }
    temp=sqrt(s_k+a[(i+1)*n+i]*a[(i+1)*n+i]);
    tempVector[0]=a[(i+1)*n+i]-temp;
    for (j=i+2,k=1; j<n; ++j,++k){
      tempVector[k]=a[j*n+i];
    }
    temp = sqrt(tempVector[0]*tempVector[0]+s_k);
    s_k = 1/temp;
    for (j=0; j<dim_x; j++){
      tempVector[j]*=s_k;
    }
    //теперь по tempVector строится матрица отражений
    for (j=0; j<dim_x*dim_x; ++j){
      rotMatrix[j]=0.;
    }
    for (j=0; j<dim_x*dim_x; j+=dim_x+1){
      rotMatrix[j]=1.;
    }
    for (j=0; j<dim_x; ++j){
      for (k=j; k<dim_x; ++k){
	rotMatrix[j*dim_x + k]-=2*tempVector[j]*tempVector[k];
      }
    }
    for (j=1; j<dim_x; ++j){
      for (k=0; k<j; ++k){
	rotMatrix[j*dim_x + k]=rotMatrix[k*dim_x + j];
      }
    }
    
    //надо написать умножение как-нибудь
    for (j1=i+1, j2=0; j1<n; ++j1, ++j2){
      for (k1=i, k2=0; k1<n; ++k1, ++k2){
	tempMatrix_i[j2*(dim_x+1)+k2]=a[j1*n+k1];
      }
    }
    
    simpleMatrixMultiply(rotMatrix, tempMatrix_i, tempMatrix_ii, dim_x, dim_x, dim_x+1);
    
    //теперь в tempMatrix_ii хранится результат первого умножения
    for (j1=i+1, j2=0; j1<n; ++j2, ++j1){
      for (k1=i, k2=0; k1<n; ++k1, ++k2){
	a[j1*n + k1]=tempMatrix_ii[j2*(dim_x+1) + k2];
      }
    }
    //домножаем второй раз
    
    for (j1=0; j1<n; ++j1){
      for (k1=i+1, k2=0; k1<n; ++k1, ++k2){
	tempMatrix_i[j1*dim_x + k2]=a[j1*n + k1];
      }
    }
    simpleMatrixMultiply(tempMatrix_i, rotMatrix, tempMatrix_ii, n, dim_x, dim_x);
    for (j1=0; j1<n; ++j1){
      for (k1=i+1, k2=0; k1<n; ++k1,++k2){
	a[j1*n + k1]=tempMatrix_ii[j1*dim_x + k2];
      }
    }
    
  }
  delete[] tempMatrix;
  delete[] tempMatrix_i;
  delete[] tempMatrix_ii;
  delete[] tempVector;
  delete[] rotMatrix;
  return 0;
}

int searchEigenValues(double *a, double *out, int n){
  double *sins = new double[n-1];
  double *coss = new double[n-1];
  
  double shift;
  double norm = 0.;
  double eps = 1e-10;
  int i,j,k;
  double temp, temp1, temp2;
  double x,y;
  for (i=n; i>1; i--){
    norm=lInfMatrixNorm(a, n);
    while (fabs(a[(i-1)*n+(i-1)-1])>eps*norm){
      shift = a[(i-1)*n+(i-1)]+0.5*a[(i-1)*n+(i-1)-1];
      shiftMatrix(a, n, shift);
      for (j=0; j<i-1; ++j){
      
	x = a[j*n+j];
	y = a[(j+1)*n+j];
	temp1 = sqrt(x*x+y*y);
	temp2 = 1./temp1;
      
	sins[j]=-temp2*y;
	coss[j]=temp2*x;
      //домножение на T_{j,j+1}
	a[j*n+j]=temp1;
      
	for (k=j+1; k<i; ++k){
	  a[k*n+j]=0.;
	}
      
	for (k=j+1; k<i; ++k){
	  temp1=a[j*n+k]*coss[j]-a[(j+1)*n+k]*sins[j];
	  temp2=a[j*n+k]*sins[j]+a[(j+1)*n+k]*coss[j];
	  a[j*n+k]=temp1;
	  a[(j+1)*n+k]=temp2;
	}
      }
      //Получили R
      //A^{k+1}=R^{k}Q^{k}
      for (j=0; j<(i-1); ++j){
	for (k=0; k<(j+2); ++k){
	  temp1=a[k*n+j]*coss[j]-a[k*n+j+1]*sins[j];
	  temp2=a[k*n+j]*sins[j]+a[k*n+j+1]*coss[j];
	  a[k*n+j]=temp1;
	  a[k*n+j+1]=temp2;
	}
      }
      shiftMatrix(a, n, -shift);
      norm = lInfMatrixNorm(a, n);
    }//end while
    out[i-1] = a[(i-1)*n+(i-1)];
    //temp = traceMatrix(a, n);
    //printf("Trace after step %d:%.2lf\n", i, temp);
  }
  out[0]=a[0];
  out[1]=a[n+1];
  delete[] sins;
  delete[] coss;
  return 0;
}
