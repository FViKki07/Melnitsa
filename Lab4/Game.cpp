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

bool compareByField(const Node* obj1, const Node* obj2) {
	return obj1->fun < obj2->fun;
}

bool PutChip(Node* node, string pos, int color) { 
	stringstream ss(pos);
	string word;
	vector<string> vv = {};
	while (ss >> word) {
		vv.push_back(word);
	}
	if(vv.size() == 0) return false;
	int* state = node->get_state();
	//print_pos(state);
	if (CONVERT.count(vv[0]) == 0 || state[CONVERT.at(vv[0])] != 0) {
		std::cout << "Неправильное значение для вашей фишки " << endl;
		return false;
	}
	int clr = state[CONVERT.at(vv[0])];
	node->new_state(CONVERT.at(vv[0]));
	vector<vector<int>> v = {};
	if (FindMill(node->get_state(), color, v) && BetweenMills(node, v)) {
		if (vv.size() < v.size() + 1) {
			auto vec_s = node->oppMills.size();
			if (v.size() == 1) {
				node->oppMills.erase(node->oppMills.begin() + vec_s - 1);
			}
			else {
				node->oppMills.erase(node->oppMills.begin() + vec_s - 2, node->oppMills.begin() + vec_s - 1);
			}
			cout << "Введите значение для удаление фишки противника" << endl;
			node->new_state(CONVERT.at(vv[0]), 0);
			return false;
		}
		for (int i = 0; i < v.size(); i++) {
			if (CONVERT.count(vv[i + 1]) == 0 || state[CONVERT.at(vv[i + 1])] != ROBOTCLR) {
				std::cout << "Неправильное значение для фишки противника " << endl;
				node->new_state(CONVERT.at(vv[0]), 0);
				return false;
			}
			int pos = CONVERT.at(vv[i + 1]);
			node->new_state(pos, 0);
		

			if (ROBOTCLR == 1) {
				node->white--;
				node->new_Steal_w(node->steal_w + 1);
			}
			else {
				node->black--;
				node->new_Steal_b(node->steal_b + 1);
			}
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

		if ( state[a] == state[b] && state[c] == 0 ||
			state[b] == state[c] && state[a] ==0 ||
			state[a] == state[c] && state[b] == 0) { 
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
		if (FindMill(node->get_state(), HUMANCLR, v))
			clossedMorris = -1;
	}
	else {
		if (FindMill(node->get_state(), ROBOTCLR, v))
			clossedMorris = 1;

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
	return 18 * clossedMorris + 26 * NumberOfMorris + NumberofBlocked + 9 * NumberChip + 10 * TwoChip;
	/*(node->move == 1) ? h += node->white : node->black;
	vector<vector<int>> v;
	if (FindMill(node->state, node->move, v))
		h += v.size();
	if (node->move == ROBOTCLR)
		return h;
	return -h;*/
}


bool BetweenMills(Node* node, vector<vector<int>>& v) {//неправильно 
	int count = v.size();
	vector<vector<int>> mills = {};
	vector<vector<int>> new_mills = {};
	if (ROBOTCLR == node->move) mills = node->rMills;
	else mills = node->oppMills;
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < mills.size(); j++) {
				vector<int> rob = mills[j];
				if (!(v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])) {
					new_mills.push_back(v[i]);
					mills.push_back(v[i]);
				}
		}
		if (mills.size() == 0) {
			new_mills.push_back(v[i]);
			mills.push_back(v[i]);
		}

	}
	v = new_mills;
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

void Steal(Node* node) {
	if (node->move == HUMANCLR) {
		if (HUMANCLR == 1)
			node->steal_b++;
		else node->steal_w++;
	}
	else {
		if (ROBOTCLR == 1)
			node->steal_b++;
		else node->steal_w++;
	}
}


//for phase1

int WHITE = 9;
int BLACK = 9;
Node* LookNewPos(Node* node, int& depth) {
	if (depth >= MAX_DEPTH || (node->white + node->steal_w >= 9 && node->black + node->steal_b >= 9)) {
		int f = Heuristics1(node);
		node->fun = f;
		return node;
	}
	int* state = node->get_state();
	int MAX = INT32_MIN;
	int MIN = INT32_MAX;
	if (node->move == ROBOTCLR) {
		for (int i = 0; i < 24; i++) {
			if (state[i] == 0) {
				int* new_state = new int[24];
				std::copy(state, state + 24, new_state);
				new_state[i] = node->move;
				//pr(new_state);
				//cout << "  HEu " << node->fun << endl;
				Node* new_node = nullptr;
				if(ROBOTCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, ROBOTCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, ROBOTCLR, 1, 0, node);
				new_node->new_Steal_b(node->steal_b);
				new_node->new_Steal_w(node->steal_w);

				vector<vector<int>> v = { };
				if (FindMill(new_state, ROBOTCLR, v) && BetweenMills(new_node, v)) { //добавить new_node
					if (v.size() == 1) {
						if (ROBOTCLR == 2) {
							new_node->new_Steal_w(new_node->steal_w + 1);
							new_node->white--;
						}
						else {
							new_node->new_Steal_b(new_node->steal_b + 1);
							new_node->black--;
						}
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);
							
							int* new_state2 = new int[24];
							std::copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							int s[24];
							std::copy(new_state2, new_state2 + 24, s);
							new_node->new_state(new_state2);
							//int f = Heuristics1(new_node);
							//new_node->fun = f;

							depth += 1;
							new_node->move = HUMANCLR;
							Node* next = LookNewPos(new_node, depth);
							depth -= 1;

							node->child_fun.push_back(next);
							++it;
						}
					}
					else if (v.size() == 2) {
			
						if (ROBOTCLR == 2) {
							new_node->new_Steal_w(new_node->steal_w + 2);
							new_node->white--;
						}
						else {
							new_node->new_Steal_b(new_node->steal_b + 2);
							new_node->black--;
						}
						auto it = new_state;
						while ((it = find(it, new_state + 24, HUMANCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);
							int* new_state2 = new int[24];
							std::copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;

							auto it2 = new_state2 + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state2 + 24, HUMANCLR)) != (new_state2 + 24)) {
								pos = distance(new_state2, it2);
								int* new_state3 = new int[24];
								std:: copy(new_state2, new_state2 + 24, new_state3);
								new_state3[pos] = 0;
								new_node->new_state(new_state3);//то же у человека и тут для 2ух фишек
								//int f = Heuristics1(new_node);
								//new_node->fun = f;

								depth += 1;
								new_node->move = HUMANCLR;
							    Node* next = LookNewPos(new_node, depth);
								depth -= 1;

								node->child_fun.push_back(next);

								it2++;
							}
							++it;

						}
					}
						
				}
				else {
					//int f = Heuristics1(new_node);
					//new_node->fun = f;

					depth += 1;
					new_node->move = HUMANCLR;
					Node* next = LookNewPos(new_node, depth);
					depth -= 1;

					node->child_fun.push_back(next);
				}

			}
		}
		Node* n = *std::max_element(begin(node->child_fun), end(node->child_fun), compareByField);
		node->fun = n->fun;
		return node;
	}
	else{
		for (int i = 0; i < 24; i++) {
			if (state[i] == 0) {
				int* new_state = new int[24];
				int s[24];
				
				std::copy(state, state + 24, new_state);
				std::copy(new_state, new_state + 24, s);
				new_state[i] = node->move;
				Node* new_node = nullptr;

				if (HUMANCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, HUMANCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, HUMANCLR, 1, 0, node);
				new_node->new_Steal_b(node->steal_b);
				new_node->new_Steal_w(node->steal_w);

				vector<vector<int>> v = { };
				if (FindMill(new_state, HUMANCLR, v) && BetweenMills(new_node, v)) {
					if (v.size() == 1) {
						if (HUMANCLR == 2) {
							new_node->new_Steal_w(new_node->steal_w + 1);
							new_node->white--;
						}
						else {
							new_node->new_Steal_b(new_node->steal_b + 1);
							new_node->black--;
						}
						auto it = new_state;
						while ((it = find(it, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							int* new_state2 = new int[24];
							std::copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							new_node->new_state(new_state2);
							//int f = Heuristics1(new_node);
							//new_node->fun = f;

							depth += 1;
							new_node->move = ROBOTCLR;
							Node* next = LookNewPos(new_node, depth);
							depth -= 1;

							++it;
							node->child_fun.push_back(next);
						}
					}
					else if (v.size() == 2) {
						if (HUMANCLR == 2) {
							new_node->new_Steal_w(new_node->steal_w + 2);
							new_node->white--;
						}
						else {
							new_node->new_Steal_b(new_node->steal_b + 2);
							new_node->black--;
						}
						auto it = new_state;
						while ((it = find(it, new_state + 24, ROBOTCLR)) != (new_state + 24)) {
							int pos = distance(new_state, it);

							int* new_state2 = new int[24];
							std::copy(new_state, new_state + 24, new_state2);
							new_state2[pos] = 0;
							new_node->new_state(new_state2);
							auto it2 = new_state2 + pos + 1;//может не надо + 1
							while ((it2 = find(it2, new_state2 + 24, ROBOTCLR)) != (new_state2 + 24)) {
								pos = distance(new_state2, it2);
								
								int* new_state3 = new int[24];
								std::copy(new_state2, new_state2 + 24, new_state3);
								new_state3[pos] = 0;
								new_node->new_state(new_state3);
								//int f = Heuristics1(new_node);
								//new_node->fun = f;

								depth += 1;
								new_node->move = ROBOTCLR;
								Node* next = LookNewPos(new_node, depth);
								depth -= 1;
								node->child_fun.push_back(next);

								it2++;
							}
							++it;

						}
					}

				}
				else {
					//int f = Heuristics1(new_node);
					//new_node->fun = f;

					depth += 1;
					new_node->move = ROBOTCLR;
					Node* next = LookNewPos(new_node, depth);
					depth -= 1;
					node->child_fun.push_back(next);
				}
			}
		}
		Node* n = *std::min_element(begin(node->child_fun), end(node->child_fun), compareByField);
		node->fun = n->fun;
		return node;
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
	bool human = (HUMANCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	bool robot = (ROBOTCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	while(startNode->black + startNode->steal_b <9 || startNode->white + startNode->steal_w < 9){
		if ((HUMANCLR == 1 || startNode->black > 0 || startNode->white > 0) && human) {
			std::cout << "Введите поля: ";
			char s[8];
			cin.getline(s, 8);
			while (!PutChip(startNode, s, HUMANCLR)) {
				std::cout << "Введите поля: ";
				cin.getline(s, 8);
			}
			startNode->move = ROBOTCLR;
			if(HUMANCLR == 1)
				startNode->white += 1;
			else startNode->black += 1;
			print_pos(startNode->get_state());
		}

		if (robot) {
			int depth = 0;
			Node* max_n = LookNewPos(startNode, depth);
			for (int i = 0; i < max_n->child_fun.size(); i++) {
				if (max_n->child_fun[i]->fun == max_n->fun) {
					startNode = max_n->child_fun[i];
					break;
				}
			}
			//startNode->prevNode = nullptr;
			startNode->move = HUMANCLR;
			startNode->child_fun.clear();
			print_pos(startNode->get_state());
		}
		human = (HUMANCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
		robot = (ROBOTCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	}
}
void Part1(Node* node) {

}