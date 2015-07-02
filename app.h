#ifndef PE_APP_H
#define PE_APP_H

namespace apf {
class Mesh;
class Field;
class Vector3;
template <class T> class NumberingOf;
typedef NumberingOf<long> GlobalNumbering;
}

namespace pe {

class LinSys;

struct AppInput
{
  apf::Mesh* mesh;
  int polynomialOrder;
  int integrationOrder;
  double (*u)(apf::Vector3 const& p);
  double (*rhs)(apf::Vector3 const& p);
  const char* out;
};

class App
{
  public:

    App(AppInput& in);
    void run();

  private:

    void pre();
    void assemble();
    void post();

    apf::Mesh* mesh;
    apf::Field* sol;
    apf::GlobalNumbering* owned;
    apf::GlobalNumbering* shared;

    int polynomialOrder;
    int integrationOrder;

    LinSys* linsys;

    double (*u)(apf::Vector3 const& p);
    double (*rhs)(apf::Vector3 const& p);

    const char* out;
};

}

#endif
