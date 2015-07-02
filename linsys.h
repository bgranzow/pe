#ifndef PE_LINSYS_H
#define PE_LINSYS_H

#include <petscksp.h>

namespace apf {class DynamicVector;}

namespace pe {

class LinSys
{
  public:
    LinSys(int n, long N);
    ~LinSys();
    void addToVector(int sz, long* rows, double* vals);
    void addToMatrix(int sz, long* rows, double* vals);
    void zeroToVector(int sz, long* rows);
    void diagMatRow(int sz, long* rows);
    void synchronize();
    void solve();
    void getSolution(apf::DynamicVector& x);
  private:
    Mat A;
    Vec x;
    Vec b;
    KSP solver;
};

}

#endif
