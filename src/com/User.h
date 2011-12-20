#ifndef _USER_H_
#define _USER_H_

#include "DataModel.h"

class UserModel: public DataModel {
public:
	UserModel(){}
	UserModel(int n_u,int n_i,int n_r):DataModel(n_u,n_i,n_r){
		entities = new Entity[n_user+1];
	}
	UserModel(int n_u,int n_i,int n_r,int u_0,int i_0):DataModel(n_u,n_i,n_r,u_0,i_0){
		entities = new Entity[n_user+1];
	}

	bool findRating(int u, int i, Rating &r) {
		map<int,Rating> &rs = entities[u].ratings;
		map<int,Rating>::iterator it;
		if((it = rs.find(i)) != rs.end()) {
			r = it->second;
			return true;
		}
		return false;
	}

	double getScore_avg(int u) {
		return entities[u].score_avg;
	}

	double getScore(int u, int i) {
		map<int,Rating> &rs = entities[u].ratings;
		map<int,Rating>::iterator it;
		if((it = rs.find(i)) != rs.end()) {
			return it->second.score;
		}
		return min_score - 1;
	}

	map<int,Rating>& getRatings(int u) {
		return entities[u].ratings;
	}
};

#endif