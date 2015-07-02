#include "linsys.h"
#include "utils.h"
#include <apfDynamicVector.h>
#include <PCU.h>

namespace pe {

LinSys::LinSys(int n, long N)
{
  print("%lu total unknowns", N);
  CALL( VecCreateMPI(PETSC_COMM_WORLD, n, N, &b) );
  CALL( VecSetOption(b, VEC_IGNORE_NEGATIVE_INDICES, PETSC_TRUE) );
  CALL( MatCreateAIJ(PETSC_COMM_WORLD, n, n, N, N,
        300, PETSC_NULL, 300, PETSC_NULL, &A) );
  CALL( MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE) );
  CALL( KSPCreate(PETSC_COMM_WORLD, &solver) );
  CALL( KSPSetTolerances(solver, 1.0e-8, 1.0e-8, PETSC_DEFAULT, 100) );
  CALL( VecDuplicate(b, &x) );
}

LinSys::~LinSys()
{
  CALL( MatDestroy(&A) );
  CALL( VecDestroy(&x) );
  CALL( VecDestroy(&b) );
  CALL( KSPDestroy(&solver) );
}

void LinSys::addToVector(int sz, long* rows, double* vals)
{
  PetscInt* r = (PetscInt*)rows;
  CALL( VecSetValues(b, sz, r, vals, ADD_VALUES) );
}

void LinSys::addToMatrix(int sz, long* rows, double* vals)
{
  PetscInt* r = (PetscInt*)rows;
  CALL( MatSetValues(A, sz, r, sz, r, vals, ADD_VALUES) );
}

void LinSys::zeroToVector(int sz, long* rows)
{
  PetscInt* r = (PetscInt*)rows;
  PetscScalar vals[sz];
  for (int i=0; i < sz; ++i)
    vals[i] = 0.0;
  CALL( VecSetValues(b, sz, r, vals, INSERT_VALUES) );
}

void LinSys::diagMatRow(int sz, long* rows)
{
  PetscInt* r = (PetscInt*)rows;
  CALL( MatZeroRows(A, sz, r, 1.0, PETSC_NULL, PETSC_NULL) );
}

void LinSys::synchronize()
{
  CALL( VecAssemblyBegin(b) );
  CALL( VecAssemblyEnd(b) );
  CALL( MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY) );
  CALL( MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY) );
}

void LinSys::getSolution(apf::DynamicVector& sol)
{
  PetscInt n;
  CALL( VecGetLocalSize(x, &n) );
  sol.setSize(n);
  PetscScalar* X;
  CALL( VecGetArray( x, &X) );
  for (int i=0; i < n; ++i)
    sol[i] = X[i];
  CALL( VecRestoreArray(x, &X) );
}

void LinSys::solve()
{
  double t0 = PCU_Time();
  CALL( KSPSetOperators(solver, A, A) );
  CALL( KSPSetFromOptions(solver) );
  CALL( KSPSolve(solver, b, x) );
  double t1 = PCU_Time();
  print("linear system solved in %f seconds", t1-t0);
}

}
