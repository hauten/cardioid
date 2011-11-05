#include "initializeSimulate.hh"
#include "object_cc.hh"
#include "Simulate.hh"

#include "initializeAnatomy.hh"
#include "assignCellsToTasks.hh"
#include "diffusionFactory.hh"
#include "reactionFactory.hh"
#include "stimulusFactory.hh"
#include "getRemoteCells.hh"
#include "Anatomy.hh"

using namespace std;

void initializeSimulate(const string& name, Simulate& sim)
{

   OBJECT* obj = objectFind(name, "SIMULATE");

   objectGet(obj, "loop", sim.loop_, "0");
   objectGet(obj, "maxLoop", sim.maxLoop_, "1000");
   objectGet(obj, "dt", sim.dt_, "0.02e-3");
   objectGet(obj, "time", sim.time_, "0");
   objectGet(obj, "printRate", sim.printRate_, "20");
   objectGet(obj, "snapshotRate", sim.snapshotRate_, "100");
   

   string nameTmp;
   objectGet(obj, "anatomy", nameTmp, "anatomy");
   initializeAnatomy(sim, nameTmp, MPI_COMM_WORLD);
   sim.anatomy_.setGridSize(sim.nx_, sim.ny_, sim.nz_);
   
   objectGet(obj, "decomposition", nameTmp, "decomposition");
   assignCellsToTasks(sim, nameTmp, MPI_COMM_WORLD);
   
   getRemoteCells(sim, MPI_COMM_WORLD);
   
   objectGet(obj, "diffusion", nameTmp, "diffusion");
   sim.diffusion_ = diffusionFactory(nameTmp, sim.anatomy_);
   
   objectGet(obj, "reaction", nameTmp, "reaction");
   sim.reaction_ = reactionFactory(nameTmp, sim.anatomy_);

   vector<string> names;
   objectGet(obj, "stimulus", names);
   for (unsigned ii=0; ii<names.size(); ++ii)
      sim.stimulus_.push_back(stimulusFactory(names[ii], sim.anatomy_));
}