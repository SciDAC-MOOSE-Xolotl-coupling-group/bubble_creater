//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BubbleCreater.h"
#include "SystemBase.h"
#include "MooseRandom.h"

registerMooseObject("bubble_createrApp", BubbleCreater);

template<>
InputParameters validParams<BubbleCreater>() {
	InputParameters params = validParams<ExternalProblem>();
	// params.addRequiredCoupledVar("all_etas","Vector of all order parameters");
	params.addRequiredParam<VariableName>("bubble_var","bubble variable name");
	params.addRequiredParam<VariableName>("bnds_var","bnds variable name");
	params.addRequiredParam<int>("num_bub","the number of bubbles to create");
	params.addRequiredParam<Real>("radius_bub","radius of the bubbles");
  params.addParam<unsigned int>("rand_seed", 0, "The random seed");
	// params.addRequiredParam<Real>("lx", "length along x axis");
	// params.addRequiredParam<Real>("ly", "length along y axis");
	// params.addRequiredParam<Real>("lz", "length along z axis");
	params.addClassDescription("Creates list of bubbles along grain boundary of polycrystal structure");
	return params;
}

BubbleCreater::BubbleCreater(const InputParameters & params) :
		ExternalProblem(params),
		// Coupleable(this, false),
		// _num_eta(coupledComponents("all_etas")),
		// _eta(_num_eta),
		// _eta_names(_num_eta),
		_bubvar_name(getParam<VariableName>("bubble_var")),
		_bndsvar_name(getParam<VariableName>("bnds_var")),
		_num_bub(getParam<int>("num_bub")),
		_radius_bub(getParam<Real>("radius_bub")),
		_rand_seed(getParam<unsigned int>("rand_seed"))
		// _lx(getParam<Real>("lx")),
		// _ly(getParam<Real>("ly")),
		// _lz(getParam<Real>("lz"))
{
	// for (unsigned int i = 0; i < _num_eta; ++i)
  // {
	// 	_eta[i] = &coupledValue("all_etas", i);
  //   _eta_names[i] = getVar("all_etas", i)->name();
  // }
}

void BubbleCreater::externalSolve()
{
}

void BubbleCreater::syncSolutions(Direction direction)
{
	MooseRandom::seed(_rand_seed);
	if (direction == Direction::FROM_EXTERNAL_APP) {
		MeshBase & to_mesh = mesh().getMesh();

    // Creating the list of gb node coordinates
		unsigned int nnodes = to_mesh.n_nodes();
    Real GBcoord_x[nnodes];
		Real GBcoord_y[nnodes];
		Real GBcoord_z[nnodes];
		int GBcoord_valid[nnodes];

		for( const auto & node : as_range(to_mesh.local_nodes_begin(), to_mesh.local_nodes_end()) ) {
			auto & bnds = getVariable(0, _bndsvar_name, Moose::VarKindType::VAR_ANY, Moose::VarFieldType::VAR_FIELD_STANDARD);
			dof_id_type dof_bnds = node->dof_number(bnds.sys().number(), bnds.number(),0);

			// Get bnds value
			Real value_bnds = bnds.sys().solution()(dof_bnds);
			unsigned int nodeid = node->id();  // Reading current node ID
			GBcoord_valid[nodeid] = 0;
			if (value_bnds < 0.9) {
				GBcoord_x[nodeid] = (*node)(0);
				GBcoord_y[nodeid] = (*node)(1);
				GBcoord_z[nodeid] = (*node)(2);
				GBcoord_valid[nodeid] = 1;
			}

			// Set bnds value
			// bnds.sys().solution().set(dof_bnds, value_bnds);

			// Finalize the works on bnds
			bnds.sys().solution().close();

			Real tmpi = 0, tmpj = 0, tmpk = 0;
			int tmpvalid = 0;
			MPI_Allreduce(&GBcoord_x[nodeid], &tmpi, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			GBcoord_x[nodeid] = tmpi;
			MPI_Allreduce(&GBcoord_y[nodeid], &tmpj, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			GBcoord_y[nodeid] = tmpj;
			MPI_Allreduce(&GBcoord_z[nodeid], &tmpk, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			GBcoord_z[nodeid] = tmpk;
			MPI_Allreduce(&GBcoord_valid[nodeid], &tmpvalid, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
			GBcoord_valid[nodeid] = tmpvalid;

		}	//local GBcoord done
		printf("local GBcoord done\n");

    printf("# of nodes = %d\n", nnodes);
		//Merging GBcoord array
		int nGBcoord = 0;
		for (unsigned int i = 0; i < nnodes; i++) {
			// Real tmpi = 0, tmpj = 0, tmpk = 0;
			// int tmpvalid = 0;
			// printf("nodeID: %d, MPI_Allreduce GBcoord_x start\n",i);
			// MPI_Allreduce(&GBcoord_x[i], &tmpi, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			// GBcoord_x[i] = tmpi;
			// printf("nodeID: %d, MPI_Allreduce GBcoord_x done\n",i);
			// MPI_Allreduce(&GBcoord_y[i], &tmpj, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			// GBcoord_y[i] = tmpj;
			// MPI_Allreduce(&GBcoord_z[i], &tmpk, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			// GBcoord_z[i] = tmpk;
			// MPI_Allreduce(&GBcoord_valid[i], &tmpvalid, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
			// GBcoord_valid[i] = tmpvalid;
			nGBcoord += GBcoord_valid[i];
		}
		printf("global GBcoord done\n");
		//Filtering non-GB coordinates
		Real GBlist_x[nGBcoord];
		Real GBlist_y[nGBcoord];
		Real GBlist_z[nGBcoord];
		int nGB = 0;
		for (unsigned int i = 0; i < nnodes; i++) {
			if (GBcoord_valid[i] == 1) {
				GBlist_x[nGB] = GBcoord_x[i];
				GBlist_y[nGB] = GBcoord_y[i];
				GBlist_z[nGB] = GBcoord_z[i];
				nGB++;
			}
		} // GBlist done

		//Prepare the bubble location array
		Real bcoord_x[_num_bub];
		Real bcoord_y[_num_bub];
		Real bcoord_z[_num_bub];
		//Pick the first bubble location
		int npick = int(MooseRandom::rand()*nGB);
		Real xpick = GBlist_x[npick];
		Real ypick = GBlist_y[npick];
		Real zpick = GBlist_z[npick];
		bcoord_x[0] = xpick;
		bcoord_y[0] = ypick;
		bcoord_z[0] = zpick;

		int numbub_fin;
		//Pick the nth bubble location
		for (int i = 1; i < _num_bub; i++) {
      //Filter GBlist within the territory of n-1 th bubble
			int nGBnew = 0;
      for (int ng = 0; ng < nGB; ng++){
				Real xc = GBlist_x[ng];
				Real yc = GBlist_y[ng];
				Real zc = GBlist_z[ng];
				Real d = interdist(xc, yc, zc, bcoord_x[i-1], bcoord_y[i-1], bcoord_z[i-1]);
				if (d > 2.0*_radius_bub) {
					nGBnew++;
				}
			}
      Real buff_x[nGBnew];
			Real buff_y[nGBnew];
			Real buff_z[nGBnew];
			int ngnew = 0;
			for (int ng = 0; ng < nGB; ng++){
				Real xc = GBlist_x[ng];
				Real yc = GBlist_y[ng];
				Real zc = GBlist_z[ng];
				Real d = interdist(xc, yc, zc, bcoord_x[i-1], bcoord_y[i-1], bcoord_z[i-1]);
				if (d > 2.0*_radius_bub) {
					buff_x[ngnew] = xc;
					buff_y[ngnew] = yc;
					buff_z[ngnew] = zc;
					ngnew++;
				}
			}

			//Update GBlist
			for (int ng = 0; ng < nGBnew; ng++) {
				GBlist_x[ng] = buff_x[ng];
				GBlist_y[ng] = buff_y[ng];
				GBlist_z[ng] = buff_z[ng];
			}
			nGB = nGBnew;

			// if(nGB < 1) {
			// 	break;
			// }

			//Picking the ith bubble location
			int npick = int(MooseRandom::rand()*nGB);
			Real xpick = GBlist_x[npick];
			Real ypick = GBlist_y[npick];
			Real zpick = GBlist_z[npick];
			bcoord_x[i] = xpick;
			bcoord_y[i] = ypick;
			bcoord_z[i] = zpick;

			numbub_fin = i + 1;
		}

		//print bubble locations
		for (int i = 0; i < numbub_fin; i++) {
			printf("%d %lf %lf %lf \n", i, bcoord_x[i], bcoord_y[i], bcoord_z[i]);
		}

		FILE *pfile;
    char namebuff[200];
		int n;
		sprintf(namebuff,"bub_coords_%d_%u.txt", numbub_fin, _rand_seed);
		pfile = fopen(namebuff,"w");
		fprintf(pfile,"x y z r\n");
		for (int i = 0; i < numbub_fin; i++) {
			fprintf(pfile, "%lf %lf %lf %lf\n", bcoord_x[i], bcoord_y[i], bcoord_z[i], _radius_bub);
		}
		fclose (pfile);
		printf("Bub. pos. file created: %s\n", namebuff);


	} // if (direction == Direction::FROM_EXTERNAL_APP)

  if (direction == Direction::TO_EXTERNAL_APP) {
	}


}

bool BubbleCreater::converged()
{
	return true;
}

Real BubbleCreater::interdist(Real xto, Real yto, Real zto, Real xfrom, Real yfrom, Real zfrom)
{
	return sqrt(pow(xfrom-xto,2)+pow(yfrom-yto,2)+pow(zfrom-zto,2));
}
