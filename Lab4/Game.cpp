
#include "Game.h"
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
	if (vv[0]._Equal("-r") && node->prevNode != nullptr && node->prevNode->prevNode != nullptr) {
		node = node->prevNode->prevNode;
		return true;
	}
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


bool MoveChip(Node* node, string pos, int color) {
	stringstream ss(pos);
	string word;
	vector<string> vv = {};
	while (ss >> word) {
		vv.push_back(word);
	}
	if (vv.size()>0 && vv[0]._Equal("-r") && node->prevNode != nullptr && node->prevNode->prevNode != nullptr) {
		node = node->prevNode->prevNode;
		return true;
	}
	if (vv.size() < 2 ) return false;
	int* state = node->get_state();
	//print_pos(state);
	if (CONVERT.count(vv[0]) == 0 || state[CONVERT.at(vv[0])] != HUMANCLR ||
		CONVERT.count(vv[1]) == 0 || state[CONVERT.at(vv[1])] != 0) {
		std::cout << "Неправильное значение для вашей фишки " << endl;
		return false;
	}
	auto neig = NEIGHBORS.at(CONVERT.at(vv[0]));
	if (node->phase != 3 && count(neig.begin(), neig.end(), CONVERT.at(vv[1])) == 0) {
		std::cout << "Неправильное значение для вашей фишки " << endl;
		return false;
	}

	node->new_state(CONVERT.at(vv[0]), 0);
	node->new_state(CONVERT.at(vv[1]));
	//int clr = state[CONVERT.at(vv[0])];
	vector<vector<int>> v = {};
	if (FindMill(node->get_state(), color, v) && BetweenMills(node, v)) {
		if (vv.size() < v.size() + 2) {
			cout << "Введите значение для удаление фишки противника" << endl;
			node->new_state(CONVERT.at(vv[0]));
			node->new_state(CONVERT.at(vv[1]), 0);
			return false;
		}
		for (int i = 0; i < v.size(); i++) {
			if (CONVERT.count(vv[2]) == 0 || state[CONVERT.at(vv[2])] != ROBOTCLR) {
				auto vec_s = node->oppMills.size();
				if (v.size() == 1) {
					node->oppMills.erase(node->oppMills.begin() + vec_s - 1);
				}
				else {
					node->oppMills.erase(node->oppMills.begin() + vec_s - 2, node->oppMills.begin() + vec_s - 1);
				}
				std::cout << "Неправильное значение для фишки противника " << endl;
				node->new_state(CONVERT.at(vv[0]));
				node->new_state(CONVERT.at(vv[1]), 0);
				return false;
			}
			int pos = CONVERT.at(vv[2]);
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
	}
	return true;
}

bool End_of_Game(Node* node){
	int r = (ROBOTCLR == 1) ? node->white : node->black;
	int h = (HUMANCLR == 1) ? node->white : node->black;

	if (ROBOTCLR == node->move) {
		vector<int> b = {};
		if (r < 3 || !AbilityMove(node->get_state(), node->move, b))
			return true;
		return false;
	}
	else {
		vector<int> b = {};
		if (h < 3 || !AbilityMove(node->get_state(), node->move, b))
			return true;
		return false;
	}
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
	return true;

}

int FindTwoChip(Node* node, int clr) {
	int count = 0;
	auto state = node->get_state();
	auto mills = (ROBOTCLR == clr) ? node->rMills : node->oppMills;
	for (auto it : MILLS) {
		int a = it[0];
		int b = it[1];
		int c = it[2];

		for (int i = 0; i < mills.size(); i++) 
			if (a == mills[i][0] && b == mills[i][1] ||
				a == mills[i][0] && c == mills[i][2] ||
				c == mills[i][2] && b == mills[i][1])
				continue;
		
		if ( state[a] == state[b]==clr && state[c] == 0 ||
			state[b] == state[c]==clr && state[a] ==0 ||
			state[a] == state[c]==clr && state[b] == 0) { 
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
	auto mills = (ROBOTCLR == node->move) ? node->oppMills : node->rMills;
	int clossedMorris = 0;
	vector<vector<int>> v;
	if (node->move == ROBOTCLR) {
		if (FindMill(node->get_state(), HUMANCLR, v)) {
			clossedMorris = -1;
			for (int i = 0; i < v.size(); i++) {
				bool flag = false;
				for (int j = 0; j < mills.size(); j++) {
					vector<int> rob = mills[j];
					if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])
						flag = true;
				}
				if (!flag) {
					clossedMorris = -1; 
					break;
				}
			}
		}
	}
	else {
		if (FindMill(node->get_state(), ROBOTCLR, v))
			clossedMorris = 1;
			for (int i = 0; i < v.size(); i++) {
				bool flag = false;
				for (int j = 0; j < mills.size(); j++) {
					vector<int> rob = mills[j];
					if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])
						flag = true;
				}
				if (!flag) {
					clossedMorris = 1;
					break;
				}
			}

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
	int TwoChip = FindTwoChip(node, ROBOTCLR) ;
	TwoChip -= FindTwoChip(node, HUMANCLR);// : TwoChip -= FindTwoChip(node->state, 1);

	//8
	int WinConfiguration = 0;
	if (End_of_Game(node) && node->move == ROBOTCLR)
		WinConfiguration = -1;
	if (End_of_Game(node) && node->move != ROBOTCLR)
		WinConfiguration = 1;


	//(node->move == 2) ? node->move = 1 : node->move = 2;
	if(node->phase == 1)
		return 18 * clossedMorris + 26 * NumberOfMorris + NumberofBlocked + 9 * NumberChip + 10 * TwoChip;
	if (node->phase == 2)
		return 14 * clossedMorris + 43 * NumberOfMorris + 10 * NumberofBlocked + 11 * NumberChip + 1086 * WinConfiguration;
	else {
		return 16 * clossedMorris + 10 * TwoChip + 1190 * WinConfiguration;
	}
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
		bool flag = false;
		for (int j = 0; j < mills.size(); j++) {
				vector<int> rob = mills[j];
				if (v[i][0] == rob[0] && v[i][1] == rob[1] && v[i][2] == rob[2])
					flag = true;
				
				
		}
		if (!flag) {
			new_mills.push_back(v[i]);
			mills.push_back(v[i]);
		}/*
		if (mills.size() == 0) {
			new_mills.push_back(v[i]);
			mills.push_back(v[i]);
		}*/

	}
	v = new_mills;
	if (ROBOTCLR == node->move)  node->rMills = mills;
	else node->oppMills = mills;
	if (v.size() > 0) return true;
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

string GetKey(int val) {
	return find_if(CONVERT.begin(), CONVERT.end(),
		[&](const auto& pair) { return pair.second == val; })->first;

}

void StartGame() {
	const int n = 24;
	int arr[n];
	cout << "\nПоле до начала игры\n0-это пустые ячейки\n1 - это белые фишки\n2 - это черные фишки" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	//double new_arr[24] = {1,0,2,0,2,1,0,0,0,0,1,0,0,0,0,2,0,2,0,0,2,0,0,2};
	//copy(new_arr, new_arr + 24, arr);
	//print_pos(arr);
	//arr[3] = 1;
	//arr[1] = 2;
	//arr[4] = 1;

	//Node* startNode = new Node(arr, 3, 6, 1, 0, 0);
	Node* startNode = new Node(arr, 0, 0, 1, 1, 0);
	//FirstPlayer();
	//ROBOTCLR = 1;
	//HUMANCLR = 2;
	//print_pos(startNode->get_state());
	bool human = (HUMANCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	bool robot = (ROBOTCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	while((startNode->black + startNode->steal_b <9 || startNode->white + startNode->steal_w < 9 ) ){//erase fasle!!!


		cout << "Белых фишек осталось " << 9 - startNode->white - startNode->steal_w << endl;
		cout << "Черных фишек осталось " << 9 - startNode->black - startNode->steal_b << endl;
		cout << "Ваши мельницы:" << endl;
		for (auto m : startNode->oppMills)
			cout << GetKey(m[0]) << " " << GetKey(m[1]) << " " << GetKey(m[2]) << endl;
		cout << "Бота мельницы:" << endl;
		for (auto m : startNode->rMills)
			cout << GetKey(m[0]) << " " << GetKey(m[1]) << " " << GetKey(m[2]) << endl;

		if ((HUMANCLR == 1 || startNode->black > 0 || startNode->white > 0) && human) {
			std::cout << "Введите поля: ";
			char s[8];
			cin.getline(s, 8);
			auto new_node = new Node(arr, 0, 0, 1, 1, 0);
			new_node->NewNode(startNode);
			while (!PutChip(new_node, s, HUMANCLR)) {
				std::cout << "Введите поля: ";
				cin.getline(s, 8);
			}
			new_node->prevNode = startNode;
			startNode = new_node;
			startNode->move = ROBOTCLR;
			if(HUMANCLR == 1)
				startNode->white += 1;
			else startNode->black += 1;
			print_pos(startNode->get_state());
		}

		if (robot) {
			int depth = 0;
			int alpha = INT16_MIN;
			int beta = INT16_MAX;
			Node* max_n = LookNewPos(startNode, depth, alpha, beta);
			std::vector<Node*> max_elements;
			std::copy_if(max_n->child_fun.begin(),  max_n->child_fun.end(), std::back_inserter(max_elements), [max_n](Node* value) {
				return value->fun == max_n->fun;
				});

			std::srand(std::time(0));
			int random_index = std::rand() % max_elements.size();
			Node* mx = max_elements[random_index];
			mx->prevNode = startNode;
			startNode =mx;
			/*
			for (int i = 0; i < max_n->child_fun.size(); i++) {
				if (max_n->child_fun[i]->fun == max_n->fun) {
					startNode = max_n->child_fun[i];
					break;
				}
				delete[] max_n->child_fun[i]->get_state();
				delete max_n->child_fun[i];
			}
			startNode->prevNode = nullptr;*/
			startNode->move = HUMANCLR;
			startNode->child_fun.clear();
			print_pos(startNode->get_state());
		}
		human = (HUMANCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
		robot = (ROBOTCLR == 1) ? startNode->white + startNode->steal_w < 9 : startNode->black + startNode->steal_b < 9;
	}



	std::cout << "Переместить свою фишку на соседнее свободное место.\nВведите сначала поле фишки, а потом поле,куда хотите ее переместить ( в конце поле для удаления, если потребуется)" << endl;
	while (startNode->white > 2  && startNode->black > 2) {
		cout << "Белых фишек осталось " << startNode->white<< endl;
		cout << "Черных фишек осталось " <<  startNode->black << endl;
		cout << "Ваши мельницы:" << endl;
		for (auto m : startNode->oppMills)
			cout << GetKey(m[0]) << " " << GetKey(m[1]) << " " << GetKey(m[2]) << endl;
		cout << "Бота мельницы:" << endl;
		for (auto m : startNode->rMills)
			cout << GetKey(m[0]) << " " << GetKey(m[1]) << " " << GetKey(m[2]) << endl;
		human = (HUMANCLR == 1) ? startNode->white == 3 : startNode->black == 3;
		robot = (ROBOTCLR == 1) ? startNode->white == 3 : startNode->black == 3;
		if (HUMANCLR == startNode->move) {
			if (End_of_Game(startNode)) 
				break;

			if (human) {
				startNode->phase = 3;
				std::cout << "Переместить свою фишку на ЛЮБОЕ свободное место.\nВведите сначала поле фишки, а потом поле,куда хотите ее переместить ( в конце поле для удаления, если потребуется)" << endl;
			}
			else startNode->phase = 2;
			std::cout << "Введите поля: ";
			char s[12];
			cin.getline(s, 12);
			while (!MoveChip(startNode, s, HUMANCLR)) {
				std::cout << "Введите поля: ";
				cin.getline(s, 12);
			}
			startNode->move = ROBOTCLR;
			print_pos(startNode->get_state());
		}
		else {
			int depth = 0;
			int alpha = INT16_MIN;
			int beta = INT16_MAX;
			if (End_of_Game(startNode)) 
				break;
			if (robot) {
				startNode->phase = 3;
				std::cout << "Теперь бот может перемещать свои фишки куда угодно" << endl;
			}
			else startNode->phase = 2;
			Node* max_n = Part23(startNode, depth, alpha, beta);
			std::vector<Node*> max_elements;
			std::copy_if(max_n->child_fun.begin(), max_n->child_fun.end(), std::back_inserter(max_elements), [max_n](Node* value) {
				return value->fun == max_n->fun;
				});

			std::srand(std::time(0));
			int random_index = std::rand() % max_elements.size();
			Node* mx = max_elements[random_index];
			mx->prevNode = startNode;
			startNode = mx;
			/*for (int i = 0; i < max_n->child_fun.size(); i++) {
				if (max_n->child_fun[i]->fun == max_n->fun) {
					startNode = max_n->child_fun[i];
					break;
				}
				delete[] max_n->child_fun[i]->get_state();
				delete max_n->child_fun[i];
			}*/
			//startNode->prevNode = nullptr;
			startNode->move = HUMANCLR;
			startNode->child_fun.clear();
			print_pos(startNode->get_state());
		}
	}
	if (End_of_Game(startNode) && startNode->move == ROBOTCLR ) 
		cout << "Вы выиграли!" << endl;
	else cout << "Вы проиграли!" << endl;
}
