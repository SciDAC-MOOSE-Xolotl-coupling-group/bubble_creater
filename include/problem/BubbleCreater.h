//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BUBBLECREATER_H
#define BUBBLECREATER_H

#include "ExternalProblem.h"
#include "bubble_createrApp.h"
// #include "Coupleable.h"

class BubbleCreater;

template<>
InputParameters validParams<BubbleCreater>();

/**
 * This is an interface to call an external solver
 */
// class BubbleCreater: public ExternalProblem, public Coupleable {
class BubbleCreater: public ExternalProblem{
public:
	BubbleCreater(const InputParameters & params);
	~BubbleCreater() {
	}

	virtual void externalSolve() override;
	virtual void syncSolutions(Direction /*direction*/) override;

	virtual bool converged() override;


private:
	virtual Real interdist(Real xto, Real yto, Real zto, Real xfrom, Real yfrom, Real zfrom);
	/// The name of the variable to transfer to
	unsigned int _num_eta;
	std::vector<const VariableValue *> _eta;
	std::vector<VariableName> _eta_names;

	const VariableName & _bubvar_name;
	const VariableName & _bndsvar_name;
	int _num_bub;
	Real _radius_bub;
	unsigned int _rand_seed;
	// Real _lx, ly, lz;

};

#endif /* BUBBLECREATER_H */
