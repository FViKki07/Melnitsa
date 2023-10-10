#include "Lab4.h"
using namespace std;

/*
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
}*/

bool PutChip(Node* node, string pos, int color) {
	stringstream ss(pos);
	string word;
	vector<string> vv = {};
	while (ss >> word) {
		vv.push_back(word);
	}
	if(vv.size() == 0) return false;
	if (CONVERT.count(vv[0]) == 0)
		return false; 
	node->new_state(CONVERT.at(vv[0]));
	vector<vector<int>> v = {};
	if (FindMill(node->get_state(), color, v) && BetweenMills(node, v)) {
		for (int i = 0; i < v.size(); i++) {
			if (CONVERT.count(vv[i + 1]) == 0) return false;
			int pos = CONVERT.at(vv[i + 1]);
			node->new_state(pos, 0);
			if (ROBOTCLR == 1) node->white--;
			else node->black--;
		}

		return true;
	}
	return true;
}

bool End_of_Game(Node* node){
	vector<int> b = {};
	if (node->black < 3 || node->white < 3 || !AbilityMove(node->get_state(), node->move, b))
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
	//(node->move == 2) ? node->move = 1 : node->move = 2;
	//1
	int clossedMorris = 0;
	vector<vector<int>> v;
	if (node->move == ROBOTCLR) {
		if (FindMill(node->get_state(), node->move, v))
			clossedMorris = 1;
	}
	else {
		if (FindMill(node->get_state(), node->move, v))
			clossedMorris = -1;

	}

	//2
	int NumberOfMorris = node->rMills.size() - node->oppMills.size();

	//3
	vector<int> blocked = {};
	AbilityMove(node->get_state(), HUMANCLR, blocked);
	int NumberofBlocked = blocked.size();
	blocked.clear();
	AbilityMove(node->get_state(), ROBOTCLR, blocked);
	NumberofBlocked -= blocked.size();

	//4
	int NumberChip = 0;
	if(ROBOTCLR == 1)
		NumberChip = node->white - node->black;
	else NumberChip = node->black - node->white;

	//5
	int TwoChip = FindTwoChip(node->get_state(), ROBOTCLR) ;
	TwoChip -= FindTwoChip(node->get_state(), HUMANCLR);// : TwoChip -= FindTwoChip(node->state, 1);

	//8
	int WinConfiguration = 0;
	if (End_of_Game(node) && node->move == ROBOTCLR)
		WinConfiguration = -1;
	if (End_of_Game(node) && node->move != ROBOTCLR)
		WinConfiguration = 1;


	//(node->move == 2) ? node->move = 1 : node->move = 2;
	return 18 * clossedMorris + 26 * NumberOfMorris + 1 * NumberofBlocked + 9 * NumberChip + 15 * TwoChip;
	/*(node->move == 1) ? h += node->white : node->black;
	vector<vector<int>> v;
	if (FindMill(node->state, node->move, v))
		h += v.size();
	if (node->move == ROBOTCLR)
		return h;
	return -h;*/
}


bool BetweenMills(Node* node, vector<vector<int>>& v) {
	int count = v.size();
	vector<vector<int>> mills = {};
	if (ROBOTCLR == node->move) mills = node->rMills;
	else mills = node->oppMills;
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < mills.size(); j++) {
				vector<int> rob = mills[j];
				if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])
					v.erase(v.begin() + i);
				else mills.push_back(v[i]);
		}
		if (mills.size() == 0) {
			mills.push_back(v[i]);
		}
	}
	if (ROBOTCLR == node->move)  node->rMills = mills;
	else node->oppMills = mills;
	if (count == v.size()) return true;
	return false;
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
	int* state = node->get_state();
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
					new_node = new Node(new_state, node->white, node->black + 1, ROBOTCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, ROBOTCLR, 1, 0, node);


				vector<vector<int>> v = { };
				if (FindMill(new_state, ROBOTCLR, v) && BetweenMills(new_node, v)) {
					if (v.size() == 1) {
						if (ROBOTCLR == 2)
							new_node->white--;
						else new_node->black--;
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);
							
							int* new_state2 = new int[24];
							copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							int s[24];
							copy(new_state2, new_state2 + 24, s);
							new_node->new_state(new_state2);
							int f = Heuristics1(new_node);
							new_node->fun = f;

							depth += 1;
							new_node->move = HUMANCLR;
							Node* next = LookNewPos(new_node, depth);

							if (next != nullptr && MAX < next->fun) {
								MAX = next->fun;
								max_node = next;
							}
							++it;
						}
					}
					else{
			
						if (ROBOTCLR == 2)
							new_node->white-=2;
						else new_node->black-=2;
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);
							int* new_state2 = new int[24];
							copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;

							auto it2 = new_state2 + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state2 + 24, HUMANCLR)) != (new_state2 + 24)) {
								pos = distance(new_state2, it2);
								int* new_state3 = new int[24];
								copy(new_state2, new_state2 + 24, new_state3);
								new_state3[pos] = 0;
								new_node->new_state(new_state3);//то же у человека и тут для 2ух фишек
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
				int s[24];
				
				copy(state, state + 24, new_state);
				copy(new_state, new_state + 24, s);
				new_state[i] = node->move;
				Node* new_node = nullptr;

				if (HUMANCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, HUMANCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, HUMANCLR, 1, 0, node);

				vector<vector<int>> v = { };
				if (FindMill(new_state, HUMANCLR, v) && BetweenMills(new_node, v)) {
					if (v.size() == 1) {
						if (HUMANCLR == 2)
							new_node->white--;
						else new_node->black--;
						auto it = new_state;
						while ((it = find(it, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							int* new_state2 = new int[24];
							copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							new_node->new_state(new_state2);
							int f = Heuristics1(new_node);
							new_node->fun = f;

							depth += 1;
							new_node->move = ROBOTCLR;
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

							int* new_state2 = new int[24];
							copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							new_node->new_state(new_state2);
							auto it2 = new_state2 + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state2 + 24, ROBOTCLR)) != (new_state2 + 24)) {
								pos = distance(new_state2, it2);
								
								int* new_state3 = new int[24];
								copy(new_state2, new_state2 + 24, new_state3);
								new_state3[pos] = 0;
								new_node->new_state(new_state3);
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
						MIN = next->fun;
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

	Node* startNode = new Node(arr, 0, 0, 1, 1, 0);
	
	//FirstPlayer();
	//ROBOTCLR = 1;
	//HUMANCLR = 2;
	//print_pos(startNode->get_state());
	for (int i = 0; i < 9; i++) {
		if (HUMANCLR == 1 || i > 0) {
			std::cout << "Введите поля: ";
			char s[8];
			cin.getline(s, 8);
			while (!PutChip(startNode, s, HUMANCLR)) {
				std::cout << "Неправильное значение. ";
				std::cout << "Введите поля: ";
				cin.getline(s, 8);
			}
			startNode->move = ROBOTCLR;
			if(HUMANCLR == 1)
				startNode->white += 1;
			else startNode->black += 1;
			print_pos(startNode->get_state());
		}
		Node* max_n = nullptr;
		int depth = 0;
		max_n = LookNewPos(startNode, depth);
		while (max_n->prevNode->prevNode != nullptr)
		{
			Node* m = max_n;
			max_n = max_n->prevNode;
			delete[] m->get_state();
			delete m;
		}
		startNode = max_n;
		startNode->prevNode = nullptr;
		startNode->move = HUMANCLR;
		print_pos(startNode->get_state());
	}
}
void Part1(Node* node) {

}