#ifndef _USERTRUST_H_
#define _USERTRUST_H_

#include <iostream>
using namespace std;

double residual(double **trust1, double **trust2, int first_id, int last_id) {
	double r = 0;
	for(int u1 = first_id; u1 <= last_id; u1++) {
		for(int u2 = first_id; u2 <= last_id; u2++) {
			r += abs(trust1[u1][u2] - trust2[u1][u2]);
		}
	}
	int n_user = last_id - first_id + 1;
	return r/(n_user*n_user);
}

void cal_trust(double **sim, double **trust, int first_id, int last_id) {
	double **trust2;
	int n_user = last_id - first_id + 1;
	trust2 = new double*[last_id+1];
	for(int i = first_id; i <= last_id; i++)
		trust2[i] = new double[last_id+1];
	for(int u1 = first_id; u1 <= last_id; u1++) {
		for(int u2 = first_id; u2 <= last_id; u2++) {
			trust2[u1][u2] = trust[u1][u2];
		}
	}

	double theta,res;
	theta = 0.00001;
	int loop = 0;
	do {		
		for(int i = first_id; i <= last_id; i++) {
			for(int j = first_id; j <= last_id; j++) {
				if(i == j) continue;
				double p,q,t;
				p = q = 0;
				for(int k = first_id; k <= last_id; k++) {
					p += trust[i][k] * sim[j][k];
					q += sim[j][k];
				}
				if(q > 0) {
					t = p * 0.15 / q + 0.85 / (n_user * n_user);
					trust2[i][j] = t;
				}
			}
		}
		res=residual(trust,trust2,first_id,last_id);
		cout << ++loop << ": " << res << endl;

		double **tmp;
		tmp = trust2;
		trust2 = trust;
		trust = tmp;
	} while(res > theta);
}

#endif

