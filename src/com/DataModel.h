#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <map>
using namespace std;

struct Rating {
public:
	Rating(int u_id, int i_id, double score, long time):
	  user_id(u_id),item_id(i_id),score(score),timestamp(time) {}
	  Rating(){}
	int user_id;
	int item_id;
	double score;
	long timestamp;
};

class Entity {
public:
	Entity():rating_count(0),score_avg(0){}
	int id;
	int rating_count;
	double score_avg;
	map<int,Rating> ratings;
};

class DataModel {
public:
	DataModel():n_user(0),n_item(0),n_rating(0),user_0(0),item_0(0),entities(NULL),min_score(1),max_score(5){}
	DataModel(int n_u,int n_i,int n_r):n_user(n_u),n_item(n_i),n_rating(n_r),user_0(0),item_0(0),entities(NULL),
		min_score(1),max_score(5){}
	DataModel(int n_u,int n_i,int n_r,int u_0,int i_0):n_user(n_u),n_item(n_i),n_rating(n_r),user_0(u_0),
		item_0(i_0),entities(NULL),min_score(1),max_score(5){}
	DataModel(int n_u,int n_i,int n_r,double min_s,double max_s):n_user(n_u),n_item(n_i),n_rating(n_r),user_0(0),
		item_0(0),entities(NULL),min_score(min_s),max_score(max_s){}
	DataModel(int n_u,int n_i,int n_r,int u_0,int i_0,double min_s,double max_s):n_user(n_u),n_item(n_i),
		n_rating(n_r),user_0(u_0),item_0(i_0),entities(NULL),min_score(min_s),max_score(max_s){}
	int n_user;
	int n_item;
	int n_rating;
	int user_0;
	int item_0;
	double min_score;
	double max_score;
	Entity *entities;
};

#endif

