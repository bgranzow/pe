#ifndef PE_APP_H
#define PE_APP_H

namespace apf {
class Mesh;
class Field;
class Vector3;
template <class T> class NumberingOf;
typedef NumberingOf<int> Numbering;
}

namespace pe {

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
    void solve();
    void post();

    apf::Field* sol;
    apf::Mesh* mesh;
    apf::Numbering* owned;
    apf::Numbering* shared;

    int polynomialOrder;
    int integrationOrder;

    int numLocalEqs;
    int numGlobalEqs;

    double (*u)(apf::Vector3 const& p);
    double (*rhs)(apf::Vector3 const& p);

    const char* out;
};

}

#endif
