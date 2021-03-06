#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include "player.h"
#include "map.h"
#include "possible_moves.h"

using namespace std;

int bound_x[11][4];
int bound_y[11][4];
int bound_z[11][4];

cordinate2 ring_bound[6];

void place_bounds(){
	int temp_x[11][4] = {
		{6,1,9,4},
		{4,0,10,6},
		{3,0,10,7},
		{2,0,10,8},
		{1,0,10,9},
		{1,1,9,9},
		{0,1,9,10},
		{0,2,8,10},
		{0,3,7,10},
		{0,4,6,10},
		{1,6,4,9}
	};
	for(int i= 0; i< 11; i++){
		for(int j= 0; j< 4; j++)	bound_x[i][j] = temp_x[i][j];
	}
	int temp_y[11][4] = {
		{6,1,9,4},
		{4,0,10,6},
		{3,0,10,7},
		{2,0,10,8},
		{1,0,10,9},
		{1,1,9,5},
		{0,1,9,10},
		{0,2,8,10},
		{0,3,7,10},
		{0,4,6,10},
		{1,6,4,9}
	};
	for(int i= 0; i< 11; i++){
		for(int j= 0; j< 4; j++)	bound_y[i][j] = temp_y[i][j];
	}
	int temp_z[11][4] = {
		{1,4,4,1},
		{0,6,6,0},
		{0,7,7,0},
		{0,8,8,0},
		{0,9,9,0},
		{0,9,9,1},
		{1,10,10,1},
		{2,10,10,2},
		{3,10,10,3},
		{4,10,10,4},
		{6,9,9,6}
	};
	for(int i= 0; i< 11; i++){
		for(int j= 0; j< 4; j++)	bound_z[i][j] = temp_z[i][j];
	}
}

// map<pair<int, int>, cordinate2> hex2cart;
// pair<int, int> cart2hex[11][11][11];
// vector<marker> board_state[11][3];

void bound_ring(int x, int y, int z){
	ring_bound[0] = cordinate2 {x, bound_x[x][0], bound_x[x][1]};	//lower x
	ring_bound[1] = cordinate2 {x, bound_x[x][2], bound_x[x][3]};	//upper x
	ring_bound[2] = cordinate2 {bound_y[y][0], y, bound_y[y][1]};	//lower y
	ring_bound[3] = cordinate2 {bound_y[y][2], y, bound_y[y][3]};	//upper y
	ring_bound[4] = cordinate2 {bound_z[z][0], bound_z[z][1], z};	//lower z
	ring_bound[5] = cordinate2 {bound_z[z][2], bound_z[z][3], z};	//upper z

	cordinate2 temp;
	for(int P= 0; P< 2; P++){
		for(int i= 0; i< 5; i++){

			if (P== 0){
				if(ring_self[i][0] == -1 || ring_self[i][1] == -1){
					continue;
				}
				temp = hex2cart[pair<int, int>(ring_self[i][0], ring_self[i][1])];
			}
			else{
				if(ring_opponent[i][0] == -1 || ring_opponent[i][1] == -1){
					continue;
				}
				temp = hex2cart[pair<int, int>(ring_opponent[i][0], ring_opponent[i][1])];
			}
			if(temp.x == x){
				//this ring lies on the same x
				if(temp.y < y && ring_bound[0].y <= temp.y){
					ring_bound[0] = cordinate2 {x, temp.y + 1, temp.z + 1};
				}
				if(temp.y > y && ring_bound[1].y>= temp.y){
					ring_bound[1] = cordinate2 {x, temp.y - 1, temp.z - 1};
				}

			}
			else if(temp.y == y){
				//this ring lies on the same y
				if(temp.x < x && ring_bound[2].x<= temp.x){
					ring_bound[2] = cordinate2 {temp.x+1, y, temp.z+1};
				}
				if(temp.x > x && ring_bound[3].x>= temp.x){
					ring_bound[3] = cordinate2 {temp.x-1, y, temp.z-1};
				}

			}
			else if(temp.z == z){
				//this ring lies on the same z
				if(temp.y < y && ring_bound[5].y<= temp.y){
					ring_bound[5] = cordinate2 {temp.x-1, temp.y+1, z};
				}
				if(temp.y > y && ring_bound[4].y>= temp.y){
					ring_bound[4] = cordinate2 {temp.x+1, temp.y-1, z};
				}

			}
		}
	}
}

// bool exist_marker(int x, int y, int z){
// 	for(int i= 0; i< board_state[x].size(); i++){
// 		if(board_state[x][i].x == y)	return true;
// 	}
// 	return false;
// }

void bound_marker(int x, int y, int z){
	int exist_x = 0, j= z;
	bool flag= false;
	for(int i= y-1; i>= ring_bound[0].y; i--){
		j--;
		if(exist_x == 0 && board_state1[x][i][j]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[x][i][j]== -1){
			ring_bound[0] = cordinate2 {x, i, j};
			break;
		}
	}
	exist_x = 0;
	j= z;
	for(int i= y+1; i<= ring_bound[1].y; i++){
		j++;
		if(exist_x == 0 && board_state1[x][i][j]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[x][i][j]== -1){
			ring_bound[1] = cordinate2 {x, i, j};
			break;
		}
	}
	exist_x = 0;
	j= z;
	for(int i= x-1; i>= ring_bound[2].x; i--){
		j--;
		if(exist_x == 0 && board_state1[i][y][j]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[i][y][j]== -1){
			ring_bound[2] = cordinate2 {i, y, j};
			break;
		}
	}
	exist_x = 0;
	j= z;
	for(int i= x+1; i<= ring_bound[3].x; i++){
		j++;
		if(exist_x == 0 && board_state1[i][y][j]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[i][y][j]== -1){
			ring_bound[3] = cordinate2 {i, y, j};
			break;
		}
	}
	exist_x = 0;
	j= y;
	for(int i= x-1; i>= ring_bound[4].x; i--){
		j++;
		if(exist_x == 0 && board_state1[i][j][z]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[i][j][z]== -1){
			ring_bound[4] = cordinate2 {i, j, z};
			break;
		}
	}
	exist_x = 0;
	j= y;
	for(int i= x+1; i<= ring_bound[5].x; i++){
		j--;
		if(exist_x == 0 && board_state1[i][j][z]>= 0)	exist_x = 1;
		if(exist_x == 1 && board_state1[i][j][z]== -1){
			ring_bound[5] = cordinate2 {i, j, z};
			break;
		}
	}
}

vector<cordinate2> give_positions(int x, int y, int z){
	// cout<<"inside postions"<<endl;
	bound_ring(x, y, z);
	bound_marker(x, y, z);
	// cout<<"intermediate pos"<<endl;
	vector<cordinate2> v;
	int i, j, k;
	i= x;
	k = ring_bound[0].z - 1;
	// cout<<"##"<<i<<"@@"<<k<<endl;
	cordinate2 temp_cor;
	for(j= ring_bound[0].y; j<= ring_bound[1].y; j++){
		k++;
		temp_cor = {x, j, k};
		if(j!= y && k!= z && board_state1[x][j][k]== -1 && cart_exist_ring(x, j, k) == -1)	v.push_back(temp_cor);
	}
	k = ring_bound[2].z - 1;
	// cout<<"##"<<i<<"@@"<<k<<endl;
	for(i= ring_bound[2].x; i<= ring_bound[3].x; i++){
		k++;
		temp_cor = {i, y, k};
		if(i!= x && k!= z && board_state1[i][y][k]== -1 && cart_exist_ring(i, y, k) == -1)	v.push_back(temp_cor);
	}
	i = ring_bound[4].x - 1;
	// cout<<"##"<<i<<"@@"<<k<<endl;
	for(j= ring_bound[4].y; j>= ring_bound[5].y; j--){
		i++;
		temp_cor = {i, j, z};
		if(i!= x && k!= z && board_state1[i][j][z]== -1 && cart_exist_ring(i, j, z) == -1)	v.push_back(temp_cor);
	}
	// cout<<"ending positions"<<endl;
	return v;
}

void update_board_score(int x, int y, int z){
	int j, k, val_opponent, val_mine, cnt, score_temp;
	bool opponent_flag, mine_flag;
	mine_flag = false;
	opponent_flag = false;
	cnt = 0;
	val_mine= 0;
	val_opponent= 0;
	score_temp= 0;
	for(int j= bound_x[x][0]; j<= bound_x[x][2]; j++){
		k = bound_x[x][1]+ cnt;
		cnt++;
		if(board_state1[x][j][k]== 1){
			if(mine_flag){
				val_mine++;
			}
			else if(opponent_flag){
				opponent_flag = false;
				mine_flag = true;
				score_temp = score_temp - (val_opponent*val_opponent);
				val_opponent = 0;
				val_mine= 1;
			}
			else{
				mine_flag = true;
				opponent_flag = false;
				val_mine++;
			}
		}
		else if(board_state1[x][j][k]== 0){
			if(mine_flag){
				mine_flag = false;
				opponent_flag = true;
				score_temp = score_temp + (val_mine*val_mine);
				val_mine= 0;
				val_opponent= 1;
			}
			else if(opponent_flag){
				val_opponent++;
			}
			else{
				mine_flag = false;
				opponent_flag = true;
				val_opponent++;
			}
		}
		else{
			score_temp+= (val_mine*val_mine) - (val_opponent*val_opponent);
			val_opponent= 0;
			val_mine= 0;
			mine_flag= false;
			opponent_flag= false;
		}
	}
	score[x][0] = score_temp;

	mine_flag = false;
	opponent_flag = false;
	cnt = 0;
	val_mine= 0;
	val_opponent= 0;
	score_temp= 0;
	for(int i= bound_y[y][0]; i<= bound_y[y][2]; i++){
		k = bound_y[y][1]+ cnt;
		cnt++;
		if(board_state1[i][y][k]== 1){
			if(mine_flag){
				val_mine++;
			}
			else if(opponent_flag){
				opponent_flag = false;
				mine_flag = true;
				score_temp = score_temp - (val_opponent*val_opponent);
				val_opponent = 0;
				val_mine= 1;
			}
			else{
				mine_flag = true;
				opponent_flag = false;
				val_mine++;
			}
		}
		else if(board_state1[i][y][k]== 0){
			if(mine_flag){
				mine_flag = false;
				opponent_flag = true;
				score_temp = score_temp + (val_mine*val_mine);
				val_mine= 0;
				val_opponent= 1;
			}
			else if(opponent_flag){
				val_opponent++;
			}
			else{
				mine_flag = false;
				opponent_flag = true;
				val_opponent++;
			}
		}
		else{
			score_temp+= (val_mine*val_mine) - (val_opponent*val_opponent);
			val_opponent= 0;
			val_mine= 0;
			mine_flag= false;
			opponent_flag= false;
		}
	}
	score[y][1] = score_temp;

	mine_flag = false;
	opponent_flag = false;
	cnt = 0;
	val_mine= 0;
	val_opponent= 0;
	score_temp= 0;
	for(int i= bound_z[z][0]; i<= bound_z[z][2]; i++){
		j = bound_z[z][1] - cnt;
		cnt++;
		if(board_state1[i][j][z]== 1){
			if(mine_flag){
				val_mine++;
			}
			else if(opponent_flag){
				opponent_flag = false;
				mine_flag = true;
				score_temp = score_temp - (val_opponent*val_opponent);
				val_opponent = 0;
				val_mine= 1;
			}
			else{
				mine_flag = true;
				opponent_flag = false;
				val_mine++;
			}
		}
		else if(board_state1[i][j][z]== 0){
			if(mine_flag){
				mine_flag = false;
				opponent_flag = true;
				score_temp = score_temp + (val_mine*val_mine);
				val_mine= 0;
				val_opponent= 1;
			}
			else if(opponent_flag){
				val_opponent++;
			}
			else{
				mine_flag = false;
				opponent_flag = true;
				val_opponent++;
			}
		}
		else{
			score_temp+= (val_mine*val_mine) - (val_opponent*val_opponent);
			val_opponent= 0;
			val_mine= 0;
			mine_flag= false;
			opponent_flag= false;
		}
	}
	score[z][2] = score_temp;
}

int total_score(){
	int val= 0;
	for(int i= 0; i< 11; i++){
		val+= score[i][0];
		val+= score[i][1];
		val+= score[i][2];
	}
	return val;
}

pair<vector<pair<cordinate2, cordinate2> >, vector<pair<cordinate2, cordinate2> > > check_5(int x, int y, int z){
	bool mine_flag= false, opponent_flag= false;
	int i, j, k, val_mine, val_opponent, cnt= 0;
	vector<pair<cordinate2, cordinate2> > v_mine;
	vector<pair<cordinate2, cordinate2> > v_opponent;

	val_mine= 0;
	val_opponent= 0;
	cnt= 0;
	mine_flag= false;
	opponent_flag= false;
	cordinate2 start_opponent, end_opponent, start_mine, end_mine;

	for(j= bound_x[x][0]; j< bound_x[x][2]; j++){
		k= bound_x[x][1]+cnt;
		cnt++;
		if(cart_exist_ring(x, j, k) != -1){
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
		else if(board_state1[x][j][k]== 1){
			if(mine_flag){
				val_mine++;
				end_mine = {x, j, k};
			}
			else if(opponent_flag){
				if(val_opponent>=5){
					v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
					val_opponent= 0;
					break;
				}
				opponent_flag= false;
				mine_flag= true;
				val_mine= 1;
				val_opponent= 0;
				start_mine= {x, j, k};
			}
			else{
				val_mine= 1;
				start_mine = {x, j, k};
				val_opponent= 0;
				mine_flag= true;
			}
		}
		else if(board_state1[x][j][k]==0){
			if(mine_flag){
				if(val_mine>=5){
					v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
					val_mine= 0;
					break;
				}
				val_mine= 0;
				mine_flag= false;
				opponent_flag= true;
				val_opponent = 1;
				start_opponent = {x, j, k};
			}
			else if(opponent_flag){
				val_opponent++;
				end_opponent = {x, j, k};
			}
			else{
				val_mine= 0;
				val_opponent= 1;
				opponent_flag = true;
				start_opponent = {x, j, k};
			}
		}
		else{
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
	}
	if(val_mine>= 5)	v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
	if(val_opponent>= 5)	v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));


	val_mine= 0;
	val_opponent= 0;
	cnt= 0;
	mine_flag= false;
	opponent_flag= false;
	j = y;

	for(i= bound_y[y][0]; i< bound_y[y][2]; i++){
		k= bound_y[y][1]+cnt;
		cnt++;
		if(cart_exist_ring(i, j ,k) != -1){
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
		else if(board_state1[i][j][k]== 1){
			if(mine_flag){
				val_mine++;
				end_mine = {i, j, k};
			}
			else if(opponent_flag){
				if(val_opponent>=5){
					v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
					val_opponent= 0;
					break;
				}
				opponent_flag= false;
				mine_flag= true;
				val_mine= 1;
				val_opponent= 0;
				start_mine= {i, j, k};
			}
			else{
				val_mine= 1;
				start_mine = {i, j, k};
				val_opponent= 0;
				mine_flag= true;
			}
		}
		else if(board_state1[i][j][k]==0){
			if(mine_flag){
				if(val_mine>=5){
					v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
					val_mine= 0;
					break;
				}
				val_mine= 0;
				mine_flag= false;
				opponent_flag= true;
				val_opponent = 1;
				start_opponent = {i, j, k};
			}
			else if(opponent_flag){
				val_opponent++;
				end_opponent = {i, j, k};
			}
			else{
				val_mine= 0;
				val_opponent= 1;
				opponent_flag = true;
				start_opponent = {i, j, k};
			}
		}
		else{
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
	}
	if(val_mine>= 5)	v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
	if(val_opponent>= 5)	v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));



	val_mine= 0;
	val_opponent= 0;
	cnt= 0;
	mine_flag= false;
	opponent_flag= false;
	k = z;

	for(i= bound_z[z][0]; i< bound_z[z][2]; i++){
		j= bound_z[z][1]-cnt;
		cnt++;
		if(cart_exist_ring(i, j, k) != -1){
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
		else if(board_state1[i][j][k]== 1){
			if(mine_flag){
				val_mine++;
				end_mine = {i, j, k};
			}
			else if(opponent_flag){
				if(val_opponent>=5){
					v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
					val_opponent= 0;
					break;
				}
				opponent_flag= false;
				mine_flag= true;
				val_mine= 1;
				val_opponent= 0;
				start_mine= {i, j, k};
			}
			else{
				val_mine= 1;
				start_mine = {i, j, k};
				val_opponent= 0;
				mine_flag= true;
			}
		}
		else if(board_state1[i][j][k]==0){
			if(mine_flag){
				if(val_mine>=5){
					v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
					val_mine= 0;
					break;
				}
				val_mine= 0;
				mine_flag= false;
				opponent_flag= true;
				val_opponent = 1;
				start_opponent = {i, j, k};
			}
			else if(opponent_flag){
				val_opponent++;
				end_opponent = {i, j, k};
			}
			else{
				val_mine= 0;
				val_opponent= 1;
				opponent_flag = true;
				start_opponent = {i, j, k};
			}
		}
		else{
			if(val_mine>=5){
				v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
				val_mine= 0;
				break;
			}
			if(val_opponent>=5){
				v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));
				val_opponent= 0;
				break;
			}
			opponent_flag = false;
			mine_flag = false;
			val_mine = 0;
			val_opponent = 0;
		}
	}
	if(val_mine>= 5)	v_mine.push_back(pair<cordinate2, cordinate2> (start_mine, end_mine));
	if(val_opponent>= 5)	v_opponent.push_back(pair<cordinate2, cordinate2> (start_opponent, end_opponent));

	pair<vector<pair<cordinate2, cordinate2> >, vector<pair<cordinate2, cordinate2> > > p_return;
	p_return = {v_mine, v_opponent};
	return p_return;
}

pair<pair<int, int>, pair<int, int> > select_5(pair<cordinate2, cordinate2> p){
	pair<int, int> pos1 = cart2hex[(p.first).x][(p.first).y][(p.first).z];
	pair<int, int> pos2 = cart2hex[(p.second).x][(p.second).y][(p.second).z];

	// ofstream myfile;
	// myfile.open("writeup.txt", ios_base::app);
	// myfile<<"up1"<<endl;

	vector<pair<int, int> > mid_places = places(pos1.first, pos1.second, pos2.first, pos2.second);

	// myfile<<"params::"<<pos1.first<<" "<<pos1.second<<" "<<pos2.first<<" "<<pos2.second<<endl;

	pair<pair<int, int>, pair<int, int> > p_return;

	// myfile<<"up2"<<endl;
	// myfile.close();

	bool flag_left = false;

	if(mid_places.size() != 3){
		if((p.first).x == (p.second).x){
			if((p.first).y < (p.second).y){
				flag_left = true;
			}
		}
		else if((p.first).y == (p.second).y){
			if((p.first).x < (p.second).x){
				flag_left = true;
			}
		}
		else{
			if((p.first).x < (p.second).x){
				flag_left = true;
			}
		}
	}
	else{
		p_return = {pos1, pos2};
		return p_return;
	}
	// myfile<<"up4"<<endl;
	// myfile<<mid_places.size()<<endl;
	if(flag_left){
		pos2 = mid_places[3];
	}
	else{
		pos1 = mid_places[3];
	}
	// myfile<<"up3"<<endl;
	// myfile.close();
	p_return = {pos1, pos2};
	return p_return;
}

pair<vector<pair<cordinate2, cordinate2> >, vector<pair<cordinate2, cordinate2> > > check_5_hex_pos(int hex, int pos){
	cordinate2 cor = hex2cart[pair<int, int> (hex, pos)];
	return check_5(cor.x, cor.y, cor.z);
}

void update_multiple_board_score(int hex1, int pos1, int hex2, int pos2){
	vector<pair<int, int> > location = places(hex1, pos1, hex2, pos2);

	update_hex_board_score(hex1, pos1);
	update_hex_board_score(hex2, pos2);
	for(int i=0; i<location.size(); i++){
		pair<int, int> hex_loc = location[i];
		cordinate2 temp_loc = hex2cart[hex_loc];
		update_board_score(temp_loc.x, temp_loc.y, temp_loc.z);
	}
}

void update_hex_board_score(int hex, int pos){
	pair<int, int> temp;
	temp = {hex, pos};

	cordinate2 cord_temp = hex2cart[temp];
	update_board_score(cord_temp.x, cord_temp.y, cord_temp.z);
}
