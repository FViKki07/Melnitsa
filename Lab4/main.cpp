#include "Lab4.h"

int main() {

	setlocale(LC_ALL, "Russian");

	const int n = 24;
	int arr[n];
	int value = 0;
	std::generate(arr, arr + n, [&value]() { return value++; });
	cout << "��� ����, �� ������� �������� ����������� �������" << endl;
	print_empty(arr);

	cout<<"\n���� �� ������ ����\n0-��� ������ ������\n1 - ��� ����� �����\n2 - ��� ������ �����" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	FirstPlayer();

	//PutChip(arr, 12, 1);
	//print_pos(arr);
}