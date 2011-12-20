#ifndef _USERITEMBASED_PREDICTOR_H
#define _USERITEMBASED_PREDICTOR_H

#include "Predictor.h"
#include "User.h"
#include "Item.h"
#include "Similarity.h"

class UserItemBased_Predictor: public Predictor {
public:
	UserItemBased_Predictor(Pearson_Similarity similarity,int k,int m_n):similarity(similarity),K_NEIGHBOR(k),MIN_NEIGHBOR(m_n){}

	void train(UserModel &userModel, ItemModel &itemModel) {
		u_neighbor_cls = new NeighborCollection[userModel.n_user+userModel.user_0];
		i_neighbor_cls = new NeighborCollection[itemModel.n_item+itemModel.item_0];
		similarity.similarity(userModel, u_neighbor_cls);
		similarity.similarity(itemModel, i_neighbor_cls);
	}

	double predict(UserModel &userModel, ItemModel &itemModel, int u, int i, long t)
	{
		double p,q,s;	
		double u_s,i_s,u_sim,i_sim;
		int count = 0;
		vector<Neighbor>::iterator nb_it;
		p = q = 0;
		u_sim = i_sim = 0;
		
		for(nb_it = i_neighbor_cls[i].begin(); nb_it != i_neighbor_cls[i].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = itemModel.getScore(u,n)) >= itemModel.min_score) {
				p += (s - itemModel.getScore_avg(n)) * nb_it->similarity;
				q += abs(nb_it->similarity);
				count ++;
				if(count >= K_NEIGHBOR) {
					break;
				}
			}
		}
		i_s = itemModel.getScore_avg(i);
		if(count >= MIN_NEIGHBOR && q > 0) {
			i_sim = q / count;
			i_s += p / q;
			if(i_s < userModel.min_score) i_s = userModel.min_score;
			else if(i_s > userModel.max_score) i_s = userModel.max_score;
		}

		p = q = 0;
		count = 0;
		for(nb_it = u_neighbor_cls[u].begin(); nb_it != u_neighbor_cls[u].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = userModel.getScore(n,i)) >= userModel.min_score) {
				p += (s - userModel.getScore_avg(n)) * nb_it->similarity;
				q += abs(nb_it->similarity);
				count ++;
				if(count >= K_NEIGHBOR) {
					break;
				}
			}
		}
		u_s = userModel.getScore_avg(u);
		if(count >= MIN_NEIGHBOR && q > 0) {
			u_sim = q / count;
			u_s += p / q;
			if(u_s < userModel.min_score) u_s = userModel.min_score;
			else if(u_s > userModel.max_score) u_s = userModel.max_score;
		}
		
		if(u_sim > 0 || i_sim > 0)
			s = (u_s * u_sim + i_s * i_sim) / (u_sim + i_sim);
		else
			s = u_s;
		return s;
	}

	void predictAll(UserModel &userModel, ItemModel &itemModel, UserModel &testDataModel, UserModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int u = testDataModel.user_0; u < testDataModel.user_0 + testDataModel.n_user; u++) {
			map<int,Rating> &rs = testDataModel.getRatings(u);
			for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
				int i = it_i->first;
				double s = predict(userModel, itemModel, u, i, it_i->second.timestamp);
				resultDataModel.entities[u].ratings[i] = Rating(u,i,s,it_i->second.timestamp);
			}
		}
	}

private:
	NeighborCollection *u_neighbor_cls;
	NeighborCollection *i_neighbor_cls;
	Pearson_Similarity similarity;
	int K_NEIGHBOR;
	int MIN_NEIGHBOR;
};
#endif

