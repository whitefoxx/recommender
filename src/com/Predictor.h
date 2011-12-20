#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "DataModel.h"

class Predictor {
public:
	virtual void train(const DataModel &trainDataModel){};
	virtual void predictAll(const DataModel &trainDataModel,const DataModel &testDataModel,DataModel &resultDataModel){};
	virtual void predictAll(const DataModel &testDataModel,DataModel &resultDataModel){};
};

#endif

