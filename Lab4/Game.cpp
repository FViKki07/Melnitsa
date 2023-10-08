#include "Lab4.h"
using namespace std;

void FirstPlayer() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int num = rand() % 2;
	if (!num)
		cout << "Вы ходите первыми. Ваши фишки белые (1)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
	else cout << "Вы ходите вторыми. Ваши фишки черные (2)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
}

void PutChip(int* box, string pos, int chip) {
	if (CONVERT.count(pos) == 0)
		return; 
	box[CONVERT.at(pos)] = chip;
}

bool End_of_Game(Node* node){
	if (node->black < 3 || node->white < 3 || !AbilityMove(node))
		return true;
	return false;

}

bool AbilityMove(Node* node) {
	int m = node->move;
	int* state = node->state;
	auto it = state;
	while (( it = find(it, state + 24, m)) != (state + 24)) {
		int pos = distance(state, it);
		vector<int> neighbors = NEIGHBORS.at(pos);
		for (auto n : neighbors)
			if (state[n] == 0)
				return true;
		++it;
	}
	return false;

}

bool FindMill(int* state,int color, stack<int>& pos, int& prev) {
	for (auto it : MILLS) {
		int a = it[0];
		int b = it[1];
		int c = it[2];
		
			if (state[a] == state[b] && state[a] == state[c] && state[a] == color) {
				pos.push(a);
				pos.push(b);
				pos.push(c);
				return true;
			}
	}
	return false;
}


int Heuristics(Node* node,int color, bool move) {
	int h = 0;
	(color == 1) ? h += node->white : node->black;
	return 0;

	 
}
void Part1(Node* node) {

}