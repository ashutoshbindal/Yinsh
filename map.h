#ifndef MAP_H
#define	MAP_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct cordinate2
{
	int x;
	int y;
	int z;
};

class marker{
public:
    int x;
    int y;
    bool token;
    marker(int, int, bool);
};

extern map<pair<int, int>, cordinate2> hex2cart;
extern pair<int, int> cart2hex[11][11][11];
// extern vector<marker> board_state[11][3];
extern int board_state1[11][11][11];
extern int score[11][3];

vector<pair<int, int> > places(int, int, int, int);

void initialize();
void initialize_score();
void initialize_board_state1();

#endif
