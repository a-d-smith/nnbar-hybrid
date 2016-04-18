/* This is how you can use the hybridData. To go from a LArLite root file to writing an analysis do the following
 * 
 * (1) run the makeHybrid shell script, it takes root files as arguments
 *       makeHybrid *.root
 * (2) You will get a root file called hybridData.root (which you can rename to something sensible), you need to put that 
 *     into this directory as it contains the loadData script which converts this root file into something nice for analysis. 
 *     You need to include loadData.h, to use loadData, type:
 *       loadData dat(<hybrid data root file>);
 * (3) You can then access a vector called dat.evList which contains all of the events which were able to be hybridized
 *     each element of this vector is an event with two further vectors trList & shList. Look at loadData.h to see what
 *     data is available
 * (4) Anything that isn't prefixed with "mc" you can trust would be available in an ERAna unit, but anything that is 
 *     prefixed with "mc" is truth level, so can only be used to check your algorithms! */

#ifndef LARLITE_TESTANA_CXX
#define LARLITE_TESTANA_CXX

#include "testAna.h"

using namespace std;
namespace larlite {

  bool testAna::initialize() {

		// Load in the hybrid data		
		loadData dat("nnbarData.root");
		auto evList = dat.evList;	

		// Loop over all of the events
		for (int evNum=0; evNum < evList.size(); evNum++){
			auto trList = evList[evNum].trList;
			auto shList = evList[evNum].shList;

			
			cout << "Event " << evNum << " ----------------------------------" << endl; 
			cout << "  Tracks  : " << trList.size() << endl;
			for (auto tr : trList){
				cout << "    ID   = " << tr.mcID  << endl;
				cout << "    PDG  = " << tr.mcPDG << endl;
				cout << "    PID  = " << tr.PID   << endl << endl;
			}
			cout << "  Showers : " << shList.size() << endl;
			for (auto sh : shList){
				cout << "    ID   = " << sh.mcID  << endl;
				cout << "    PDG  = " << sh.mcPDG << endl;
				cout << "    type = " << sh.type  << endl << endl;
			}
			cout << endl;
			cin.get();
		}

    return true;
  }
  
  bool testAna::analyze(storage_manager* storage) {
  
    return true;
  }

  bool testAna::finalize() {
  
    return true;
  }

}
#endif
