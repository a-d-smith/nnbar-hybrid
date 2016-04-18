#ifndef LARLITE_PI0TRAIN_CXX
#define LARLITE_PI0TRAIN_CXX

#include "pi0Train.h"
#include "loadData.h"

namespace larlite {

  bool pi0Train::initialize() {
		// Load the data
		loadData dat("pi0Data.root");
		auto evList = dat.evList;

		// For each event
		for (auto evnt : evList){
			auto shList = evnt.shList;
			// Loop over the showers in pairs
			int i = 0;
			for (auto shi : shList){
				int j = 0;
				for (auto shj : shList){
					// Avoid double counting
					if (i > j){
						
					}
				}
			}
		}
    return true;
  }
  
  bool pi0Train::analyze(storage_manager* storage) {
    return true;
  }

  bool pi0Train::finalize() {
    return true;
  }

}
#endif
