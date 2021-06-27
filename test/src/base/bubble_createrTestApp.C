//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "bubble_createrTestApp.h"
#include "bubble_createrApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
bubble_createrTestApp::validParams()
{
  InputParameters params = bubble_createrApp::validParams();
  return params;
}

bubble_createrTestApp::bubble_createrTestApp(InputParameters parameters) : MooseApp(parameters)
{
  bubble_createrTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

bubble_createrTestApp::~bubble_createrTestApp() {}

void
bubble_createrTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  bubble_createrApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"bubble_createrTestApp"});
    Registry::registerActionsTo(af, {"bubble_createrTestApp"});
  }
}

void
bubble_createrTestApp::registerApps()
{
  registerApp(bubble_createrApp);
  registerApp(bubble_createrTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
bubble_createrTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  bubble_createrTestApp::registerAll(f, af, s);
}
extern "C" void
bubble_createrTestApp__registerApps()
{
  bubble_createrTestApp::registerApps();
}
