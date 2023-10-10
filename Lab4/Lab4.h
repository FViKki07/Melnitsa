#include <iostream>
#include <algorithm>
#include <stdlib.h> 
#include <time.h> 
#include <map>
#include <string>
#include <stack>
#include <algorithm> 
#include <vector>
#include <unordered_set>
#include <sstream>
using namespace std;

const int ROBOTCLR =1;
const int HUMANCLR = 2;

bool FindMill(int* state, int color, vector<vector<int>>& pos);

struct Node {
private:
	int* state;
public:	
	int value;
	int white;
	int black;
	int move;// 1 - ����� �����, 2 - ������ bool?
	int phase;//���� ����
	int fun;//���������
	vector<vector<int>> rMills = {};
	vector<vector<int>> oppMills = {};
	Node* prevNode;

	Node(int* s, int wh, int bl, int move, int p, int f, Node* n = nullptr) : state(s), white(wh), black(bl), move(move), prevNode(n) {
		value = 0;
		phase = p;
		fun = f;

		vector<vector<int>> v = {};
		/*if (FindMill(state, move, v)) {
			for (int i = 0; i < v.size(); i++) {
				BetweenMills2(v);
				if (move == HUMANCLR)
					oppMills.push_back(v[i]);
				else rMills.push_back(v[i]);
			}
		}*/
		if (n != nullptr) {
			if (n->rMills.size() != 0) rMills = n->rMills;
			if(n->oppMills.size() != 0) oppMills = n->oppMills;
		}
	}

	void new_state(int* new_s) {
		state = new_s;
		/*vector<vector<int>> v = {};
		if (FindMill(state, move, v)) {
			BetweenMills2( v);
			for (int i = 0; i < v.size(); i++) {
				if (move == HUMANCLR)
					oppMills.push_back(v[i]);
				else rMills.push_back(v[i]);
			}
		}*/
	}


	void new_state(int new_pos, int clr = -1) {
		if(clr == -1)
			state[new_pos] = move;
		else state[new_pos] = clr;
		/*vector<vector<int>> v = {};
		if (FindMill(state, move, v)) {
			BetweenMills2(v);
			for (int i = 0; i < v.size(); i++) {
				if (move == HUMANCLR)
					oppMills.push_back(v[i]);
				else rMills.push_back(v[i]);
			}
		}*/
	}
	int* get_state() { return state; }

private:
	void BetweenMills2(vector<vector<int>>& v)
	{
		for (int i = 0; i < v.size(); i++) {
			for (int j = 0; j < rMills.size(); j++) {
				vector<int> rob =rMills[j];
				if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])
					v.erase(v.begin() + i);
				else rMills.push_back(v[i]);
			}
			if (rMills.size() == 0) {
				rMills.push_back(v[i]);
			}
		}

	}

};

bool BetweenMills(Node* node, vector<vector<int>>& v);


const map<string, int> CONVERT = { {"a7", 0},{"d7", 1}, {"g7", 2}, {"b6", 3}, {"d6", 4}, {"f6", 5},
	{"c5", 6}, {"d5", 7}, {"e5", 8}, {"a4", 9}, {"b4", 10}, {"c4", 11}, {"e4", 12}, {"f4", 13}, {"g4", 14},
	{"c3", 15}, {"d3", 16},{"e3", 17}, {"b2", 18}, {"d2", 19}, {"f2", 20}, {"a1", 21},{"d1", 22},{"g1", 23} };

void print_empty(int* mas);

const map<int, vector<int>> NEIGHBORS = { {0, { 1,9}}, {1, {0,2, 4}}, {2, {1, 14}} , {3, {4, 10}}, {4, {1,3,5,7}}, {5, {4,13}},
  {6, {7,11}}, {7, {4,6,8}}, {8, {7,12}}, {9, {0, 10, 21}}, {10, {3,9,11,18}}, {11, {6,10,15}}, {12, {8,13,17}}, {13, {5,12,14,20}} , {14, {2,13,23}},
  {15, {11,16}}, {16, {15,17,19}}, {17, {12,16}}, {18, {10,19}}, {19, {16,18,20,22}}, {20, {13,19}}, {21, {9,22}}, {22, {19,21,23}}, {23, {14,22}} };


const int MILLS[16][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {9,10,11}, {12,13,14}, {15,16,17}, {18,19,20}, {21,22,23},
	{0,9,21}, {3,10,18}, {6,11,15}, {1,4,7},{16,19,22}, {8,12,17},{5,13,20},{2,14,23} };

const int MAX_DEPTH = 2;

void print_pos(int* mas);

void FirstPlayer();

bool PutChip(Node* node, string pos, int color);

bool AbilityMove(int* state, int color, vector<int>& blockChip);

bool FindMill(int* state, int color, vector<vector<int>>& pos );

int FindTwoChip(int* state, int color);

void StartGame();
