#ifndef _FILL_PREDICTOR_H_
#define _FILL_PREDICTOR_H_

#include "../com/Predictor.h"
#include "../com/User.h"
#include "../com/Item.h"
#include "../com/UserBased_Predictor.h"
#include <vector>
#include <set>

using namespace std;

class FillObj {
public:
	FillObj(int id):id(id){}
	int id;
};

class Fill_Predictor {
public:
	//Fill_Predictor(){}

	void fill(UserBased_Predictor &predictor, UserModel &model, vector<FillObj> *u_fobjs) {
		int u0 = model.user_0;
		int un = model.user_0 + model.n_user - 1;
		predictor.train(model);
		for(int u = u0; u <= un; u++) {
			for(vector<FillObj>::iterator it = u_fobjs[u].begin(); it != u_fobjs[u].end(); it++) {
				int i = it->id;
				double s = predictor.predict(model,u,i,0);
				model.entities[u].ratings[i] = Rating(u,i,s,0);
			}
		}
	}

	int cal_fill_objs(UserModel &userModel, NeighborCollection *item_nbs, vector<FillObj> *u_fobjs, int min_com) {
		int u0 = userModel.user_0;
		int un = userModel.user_0 + userModel.n_user - 1;
		int count = 0;
		set<int> tmp_set;
		for(int u = u0; u <= un; u++) {
			map<int,Rating> &rs = userModel.getRatings(u);
			tmp_set.clear();
			for(map<int,Rating>::iterator it = rs.begin(); it != rs.end(); it++) {
				int i = it->first;
				for(vector<Neighbor>::iterator it2 = item_nbs[i].begin(); it2 != item_nbs[i].end(); it2++) {
					if(it2->common_count >= min_com) {
						int n = it2->neighbor;
						if(rs.find(n) == rs.end() && tmp_set.find(n) == tmp_set.end()) {
							u_fobjs[u].push_back(FillObj(n));
							tmp_set.insert(n);
							count ++;
						}
						else break;
					}
				}
			}
		}
		return count;
	}

	int cal_fill_objs(UserModel &userModel, NeighborCollection *item_nbs, vector<FillObj> *u_fobjs, double min_sim) {
		int u0 = userModel.user_0;
		int un = userModel.user_0 + userModel.n_user - 1;
		int count = 0;
		set<int> tmp_set;
		for(int u = u0; u <= un; u++) {
			map<int,Rating> &rs = userModel.getRatings(u);
			tmp_set.clear();
			for(map<int,Rating>::iterator it = rs.begin(); it != rs.end(); it++) {
				int i = it->first;
				for(vector<Neighbor>::iterator it2 = item_nbs[i].begin(); it2 != item_nbs[i].end(); it2++) {
					if(it2->similarity >= min_sim) {
						int n = it2->neighbor;
						if(rs.find(n) == rs.end() && tmp_set.find(n) == tmp_set.end()) {
							u_fobjs[u].push_back(FillObj(n));
							tmp_set.insert(n);
							count ++;
						}
						else break;
					}
				}
			}
		}
		return count;
	}
};

#endif

