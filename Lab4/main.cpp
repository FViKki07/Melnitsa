#include "Game.h"
int ROBOTCLR;
int HUMANCLR;
int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "Russian");
	
	/*const int n = 24;
	int arr[n];
	int value = 0;
	std::generate(arr, arr + n, [&value]() { return value++; });
	cout << "��� ����, �� ������� �������� ����������� �������" << endl;
	print_empty(arr);

	cout<<"\n���� �� ������ ����\n0-��� ������ ������\n1 - ��� ����� �����\n2 - ��� ������ �����" << endl;
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
	*/
	//int arr3[24] = { 1,1,1,1,1,1,2,1,0,1,2,1,0,2,2,2,2,2,2,2,2,1,2,0 };
	//print_pos(arr3);
	//Node* node2 = new Node(arr3, 5, 6, 1, 0, 0);

	//cout << DoubleMorris(node2, 2) << endl;

	ROBOTCLR = 1;
	HUMANCLR = 2;
	if (argv[1][0] == '0') {
		ROBOTCLR = 1;
		HUMANCLR = 2;
	}
	else {
		ROBOTCLR = 2;
		HUMANCLR = 1;
	}
	
	StartGame();
}