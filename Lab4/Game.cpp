#include "Lab4.h"
using namespace std;

void FirstPlayer() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int num = rand() % 2;
	if (!num)
		cout << "�� ������ �������. ���� ����� ����� (1)\n����� �������� �������� ������� �� 0 �� 24 ������������." << endl;
	else cout << "�� ������ �������. ���� ����� ������ (2)\n����� �������� �������� ������� �� 0 �� 24 ������������." << endl;
}

void PutChip(int* box, int pos, int chip) {
	box[pos] = chip;
}