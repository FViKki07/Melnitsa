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
#include <ctime>

using namespace std;

extern int ROBOTCLR;
extern int HUMANCLR;

bool FindMill(int* state, int color, vector<vector<int>>& pos);

//int ROBOTCLR;
//int HUMANCLR;
//добавить для каждого ребенка его стоимость, возможно стек сделать  
struct Node {
private:
	int* state;
public:
	//int value;
	int white;
	int black;
	int move;// 1 - Ходят белые, 2 - черные bool?
	int phase;//фаза игры
	int fun;//эвристика
	vector<vector<int>> rMills = {};
	vector<vector<int>> oppMills = {};
	Node* prevNode;
	vector<Node*> child_fun = {};
	int steal_w = 0;
	int steal_b = 0;

	Node(int* s, int wh, int bl, int move, int p, int f, Node* n = nullptr) : state(s), white(wh), black(bl), move(move), prevNode(n) {
		//value = 0;
		phase = p;
		fun = f;

		if (n != nullptr) {
			if (n->rMills.size() != 0) rMills = n->rMills;
			if (n->oppMills.size() != 0) oppMills = n->oppMills;
		}
	}
	void NewNode(Node* node) {
		state = node->state;
		white = node->white;
		black = node->black;
		//move = HUMANCLR;
		phase = node->phase;
		rMills = node->rMills;
		oppMills = node->oppMills;
		prevNode = node->prevNode;
		steal_w = node->steal_w;
		steal_b = node->steal_b;
		fun = 0;
	}
	void new_Steal_w(int s) { steal_w = s; }
	void new_Steal_b(int s) { steal_b = s; }

	void new_state(int* new_s) {
		state = new_s;
	}

	void new_state(int new_pos, int clr = -1) {
		if (clr == -1)
			state[new_pos] = move;
		else state[new_pos] = clr;
	}

	int* get_state() { return state; }

	~Node() {
		if (state)
			delete[] state;
	}
};


bool compareByField(const Node* obj1, const Node* obj2);

bool BetweenMills(Node* node, vector<vector<int>>& v);


const map<string, int> CONVERT = { {"a7", 0},{"d7", 1}, {"g7", 2}, {"b6", 3}, {"d6", 4}, {"f6", 5},
	{"c5", 6}, {"d5", 7}, {"e5", 8}, {"a4", 9}, {"b4", 10}, {"c4", 11}, {"e4", 12}, {"f4", 13}, {"g4", 14},
	{"c3", 15}, {"d3", 16},{"e3", 17}, {"b2", 18}, {"d2", 19}, {"f2", 20}, {"a1", 21},{"d1", 22},{"g1", 23} };

const map<int, string> CONVERTR = { {0,"a7"},{1,"d7"}, {2,"g7"}, {3,"b6"}, {4,"d6"}, {5,"f6"},
	{6, "c5"}, {7,"d5"}, {8,"e5"}, {9,"a4"}, {10, "b4"}, {11, "c4"}, {12, "e4"}, {13, "f4"}, {14, "g4"},
	{15, "c3"}, {16, "d3"},{17, "e3"}, {18 ,"b2"}, {19, "d2"}, {20, "f2"}, {21, "a1"},{22, "d1"},{23, "g1"} };

void print_empty(int* mas);

const map<int, vector<int>> NEIGHBORS = { {0, { 1,9}}, {1, {0,2, 4}}, {2, {1, 14}} , {3, {4, 10}}, {4, {1,3,5,7}}, {5, {4,13}},
  {6, {7,11}}, {7, {4,6,8}}, {8, {7,12}}, {9, {0, 10, 21}}, {10, {3,9,11,18}}, {11, {6,10,15}}, {12, {8,13,17}}, {13, {5,12,14,20}} , {14, {2,13,23}},
  {15, {11,16}}, {16, {15,17,19}}, {17, {12,16}}, {18, {10,19}}, {19, {16,18,20,22}}, {20, {13,19}}, {21, {9,22}}, {22, {19,21,23}}, {23, {14,22}} };


const int MILLS[16][3] = { {0,1,2}, {3,4,5}, {6,7,8}, {9,10,11}, {12,13,14}, {15,16,17}, {18,19,20}, {21,22,23},
	{0,9,21}, {3,10,18}, {6,11,15}, {1,4,7},{16,19,22}, {8,12,17},{5,13,20},{2,14,23} };

const int MAX_DEPTH = 5;

void print_pos(int* mas);

void FirstPlayer();

bool PutChip(Node* node, string pos, int color);

bool AbilityMove(int* state, int color, vector<int>& blockChip);

bool FindMill(int* state, int color, vector<vector<int>>& pos);
int DoubleMorris(Node* node, int clr);
int FindTwoChip(Node* node, int clr);
int Heuristics1(Node* node);
Node* Part23(Node* node, int& depth, int alpha, int beta);
void StartGame();

Node* LookNewPos(Node* node, int& depth, int alpha, int beta);
Node* Part2(Node* node, int& depth, int alpha, int beta);
