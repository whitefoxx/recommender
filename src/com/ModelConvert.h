#ifndef _MODELCONVERT_H_
#define _MODELCONVERT_H_

void convert(RatingBase *ratingModel, UserItemBase *user_itemModel, int n_rating, int n_user, int n_item)
{
	for(int i = 0; i < n_rating; i++) {
		long id = ratingModel[i].user_id * n_item + ratingModel[i].item_id;
		user_itemModel[id] = UserItemBase(id, ratingModel[i]);
	}
}

#endif

