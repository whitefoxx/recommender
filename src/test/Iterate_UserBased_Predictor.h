#ifndef _ITERATE_USERBASED_PREDICTOR_H_
#define _ITERATE_USERBASED_PREDICTOR_H_

#include "../com/Predictor.h"
#include "../com/User.h"
#include "../com/Similarity.h"
#include <iostream>
using namespace std;

class Iterate_UserBased_Predictor {
public:
	Iterate_UserBased_Predictor(Pearson_Similarity similarity,int k,int m_n)
		:similarity(similarity),K_NEIGHBOR(k),MIN_NEIGHBOR(m_n){}

	double predict(UserModel &trainDataModel, int u, int i, long t)
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

	void train(UserModel &trainDataModel) {
		neighbor_cls = new NeighborCollection[trainDataModel.n_user+trainDataModel.user_0];
		similarity.similarity(trainDataModel, neighbor_cls);
		double rmse = 0;
		int max_loop = 10;
		for(int lp = 0; lp < max_loop; lp++) {
			rmse = 0;
			for(int u = trainDataModel.user_0; u < trainDataModel.user_0 + trainDataModel.n_user; u++) {
				map<int,Rating> &rs = trainDataModel.getRatings(u);
				for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
					int i = it_i->first;
					double p,q,s,ssum;
					//predict
					int count = 0;
					vector<Neighbor>::iterator nb_it;
					p = q = ssum = 0;
					for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
						int n = nb_it->neighbor;
						if((s = trainDataModel.getScore(n,i)) >= trainDataModel.min_score) {
							p += (s - trainDataModel.getScore_avg(n)) * nb_it->similarity;
							q += abs(nb_it->similarity);
							count ++;
							ssum += nb_it->similarity;
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

					if(count > 0) {
						double err = (it_i->second.score - s) / count;
						for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
							int n = nb_it->neighbor;
							double tmp;
							if((tmp = trainDataModel.getScore(n,i)) >= trainDataModel.min_score) {
								if((tmp - trainDataModel.getScore_avg(n)) > 0) {
									nb_it->similarity += err*ssum*0.1/(tmp - trainDataModel.getScore_avg(n));
								}
							}
						}
					}
					
					rmse += pow(it_i->second.score - s,2);
				}
			}

			rmse = sqrt(rmse/trainDataModel.n_rating);
			cout << "Loop " << lp+1 << " rmse: " << rmse << endl;
		}
	}

	void predictAll(UserModel &trainDataModel, UserModel &testDataModel, UserModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int u = testDataModel.user_0; u < testDataModel.user_0 + testDataModel.n_user; u++) {
			map<int,Rating> &rs = testDataModel.getRatings(u);
			for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
				int i = it_i->first;
				double s = predict(trainDataModel, u, i, it_i->second.timestamp);
				resultDataModel.entities[u].ratings[i] = Rating(u,i,s,it_i->second.timestamp);
			}
		}
	}

private:
	private:
	NeighborCollection *neighbor_cls;
	Pearson_Similarity similarity;
	int K_NEIGHBOR;
	int MIN_NEIGHBOR;
};

#endif

