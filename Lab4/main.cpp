#include "Lab4.h"

int main() {

	setlocale(LC_ALL, "Russian");

	const int n = 24;
	int arr[n];
	int value = 0;
	std::generate(arr, arr + n, [&value]() { return value++; });
	cout << "Это поле, на котором показана расстановка позиций" << endl;
	print_empty(arr);

	cout<<"\nПоле до начала игры\n0-это пустые ячейки\n1 - это белые фишки\n2 - это черные фишки" << endl;
	std::generate(arr, arr + n, []() { return 0; });
	print_pos(arr);

	FirstPlayer();

	//PutChip(arr, 12, 1);
	//print_pos(arr);
}