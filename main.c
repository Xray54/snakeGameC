#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <math.h>


//����� �ɼ� ����
//����� �ɼ� ������ �����ؾ���
//������ ũ�� ���� ũ�⸸ŭ �̵��� ������ ����ϴ�.
#define INPUT_SIZE_X 30			// 10 ~ 38 ���� �⺻��:30
#define INPUT_SIZE_Y 20			// 10 ~ 22 ���� �⺻��:20

//�ʱ�ȭ���� �̻��ϰ� ǥ�õɶ� �ٲ㺸���� 
//WIN7 = "%c%c" 
//WIN10 = "%c%c "
#define SPECIAL_CHARCTERS "%c%c "

//���� �ӵ� ����
#define SPEED 100
//����� �ɼ� ��

#define BOARD_SIZE_X (INPUT_SIZE_X * 2 + 4)
#define BOARD_SIZE_Y (INPUT_SIZE_Y + 2)	

#define EMPTY_FLAG 0
#define FOOD_FLAG 1
#define WALL_FLAG 2
#define SNAKE_HEAD 3
#define SNAKE_DIRECTION_UP 4
#define SNAKE_DIRECTION_RIGHT 5
#define SNAKE_DIRECTION_DOWN 6
#define SNAKE_DIRECTION_LEFT 7

enum concol
{
	BLACK = 0,
	DARK_BLUE = 1,
	DARK_GREEN = 2,
	DARK_AQUA, DARK_CYAN = 3,
	DARK_RED = 4,
	DARK_PURPLE = 5, DARK_PINK = 5, DARK_MAGENTA = 5,
	DARK_YELLOW = 6,
	DARK_WHITE = 7,
	GRAY = 8,
	BLUE = 9,
	GREEN = 10,
	AQUA = 11, CYAN = 11,
	RED = 12,
	PURPLE = 13, PINK = 13, MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

typedef struct _point
{
	int x, y;
}_POINT;

void print_wall();
void setup_start_location(_POINT *head, _POINT *tail);
void setup_wall(int board[BOARD_SIZE_Y][BOARD_SIZE_X]);
void flagged_board(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT point, int flag);
void gotoxy(int x, int y);
int get_direction(char key);
int check_front(const int board[BOARD_SIZE_Y][BOARD_SIZE_X],_POINT head, int direction);
_POINT generate_food(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT head, int direction);
void head_forward(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT *head, int direction);
void tail_forward(const int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT *tail,int body_length);
void print_location(char ch[3], _POINT location);
void hidecursor();
void setColor(int color, int bgcolor);

int main()
{
	char key;
	int front, body_length = 1;
	int direction = SNAKE_DIRECTION_RIGHT; //�����ʱ�ȭ
	int new_direction;
	int board[BOARD_SIZE_Y][BOARD_SIZE_X] = { 0 }; //������
	int gameover = 0;
	_POINT head, tail;

	srand(time(0));	//�õ尪 �ʱ�ȭ

	setup_start_location(&head, &tail);
	setup_wall(board);
	flagged_board(board, head, SNAKE_HEAD);

	hidecursor();	//Ŀ�� ����

//	system("mode con cols=80 lines=25");

	print_wall();	//�����

	setColor(RED, DARK_GREEN);
	print_location("��", generate_food(board, head, direction));	//���� �ϳ� ���� //���� ���

	while (1) {
		if (kbhit()) {	//Ű�Է������� ����Ű ����
			key = _getch();
			if (key == -32)
				key = _getch();
			new_direction = get_direction(key);

			if (abs(direction - new_direction) != 2)  //����Ű�� �����ؿ� ������ �ݴ�����̸� �Է°� ����
				direction = new_direction;
		}


		front = check_front(board, head, direction);
		if (front >= WALL_FLAG) {
			_POINT mid = { BOARD_SIZE_X / 2, BOARD_SIZE_Y / 2 };
			setColor(RED, DARK_GREEN);
			print_location("GAME OVER", mid);//�÷��̾� �Ӹ� ���
			_getch();
			return 0;
		}

		head_forward(board, &head, direction);//�÷��̾� �Ӹ� ����
		setColor(BLUE, DARK_GREEN);
		print_location("��", head);//�÷��̾� �Ӹ� ���

		if(front != FOOD_FLAG){
			print_location("  ", tail);	//���� ��ġ ���� �����
			tail_forward(board, &tail, body_length);
		}
		else {
			body_length++;	//������ �ø���
			setColor(RED, DARK_GREEN);
			print_location("��", generate_food(board, head, direction));	//���� �ϳ� ���� //���� ���
		}	

		Sleep(SPEED); //���̵� ����
	}

}
void gotoxy(int x, int y)
{
	COORD Pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
int get_direction(char key) {
	switch (key) {
	case 72:  //����(��) ������ ȭ��ǥ Ű �Է�
		return SNAKE_DIRECTION_UP;
	case 75:  //����(��) ������ ȭ��ǥ Ű �Է�
		return SNAKE_DIRECTION_LEFT;
	case 77:  //������(��) ������ ȭ��ǥ Ű �Է�
		return SNAKE_DIRECTION_RIGHT;
	case 80:  //�Ʒ���(��) ������ ȭ��ǥ Ű �Է�
		return SNAKE_DIRECTION_DOWN;
	default:
		return NULL;
	}
}
int check_front(const int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT head, int direction)
{
	switch (direction) {
	case SNAKE_DIRECTION_UP:
		head.y--;
		break;
	case SNAKE_DIRECTION_RIGHT:
		head.x += 2;
		break;
	case SNAKE_DIRECTION_DOWN:
		head.y++;
		break;
	case SNAKE_DIRECTION_LEFT:
		head.x -= 2;
		break;
	}

	return board[head.y][head.x];
}
void print_wall()
{
	unsigned char a = 0xa6;
	unsigned char b[12];
	for (int i = 1; i < 12; i++)
		b[i] = 0xa0 + i;

	//ù��°��

	printf(SPECIAL_CHARCTERS, a, b[3]);
	for (int i = 0; i < INPUT_SIZE_X; i++)
		printf(SPECIAL_CHARCTERS, a, b[1]);
	printf(SPECIAL_CHARCTERS, a, b[4]);
	printf("\n");

	//�ι�°�ٺ��� �������� ������
	for (int i = 0; i < INPUT_SIZE_Y; i++) {
		printf(SPECIAL_CHARCTERS, a, b[2]);
		for (int j = 0; j < INPUT_SIZE_X; j++) {
			setColor(WHITE, DARK_GREEN);
			printf("  ");
			setColor(WHITE, BLACK);
		}
		printf(SPECIAL_CHARCTERS, a, b[2]);
		printf("\n");
	}

	//��������
	printf(SPECIAL_CHARCTERS, a, b[6]);
	for (int i = 0; i < INPUT_SIZE_X; i++)
		printf(SPECIAL_CHARCTERS, a, b[1]);
	printf(SPECIAL_CHARCTERS, a, b[5]);
	printf("\n");
}
void setup_start_location(_POINT * head, _POINT * tail)
{
	//�÷��̾� �ʱ���ġ �߰� ����
	head->x = BOARD_SIZE_X / 2;
	if (head->x % 2 != 0)
		head->x--;
	head->y = BOARD_SIZE_Y / 2;
	tail->x = head->x;
	tail->y = head->y;
}
void flagged_board(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT point, int flag)
{
	board[point.y][point.x] = flag;
}
void setup_wall(int board[BOARD_SIZE_Y][BOARD_SIZE_X])
{
	for (int i = 0; i < BOARD_SIZE_X; i++) {
		board[0][i] = WALL_FLAG;
		board[BOARD_SIZE_Y - 1][i] = WALL_FLAG;
	}
	for (int i = 1; i < BOARD_SIZE_Y; i++) {
		board[i][0] = WALL_FLAG;
		board[i][BOARD_SIZE_X - 2] = WALL_FLAG;
	}
}
_POINT generate_food(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT head, int direction)
{
	_POINT food;

	do {
		food.x = rand() % INPUT_SIZE_X * 2 + 2;
		food.y = rand() % INPUT_SIZE_Y + 1;
	} while (board[food.y][food.x] != EMPTY_FLAG);
	flagged_board(board, food, FOOD_FLAG);

	return food;
}
void head_forward(int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT *head, int direction)
{

	switch (direction) {
	case SNAKE_DIRECTION_UP:
		flagged_board(board, *head, SNAKE_DIRECTION_UP);
		head->y--;
		break;
	case SNAKE_DIRECTION_RIGHT:
		flagged_board(board, *head, SNAKE_DIRECTION_RIGHT);
		head->x += 2;
		break;
	case SNAKE_DIRECTION_DOWN:
		flagged_board(board, *head, SNAKE_DIRECTION_DOWN);
		head->y++;
		break;
	case SNAKE_DIRECTION_LEFT:
		flagged_board(board, *head, SNAKE_DIRECTION_LEFT);
		head->x -= 2;
		break;
	}
	flagged_board(board, *head, SNAKE_HEAD);
}
void print_location(char ch[3], _POINT location)
{
	gotoxy(location.x, location.y);
	printf("%s", ch);
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void setColor(int color, int bgcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | (color & 0xf));
}
void tail_forward(const int board[BOARD_SIZE_Y][BOARD_SIZE_X], _POINT *tail,int body_length)
{
	_POINT old = *tail;

	switch (board[tail->y][tail->x]) {
	case SNAKE_DIRECTION_UP:
		tail->y--;
		break;
	case SNAKE_DIRECTION_RIGHT:
		tail->x += 2;
		break;
	case SNAKE_DIRECTION_DOWN:
		tail->y++;
		break;
	case SNAKE_DIRECTION_LEFT:
		tail->x -= 2;
		break;
	}

	flagged_board(board, old, EMPTY_FLAG);	//���� �����
}