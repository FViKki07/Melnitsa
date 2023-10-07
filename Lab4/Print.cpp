#include "Lab4.h"

using namespace std;

void print_empty(int* mas) {
	cout << endl;
	cout << "   "<<mas[0] << "-----------" << mas[1] << "-----------" << mas[2] << endl;
	cout << "   |           |           |" << endl;
	cout << "   |   " << mas[3] << "-------" << mas[4] << "-------" << mas[5] << "   " << "|"<<endl;
	cout << "   |   |       |       |   |" << endl;
	cout << "   |   |   " << mas[6] << "---" << mas[7] << "---" << mas[8] << "   |   |" << endl;
	cout << "   |   |   |       |   |   |" << endl;
	cout << "   "<< mas[9] << "---" << mas[10] << "--" << mas[11] << "     " << mas[12] << "---" << mas[13] << "--" << mas[14] << endl;
	cout << "   |   |   |       |   |   |" << endl;
	cout << "   |   |   "<< mas[15] <<"--"<< mas[16] <<"--"<< mas[17] <<"  |   |" << endl;
	cout << "   |   |       |       |   |" << endl;
	cout << "   |   " << mas[18] << "------" << mas[19] << "------" << mas[20] << "  " << "|" << endl;
	cout << "   |           |           |" << endl;
	cout << "   "<< mas[21] << "----------" << mas[22] << "----------" << mas[23] << endl;
}

void print_pos(int* mas) {
	cout << endl;
	cout << "   " << mas[0] << "-----------" << mas[1] << "-----------" << mas[2] << endl;
	cout << "   |           |           |" << endl;
	cout << "   |   " << mas[3] << "-------" << mas[4] << "-------" << mas[5] << "   " << "|" << endl;
	cout << "   |   |       |       |   |" << endl;
	cout << "   |   |   " << mas[6] << "---" << mas[7] << "---" << mas[8] << "   |   |" << endl;
	cout << "   |   |   |       |   |   |" << endl;
	cout << "   " << mas[9] << "---" << mas[10] << "---" << mas[11] << "       " << mas[12] << "---" << mas[13] << "---" << mas[14] << endl;
	cout << "   |   |   |       |   |   |" << endl;
	cout << "   |   |   " << mas[15] << "---" << mas[16] << "---" << mas[17] << "   |   |" << endl;
	cout << "   |   |       |       |   |" << endl;
	cout << "   |   " << mas[18] << "-------" << mas[19] << "-------" << mas[20] << "   " << "|" << endl;
	cout << "   |           |           |" << endl;
	cout << "   " << mas[21] << "-----------" << mas[22] << "-----------" << mas[23] << endl;
}