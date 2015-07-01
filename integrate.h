#ifndef PE_INTEGRATE_H
#define PE_INTEGRATE_H

#include <apf.h>
#include <apfDynamicVector.h>
#include <apfDynamicMatrix.h>

namespace pe {

struct IntegrateInput
{
  int order;
  apf::Field* field;
  double (*rhs)(apf::Vector3 const& p);
};

class Integrate : public apf::Integrator
{
  public:
    Integrate(IntegrateInput& in);
    void inElement(apf::MeshElement*);
    void outElement();
    void atPoint(apf::Vector3 const& p, double w, double dv);
    apf::DynamicVector fe;
    apf::DynamicMatrix ke;
  private:
    int ndofs;
    int ndims;
    apf::Field* u;
    apf::Element* e;
    double (*rhs)(apf::Vector3 const& p);
};

}

#endif
