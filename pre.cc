#include "app.h"
#include "linsys.h"
#include <apf.h>
#include <apfNumbering.h>
#include <PCU.h>

namespace pe {

static apf::Field* createSolutionField(apf::Mesh* m, int o)
{
  apf::Field* f = apf::createLagrangeField(m, "u", apf::SCALAR, o);
  apf::zeroField(f);
  return f;
}

static apf::GlobalNumbering* createNumbering(
    apf::Mesh* m,
    apf::Field* f,
    const char* name)
{
  apf::FieldShape* fs = apf::getShape(f);
  apf::Numbering* n = apf::numberOwnedNodes(m, name, fs);
  return apf::makeGlobal(n);
}

static long countTotalNodes(int n)
{
  long N = (long)n;
  PCU_Add_Longs(&N, 1);
  return N;
}

void App::pre()
{
  sol = createSolutionField(mesh, polynomialOrder);
  owned = createNumbering(mesh, sol, "owned");
  shared = createNumbering(mesh, sol, "shared");
  apf::synchronize(shared);
  int n = apf::countNodes(owned);
  long N = countTotalNodes(n);
  linsys = new LinSys(n,N);
}

}
