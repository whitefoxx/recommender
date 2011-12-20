#include "FileDataReader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

bool FileDataReader::readData(UserModel &userModel)
{
	ifstream datafile(dataset_path.c_str(), ifstream::in);
	if(!datafile.good())
		return false;
	int user,item;
	int user_0 = userModel.user_0;
	int item_0 = userModel.item_0;
	int user_n = userModel.user_0 + userModel.n_user - 1;
	double score;
	long timestamp;
	while(datafile >> user >> item >> score >> timestamp) {
		user -= user_0;
		item -= item_0;
		userModel.entities[user].id = user;
		userModel.entities[user].ratings[item] = Rating(user,item,score,timestamp);
		userModel.entities[user].score_avg += score;
		userModel.entities[user].rating_count ++;
	}
	for(int i = user_0; i <= user_n; i++) {
		if(userModel.entities[i].rating_count > 0)
			userModel.entities[i].score_avg /= userModel.entities[i].rating_count;
	}
	return true;
}

bool FileDataReader::readData(ItemModel &itemModel)
{
	ifstream datafile(dataset_path.c_str(), ifstream::in);
	if(!datafile.good())
		return false;
	int user,item;
	int user_0 = itemModel.user_0;
	int item_0 = itemModel.item_0;
	int item_n = itemModel.item_0 + itemModel.n_item - 1;
	double score;
	long timestamp;
	while(datafile >> user >> item >> score >> timestamp) {
		user -= user_0;
		item -= item_0;
		itemModel.entities[item].id = item;
		itemModel.entities[item].ratings[user] = Rating(user,item,score,timestamp);
		itemModel.entities[item].score_avg += score;
		itemModel.entities[item].rating_count ++;
	}
	for(int i = item_0; i <= item_n; i++) {
		if(itemModel.entities[i].rating_count > 0)
			itemModel.entities[i].score_avg /= itemModel.entities[i].rating_count;
	}
	return true;
}

bool FileDataReader::readData(RatingModel &ratingModel)
{
	ifstream datafile(dataset_path.c_str(), ifstream::in);
	if(!datafile.good())
		return false;
	int user,item;
	double score;
	long timestamp;
	long i = 0;
	while(datafile >> user >> item >> score >> timestamp) {
		user -= ratingModel.user_0;
		item -= ratingModel.item_0;
		ratingModel.ratings[i++] = Rating(user,item,score,timestamp);
	}
	return true;
}

bool FileDataReader::readData(UserItemModel &userItemModel)
{
	ifstream datafile(dataset_path.c_str(), ifstream::in);
	if(!datafile.good())
		return false;
	int user,item;
	double score;
	long timestamp;
	while(datafile >> user >> item >> score >> timestamp) {
		user -= userItemModel.user_0;
		item -= userItemModel.item_0;
		long id = user * userItemModel.n_item + item;
		userItemModel.userItemRatings[id] = UserItem(id, Rating(user,item,score,timestamp));
	}
	
	return true;
}

bool FileDataReader::readUserSimRank(double *userRanks, string filepath)
{
	ifstream fin(filepath.c_str(),ifstream::in);
	if(!fin.good())
		return false;
	int u;
	double rank;
	while(fin >> u >> rank) {
		userRanks[u] = rank;
	}
	fin.close();
	return true;
}

bool FileDataReader::readUserComRank(double *userRanks, string filepath)
{
	ifstream fin(filepath.c_str(),ifstream::in);
	if(!fin.good())
		return false;
	int u;
	double rank;
	while(fin >> u >> rank) {
		userRanks[u] = rank;
	}
	fin.close();
	return true;
}

