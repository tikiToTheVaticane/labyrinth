//��������� ����������� ������� �������� ������ �� ���������
#include <iostream>
#include <fstream>
using namespace std;

struct Coordinate{
	int y; //������ row
	int x; //������� col

	friend ostream& operator<<(ostream &os, const Coordinate &tmp){
		os << tmp.x << "  " << tmp.y << endl;
		return os;
	}
	
	/*friend istream& operator>>(istream &is, Cell &tmp){
		is >> tmp.row >> tmp.col;
		return is;
	}*/

	Coordinate& operator= (const Coordinate &tmp){
		x = tmp.x;
		y = tmp.y;
		return *this;
	}

	bool operator!=(const Coordinate &tmp){
		if ((y == tmp.y) && (x == tmp.x))
			return false;
		else
			return true;
	}
};

int NumAnsw = 0;
Coordinate Arr [1000];

class Queue{
private:
	Coordinate *C; 
	int size;
	int first, last; //������ ��������� ������� � ���������� �������� 
	int n; //�������� ���������� ��������� � �������
public:
	Queue (){
		first = -1; last = -1; //������� ������
		n = 0; size = 100;
		C = new Coordinate [size];
	}

	Queue (int tmpSize){
		first = -1; last = -1;
		n = 0; size = tmpSize;
		C = new Coordinate [size];
	}

	Queue (const Queue &tmp){
		if (C != NULL) delete [] C;
		first = tmp.first; last = tmp.last;
		n = tmp.n; size = tmp.size;
		C = new Coordinate [size];
		for (int i = 0; i < size; i++)
			C[i] = tmp.C[i];
	}

	virtual ~Queue (){
		delete [] C;
	}

//--------------------------------------------------------------

	void addLast (const Coordinate &tmp){
		if (n >= size) return;
		if (n == 0){
			first = 0; last = 0;
			C[last] = tmp; n++;
		}
		else{
			last = (last + 1) % size;
			C[last] = tmp; n++;
		}
	}

	Coordinate delFirst (){
		Coordinate tmp;
		if (n == 0){
			tmp.x = -1; 
			tmp.y = -1;
			return tmp;
		}
		if (n == 1){
			tmp = C[first]; n = 0;
			first = -1; last = -1;
		}
		else{
			tmp = C[first]; n--;
			first = (first + 1) % size;
		}
		return tmp;
	}
};

void outputSearch (bool **labyrinth, const Coordinate &start, const Coordinate &finish, const Coordinate &size){
	NumAnsw = 0;
	int **dir;
	dir = new int* [size.y];
	for (int i = 0; i < size.y; i++){
		dir [i] = new int [size.x]; 
		for (int j = 0; j < size.x; j++)
			dir [i][j] = 0;
	}
	dir [finish.y][finish.x] = 0;
	dir [start.y][start.x] = -3;
	///////////////////////////////////////////////////
	int answer = size.y * size.x; 
	Queue q (answer); answer = 0;
	Coordinate tmpc; //tmpc-
	q.addLast (start);
	tmpc = q.delFirst ();
	while (tmpc != finish){
	    if ((tmpc.x != 0) && (dir [tmpc.y][tmpc.x - 1] == 0) && (!labyrinth [tmpc.y][tmpc.x - 1])){
			dir [tmpc.y][--tmpc.x] = -1;
			q.addLast(tmpc); tmpc.x++;
    	}
    	if ((tmpc.x != size.x - 1) && (dir [tmpc.y][tmpc.x + 1] == 0) && (!labyrinth [tmpc.y][tmpc.x + 1])){
			dir [tmpc.y][++tmpc.x] = 1;
		    q.addLast(tmpc); tmpc.x--;
    	}
    	if ((tmpc.y != 0) && (dir[tmpc.y - 1][tmpc.x] == 0) && (!labyrinth [tmpc.y - 1][tmpc.x])){
			dir [--tmpc.y][tmpc.x] = -2;
			q.addLast(tmpc); tmpc.y++;
    	}
    	if ((tmpc.y != size.y - 1) && (dir [tmpc.y + 1][tmpc.x] == 0) && (!labyrinth [tmpc.y + 1][tmpc.x])){
			dir [++tmpc.y][tmpc.x] = 2;
			q.addLast(tmpc); tmpc.y--;
    	}
		tmpc = q.delFirst (); 
		if ((tmpc.y == -1) && (tmpc.x == -1)){
			answer = -1; tmpc = finish;
		}
	}
    
	if (answer != -1){
    	tmpc = finish;
	    while (tmpc != start){
			if (dir [tmpc.y][tmpc.x] == -1){ Arr [NumAnsw++] = tmpc; tmpc.x++; }
	    	else
				if (dir [tmpc.y][tmpc.x] == 1){ Arr [NumAnsw++] = tmpc; tmpc.x--; }
	    		else
					if (dir [tmpc.y][tmpc.x] == -2){ Arr [NumAnsw++] = tmpc; tmpc.y++; }
		    		else
						if (dir [tmpc.y][tmpc.x] == 2){ Arr [NumAnsw++] = tmpc; tmpc.y--; }
			//cout << tmpc << endl;
	    	answer++;
	    }
	}
	Arr [NumAnsw++] = start;
	//for (int i = 0; i < NumAnsw; i++)
	//	cout << Arr [i];
}