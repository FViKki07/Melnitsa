#include "Game.h"

//for phase1

int WHITE = 9;
int BLACK = 9;
Node* LookNewPos(Node* node, int& depth, int alpha, int beta) {
	if ( depth >= MAX_DEPTH || (node->white + node->steal_w >= 9 && node->black + node->steal_b >= 9)) {
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
				if (ROBOTCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, ROBOTCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, ROBOTCLR, 1, 0, node);
				new_node->new_Steal_b(node->steal_b);
				new_node->new_Steal_w(node->steal_w);
				new_node->oppMills = node->oppMills;
				new_node->rMills = node->rMills;

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

							Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, HUMANCLR, 1, 0, node);
							new_node2->oppMills = new_node->oppMills;
							new_node2->rMills = new_node->rMills;
							new_node2->new_Steal_b(new_node->steal_b);
							new_node2->new_Steal_w(new_node->steal_w);
							depth += 1;
							//new_node->move = HUMANCLR;
							Node* next = LookNewPos(new_node2, depth, alpha, beta);
							depth -= 1;

							node->child_fun.push_back(next);
							//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							alpha = max(alpha, next->fun);
							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
								break;
							}
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
								std::copy(new_state2, new_state2 + 24, new_state3);
								new_state3[pos] = 0;
								//new_node->new_state(new_state3);//то же у человека и тут для 2ух фишек

								Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, HUMANCLR, 1, 0, node);
								new_node2->oppMills = new_node->oppMills;
								new_node2->rMills = new_node->rMills;
								new_node2->new_Steal_b(new_node->steal_b);
								new_node2->new_Steal_w(new_node->steal_w);
								depth += 1;
								Node* next = LookNewPos(new_node2, depth, alpha, beta);
								depth -= 1;

								node->child_fun.push_back(next);

								//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
								alpha = max(alpha, next->fun);
								if (beta <= alpha) {
									node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
									break;
								}


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
					Node* next = LookNewPos(new_node, depth, alpha, beta);
					depth -= 1;

					node->child_fun.push_back(next);

					//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
					alpha = max(alpha, next->fun);
					if (beta <= alpha) {
						node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
						break;
					}

				}

			}
		}
		Node* n = *std::max_element(begin(node->child_fun), end(node->child_fun), compareByField);
		node->fun = n->fun;
		return node;
	}
	else {
		for (int i = 0; i < 24; i++) {
			if (state[i] == 0) {
				int* new_state = new int[24];
				//int s[24];

				std::copy(state, state + 24, new_state);
				//std::copy(new_state, new_state + 24, s);
				new_state[i] = node->move;
				Node* new_node = nullptr;

				if (HUMANCLR == 2)
					new_node = new Node(new_state, node->white, node->black + 1, HUMANCLR, 1, 0, node);
				else new_node = new Node(new_state, node->white + 1, node->black, HUMANCLR, 1, 0, node);
				new_node->new_Steal_b(node->steal_b);
				new_node->new_Steal_w(node->steal_w);
				new_node->oppMills = node->oppMills;
				new_node->rMills = node->rMills;
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
							//new_node->new_state(new_state2);
							//int f = Heuristics1(new_node);
							//new_node->fun = f;
							Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, ROBOTCLR, 1, 0, node);
							new_node2->oppMills = new_node->oppMills;
							new_node2->rMills = new_node->rMills;
							new_node2->new_Steal_b(new_node->steal_b);
							new_node2->new_Steal_w(new_node->steal_w);
							depth += 1;
							//new_node->move = ROBOTCLR;
							Node* next = LookNewPos(new_node2, depth, alpha, beta);
							depth -= 1;

							++it;
							node->child_fun.push_back(next);


							//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							beta = min(beta, next->fun);

							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
								break;
							}
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
								Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, ROBOTCLR, 1, 0, node);
								//int f = Heuristics1(new_node);
								//new_node->fun = f;
								new_node2->oppMills = new_node->oppMills;
								new_node2->rMills = new_node->rMills;
								new_node2->new_Steal_b(new_node->steal_b);
								new_node2->new_Steal_w(new_node->steal_w);
								depth += 1;
								//new_node->move = ROBOTCLR;
								Node* next = LookNewPos(new_node2, depth, alpha, beta);
								depth -= 1;
								node->child_fun.push_back(next);


								//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;

								beta = min(beta, next->fun);

								if (beta <= alpha) {
									node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
									break;
								}

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
					Node* next = LookNewPos(new_node, depth, alpha, beta);
					depth -= 1;
					node->child_fun.push_back(next);

					//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
					beta = min(beta, next->fun);

					if (beta <= alpha) {
						node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
						break;
					}
				}
			}
		}
		Node* n = *std::min_element(begin(node->child_fun), end(node->child_fun), compareByField);
		node->fun = n->fun;
		return node;
	}
}

/*






*/

Node* Part23(Node* node, int& depth, int alpha, int beta) {
	vector<int> v = {};
	if (depth >= MAX_DEPTH || node->white < 3 || node->black <= 2 || !AbilityMove(node->get_state(), node->move, v)) {
		int f = Heuristics1(node);
		node->fun = f;
		return node;
	}
	int* state = node->get_state();
	int MAX = INT32_MIN;
	int MIN = INT32_MAX;
	bool human = (HUMANCLR == 1) ? node->white == 3 : node->black  == 3;
	bool robot = (ROBOTCLR == 1) ? node->white == 3 : node->black == 3;
	if (node->move == ROBOTCLR) {
		if (!robot) {
			auto iter = node->get_state();
			auto state = node->get_state();
			while ((iter = find(iter, state + 24, ROBOTCLR)) != (state + 24)) {
				int pos_c = distance(state, iter);
				auto neig = NEIGHBORS.at(pos_c);
				for (int i = 0; i < neig.size(); i++) {
					if (state[neig[i]] == 0) {
						int* new_state = new int[24];
						std::copy(state, state + 24, new_state);
						new_state[neig[i]] = node->move;
						new_state[pos_c] = 0;
						//pr(new_state);
						//cout << "  HEu " << node->fun << endl;
						Node* new_node = new Node(new_state, node->white, node->black, ROBOTCLR, 2, 0, node);
						new_node->new_Steal_b(node->steal_b);
						new_node->new_Steal_w(node->steal_w);
						new_node->oppMills = node->oppMills;
						new_node->rMills = node->rMills;


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
									//int s[24];
									//std::copy(new_state2, new_state2 + 24, s);
									Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, HUMANCLR, 2, 0, node);
									new_node2->oppMills = new_node->oppMills;
									new_node2->rMills = new_node->rMills;
									new_node2->new_Steal_b(new_node->steal_b);
									new_node2->new_Steal_w(new_node->steal_w);


									depth += 1;
									Node* next = Part23(new_node2, depth, alpha, beta);
									depth -= 1;

									node->child_fun.push_back(next);
									//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
									alpha = max(alpha, next->fun);
									if (beta <= alpha) {
										node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
										break;
									}
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
										std::copy(new_state2, new_state2 + 24, new_state3);
										new_state3[pos] = 0;
										//new_node->new_state(new_state3);//то же у человека и тут для 2ух фишек
										Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, HUMANCLR, 2, 0, node);
										depth += 1;
										new_node2->oppMills = new_node->oppMills;
										new_node2->rMills = new_node->rMills;
										new_node2->new_Steal_b(new_node->steal_b);
										new_node2->new_Steal_w(new_node->steal_w);

										//new_node->move = HUMANCLR;
										Node* next = Part23(new_node, depth, alpha, beta);
										depth -= 1;

										node->child_fun.push_back(next);

										//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
										alpha = max(alpha, next->fun);
										if (beta <= alpha) {
											node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
											break;
										}


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
							Node* next = Part23(new_node, depth, alpha, beta);
							depth -= 1;

							node->child_fun.push_back(next);

							//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							alpha = max(alpha, next->fun);
							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
								break;
							}

						}

					}
				}
				iter++;

			}
			Node* n = *std::max_element(begin(node->child_fun), end(node->child_fun), compareByField);
			node->fun = n->fun;
			return node;
		}
		else { // для фазы 3
			auto iter = node->get_state();
			auto state = node->get_state();
			while ((iter = find(iter, state + 24, ROBOTCLR)) != (state + 24)) {
				int pos_c = distance(state, iter);
				for (int i = 0; i < 24; i++) {
					if (state[i] == 0) {
						int* new_state = new int[24];
						std::copy(state, state + 24, new_state);
						new_state[i] = node->move;
						new_state[pos_c] = 0;
						//pr(new_state);
						//cout << "  HEu " << node->fun << endl;
						Node* new_node = new Node(new_state, node->white, node->black, ROBOTCLR, 2, 0, node);
						new_node->new_Steal_b(node->steal_b);
						new_node->new_Steal_w(node->steal_w);
						new_node->oppMills = node->oppMills;
						new_node->rMills = node->rMills;
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
									//int s[24];
									//std::copy(new_state2, new_state2 + 24, s);
									Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, HUMANCLR, 2, 0, node);
									//int f = Heuristics1(new_node);
									//new_node->fun = f;
									new_node2->oppMills = new_node->oppMills;
									new_node2->rMills = new_node->rMills;
									new_node2->new_Steal_b(new_node->steal_b);
									new_node2->new_Steal_w(new_node->steal_w);
									depth += 1;

									if (human) new_node2->phase = 3;
									Node* next = Part23(new_node2, depth, alpha, beta);
									depth -= 1;

									node->child_fun.push_back(next);
									//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
									alpha = max(alpha, next->fun);
									if (beta <= alpha) {
										node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
										break;
									}
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
										std::copy(new_state2, new_state2 + 24, new_state3);
										new_state3[pos] = 0;
										Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, HUMANCLR, 2, 0, node);//то же у человека и тут для 2ух фишек
										new_node2->oppMills = new_node->oppMills;
										new_node2->rMills = new_node->rMills;
										new_node2->new_Steal_b(new_node->steal_b);
										new_node2->new_Steal_w(new_node->steal_w);
										depth += 1;
										if (human) new_node2->phase = 3;
										Node* next = Part23(new_node2, depth, alpha, beta);
										depth -= 1;

										node->child_fun.push_back(next);

										//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
										alpha = max(alpha, next->fun);
										if (beta <= alpha) {
											node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
											break;
										}


										it2++;
									}
									++it;

								}
							}

						}
						else {

							depth += 1;
							new_node->move = HUMANCLR;
							if (human) new_node->phase = 3;
							Node* next = Part23(new_node, depth, alpha, beta);
							depth -= 1;

							node->child_fun.push_back(next);

							//alpha = (*std::max_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							alpha = max(alpha, next->fun);
							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MAX;
								break;
							}

						}

					}
				}
				iter++;

			}
			Node* n = *std::max_element(begin(node->child_fun), end(node->child_fun), compareByField);
			node->fun = n->fun;
			return node;
		}
	}
	else {
		if (!human) {
			auto iter = node->get_state();
			auto state = node->get_state();
			while ((iter = find(iter, state + 24, HUMANCLR)) != (state + 24)) {
				int pos_c = distance(state, iter);
				auto neig = NEIGHBORS.at(pos_c);
				for (int i = 0; i < neig.size(); i++) {
					if (state[neig[i]] == 0) {
						int* new_state = new int[24];
						std::copy(state, state + 24, new_state);
						new_state[neig[i]] = node->move;
						new_state[pos_c] = 0;
						Node* new_node = new Node(new_state, node->white, node->black, HUMANCLR, 2, 0, node);

						new_node->new_Steal_b(node->steal_b);
						new_node->new_Steal_w(node->steal_w);
						new_node->oppMills = node->oppMills;
						new_node->rMills = node->rMills;
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
									Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, ROBOTCLR, 2, 0, node);
									new_node2->oppMills = new_node->oppMills;
									new_node2->rMills = new_node->rMills;
									new_node2->new_Steal_b(new_node->steal_b);
									new_node2->new_Steal_w(new_node->steal_w);
									//int f = Heuristics1(new_node);
									//new_node->fun = f;

									depth += 1;
									if (robot) new_node2->phase = 3;
									Node* next = Part23(new_node2, depth, alpha, beta);
									depth -= 1;

									++it;
									node->child_fun.push_back(next);


									//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
									beta = min(beta, next->fun);

									if (beta <= alpha) {
										node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
										break;
									}
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
										Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, ROBOTCLR, 2, 0, node);
										new_node2->oppMills = new_node->oppMills;
										new_node2->rMills = new_node->rMills;
										new_node2->new_Steal_b(new_node->steal_b);
										new_node2->new_Steal_w(new_node->steal_w);
										//int f = Heuristics1(new_node);
										//new_node->fun = f;

										depth += 1;
										
										if (robot) new_node2->phase = 3;
										Node* next = Part23(new_node2, depth, alpha, beta);
										depth -= 1;
										node->child_fun.push_back(next);


										//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
										beta = min(beta, next->fun);
										if (beta <= alpha) {
											node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
											break;
										}

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
							if (robot) new_node->phase = 3;
							Node* next = Part23(new_node, depth, alpha, beta);
							depth -= 1;
							node->child_fun.push_back(next);

							//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							beta = min(beta, next->fun);
							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
								break;
							}
						}
					}
				}
				iter++;
			}
			Node* n = *std::min_element(begin(node->child_fun), end(node->child_fun), compareByField);
			node->fun = n->fun;
			return node;
		}
		else {
			auto iter = node->get_state();
			auto state = node->get_state();
			while ((iter = find(iter, state + 24, HUMANCLR)) != (state + 24)) {
				int pos_c = distance(state, iter);
				for (int i = 0; i < 24; i++) {
					if (state[i] == 0) {
						int* new_state = new int[24];
						std::copy(state, state + 24, new_state);
						new_state[i] = node->move;
						new_state[pos_c] = 0;
						Node* new_node = new Node(new_state, node->white, node->black, HUMANCLR, 2, 0, node);

						new_node->new_Steal_b(node->steal_b);
						new_node->new_Steal_w(node->steal_w);
						new_node->oppMills = node->oppMills;
						new_node->rMills = node->rMills;
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
									Node* new_node2 = new Node(new_state2, new_node->white, new_node->black, ROBOTCLR, 2, 0, node);
									new_node2->oppMills = new_node->oppMills;
									new_node2->rMills = new_node->rMills;
									new_node2->new_Steal_b(new_node->steal_b);
									new_node2->new_Steal_w(new_node->steal_w);
									//int f = Heuristics1(new_node);
									//new_node->fun = f;

									depth += 1;
									if (robot) new_node2->phase = 3;
									Node* next = Part23(new_node2, depth, alpha, beta);
									depth -= 1;

									++it;
									node->child_fun.push_back(next);


									//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
									beta = min(beta, next->fun);

									if (beta <= alpha) {
										node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
										break;
									}
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
										Node* new_node2 = new Node(new_state3, new_node->white, new_node->black, ROBOTCLR, 2, 0, node);
										//int f = Heuristics1(new_node);
										//new_node->fun = f;
										new_node2->oppMills = new_node->oppMills;
										new_node2->rMills = new_node->rMills;
										new_node2->new_Steal_b(new_node->steal_b);
										new_node2->new_Steal_w(new_node->steal_w);
										depth += 1;
										

										if (robot) new_node2->phase = 3;
										Node* next = Part23(new_node2, depth, alpha, beta);
										depth -= 1;
										node->child_fun.push_back(next);


										//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
										beta = min(beta, next->fun);
										if (beta <= alpha) {
											node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
											break;
										}

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
							if (robot) new_node->phase = 3;
							Node* next = Part23(new_node, depth, alpha, beta);
							depth -= 1;
							node->child_fun.push_back(next);

							//beta = (*std::min_element(begin(node->child_fun), end(node->child_fun), compareByField))->fun;
							beta = min(beta, next->fun);

							if (beta <= alpha) {
								node->child_fun[node->child_fun.size() - 1]->fun = INT16_MIN;
								break;
							}
						}
					}
				}
				iter++;
			}
			Node* n = *std::min_element(begin(node->child_fun), end(node->child_fun), compareByField);
			node->fun = n->fun;
			return node;
		}
	}

}