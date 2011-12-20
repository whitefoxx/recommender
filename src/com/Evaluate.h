#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include "User.h"
#include "Item.h"
#include "Rating.h"
#include <cmath>

double evl_rmse(RatingModel testModel,RatingModel resultModel)
{
	double sum = 0;
	int count = 0;
	double rmse;
	for(int i = 0; i < testModel.n_rating; i++) {
		sum += pow(testModel.ratings[i].score - resultModel.ratings[i].score, 2);
		count ++;
	}
	rmse = sqrt(sum/count);
	return rmse;
}

double evl_rmse(UserModel testModel,UserModel resultModel)
{
	double sum = 0;
	int count = 0;
	double rmse;
	for(int u = testModel.user_0; u < testModel.n_user + testModel.user_0; u++) {
		map<int,Rating> &rs_1 = testModel.getRatings(u);
		map<int,Rating> &rs_2 = resultModel.getRatings(u);
		map<int,Rating>::iterator it2;
		for(map<int,Rating>::iterator it1 = rs_1.begin(); it1 != rs_1.end(); it1++) {
			int item = it1->first;
			if((it2 = rs_2.find(item)) != rs_2.end()) {
				sum += pow(it1->second.score - it2->second.score, 2);
				count ++;
			}
		}
	}
	rmse = sqrt(sum/count);
	return rmse;
}

double evl_rmse(ItemModel testModel,ItemModel resultModel)
{
	double sum = 0;
	int count = 0;
	double rmse;
	for(int i = testModel.item_0; i < testModel.n_item + testModel.item_0; i++) {
		map<int,Rating> &rs_1 = testModel.getRatings(i);
		map<int,Rating> &rs_2 = resultModel.getRatings(i);
		map<int,Rating>::iterator it2;
		for(map<int,Rating>::iterator it1 = rs_1.begin(); it1 != rs_1.end(); it1++) {
			int user = it1->first;
			if((it2 = rs_2.find(user)) != rs_2.end()) {
				sum += pow(it1->second.score - it2->second.score, 2);
				count ++;
			}
		}
	}
	rmse = sqrt(sum/count);
	return rmse;
}

double evl_mae(UserModel testModel,UserModel resultModel)
{
	double sum = 0;
	int count = 0;
	double mae;
	for(int u = testModel.user_0; u < testModel.n_user + testModel.user_0; u++) {
		map<int,Rating> &rs_1 = testModel.getRatings(u);
		map<int,Rating> &rs_2 = resultModel.getRatings(u);
		map<int,Rating>::iterator it2;
		for(map<int,Rating>::iterator it1 = rs_1.begin(); it1 != rs_1.end(); it1++) {
			int item = it1->first;
			if((it2 = rs_2.find(item)) != rs_2.end()) {
				sum += abs(it1->second.score - it2->second.score);
				count ++;
			}
		}
	}
	mae = sum/count;
	return mae;
}

double evl_mae(ItemModel testModel,ItemModel resultModel)
{
	double sum = 0;
	int count = 0;
	double mae;
	for(int i = testModel.item_0; i < testModel.n_item + testModel.item_0; i++) {
		map<int,Rating> &rs_1 = testModel.getRatings(i);
		map<int,Rating> &rs_2 = resultModel.getRatings(i);
		map<int,Rating>::iterator it2;
		for(map<int,Rating>::iterator it1 = rs_1.begin(); it1 != rs_1.end(); it1++) {
			int user = it1->first;
			if((it2 = rs_2.find(user)) != rs_2.end()) {
				sum += abs(it1->second.score - it2->second.score);
				count ++;
			}
		}
	}
	mae = sum/count;
	return mae;
}

double evl_mae(RatingModel testModel,RatingModel resultModel)
{
	double sum = 0;
	int count = 0;
	double mae;
	for(int i = 0; i < testModel.n_rating; i++) {
		sum += abs(testModel.ratings[i].score - resultModel.ratings[i].score);
		count ++;
	}
	mae = sum/count;
	return mae;
}

#endif

