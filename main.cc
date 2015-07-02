#include "app.h"
#include "utils.h"
#include <petscsys.h>
#include <apf.h>
#include <apfShape.h>
#include <apfMesh2.h>
#include <apfMDS.h>
#include <gmi_mesh.h>
#include <PCU.h>

namespace {

double u(apf::Vector3 const& p)
{
  double x = p[0];
  double y = p[1];
  return x*y*(1.-x)*(1.-y);
}

double rhs(apf::Vector3 const& p)
{
  double x = p[0];
  double y = p[1];
  return 2.*(x*(1.-x)+y*(1.-y)); 
}

void initialize()
{
  MPI_Init(0,0);
  PCU_Comm_Init();
  PetscInitialize(0,0,0,0);
}

void finalize()
{
  PetscFinalize();
  PCU_Comm_Free();
  MPI_Finalize();
}

}

int main(int argc, char** argv)
{
  ASSERT(argc == 4);
  const char* geom = argv[1];
  const char* mesh = argv[2];
  const char* out = argv[3];
  initialize();
  gmi_register_mesh();
  apf::Mesh2* m = apf::loadMdsMesh(geom, mesh);
  pe::AppInput in = { m, 1, 1, u, rhs, out };
  pe::App app(in);
  app.run();
  finalize();
}
