#ifndef _STAT_H_
#define _STAT_H_

#include "Rating.h"

double cal_globalAvg(const RatingModel &data)
{
	double sum = 0;
	for(int i = 0; i < data.n_rating; i++) {
		sum += data.ratings[i].score;
	}
	return sum / data.n_rating;
}

#endif

