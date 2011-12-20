#include "../com/FileDataReader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../com/Similarity.h"
#include "../com/UserBased_Predictor.h"
#include "../com/ItemBased_Predictor.h"
#include "../com/UserItemBased_Predictor.h"
#include "../com/MF_Predictor.h"
#include "../com/Evaluate.h"
#include "../com/UserTrust.h"

//#include "../test/Fill_Predictor.h"
//#include "../test/Iterate_UserBased_Predictor.h"

void test_FileDataReader()
{
	string data_path = "E:/data/resys/corpus/1/train/base";
	FileDataReader data_reader = FileDataReader(data_path);

	int user_0, user_n;
	user_0 = 1;
	user_n = 943;
	UserModel userModel(943,1682,80000,1,1);
	if(data_reader.readData(userModel)) {
		for(int i = user_0; i < 10; i++) {
			cout << userModel.entities[i].id << " " << userModel.entities[i].rating_count << " "
				<< userModel.entities[i].score_avg << endl;
		}
	}

	string user_sim_file = "E:/data/resys/corpus/1/user_sims.txt";
	NeighborCollection *user_neighbors = new NeighborCollection[user_n+user_0];
	int min_com = 10;
	double min_sim = 0;
	int shrink_parameter = 10;
	Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
	Pearson_Similarity similarity(shrinker);
	similarity.similarity(userModel, user_neighbors);
	FILE* out = fopen(user_sim_file.c_str(),"wb");
	vector<Neighbor>::iterator nb_it;
	for(int u = user_0; u < user_0 + user_n; u++) {
		for(nb_it = user_neighbors[u].begin(); nb_it != user_neighbors[u].end(); nb_it++) {
			fprintf(out,"%d\t%5d\t%.6lf\t%d\n",u,nb_it->neighbor,nb_it->similarity,nb_it->common_count);
		}
	}
	fclose(out);
}

void test_UserBased_Predictor()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/2/train/base";
	string test_data = "E:/data/resys/corpus/2/test/u2.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating,1,1);
	UserModel testModel(n_user,n_item,20000,1,1);
	UserModel resultModel(n_user,n_item,20000,1,1);

	/*double *userRanks = new double[n_user+1];
	string filepath = "E:/data/resys/corpus/1/user_ranks.txt";
	train_reader.readUserSimRank(userRanks,filepath);*/

	if(train_reader.readData(trainModel) && test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.0;
		int shrink_parameter = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		NeighborCollection *neighbor_cls;
		neighbor_cls = new NeighborCollection[trainModel.n_user+trainModel.user_0];		
		UserBased_Predictor predictor = UserBased_Predictor(30,0);
		begin = clock();
		//predictor.train(trainModel);
		similarity.similarity(trainModel, neighbor_cls);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(trainModel,neighbor_cls,testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}

void test_UserTrustBased_Predictor()
{
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/2/train/base";
	string test_data = "E:/data/resys/corpus/2/test/u2.test";
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating,1,1);
	UserModel testModel(n_user,n_item,20000,1,1);
	UserModel resultModel(n_user,n_item,20000,1,1);

	double **trust;
	trust = new double*[n_user+1];
	for(int i = 1; i <= n_user; i++)
		trust[i] = new double[n_user+1];
	string fname = "E:/data/resys/corpus/2/user_trust.txt";
	ifstream fin(fname.c_str());
	int i,j;
	double sim;
	while(fin >> i) {
		for(int j = 1; j <= n_user; j++)
			fin >> trust[i][j] >> sim;
	}

	if(train_reader.readData(trainModel) && test_reader.readData(testModel)) {
		int min_com = 0;
		double min_sim = 0.0;
		int shrink_parameter = 30;
		Similarity_Shrinking shrinker(min_com,min_sim,shrink_parameter);
		Pearson_Similarity similarity(shrinker);
		NeighborCollection *neighbor_cls;
		neighbor_cls = new NeighborCollection[trainModel.n_user+trainModel.user_0];		
		UserBased_Predictor predictor = UserBased_Predictor(30,0);
		begin = clock();
		//predictor.train(trainModel);
		similarity.similarity(trainModel, neighbor_cls);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(trainModel,neighbor_cls,trust,testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}

void calUserTrust(){
	clock_t begin,end;
	string train_data = "E:/data/resys/corpus/2/train/base";
	FileDataReader train_reader = FileDataReader(train_data);
	int n_user, n_item, n_rating;
	n_user = 943;
	n_item = 1682;
	n_rating = 80000;
	UserModel trainModel(n_user,n_item,n_rating,1,1);
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

		double **sim;
		sim = new double*[n_user+1];
		for(int i = 0; i <= n_user; i++) {
			sim[i] = new double[n_user+1];
			memset(sim[i],0,sizeof(double)*(n_user+1));
		}
		double **trust;
		trust = new double*[n_user+1];
		for(int i = 0; i <= n_user; i++)
			trust[i] = new double[n_user+1];
		
		for(int i = 1; i <= n_user; i++) {
			vector<Neighbor>::iterator it;
			for(it = neighbor_cls[i].begin(); it != neighbor_cls[i].end(); it++) {
				int nb = it->neighbor;
				sim[i][nb] = it->similarity;
			}
			sim[i][i] = 1;
		}

		for(int i = 1; i <= n_user; i++) {
			double sum = 0;
			for(int j = 1; j <= n_user; j++) {
				sum += sim[j][i];
			}
			sum -= 1;
			for(int j = 1; j <= n_user; j++) {
				if(i == j){
					trust[j][i] = 1;
					continue;
				}
				if(sum > 0)
					trust[j][i] = sim[j][i]/sum;
				else
					trust[j][i] = 0;
				sim[j][i] = trust[j][i];
			}
		}

		cal_trust(sim, trust, 1, n_user);
		string trust_f = "E:/data/resys/corpus/2/user_trust.txt";
		ofstream out(trust_f.c_str(), ofstream::out);
		for(int i = 1; i <= n_user; i++) {
			out << i << endl;
			for(int j = 1; j <= n_user; j++) {
				out << trust[i][j] << " " << sim[i][j] << endl;
			}
		}
		out.close();
	}
}

void test_MF_Predictor()
{
	clock_t begin,end;
	string corpus = "E:/data/resys/corpus/6/";
	string train_data = corpus + "train/base";
	string test_data = corpus + "test/test";
	string size_data = corpus + "size";
	int n_user, n_item, n_rating, test_rating;
	FileDataReader train_reader = FileDataReader(train_data);
	FileDataReader test_reader = FileDataReader(test_data);	
	ifstream sizefile(size_data.c_str(), ifstream::in);
	sizefile >> n_user >> n_item >> n_rating >> test_rating;

	RatingModel trainModel(n_user,n_item,n_rating,1,1);
	RatingModel testModel(n_user,n_item,test_rating,1,1);
	RatingModel resultModel(n_user,n_item,test_rating,1,1);

	if(train_reader.readData(trainModel) && test_reader.readData(testModel)) {
		int k = 80;
		double lrate = 0.008;
		double lambda = 0.01;
		MF_Predictor predictor = MF_Predictor(trainModel,k,lrate,lambda);

		begin = clock();
		predictor.train(trainModel);
		end = clock();
		cout << "Train time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		begin = end;
		predictor.predictAll(testModel,resultModel);
		end = clock();
		cout << "Predict time: " << (end - begin) * 1.0 / CLOCKS_PER_SEC << endl;
		double rmse = evl_rmse(testModel, resultModel);
		cout << "RMSE: " << rmse << endl;
		double mae = evl_mae(testModel, resultModel);
		cout << "MAE: " << mae << endl;
	}
}

int main()
{
	//test_FileDataReader();
	//test_UserBased_Predictor();
	//test_ItemBased_Predictor();
	//test_UserItemBased_Predictor();
	//test_Fill_Predictor();
	//test_UserBased_Predictor();

	//printUserSim();
	//printUserCom();
	//test_UserBased_Predictor();
	//calUserTrust();
	//test_UserTrustBased_Predictor();

	test_MF_Predictor();
	return 0;
}

