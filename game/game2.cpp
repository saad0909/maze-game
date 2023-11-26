#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <windows.h>
using namespace std;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

void color(int x) {
	SetConsoleTextAttribute(out, x);
}

template<class type>
class node {
	type value;
	node<type>* next;
public:
	node(type value, node<type>* next) {
		this->value = value;
		this->next = next;
	}
	template<class F>
	friend class myStack;
};

template<class type>
class myStack {
	int length;
	node<type>* tail;
public:
	myStack() {
		tail = nullptr;
		length = 0;
	}
	void push(type value) {
		tail = new node<type>(value, tail);
		length++;
	}
	type pop() {
		if (length > 0) {
			length--;
			type dup = tail->value;
			node<type>* dup_ptr = tail;
			tail = tail->next;
			delete dup_ptr;
			return dup;
		}
	}
	bool empty() {
		if (length) return false;
		return true;
	}
	void print() {
		node<type>* dup_ptr = tail;
		while (dup_ptr != nullptr) {
			cout << dup_ptr->value << endl;
			dup_ptr = dup_ptr->next;
		}
	}
	~myStack() {
		node<type>* dup_ptr = tail;
		while (dup_ptr != nullptr) {
			tail = dup_ptr;
			dup_ptr = dup_ptr->next;
			delete tail;
		}
	}
};

void Print(int r, int c, int curr_room, char*** rooms, int** dims, bool show) {
	system("cls");
	char ch = rooms[curr_room][r][c];
	for (int i = 0; i < dims[curr_room][0]; i++) {
		for (int j = 0; j < dims[curr_room][1]; j++)
			if (i == r && j == c) {
				color(23);
				cout << "o";
				color(15);
			}
			else if (rooms[curr_room][i][j] == '0') {
				color(23);
				cout << ' ';
				color(15);
			}
			else if (rooms[curr_room][i][j] == 'X') {
				color(70);
				cout << '#';
				color(15);
			}
			else if (rooms[curr_room][i][j] == 'G') {
				color(23);
				cout << ' ';
				color(15);
			}
			else if (rooms[curr_room][i][j] == 'D') {
				color(23);
				cout << ' ';
				color(15);
			}
			else cout << rooms[curr_room][i][j];
		cout << endl;
	}
	cout << endl << endl;
	cout << "you are in room: " << curr_room << endl;
	if (show) {
		if (ch == 'D') {
			cout << "Devil killed you" << endl;
		}
		else {
			cout << "You encountered Ghost" << endl;
			Sleep(1000);
		}
	}
}

struct pos {
	int y;
	int x;
	int room;
};

void start_game_recursive(int file_count, int curr_room, char*** rooms, int** dims,int** cords, myStack<pos>& ms, int curr_x, int curr_y, bool& win, int treasure_room_no) {
	Print(curr_y, curr_x, curr_room, rooms, dims, false);
	if (rooms[curr_room][curr_y][curr_x] == 'D') {
		Print(curr_y, curr_x, curr_room, rooms, dims, true);
		ms.push(pos{ curr_y,curr_x,curr_room });
		return;
	}
	if (rooms[curr_room][curr_y][curr_x] == 'T') {
		win = true;
		return;
	}
	if (rooms[curr_room][curr_y][curr_x] == 'G') {
		curr_room = treasure_room_no;
		curr_x = cords[curr_room][1];
		curr_y = cords[curr_room][0];
		Print(curr_y, curr_x, curr_room, rooms, dims, true);
		start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
		ms.push(pos{ curr_y,curr_x,curr_room });
		return;

	}
	if (rooms[curr_room][curr_y][curr_x] == 'P') {
		int toss = 0 + (rand() % 2); //0 will be head and 1 will be tail
		if (toss == 0) {
			int prev_room = curr_room;
			while (curr_room == prev_room) {
				curr_room = 0 + (rand() % file_count);
			}
			curr_x = cords[curr_room][1];
			curr_y = cords[curr_room][0];
			Print(curr_y, curr_x, curr_room, rooms, dims, false);
			start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
			ms.push(pos{ curr_y,curr_x,curr_room });
			return;
		}
		else {
			if (curr_room == 0) return;
			else {
				curr_room -= 1;
				curr_x = cords[curr_room][1];
				curr_y = cords[curr_room][0];
				Print(curr_y, curr_x, curr_room, rooms, dims, false);
				start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
				ms.push(pos{ curr_y,curr_x,curr_room });
				return;
			}
		}
	}
	char c = _getch();
	if (c == -32) c = _getch();
	if (c == 'q') return;
	else if (c == KEY_UP) {
		if (curr_y - 1 >= 0 && rooms[curr_room][curr_y - 1][curr_x] != 'X') {
			curr_y -= 1;
			start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
			ms.push(pos{ curr_y,curr_x,curr_room });
			return;
		}
	}
	else if (c == KEY_DOWN) {
		if (curr_y + 1 < dims[curr_room][0] && rooms[curr_room][curr_y + 1][curr_x] != 'X') {
			curr_y += 1;
			start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
			ms.push(pos{ curr_y,curr_x,curr_room });
			return;
		}
	}
	else if (c == KEY_LEFT) {
		if (curr_x - 1 >= 0 && rooms[curr_room][curr_y][curr_x - 1] != 'X') {
			curr_x -= 1;
			start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
			ms.push(pos{ curr_y,curr_x,curr_room });
			return;
		}
	
	}
	else if (c == KEY_RIGHT) {
		if (curr_x + 1 < dims[curr_room][1] && rooms[curr_room][curr_y][curr_x + 1] != 'X') {
			curr_x += 1;
			start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
			ms.push(pos{ curr_y,curr_x,curr_room });
			return;
		}
	}
	start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
}

void start_game_recursive(int file_count, char*** rooms, int** dims, int** cords) {
	srand(time(0));
	int treasure_room_no = 0;
	bool exit = false;
	for (int i = 0; !exit && i < file_count; i++) {
		for (int k = 0; !exit && k < dims[i][0]; k++) {
			for (int o = 0; !exit && o < dims[i][1]; o++) {
				if (rooms[i][k][o] == 'T') {
					cout << "done" << endl;
					treasure_room_no = 1;
					exit = true;
				}
			}
		}
	}
	int curr_room = 0 + (rand() % file_count);
	int curr_x, curr_y;
	curr_y = cords[curr_room][0];
	curr_x = cords[curr_room][1];
	myStack<pos> ms;
	bool win = false;
	start_game_recursive(file_count, curr_room, rooms, dims, cords, ms, curr_x, curr_y, win, treasure_room_no);
	ms.push(pos{ curr_y, curr_x, curr_room });
	if (win) {
		cout << "you found the treasure" << endl;
		cout << "below is your path" << endl;

		while (!ms.empty()) {
			pos temp = ms.pop();
			cout << "(" << temp.x << ", " << temp.y << ")" << temp.room << endl;
		}
	}
	else cout << "you lost" << endl;
}

void start_game_iterative(int file_count, char*** rooms, int** dims, int** cords) {
	srand(time(0));
	int treasure_room_no = 0;
	bool exit = false;
	for (int i = 0; !exit && i < file_count; i++) {
		for (int k = 0; !exit && k < dims[i][0]; k++) {
			for (int o = 0; !exit && o < dims[i][1]; o++) {
				if (rooms[i][k][o] == 'T') {
					cout << "done" << endl;
					treasure_room_no = 1;
					exit = true;
				}
			}
		}
	}
	int curr_room = 0 + (rand() % file_count);
	char c;
	int curr_x, curr_y;
	curr_y = cords[curr_room][0];
	curr_x = cords[curr_room][1];
	char wall = 'X';
	exit = false;
	myStack<pos> ms;
	ms.push({ curr_y, curr_x, curr_room });
	string msg = "quit";
	Print(curr_y, curr_x, curr_room, rooms, dims, false);
	while (!exit) {
		switch ((c = _getch())) {
		case KEY_LEFT:
			if (curr_x - 1 >= 0 && rooms[curr_room][curr_y][curr_x - 1] != wall) {
				curr_x = curr_x - 1;
				ms.push({ curr_y, curr_x, curr_room });
			}
			Print(curr_y, curr_x, curr_room, rooms, dims, false);
			break;
		case KEY_RIGHT:
			if (curr_x + 1 < dims[curr_room][1] && rooms[curr_room][curr_y][curr_x + 1] != wall) {
				curr_x = curr_x + 1;
				ms.push({ curr_y, curr_x, curr_room });
			}
			Print(curr_y, curr_x, curr_room, rooms, dims, false);
			break;
		case KEY_UP:
			if (curr_y - 1 >= 0 && rooms[curr_room][curr_y - 1][curr_x] != wall) {
				curr_y = curr_y - 1;
				ms.push({ curr_y, curr_x, curr_room });
			}
			Print(curr_y, curr_x, curr_room, rooms, dims, false);
			break;
		case KEY_DOWN:
			if (curr_y + 1 < dims[curr_room][0] && rooms[curr_room][curr_y + 1][curr_x] != wall) {
				curr_y = curr_y + 1;
				ms.push({ curr_y, curr_x, curr_room });
			}
			Print(curr_y, curr_x, curr_room, rooms, dims, false);
			break;
		case 'q':
			exit = true;
		default:
			break;
		}
		if (rooms[curr_room][curr_y][curr_x] == 'D') {
			exit = true;
			msg = "lost";
			Print(curr_y, curr_x, curr_room, rooms, dims, true);
		}
		if (rooms[curr_room][curr_y][curr_x] == 'T') {
			msg = "win";
			exit = true;
		}
		if (rooms[curr_room][curr_y][curr_x] == 'G') {
			curr_room = treasure_room_no;
			curr_x = cords[curr_room][1];
			curr_y = cords[curr_room][0];
			ms.push({ curr_y,curr_x,curr_room });
			Print(curr_y, curr_x, curr_room, rooms, dims, true);
		}
		if (rooms[curr_room][curr_y][curr_x] == 'P') {
			int toss = 0 + (rand() % 2); //0 will be head and 1 will be tail
			if (toss == 0) {
				int prev_room = curr_room;
				while (curr_room == prev_room) {
					curr_room = 0 + (rand() % file_count);
				}
				curr_x = cords[curr_room][1];
				curr_y = cords[curr_room][0];
				ms.push({ curr_y,curr_x,curr_room });
				Print(curr_y, curr_x, curr_room, rooms, dims, false);
			}
			else {
				if (curr_room == 0) {
					exit = true;
					msg = "lost";
				}
				else {
					curr_room -= 1;
					curr_x = cords[curr_room][1];
					curr_y = cords[curr_room][0];
					ms.push({ curr_y,curr_x,curr_room });
					Print(curr_y, curr_x, curr_room, rooms, dims, false);
				}
			}
		}
	}
	cout << "you " << msg << endl;
	myStack<pos> stk;
	if (msg == "win") {
		while (!ms.empty()) {
			stk.push(ms.pop());
		}
		while (!stk.empty()) {
			pos temp = stk.pop();
			cout << "(" << temp.x << ", " << temp.y << ")" << temp.room << endl;
		}
	}
}

void file_reading() {
	ifstream read("input.txt");
	char c;
	bool fline = true;
	int file_count = 0;
	string temp = "";
	int line = 0;
	char** arr = nullptr;
	while (!read.eof()) {
		read.get(c);
		if (c >= '0' && c <= '9' && fline) {
			file_count *= 10;
			file_count += c - '0';
		}
		else if (c == '\n' && fline) {
			fline = false;
			arr = new char* [file_count];
		}
		else if (!fline && c != '\n' && c != '.') {
			temp += c;
		}
		else if (!fline && c == '.') {
			arr[line] = new char[temp.length() + 5];
			arr[line][temp.length()] = '.';
			arr[line][temp.length() + 1] = 't';
			arr[line][temp.length() + 2] = 'x';
			arr[line][temp.length() + 3] = 't';
			arr[line][temp.length() + 4] = '\0';
			for (int i = 0; i < temp.length(); i++) arr[line][i] = temp[i];
		}
		else if (!fline && c == '\n') {
			line++;
			temp = "";
		}
	}
	read.close();
	char*** rooms = new char** [file_count];
	int** cords = new int* [file_count];
	for (int i = 0; i < file_count; i++) { 
		cords[i] = new int[2]; 
		cords[i][0] = 0;
		cords[i][1] = 0;
	}
	int** dims = new int* [file_count];
	for (int i = 0; i < file_count; i++) {
		dims[i] = new int[2];
		dims[i][0] = 0;
		dims[i][1] = 0;
	}
	for (int i = 0; i < file_count; i++) {
		ifstream read(arr[i]);
		line = 0;
		char data[1000];
		while (!read.eof()) {
			read.getline(data, 1000);
				bool comma = false;
				int index = 0;
				for (int o = 0; o < strlen(data); o++) {
					if (line == 0) {
						if (data[o] == ',') comma = true;
						else if (data[o] >= '0' && data[o] <= '9') {
							if (!comma) { dims[i][0] *= 10; dims[i][0] += data[o] - '0'; }
							else { dims[i][1] *= 10; dims[i][1] += data[o] - '0'; }
						}
					}
					else if (line == 1) {
						if (data[o] == ',') comma = true;
						else if (data[o] >= '0' && data[o] <= '9') {
							if (!comma) {  cords[i][0]*= 10; cords[i][0] += data[o] - '0'; }
							else { cords[i][1] *= 10; cords[i][1] += data[o] - '0'; }
						}
					}
					else {
						if ((data[o] >= 'A' && data[o] <= 'Z') || (data[o] >= '0' && data[o] <= '9'))
							rooms[i][line - 2][index++] = data[o];
					}
				}
			if (line == 1) {
				rooms[i] = new char* [dims[i][0]];
				for (int k = 0; k < dims[i][0]; k++) rooms[i][k] = new char[dims[i][1]];
			}
			line++;
		}
	}
	char ch = '\0';
	while (ch != '3') {
		cout << "Enter 1 for recursive" << endl;
		cout << "Enter 2 for iterative" << endl;
		cout << "Enter 3 to end the program" << endl;
		ch = _getch();
		if (ch == '1') {
			start_game_recursive(file_count, rooms, dims, cords);
		}
		else if (ch == '2') {
			start_game_iterative(file_count, rooms, dims, cords);
		}
		system("pause");
		system("cls");
	}
	for (int i = 0; i < file_count; i++) delete[] arr[i];
	delete[] arr;
	for (int i = 0; i < file_count; i++)delete[] cords[i];
	delete[] cords;
	for (int i = 0; i < file_count; i++) {
		for (int o = 0; o < dims[i][0]; o++) {
			delete[] rooms[i][o];
		}
		delete[] rooms[i];
	}
	delete[] rooms;
	for (int i = 0; i < file_count; i++)delete[] dims[i];
	delete[] dims;
}

int main() {
	file_reading();
}