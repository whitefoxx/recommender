#ifndef _RATING_H_
#define _RATING_H_

#include "DataModel.h"

class RatingModel: public DataModel {
public:
	RatingModel():ratings(NULL){}
	RatingModel(int n_u,int n_i,int n_r):DataModel(n_u,n_i,n_r){ 
		ratings = new Rating[n_rating];
	}
	RatingModel(int n_u,int n_i,int n_r,int u_0,int i_0):DataModel(n_u,n_i,n_r,u_0,i_0){ 
		ratings = new Rating[n_rating];
	}
	Rating *ratings;
};

#endif