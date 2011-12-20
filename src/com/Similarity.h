#ifndef _SIMILARITY_H_
#define _SIMILARITY_H_

#include "User.h"
#include "Item.h"
#include <vector>
#include <algorithm>

struct Neighbor {
	int neighbor;
	double similarity;
	int common_count;
	Neighbor():common_count(0){}
	Neighbor(int nb,double sim,int com_c):neighbor(nb),similarity(sim),common_count(com_c){}
};

inline bool compare_des_by_com(Neighbor a, Neighbor b)
{
	return a.common_count > b.common_count;
}
inline bool compare_asc_by_com(Neighbor a, Neighbor b)
{
	return a.common_count < b.common_count;
}
inline bool compare_des_by_sim(Neighbor a, Neighbor b)
{
	return a.similarity > b.similarity;
}
inline bool compare_asc_by_sim(Neighbor a, Neighbor b)
{
	return a.similarity < b.similarity;
}

class NeighborCollection {
public:
	NeighborCollection(){}
	inline void insert(Neighbor nb) {
		neighbors.push_back(nb);
	}

	inline vector<Neighbor>::iterator begin() {
		return neighbors.begin();
	}

	inline vector<Neighbor>::iterator end() {
		return neighbors.end();
	}

	inline void sort_by_sim(bool des=true) {
		if(des)
			std::sort(neighbors.begin(), neighbors.end(), compare_des_by_sim);
		else
			std::sort(neighbors.begin(), neighbors.end(), compare_asc_by_sim);
	}

	inline void sort_by_com(bool des=true) {
		if(des)
			std::sort(neighbors.begin(), neighbors.end(), compare_des_by_com);
		else
			std::sort(neighbors.begin(), neighbors.end(), compare_asc_by_com);
	}

	vector<Neighbor> neighbors;
};

class Similarity_Shrinking {
public:
	Similarity_Shrinking():min_com(0),min_sim(-1),shrink_parameter(0) {}
	Similarity_Shrinking(int min_com,double min_sim,int sh_p):
	  min_com(min_com),min_sim(min_sim),shrink_parameter(sh_p) {}
	int min_com;
	double min_sim;
	int shrink_parameter;
	inline double shrinking(double sim, int com) {
		if(com >= min_com) {
			//double association = com * 1.0 / (com+shrink_parameter);
			//sim = 2 * sim * association / (sim + association);
			sim = sim * (com * 1.0/(com+shrink_parameter));
			if(sim >= min_sim)
				return sim;
		}
		return min_sim - 1.0;
	}
};

class Pearson_Similarity {
public:
	Pearson_Similarity(Similarity_Shrinking shrinker):shrinker(shrinker){}
	void similarity(UserModel &userModel, NeighborCollection *user_neighbors, int sort_by);
	void similarity(ItemModel &itemModel, NeighborCollection *item_neighbors, int sort_by);
	void similarity(ItemModel &itemModel, double *user_com_ranks, NeighborCollection *item_neighbors, int sort_by);
	Similarity_Shrinking shrinker;
private:
	inline void cal_similarity(DataModel &model, NeighborCollection *neighbors, int first_id, int last_id);
	inline void cal_similarity(DataModel &model, double *user_com_ranks, NeighborCollection *neighbors, int first_id, int last_id);
};

/* TODO: 还可以改进 */
void Pearson_Similarity::cal_similarity(DataModel &model, NeighborCollection *neighbors, int first_id, int last_id)
{
	int item, com;
	double score1, score2;
	double p, q1, q2, sim;
	map<int,Rating>::iterator it1,it2;
	for(int u1 = first_id; u1 <= last_id; u1++) {
		for(int u2 = u1+1; u2 <= last_id; u2++) {
			p = q1 = q2 = 0;
			com = 0;
			for(it1 = model.entities[u1].ratings.begin();
				it1 != model.entities[u1].ratings.end(); it1++) {
					item = it1->first;
					if((it2 = model.entities[u2].ratings.find(item)) !=
						model.entities[u2].ratings.end()) {
							score1 = it1->second.score;
							score2 = it2->second.score;
							p += (score1 - model.entities[u1].score_avg) *
								 (score2 - model.entities[u2].score_avg);
							q1 += pow(score1 - model.entities[u1].score_avg,2);
							q2 += pow(score2 - model.entities[u2].score_avg,2);
							com ++;
					}
			}
			if(q1 > 0 && q2 > 0) {
				sim = p / sqrt(q1 * q2);
				if((sim=shrinker.shrinking(sim,com)) >= shrinker.min_sim) {
					neighbors[u1].insert(Neighbor(u2,sim,com));
					neighbors[u2].insert(Neighbor(u1,sim,com));
				}
			}
		}
	}
}

void Pearson_Similarity::cal_similarity(DataModel &model, double *user_com_ranks, NeighborCollection *neighbors, int first_id, int last_id)
{
	int user, com;
	double score1, score2;
	double p, q1, q2, sim;
	map<int,Rating>::iterator it1,it2;
	for(int i1 = first_id; i1 <= last_id; i1++) {
		for(int i2 = i1+1; i2 <= last_id; i2++) {
			p = q1 = q2 = 0;
			com = 0;
			for(it1 = model.entities[i1].ratings.begin();
				it1 != model.entities[i1].ratings.end(); it1++) {
					user = it1->first;
					if((it2 = model.entities[i2].ratings.find(user)) !=
						model.entities[i2].ratings.end()) {
							score1 = it1->second.score;
							score2 = it2->second.score;
							p += (score1 - model.entities[i1].score_avg) *
								 (score2 - model.entities[i2].score_avg) * user_com_ranks[user];
							q1 += pow((score1 - model.entities[i1].score_avg) * user_com_ranks[user],2);
							q2 += pow((score2 - model.entities[i2].score_avg) * user_com_ranks[user],2);
							com ++;
					}
			}
			if(q1 > 0 && q2 > 0) {
				sim = p / sqrt(q1 * q2);
				if((sim=shrinker.shrinking(sim,com)) >= shrinker.min_sim) {
					neighbors[i1].insert(Neighbor(i2,sim,com));
					neighbors[i2].insert(Neighbor(i1,sim,com));
				}
			}
		}
	}
}

void Pearson_Similarity::similarity(UserModel &userModel, NeighborCollection *user_neighbors, int sort_by=1)
{
	int first_id = userModel.user_0;
	int last_id = userModel.n_user + userModel.user_0 - 1;
	cal_similarity(userModel, user_neighbors, first_id, last_id);
	if(sort_by == 1) {
		for(int u = first_id; u <= last_id; u++)
			user_neighbors[u].sort_by_sim();
	}
	else {
		for(int u = first_id; u <= last_id; u++)
			user_neighbors[u].sort_by_com();
	}
}

void Pearson_Similarity::similarity(ItemModel &itemModel, NeighborCollection *item_neighbors, int sort_by=1)
{
	int first_id = itemModel.item_0;
	int last_id = itemModel.n_item + itemModel.item_0 - 1;
	cal_similarity(itemModel, item_neighbors, first_id, last_id);
	if(sort_by == 1) {
		for(int u = first_id; u <= last_id; u++)
			item_neighbors[u].sort_by_sim();
	}
	else {
		for(int u = first_id; u <= last_id; u++)
			item_neighbors[u].sort_by_com();
	}
}

void Pearson_Similarity::similarity(ItemModel &itemModel, double *user_com_ranks, NeighborCollection *item_neighbors, int sort_by=1)
{
	int first_id = itemModel.item_0;
	int last_id = itemModel.n_item + itemModel.item_0 - 1;
	cal_similarity(itemModel, user_com_ranks, item_neighbors, first_id, last_id);
	if(sort_by == 1) {
		for(int u = first_id; u <= last_id; u++)
			item_neighbors[u].sort_by_sim();
	}
	else {
		for(int u = first_id; u <= last_id; u++)
			item_neighbors[u].sort_by_com();
	}
}

#endif

