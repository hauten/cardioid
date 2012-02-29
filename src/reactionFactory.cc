#include "reactionFactory.hh"

#include <cassert>
#include "object_cc.hh"

#include "TT04_CellML_Reaction.hh" // TT04 implementation from CellML (Nov 2011)
#include "TT06_CellML_Reaction.hh" // TT06 implementation from CellML (Nov 2011)
#include "TT06Dev_Reaction.hh"
#include "TT06_RRG_Reaction.hh"    // TT06 with modifications from Rice et al.
#include "ReactionFHN.hh"
#include "NullReaction.hh"
#include "Threading.hh"

using namespace std;

namespace
{
   Reaction* scanTT04_CellML(OBJECT* obj, const Anatomy& anatomy);
   Reaction* scanTT06_CellML(OBJECT* obj, const Anatomy& anatomy);
   Reaction* scanTT06Dev(OBJECT* obj, const Anatomy& anatomy,coreGroup *group);
   Reaction* scanTT06_RRG(OBJECT* obj, const Anatomy& anatomy);
   Reaction* scanFHN(OBJECT* obj, const Anatomy& anatomy);
}


Reaction* reactionFactory(const string& name, const Anatomy& anatomy, coreGroup* group)
{
   OBJECT* obj = objectFind(name, "REACTION");
   string method; objectGet(obj, "method", method, "undefined");

   if (method == "undefined")
      assert(1==0);
   else if (method == "TT04_CellML" || method == "tenTusscher04_CellML")
      return scanTT04_CellML(obj, anatomy);
   else if (method == "TT06_CellML" || method == "tenTusscher06_CellML")
      return scanTT06_CellML(obj, anatomy);
   else if (method == "TT06Dev" )
      return scanTT06Dev(obj, anatomy,group);
   else if (method == "TT06_RRG" )
      return scanTT06_RRG(obj, anatomy);
   else if (method == "FHN" || method == "FitzhughNagumo")
      return scanFHN(obj, anatomy);
   else if (method == "null")
      return new NullReaction();
   assert(false); // reachable only due to bad input
   return 0;
}

namespace
{
   Reaction* scanTT04_CellML(OBJECT* obj, const Anatomy& anatomy)
   {
      TT04_CellML_Reaction::IntegratorType integrator;
      string tmp;
      objectGet(obj, "integrator", tmp, "rushLarsen");
      if      (tmp == "rushLarsen")   integrator = TT04_CellML_Reaction::rushLarsen;
      else if (tmp == "rushLarson")   integrator = TT04_CellML_Reaction::rushLarsen;
      else if (tmp == "forwardEuler") integrator = TT04_CellML_Reaction::forwardEuler;
      else    assert(false);    
      
      return new TT04_CellML_Reaction(anatomy, integrator);
   }
}

namespace
{
   Reaction* scanTT06_CellML(OBJECT* obj, const Anatomy& anatomy)
   {
      TT06_CellML_Reaction::IntegratorType integrator;
      string tmp;
      objectGet(obj, "integrator", tmp, "rushLarsen");
      if      (tmp == "rushLarsen")   integrator = TT06_CellML_Reaction::rushLarsen;
      else if (tmp == "forwardEuler") integrator = TT06_CellML_Reaction::forwardEuler;
      else    assert(false);    
      return new TT06_CellML_Reaction(anatomy, integrator);
   }
}

namespace
{
   Reaction* scanTT06Dev(OBJECT* obj, const Anatomy& anatomy, coreGroup *group)
   {
      double tolerance=0.0 ; 
      int mod=0; 
      objectGet(obj, "tolerance", tolerance, "0.0") ;
      objectGet(obj, "mod", mod, "0") ;
      Reaction *reaction = new TT06Dev_Reaction(anatomy, tolerance,mod, group);
      return  reaction; 
   }
}

namespace
{
   Reaction* scanTT06_RRG(OBJECT* obj, const Anatomy& anatomy)
   {
      Reaction *reaction = new TT06_RRG_Reaction(anatomy);
      return  reaction; 
   }
}

namespace
{
   Reaction* scanFHN(OBJECT* obj, const Anatomy& anatomy)
   {
      // None of the FHN model parameters are currently wired to the
      // input deck.
      return new ReactionFHN(anatomy);
   }
}
