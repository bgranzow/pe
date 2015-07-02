#include "app.h"
#include "utils.h"
#include "linsys.h"
#include "integrate.h"
#include <apf.h>
#include <apfNumbering.h>
#include <apfDynamicVector.h>
#include <apfDynamicMatrix.h>
#include <gmi.h>
#include <PCU.h>

namespace pe {

static void addToSystem(
    apf::DynamicVector& fe,
    apf::DynamicMatrix& ke,
    apf::MeshEntity* e,
    apf::GlobalNumbering* n,
    LinSys* ls)
{
  apf::NewArray<long> numbers;
  int sz = apf::getElementNumbers(n, e, numbers);
  ls->addToVector(sz, &numbers[0], &fe[0]);
  ls->addToMatrix(sz, &numbers[0], &ke(0,0));
}

static void assembleSystem(
    int o,
    apf::Mesh* m,
    apf::Field* f,
    double (*rhs)(apf::Vector3 const& p),
    apf::GlobalNumbering* n,
    LinSys* ls)
{
  IntegrateInput in = { o, f, rhs};
  Integrate integrate(in);
  apf::MeshEntity* elem;
  apf::MeshIterator* elems = m->begin(m->getDimension());
  while ((elem = m->iterate(elems)))
  {
    apf::MeshElement* me = apf::createMeshElement(m, elem);
    integrate.process(me);
    addToSystem(integrate.fe, integrate.ke, elem, n, ls);
    apf::destroyMeshElement(me);
  }
  m->end(elems);
  ls->synchronize();
}

static void applyBCToSystem(
    apf::Mesh* m,
    apf::GlobalNumbering* gn,
    LinSys* ls)
{
  gmi_model* model = m->getModel();
  gmi_ent* boundary;
  gmi_iter* boundaries = gmi_begin(model, m->getDimension()-1);
  while ((boundary = gmi_next(model, boundaries)))
  {
    apf::DynamicArray<apf::Node> nodes;
    apf::ModelEntity* b = reinterpret_cast<apf::ModelEntity*>(boundary);
    apf::getNodesOnClosure(m, b, nodes);
    int nnodes = nodes.getSize();
    long rows[nnodes];
    for (int n=0; n < nnodes; ++n)
      rows[n] = apf::getNumber(gn, nodes[n]);
    ls->diagMatRow(nnodes, rows);
    ls->zeroToVector(nnodes, rows);
  }
  gmi_end(model, boundaries);
  ls->synchronize();
}

void App::assemble()
{
  double t0 = PCU_Time();
  assembleSystem(polynomialOrder, mesh, sol, rhs, shared, linsys);
  applyBCToSystem(mesh, shared, linsys);
  double t1 = PCU_Time();
  print("assembled in %f seconds", t1-t0);
}

}
