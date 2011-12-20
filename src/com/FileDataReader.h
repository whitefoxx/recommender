#ifndef _FILEDATAREADER_H_
#define _FILEDATAREADER_H_

#include <string>
#include "Item.h"
#include "User.h"
#include "Rating.h"
#include "UserItem.h"
using namespace std;

class FileDataReader {
public:
	FileDataReader(string dataset_path): dataset_path(dataset_path) {}
	bool readData(UserModel &userModel);
	bool readData(ItemModel &itemModel);
	bool readData(RatingModel &ratingModel);
	bool readData(UserItemModel &user_itemModel);
	bool readUserSimRank(double *userRanks, string filepath);
	bool readUserComRank(double *userRanks, string filepath);

private:
	string dataset_path;
};

#endif