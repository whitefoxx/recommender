#ifndef _ITEM_H_
#define _ITEM_H_

#include "DataModel.h"

class ItemModel: public DataModel {
public:
	ItemModel(){}
	ItemModel(int n_u,int n_i,int n_r):DataModel(n_u,n_i,n_r){
		entities = new Entity[n_item+1];
	}
	ItemModel(int n_u,int n_i,int n_r,int u_0,int i_0):DataModel(n_u,n_i,n_r,u_0,i_0){
		entities = new Entity[n_item+1];
	}

	bool findRating(int u, int i, Rating &r) {
		map<int,Rating> &rs = entities[i].ratings;
		map<int,Rating>::iterator it;
		if((it = rs.find(u)) != rs.end()) {
			r = it->second;
			return true;
		}
		return false;
	}

	double getScore_avg(int i) {
		return entities[i].score_avg;
	}

	double getScore(int u, int i) {
		map<int,Rating> &rs = entities[i].ratings;
		map<int,Rating>::iterator it;
		if((it = rs.find(u)) != rs.end()) {
			return it->second.score;
		}
		return min_score - 1;
	}

	map<int,Rating>& getRatings(int i) {
		return entities[i].ratings;
	}
};

#endif