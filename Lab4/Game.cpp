#include "Lab4.h"
using namespace std;

int ROBOTCLR = 0;
int HUMANCLR = 0;
vector<vector<int>> rMills = { };
vector<vector<int>> oppMills = {};

struct Node {
	int* state;
	int value;
	int white;
	int black;
	int move;// 1 - Ходят белые, 2 - черные bool?
	int phase;//фаза игры
	int fun;//эвристика
	Node* prevNode;

	Node(int* s, int w, int b, int m, int p, int f, Node* n = nullptr) : state(s), white(w), black(b), move(m), prevNode(n) {
		value = 0;
		phase = p;
		fun = f;

		vector<vector<int>> v = {};
		if (FindMill(state, move, v)) {
			for (int i = 0; i < v.size(); i++) {
				if (move == HUMANCLR)
					oppMills.push_back(v[i]);
				else rMills.push_back(v[i]);
			}
		}
	}
};

void FirstPlayer() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int num = rand() % 2;
	if (!num) {
		HUMANCLR = 1;
		ROBOTCLR = 2;
		cout << "Вы ходите первыми. Ваши фишки белые (1)" << endl;
	}
	else {

		HUMANCLR = 2;
		ROBOTCLR = 1;
		cout << "Вы ходите вторыми. Ваши фишки черные (2)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
	}
}

bool PutChip(int* box, string pos, int color) {
	if (CONVERT.count(pos) == 0)
		return false; 
	box[CONVERT.at(pos)] = color;
	vector<vector<int>> v = {};
	if (FindMill(box, color, v)) {
		for (int i = 0; i < v.size(); i++) {
			if (color == HUMANCLR)
				oppMills.push_back(v[i]);
			else rMills.push_back(v[i]);
		}
		return true;
	}
	return false;
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
	(node->move = 2) ? node->move = 1 : node->move = 2;
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
	AbilityMove(node->state, HUMANCLR, blocked);
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
	TwoChip -= FindTwoChip(node->state, HUMANCLR);// : TwoChip -= FindTwoChip(node->state, 1);

	//8
	int WinConfiguration = 0;
	if (End_of_Game(node) && node->move == ROBOTCLR)
		WinConfiguration = -1;
	if (End_of_Game(node) && node->move != ROBOTCLR)
		WinConfiguration = 1;


	(node->move = 2) ? node->move = 1 : node->move = 2;
	return 18 * clossedMorris + 26 * NumberOfMorris + 1 * NumberofBlocked + 9 * NumberChip + 15 * TwoChip;
	/*(node->move == 1) ? h += node->white : node->black;
	vector<vector<int>> v;
	if (FindMill(node->state, node->move, v))
		h += v.size();
	if (node->move == ROBOTCLR)
		return h;
	return -h;*/
}


void BetweenMills(vector<vector<int>>& v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < rMills.size(); j++) {
			if (v[i].size() != 0 && rMills[j].size() != 0) {
				vector<int> rob = rMills[j];
				if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[3])
					v.erase(v.begin() + i);
				else rMills.push_back(v[i]);
			}
		}
	}
}

void pr(int* p) {
	for (int i = 0; i < 24; i++) {
		cout << *p << " ";
		p++;
	}

}

//for phase1

Node* max_node = nullptr;
Node* LookNewPos(Node* node, int& depth) {
	if (depth >= MAX_DEPTH || node->white == 9 || node->black == 9) {
		return node;
	}
	int* state = node->state;
	int MAX = INT32_MIN;
	int MIN = INT32_MAX;
	if (node->move == ROBOTCLR) {
		for (int i = 0; i < 24; i++) {
			if (state[i] == 0) {
				int* new_state = new int[24];
				copy(state, state + 24, new_state);
				new_state[i] = node->move;
				//pr(new_state);
				//cout << "  HEu " << node->fun << endl;
				Node* new_node = nullptr;
				if(ROBOTCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, HUMANCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, HUMANCLR, 1, 0, node);


				vector<vector<int>> v = { };
				if (FindMill(new_state, ROBOTCLR, v)) {
					BetweenMills(v);
					if (v.size() == 1) {
						if (ROBOTCLR == 2)
							new_node->white--;
						else new_node->black--;
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);
							
							new_state[pos] = 0;
							int f = Heuristics1(new_node);
							new_node->fun = f;

							depth += 1;
							Node* next = LookNewPos(new_node, depth);
							++it;

							if (next != nullptr && MAX < next->fun) {
								MAX = next->fun;
								max_node = next;
							}
						}
					}
					else {
						if (ROBOTCLR == 2)
							new_node->white-=2;
						else new_node->black-=2;
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							new_state[pos] = 0;
							auto it2 = new_state + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state + 24, HUMANCLR)) != (new_state + 24)) {
								pos = distance(new_state, it2);
								new_state[pos] = 0;
								int f = Heuristics1(new_node);
								new_node->fun = f;

								depth += 1;
								Node* next = LookNewPos(new_node, depth);
								
								if (next != nullptr && MAX < next->fun) {
									MAX = next->fun;
									max_node = next;
								}

								it2++;
							}
							++it;

						}
					}
						
				}
				else {
					int f = Heuristics1(new_node);
					new_node->fun = f;

					depth += 1;
					Node* next = LookNewPos(new_node, depth);

					if (next != nullptr && MAX < next->fun) {
						MAX = next->fun;
						max_node = next;
					}
				}

			}
		}
		return max_node;
	}
	else{
		for (int i = 0; i < 24; i++) {
			if (state[i] == 0) {
				int* new_state = new int[24];
				copy(state, state + 24, new_state);
				new_state[i] = node->move;
				Node* new_node = nullptr;

				if (HUMANCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, ROBOTCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, ROBOTCLR, 1, 0, node);

				vector<vector<int>> v = { };
				if (FindMill(new_state, HUMANCLR, v)) {
					BetweenMills(v);
					if (v.size() == 1) {
						if (HUMANCLR == 2)
							new_node->white--;
						else new_node->black--;
						auto it = new_state;
						while ((it = find(it, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							new_state[pos] = 0;
							int f = Heuristics1(new_node);
							new_node->fun = f;

							depth += 1;
							Node* next = LookNewPos(new_node, depth);
							++it;

							if (next != nullptr && MIN > next->fun) {
								MIN = next->fun;
								max_node = next;
							}
						}
					}
					else {
						if (ROBOTCLR == 2)
							new_node->white -= 2;
						else new_node->black -= 2;
						auto it = new_state;
						while ((it = find(it, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							new_state[pos] = 0;
							auto it2 = new_state + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
								pos = distance(new_state, it2);
								new_state[pos] = 0;
								int f = Heuristics1(new_node);
								new_node->fun = f;

								depth += 1;
								Node* next = LookNewPos(new_node, depth);

								if (next != nullptr && MIN > next->fun) {
									MIN = next->fun;
									max_node = next;
								}

								it2++;
							}
							++it;

						}
					}

				}
				else {
					int f = Heuristics1(new_node);
					new_node->fun = f;

					depth += 1;
					Node* next = LookNewPos(new_node, depth);

					if (next != nullptr && MIN > next->fun) {
						MAX = next->fun;
						max_node = next;
					}
				}
			}
		}
		return max_node;
	}
}

void StartGame() {
	const int n = 24;
	int arr[n];
	cout << "\nПоле до начала игры\n0-это пустые ячейки\n1 - это белые фишки\n2 - это черные фишки" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	//arr[3] = 1;
	//arr[1] = 2;
	//arr[4] = 1;

	Node* startNode = new Node(arr, 2, 1, 1, 1, 0);
	
	//FirstPlayer();
	ROBOTCLR = 1;
	HUMANCLR = 2;
	print_pos(startNode->state);
	for (int i = 0; i < 9; i++) {
		if (HUMANCLR == 1 || i > 0) {
			string inputstr;
			std::cout << "Введите поля: ";
			std::cin >> inputstr;
			while (CONVERT.count(inputstr) == 0) {
				std::cout << "Неправильное значение. ";
				std::cout << "Введите поля: ";
				std::cin >> inputstr;
			}
			PutChip(startNode->state, inputstr, HUMANCLR);
			startNode->move = ROBOTCLR;
			if(HUMANCLR == 1)
				startNode->white += 1;
			else startNode->black += 1;
			print_pos(startNode->state);
		}
		Node* max_n = nullptr;
		int depth = 0;
		max_n = LookNewPos(startNode, depth);
		while (max_n->prevNode->prevNode != nullptr)
		{
			Node* m = max_n;
			max_n = max_n->prevNode;
			delete[] m->state;
			delete m;
		}
		startNode = max_n;
		startNode->prevNode = nullptr;
		print_pos(startNode->state);
	}
}
void Part1(Node* node) {

}