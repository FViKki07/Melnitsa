#include "Lab4.h"
using namespace std;

void FirstPlayer() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int num = rand() % 2;
	if (!num)
		cout << "Вы ходите первыми. Ваши фишки белые (1)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
	else cout << "Вы ходите вторыми. Ваши фишки черные (2)\nЧтобы походить выберите позицию от 0 до 24 включительно." << endl;
}

void PutChip(int* box, int pos, int chip) {
	box[pos] = chip;
}