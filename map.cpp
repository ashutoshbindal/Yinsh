#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>
#include "map.h"

using namespace std;

map<pair<int, int>, cordinate2> hex2cart;
pair<int, int> cart2hex[11][11][11];
// vector<marker> board_state[11][3];
int board_state1[11][11][11];
int score[11][3];

vector<pair<int, int> > places(int h1, int p1, int h2, int p2){
	pair<int, int> temp1;
	temp1 = {h1, p1};
	pair<int, int> temp2;
	temp2 = {h2, p2};
	cordinate2 temp3, temp4;
	temp3 = hex2cart[temp1];
	temp4 = hex2cart[temp2];
	vector<pair<int, int> > v;
	pair<int, int> p;
	int flag = 1, x_val, y_val, z_val;
	if(temp3.x == temp4.x){
		int y = temp3.y - temp4.y;

		// ofstream fout;
		// fout.open("func_check.txt", ios_base::app);
		// fout<<"y::"<<y<<endl;
		// fout<<"hex::"<<temp1.first<<" "<<temp1.second<<" "<<temp2.first<<" "<<temp2.second<<endl;
		// fout<<"cart::"<<temp3.x<<" "<<temp3.y<<" "<<temp3.z<<" "<<temp4.x<<" "<<temp4.y<<" "<<temp4.z<<endl;
		// fout.close();

		if(y> 1){
			for(int i= 0; i< y-1; i++){
				x_val = temp3.x;
				y_val = temp4.y + (i+1);
				z_val = temp4.z + (i+1);
				p = cart2hex[x_val][y_val][z_val];
				v.push_back(p);
			}
		}
		if(y< -1){
			y = abs(y);
			for(int i= 0; i< y-1; i++){
				x_val = temp3.x;
				y_val = temp3.y + (i+1);
				z_val = temp3.z + (i+1);
				p = cart2hex[x_val][y_val][z_val];
				// fout<<"points::"<<p.first<<" "<<p.second<<endl;
				v.push_back(p);
			}
			// fout<<v.size()<<endl;
		}

		// fout.close();
	}
	if(temp3.y == temp4.y){
		int y = temp3.x - temp4.x;
		if(y> 1){
			for(int i= 0; i< y-1; i++){
				x_val = temp4.x + (i+1);
				y_val = temp4.y;
				z_val = temp4.z + (i+1);
				p = cart2hex[x_val][y_val][z_val];
				v.push_back(p);
			}
		}
		if(y< -1){
			y = abs(y);
			for(int i= 0; i< y-1; i++){
				x_val = temp3.x + (i+1);
				y_val = temp3.y;
				z_val = temp3.z + (i+1);
				p = cart2hex[x_val][y_val][z_val];
				v.push_back(p);
			}
		}
	}
	if(temp3.z == temp4.z){
		int y = temp3.x - temp4.x;
		if(y> 1){
			for(int i= 0; i< y-1; i++){
				x_val = temp4.x + (i+1);
				y_val = temp4.y - (i+1);
				z_val = temp4.z;
				p = cart2hex[x_val][y_val][z_val];
				v.push_back(p);
			}
		}
		if(y< -1){
			y = abs(y);
			for(int i= 0; i< y-1; i++){
				x_val = temp3.x + (i+1);
				y_val = temp3.y - (i+1);
				z_val = temp3.z;
				p = cart2hex[x_val][y_val][z_val];
				v.push_back(p);
			}
		}
	}
	return v;
}

void initialize_score(){
	for(int i=0; i<11; i++){
		for(int j=0; j<3; j++){
			score[i][j] = 0;
		}
	}
}

void initialize_board_state1(){
	for(int i= 0; i< 11; i++){
		for(int j= 0; j< 11; j++){
			for(int k= 0; k< 11; k++){
				board_state1[i][j][k] = -1;
			}
		}
	}
}

void initialize(){
	//hex2car
	pair<int, int> temp1;
	cordinate2 temp2;

	//hex0
	temp1 = {0, 0};
	temp2 = {5, 5, 5};
	hex2cart[temp1] = temp2;

	//hex1
	vector<int> x = {6, 7, 7, 6, 5, 5};
	vector<int> y = {7, 6, 5, 5, 6, 7};
	vector<int> z = {7, 7, 6, 5, 5, 6};
	for(int i= 0; i< 6; i++){
		temp1 = {1, i};
		temp2 = {x[i]-1, y[i]-1, z[i]-1};
		hex2cart[temp1] = temp2;
	}

	//hex2
	vector<int>().swap(x);
	vector<int>().swap(y);
	vector<int>().swap(z);
	x = {6, 7, 8, 8, 8, 7, 6, 5, 4, 4, 4, 5};
	y = {8, 7, 6, 5, 4, 4, 4, 5, 6, 7, 8, 8};
	z = {8, 8, 8, 7, 6, 5, 4, 4, 4, 5, 6, 7};
	for(int i= 0; i< 12; i++){
		temp1 = {2, i};
		temp2 = {x[i]-1, y[i]-1, z[i]-1};
		hex2cart[temp1] = temp2;
	}

	//hex3
	vector<int>().swap(x);
	vector<int>().swap(y);
	vector<int>().swap(z);
	x = {6,7,8,9,9,9,9,8,7,6,5,4,3,3,3,3,4,5};
	y = {9,8,7,6,5,4,3,3,3,3,4,5,6,7,8,9,9,9};
	z = {9,9,9,9,8,7,6,5,4,3,3,3,3,4,5,6,7,8};
	for(int i= 0; i< 18; i++){
		temp1 = {3, i};
		temp2 = {x[i]-1, y[i]-1, z[i]-1};
		hex2cart[temp1] = temp2;
	}

	//hex4
	vector<int>().swap(x);
	vector<int>().swap(y);
	vector<int>().swap(z);
	x = {6,7,8,9,10,10,10,10,10,9,8,7,6,5,4,3,2,2,2,2,2,3,4,5};
	y = {10,9,8,7,6,5,4,3,2,2,2,2,2,3,4,5,6,7,8,9,10,10,10,10};
	z = {10,10,10,10,10,9,8,7,6,5,4,3,2,2,2,2,2,3,4,5,6,7,8,9};
	for(int i= 0; i< 24; i++){
		temp1 = {4, i};
		temp2 = {x[i]-1, y[i]-1, z[i]-1};
		hex2cart[temp1] = temp2;
	}

	//hex5
	vector<int>().swap(x);
	vector<int>().swap(y);
	vector<int>().swap(z);
	x = {6,7,8,9,10, 11,11,11,11,11, 11,10,9,8,7, 6,5,4,3,2, 1,1,1,1,1, 1,2,3,4,5};
	y = {11,10,9,8,7,6,5,4,3,2,1,1,1,1,1,1,2,3,4,5,6,7,8,9,10,11,11,11,11,11};
	z = {11,11,11,11,11,11,10,9,8,7,6,5,4,3,2,1,1,1,1,1,1,2,3,4,5,6,7,8,9,10};
	for(int i= 0; i< 30; i++){
		temp1 = {5, i};
		temp2 = {x[i]-1, y[i]-1, z[i]-1};
		hex2cart[temp1] = temp2;
	}

	vector<int>().swap(x);
	vector<int>().swap(y);
	vector<int>().swap(z);

	//cart2hex

	//hex0
	temp1 = {0, 0};
	temp2 = hex2cart[temp1];
	cart2hex[temp2.x][temp2.y][temp2.z] = {0, 0};

	for(int h = 1; h< 6; h++){
		for(int p= 0; p< 6*h; p++){
			temp1 = {h, p};
			temp2 = hex2cart[temp1];
			cart2hex[temp2.x][temp2.y][temp2.z] = {h, p};
		}
	}
}

marker::marker(int a, int b, bool m){
	this->x = a;
	this->y = b;
	this->token = m;
}
