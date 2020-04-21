// RUN: %clang_cc1 -verify -fopenmp -ast-print %s | FileCheck %s

#ifndef HEADER
#define HEADER

template<typename IndexT>
void test(double *A, IndexT k)
{
  #pragma omp task depend(out: A[k]) 
  {
    ;
  }
}
// CHECK: template <typename IndexT> void test(double *A, IndexT k) {
// CHECK:     #pragma omp task depend(out : A[k])
// CHECK:         {
// CHECK:             ;
// CHECK:         }
// CHECK: }
// CHECK: template<> void test<int>(double *A, int k) {
// CHECK:     #pragma omp task depend(out : A[k])
// CHECK:         {
// CHECK:             ;
// CHECK:         }
// CHECK: }


struct lValueVector {
  int operator [] (int index) {
    return index + 42;
  }
};
template<typename BaseTypeT, typename IndexT>
void test2(BaseTypeT A, IndexT k)
{
  #pragma omp task depend(out: A[k]) // expected-error {{expected addressable lvalue expression, array element or array section}}
  {
    ;
  }
}
int driver(double *A)
{
  int k = 42;
  test(A, k);
  test2(lValueVector(), k); // expected-note {{in instantiation of function template specialization 'test2<lValueVector, int>' requested here}} 
  return 0;
}

#endif