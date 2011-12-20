#ifndef _ITEMBASED_PREDICTOR_H
#define _ITEMBASED_PREDICTOR_H

#include "Predictor.h"
#include "Item.h"
#include "Similarity.h"

class ItemBased_Predictor: public Predictor {
public:
	ItemBased_Predictor(Pearson_Similarity similarity,int k,int m_n):similarity(similarity),K_NEIGHBOR(k),MIN_NEIGHBOR(m_n){}

	void train(ItemModel &trainDataModel) {
		neighbor_cls = new NeighborCollection[trainDataModel.n_item+trainDataModel.item_0];
		similarity.similarity(trainDataModel, neighbor_cls);
	}

	void train(ItemModel &trainDataModel, double *userRanks) {
		neighbor_cls = new NeighborCollection[trainDataModel.n_item+trainDataModel.item_0];
		similarity.similarity(trainDataModel, userRanks, neighbor_cls);
	}

	double predict(ItemModel &trainDataModel, int u, int i, long t)
	{
		double p,q,s;		
		int count = 0;
		vector<Neighbor>::iterator nb_it;
		p = q = 0;
		for(nb_it = neighbor_cls[i].begin(); nb_it != neighbor_cls[i].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = trainDataModel.getScore(u,n)) >= trainDataModel.min_score) {
				p += (s - trainDataModel.getScore_avg(n)) * nb_it->similarity* nb_it->similarity;
				q += abs(nb_it->similarity* nb_it->similarity);
				count ++;
				if(count >= K_NEIGHBOR) {
					break;
				}
			}
		}
		s = trainDataModel.getScore_avg(i);
		if(count >= MIN_NEIGHBOR && q > 0) {
			s += p / q;
			if(s < trainDataModel.min_score) s = trainDataModel.min_score;
			else if(s > trainDataModel.max_score) s = trainDataModel.max_score;
		}
		return s;
	}

	void predictAll(ItemModel &trainDataModel, ItemModel &testDataModel, ItemModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int i = testDataModel.item_0; i < testDataModel.item_0 + testDataModel.n_item; i++) {
			map<int,Rating> &rs = testDataModel.getRatings(i);
			for(map<int,Rating>::iterator it_u = rs.begin(); it_u != rs.end(); it_u++) {
				int u = it_u->first;
				double s = predict(trainDataModel, u, i, it_u->second.timestamp);
				resultDataModel.entities[i].ratings[u] = Rating(u,i,s,it_u->second.timestamp);
			}
		}
	}

private:
	NeighborCollection *neighbor_cls;
	Pearson_Similarity similarity;
	int K_NEIGHBOR;
	int MIN_NEIGHBOR;
};
#endif

