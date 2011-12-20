#ifndef _USERBASED_PREDICTOR_H
#define _USERBASED_PREDICTOR_H

#include "Predictor.h"
#include "User.h"
#include "Similarity.h"

class UserBased_Predictor: public Predictor {
public:
	UserBased_Predictor(int k,int m_n):K_NEIGHBOR(k),MIN_NEIGHBOR(m_n){}

	double predict(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		int u, int i, long t)
	{
		double p,q,s;		
		int count = 0;
		vector<Neighbor>::iterator nb_it;
		p = q = 0;
		for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = trainDataModel.getScore(n,i)) >= trainDataModel.min_score) {
				p += (s - trainDataModel.getScore_avg(n)) * nb_it->similarity;
				q += abs(nb_it->similarity);
				count ++;
				if(count >= K_NEIGHBOR) {
					break;
				}
			}
		}
		s = trainDataModel.getScore_avg(u);
		if(count >= MIN_NEIGHBOR && q > 0) {
			s += p / q;
			if(s < trainDataModel.min_score) s = trainDataModel.min_score;
			else if(s > trainDataModel.max_score) s = trainDataModel.max_score;
		}
		return s;
	}

	double predict(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		double **trust, int u, int i, long t)
	{
		double p,q,s;		
		int count = 0;
		vector<Neighbor>::iterator nb_it;
		p = q = 0;
		for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = trainDataModel.getScore(n,i)) >= trainDataModel.min_score) {
					double w = 2 * nb_it->similarity*trust[u][n]/(nb_it->similarity + trust[u][n]);
					p += (s - trainDataModel.getScore_avg(n)) * (w);
					q += abs(w);
					count ++;
					if(count >= K_NEIGHBOR) {
						break;
					}

			}
		}
		s = trainDataModel.getScore_avg(u);
		if(count >= MIN_NEIGHBOR && q > 0) {
			s += p / q;
			if(s < trainDataModel.min_score) s = trainDataModel.min_score;
			else if(s > trainDataModel.max_score) s = trainDataModel.max_score;
		}
		return s;
	}

	void predictAll(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		UserModel &testDataModel, UserModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int u = testDataModel.user_0; u < testDataModel.user_0 + testDataModel.n_user; u++) {
			map<int,Rating> &rs = testDataModel.getRatings(u);
			for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
				int i = it_i->first;
				double s = predict(trainDataModel, neighbor_cls, u, i, it_i->second.timestamp);
				resultDataModel.entities[u].ratings[i] = Rating(u,i,s,it_i->second.timestamp);
			}
		}
	}

	void predictAll(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		double **trust, UserModel &testDataModel, UserModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int u = testDataModel.user_0; u < testDataModel.user_0 + testDataModel.n_user; u++) {
			map<int,Rating> &rs = testDataModel.getRatings(u);
			for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
				int i = it_i->first;
				double s = predict(trainDataModel, neighbor_cls, trust, u, i, it_i->second.timestamp);
				resultDataModel.entities[u].ratings[i] = Rating(u,i,s,it_i->second.timestamp);
			}
		}
	}

private:
	int K_NEIGHBOR;
	int MIN_NEIGHBOR;
};
#endif

