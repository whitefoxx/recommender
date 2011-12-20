void test_ItemBased_Predictor()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/1/train/base";
	string test_data = "E:/data/resys/corpus/1/test/u1.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	ItemModel trainModel(n_user,n_item,n_rating);
	ItemModel testModel(n_user,n_item,20000);
	ItemModel resultModel(n_user,n_item,20000);

	double *userRanks = new double[n_user+1];
	string filepath = "E:/data/resys/corpus/1/user_com_ranks.txt";
	train_reader.readUserComRank(userRanks,filepath);

	if(train_reader.readData(trainModel) && test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.0;
		int shrink_parameter = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		ItemBased_Predictor predictor = ItemBased_Predictor(similarity,30,0);
		begin = clock();
		//predictor.train(trainModel);
		predictor.train(trainModel,userRanks);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(trainModel,testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}


void test_Iterate_UserBased_Predictor()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/1/train/base";
	string test_data = "E:/data/resys/corpus/1/test/u1.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating);
	UserModel testModel(n_user,n_item,20000);
	UserModel resultModel(n_user,n_item,20000);
	if(train_reader.readData(trainModel) && test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.00001;
		int shrink_parameter = 25;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		UserBased_Predictor predictor = UserBased_Predictor(30,0);
		begin = clock();
		predictor.train(trainModel);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(trainModel,testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
	}
}

void test_Fill_Predictor()
{
	string train_data = "E:/data/resys/corpus/1/train/base";
	string test_data = "E:/data/resys/corpus/1/test/u1.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel userModel(n_user,n_item,n_rating);
	UserModel testModel(n_user,n_item,20000);
	UserModel resultModel(n_user,n_item,20000);
	ItemModel itemModel(n_user,n_item,n_rating);
	if(train_reader.readData(userModel) && train_reader.readData(itemModel) 
		&& test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.00001;
		int shrink_parameter = 25;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		UserBased_Predictor predictor = UserBased_Predictor(similarity,30,0);

		min_com = 30;
		min_sim = 0.3;
		shrink_parameter = 40;
		NeighborCollection *item_nbs = new NeighborCollection[n_item+1];
		vector<FillObj> *u_fobjs = new vector<FillObj>[n_user+1];
		Similarity_Shrinking shrinker2(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity2(shrinker2);
		similarity2.similarity(itemModel,item_nbs,1);
		//similarity2.similarity(itemModel,item_nbs,1);
		Fill_Predictor fill_predictor;
		//int fill_count = fill_predictor.cal_fill_objs(userModel,item_nbs,u_fobjs,min_com);
		int fill_count = fill_predictor.cal_fill_objs(userModel,item_nbs,u_fobjs,min_sim);
		cout << fill_count << endl;
		fill_predictor.fill(predictor,userModel,u_fobjs);

		predictor.train(userModel);
		predictor.predictAll(userModel,testModel,resultModel);
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}

void test_UserItemBased_Predictor()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/1/train/base";
	string test_data = "E:/data/resys/corpus/1/test/u1.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel userModel(n_user,n_item,n_rating);
	ItemModel itemModel(n_user,n_item,n_rating);
	UserModel testModel(n_user,n_item,20000);
	UserModel resultModel(n_user,n_item,20000);
	if(train_reader.readData(userModel) && train_reader.readData(itemModel) && test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.05;
		int shrink_parameter = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		UserItemBased_Predictor predictor = UserItemBased_Predictor(similarity,15,0);
		begin = clock();
		predictor.train(userModel,itemModel);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(userModel,itemModel,testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}

void printUserSim()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/1/train/base";
	FileDataReader train_reader = FileDataReader(train_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating);
	if(train_reader.readData(trainModel)) {
		int min_com = 0;
		double min_sim = 0.0;
		int shrink_parameter = 30;
		int k_neighbors = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		NeighborCollection *neighbor_cls;
		neighbor_cls = new NeighborCollection[trainModel.n_user+trainModel.user_0];
		similarity.similarity(trainModel, neighbor_cls);

		string sim_file = "E:/data/resys/corpus/1/user_sims.txt";
		ofstream fout(sim_file.c_str());
		vector<Neighbor>::iterator nb_it;
		for(int u = trainModel.user_0; u < trainModel.user_0 + trainModel.n_user; u++) {
			double sum = 0;
			int count = 0;
			for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
				int n = nb_it->neighbor;
				double sim = nb_it->similarity;
				sum += sim;
				count ++;
				if(count >= k_neighbors)
					break;
			}
			count = 0;
			if(sum > 0) {
				for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
					int n = nb_it->neighbor;
					double sim = nb_it->similarity;
					sim = sim/sum;
					fout << u << " " << n << " " << sim << endl;
					count ++;
					if(count >= k_neighbors)
						break;
				}
			}
		}
		fout.close();
	}
}

void printUserCom()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/1/train/base";
	FileDataReader train_reader = FileDataReader(train_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating);
	if(train_reader.readData(trainModel)) {
		int min_com = 0;
		double min_sim = 0.0;
		int shrink_parameter = 30;
		int k_neighbors = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		NeighborCollection *neighbor_cls;
		neighbor_cls = new NeighborCollection[trainModel.n_user+trainModel.user_0];
		similarity.similarity(trainModel, neighbor_cls);

		string sim_file = "E:/data/resys/corpus/1/user_coms.txt";
		ofstream fout(sim_file.c_str());
		vector<Neighbor>::iterator nb_it;
		for(int u = trainModel.user_0; u < trainModel.user_0 + trainModel.n_user; u++) {
			int sum = 0;
			//int count = 0;
			for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
				sum += nb_it->common_count;
				//count ++;
				//if(count >= k_neighbors)
					//break;
			}
			//count = 0;
			if(sum > 0) {
				for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
					int n = nb_it->neighbor;
					double sim = nb_it->common_count*1.0/sum;
					fout << u << " " << n << " " << sim << endl;
					//count ++;
					//if(count >= k_neighbors)
						//break;
				}
			}
		}
		fout.close();
	}
}


	void predictAll(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		double *userRanks, UserModel &testDataModel, UserModel &resultDataModel)
	{
		//train(trainDataModel);
		for(int u = testDataModel.user_0; u < testDataModel.user_0 + testDataModel.n_user; u++) {
			map<int,Rating> &rs = testDataModel.getRatings(u);
			for(map<int,Rating>::iterator it_i = rs.begin(); it_i != rs.end(); it_i++) {
				int i = it_i->first;
				double s = predict(trainDataModel, neighbor_cls, userRanks, u, i, it_i->second.timestamp);
				resultDataModel.entities[u].ratings[i] = Rating(u,i,s,it_i->second.timestamp);
			}
		}
	}

double predict(UserModel &trainDataModel, NeighborCollection *neighbor_cls, 
		double *userRanks, int u, int i, long t)
	{
		double p,q,s;		
		int count = 0;
		vector<Neighbor>::iterator nb_it;
		p = q = 0;
		for(nb_it = neighbor_cls[u].begin(); nb_it != neighbor_cls[u].end(); nb_it++) {
			int n = nb_it->neighbor;
			if((s = trainDataModel.getScore(n,i)) >= trainDataModel.min_score) {
					double w = 2 * nb_it->similarity*userRanks[n]/(nb_it->similarity+userRanks[n]);
					p += (s - trainDataModel.getScore_avg(n)) * (w);
					q += abs(w);
					count ++;
					if(count >= K_NEIGHBOR) {
						break;
					}

			}
		}
		s = trainDataModel.getScore_avg(u);
		if(count >= MIN_NEIGHBOR && q > 0) {
			s += p / q;
			if(s < trainDataModel.min_score) s = trainDataModel.min_score;
			else if(s > trainDataModel.max_score) s = trainDataModel.max_score;
		}
		return s;
	}