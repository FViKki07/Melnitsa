#include "Lab4.h"
using namespace std;

int ROBOTCLR = 0;
vector<vector<int>> rMills = { {} };
vector<vector<int>> oppMills = { {} };

void FirstPlayer() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int num = rand() % 2;
	if (!num) {
		ROBOTCLR = 2;
		cout << "Вы ходите первыми. Ваши фишки белые (1)" << endl;
	}
	else {
		ROBOTCLR = 1;
		cout << "Вы ходите вторыми. Ваши фишки черные (2)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
	}
}

void PutChip(int* box, string pos, int color) {
	if (CONVERT.count(pos) == 0)
		return; 
	box[CONVERT.at(pos)] = color;
}

bool End_of_Game(Node* node){
	vector<int> b = {};
	if (node->black < 3 || node->white < 3 || !AbilityMove(node->state, node->move, b))
		return true;
	return false;

}

bool AbilityMove(int* state, int color, vector<int>& blockChip) {
	int m =color;
	auto it = state;
	int count = 0;
	bool flag = false;
	while (( it = find(it, state + 24, m)) != (state + 24)) {
		int pos = distance(state, it);
		count++;
		vector<int> neighbors = NEIGHBORS.at(pos);
		for (auto n : neighbors)
			if (state[n] == 0) {
				flag = true;
				break;
			}
		if (!flag) blockChip.push_back(pos);
		flag = false;
		++it;
	}
	if (blockChip.size() == count)
		return false;
	return true;;

}

int FindTwoChip(int* state, int color) {
	int count = 0;
	for (auto it : MILLS) {
		int a = it[0];
		int b = it[1];
		int c = it[2];

		if ( ((state[a] == state[b] && state[c] != state[a] || state[a] == state[c] && state[b] != state[a]) && state[a] == color) ||
			(state[b] == color && state[b] == state[c]) && state[b] != state[a]) { //может добавить проверку, что третья клетка пустая или убрать проверку про собранные мельницы
			count++;
		}
	}

	return count;
}


bool FindMill(int* state,int color,vector<vector<int>>& pos) {
	for (auto it : MILLS) {
		int a = it[0];
		int b = it[1];
		int c = it[2];
		
			if (state[a] == state[b] && state[a] == state[c] && state[a] == color) { 
				vector<int> newMill = { a,b,c };
				pos.push_back(newMill);
			}
	}
	if(pos.size() == 0)
		return false;
	return true;
}


int Heuristics1(Node* node) {
	//1
	int clossedMorris = 0;
	vector<vector<int>> v;
	if (node->move == ROBOTCLR) {
		if (FindMill(node->state, node->move, v))
			clossedMorris = 1;
	}
	else {
		if (FindMill(node->state, node->move, v))
			clossedMorris = -1;

	}

	//2
	int NumberOfMorris = rMills.size() - oppMills.size();

	//3
	vector<int> blocked = {};
	(ROBOTCLR == 1) ? AbilityMove(node->state, 2, blocked) : AbilityMove(node->state, 1, blocked);
	int NumberofBlocked = blocked.size();
	blocked.clear();
	AbilityMove(node->state, ROBOTCLR, blocked);
	NumberofBlocked -= blocked.size();

	//4
	int NumberChip = 0;
	if(ROBOTCLR == 1)
		NumberChip = node->white - node->black;
	else NumberChip = node->black - node->white;

	//5
	int TwoChip = FindTwoChip(node->state, ROBOTCLR) ;
	(ROBOTCLR == 1) ? TwoChip -= FindTwoChip(node->state, 2) : TwoChip -= FindTwoChip(node->state, 1);

	//8
	int WinConfiguration = 0;
	if (End_of_Game(node) && node->move == ROBOTCLR)
		WinConfiguration = -1;
	if (End_of_Game(node) && node->move != ROBOTCLR)
		WinConfiguration = 1;


	return 18 * clossedMorris + 26 * NumberOfMorris + 1 * NumberofBlocked + 9 * NumberChip + 10 * TwoChip;
	/*(node->move == 1) ? h += node->white : node->black;
	vector<vector<int>> v;
	if (FindMill(node->state, node->move, v))
		h += v.size();
	if (node->move == ROBOTCLR)
		return h;
	return -h;*/
}

void StartGame() {
	const int n = 24;
	int arr[n];
	cout << "\nПоле до начала игры\n0-это пустые ячейки\n1 - это белые фишки\n2 - это черные фишки" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	Node* startNode = new Node(arr, 9, 9, 1, 1, 0);
	FirstPlayer();
	if (ROBOTCLR == 2) {
		string inputstr;
		std::cout << "Введите поля: ";
		std::cin >> inputstr;
		while (CONVERT.count(inputstr) == 0) {
			std::cout << "Неправильное значение";
			std::cout << "Введите поля: ";
			std::cin >> inputstr;
		}
		PutChip(startNode->state,inputstr, 1);
		print_pos(startNode->state);
	}

}
void Part1(Node* node) {

}