#include "app.h"
#include "linsys.h"
#include "utils.h"
#include <apf.h>
#include <apfNumbering.h>
#include <apfDynamicVector.h>
#include <PCU.h>

namespace pe {

static void cleanup(
    apf::Field* f,
    apf::GlobalNumbering* o,
    apf::GlobalNumbering* s,
    LinSys* ls)
{
  apf::destroyField(f);
  apf::destroyGlobalNumbering(o);
  apf::destroyGlobalNumbering(s);
  delete ls;
}

static void attachSolution(
    apf::Field* f,
    apf::GlobalNumbering* n,
    LinSys* ls)
{
  apf::DynamicVector x;
  ls->getSolution(x);
  apf::DynamicArray<apf::Node> nodes;
  apf::getNodes(n, nodes);
  ASSERT(nodes.getSize() == x.getSize());
  for (std::size_t i=0; i < nodes.getSize(); ++i)
    apf::setScalar(f, nodes[i].entity, nodes[i].node, x[i]);
  apf::synchronize(f);
}

void App::post()
{
  attachSolution(sol, owned, linsys);
  apf::writeVtkFiles(out, mesh);
  cleanup(sol, owned, shared, linsys);
}

}
