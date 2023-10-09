#include "Lab4.h"

int main() {

	setlocale(LC_ALL, "Russian");
	
	/*const int n = 24;
	int arr[n];
	int value = 0;
	std::generate(arr, arr + n, [&value]() { return value++; });
	cout << "Это поле, на котором показана расстановка позиций" << endl;
	print_empty(arr);

	cout<<"\nПоле до начала игры\n0-это пустые ячейки\n1 - это белые фишки\n2 - это черные фишки" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	FirstPlayer();

	PutChip(arr, "a2", 1);
	print_pos(arr);

	int arr2[24] = {1,1,1,2,2,1,0,2,0,2,2,1,0,1,0,0,2,0,1,2,0,0,0,0};
	print_pos(arr2);
	Node* node = new Node(arr2, 5, 6, 2,0,0);
	vector<vector<int>> st;
	int p = -1;
	cout << FindMill(arr2, 1, st)<<endl;

	int arr3[24] = { 1,1,1,2,2,1,2,0,0,2,2,1,0,2,2,2,0,0,1,2,0,0,0,0 };
	print_pos(arr3);
	vector<int> block = {};
	Node* node2 = new Node(arr3, 5, 6, 1, 0, 0);
	cout<< AbilityMove(node2->state, node2->move, block);
	cout << FindTwoChip(arr3,1) <<endl;*/

	StartGame();
}