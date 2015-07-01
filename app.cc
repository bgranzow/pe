#include "app.h"
#include "utils.h"
#include <PCU.h>

namespace pe {

App::App(AppInput& in) :
  mesh(in.mesh),
  polynomialOrder(in.polynomialOrder),
  integrationOrder(in.integrationOrder),
  u(in.u),
  rhs(in.rhs),
  out(in.out)
{
}

void App::solve()
{
  double t0 = PCU_Time();
  double t1 = PCU_Time();
  print("linear system solved in %f seconds", t1-t0);
}

void App::run()
{
  print("sovling poisson's equation!");
  pre();
  solve();
  post();
}

}
