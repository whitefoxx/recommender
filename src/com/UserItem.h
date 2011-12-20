#ifndef _USERITEM_H_
#define _USERITEM_H_

#include "Rating.h"
#include "DataModel.h"

class UserItem {
public:
	UserItem(long id, Rating r):id(id),rating(r){}
	UserItem():id(-1),rating(Rating()){}
	long id;
	Rating rating;
};

class UserItemModel: public DataModel {
public:
	UserItemModel():userItemRatings(NULL){}
	UserItemModel(int n_u,int n_i,int n_r):DataModel(n_u,n_i,n_r){
		userItemRatings = new UserItem[n_user * n_item + n_item];
	}
	UserItemModel(int n_u,int n_i,int n_r,int u_0,int i_0):DataModel(n_u,n_i,n_r,u_0,i_0){
		userItemRatings = new UserItem[n_user * n_item + n_item];
	}
	UserItem *userItemRatings;
};

#endif