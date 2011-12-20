#ifndef _MF_PREDICTOR_H
#define _MF_PREDICTOR_H

#include "Predictor.h"
#include "Rating.h"
#include "Stat.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

class MF_Predictor: public Predictor {
public:
	MF_Predictor(const RatingModel &data, int k, double lrate, double lambda){
		n_user = data.n_user;
		n_item = data.n_item;
		n_rating = data.n_rating;
		K = k;
		this->lrate = lrate;
		this->lambda = lambda;
		user_bias = new double[n_user];
		item_bias = new double[n_item];
		P = new double*[n_user];
		Q = new double*[n_item];
		for(int i = 0; i < n_user; i++) {
			P[i] = new double[K];	
			user_bias[i] = 0;
		}
		for(int i = 0; i < n_item; i++) {
			Q[i] = new double[K];
			item_bias[i] = 0;
		}
		for(int i = 0; i < n_user; i++)
			for(int j = 0; j < K; j++)
				P[i][j] = 0.1 * ((rand()+0.0)/RAND_MAX) / sqrt(float(K));

		for(int i = 0; i < n_item; i++)
			for(int j = 0; j < K; j++)
				Q[i][j] = 0.1 * ((rand()+0.0)/RAND_MAX) / sqrt(float(K));
		
		global_avg = cal_globalAvg(data);
	}

	void train(RatingModel &data)
	{
		for(int lp = 0; lp < 22; lp++) {
			for(int i = 0; i < data.n_rating; i++) {
				int user = data.ratings[i].user_id;
				int item = data.ratings[i].item_id;
				int time = data.ratings[i].timestamp;
				double score = data.ratings[i].score;
				double e = score - predict(user, item, time);
				for(int k = 0; k < K; k++) {
					P[user][k] += lrate * (e * Q[item][k] - lambda * P[user][k]);
					Q[item][k] += lrate * (e * P[user][k] - lambda * Q[item][k]);
				}
				user_bias[user] += lrate * (e - lambda * user_bias[user]);
				item_bias[item] += lrate * (e - lambda * item_bias[item]);
			}
		}
	}

	void predictAll(RatingModel &test,RatingModel &result)
	{
		for(int i = 0; i < test.n_rating; i++)
		{
			int user = test.ratings[i].user_id;
			int item = test.ratings[i].item_id;
			int time = test.ratings[i].timestamp;
			double score = predict(user, item, time);
			if(score > 5) score = 5;
			if(score < 1) score = 1;
			result.ratings[i].user_id = user;
			result.ratings[i].item_id = item;
			result.ratings[i].timestamp = time;
			result.ratings[i].score = score;
		}
	}

	inline double predict(int user, int item, int time)
	{
		double score = 0.0;
		for(int k = 0; k < K; k++)
			score += P[user][k] * Q[item][k];
		score += user_bias[user] + item_bias[item] + global_avg;
		return score;
	}

private:
	int n_user;
	int n_item;
	int n_rating;
	double **P;
	double **Q;
	double *user_bias;
	double *item_bias;
	double lrate;
	double lambda;
	double global_avg;
	int K;
	int max_loop;
};
#endif

