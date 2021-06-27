#include "bubble_createrApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
bubble_createrApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy DirichletBC, that is, set DirichletBC default for preset = true
  params.set<bool>("use_legacy_dirichlet_bc") = false;

  return params;
}

bubble_createrApp::bubble_createrApp(InputParameters parameters) : MooseApp(parameters)
{
  bubble_createrApp::registerAll(_factory, _action_factory, _syntax);
}

bubble_createrApp::~bubble_createrApp() {}

void
bubble_createrApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"bubble_createrApp"});
  Registry::registerActionsTo(af, {"bubble_createrApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
bubble_createrApp::registerApps()
{
  registerApp(bubble_createrApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
bubble_createrApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  bubble_createrApp::registerAll(f, af, s);
}
extern "C" void
bubble_createrApp__registerApps()
{
  bubble_createrApp::registerApps();
}
