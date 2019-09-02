#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <math.h>


//사용자 옵션 시작
//사용자 옵션 변수로 변경해야함
//게임판 크기 설정 크기만큼 이동할 공간이 생깁니다.
#define INPUT_SIZE_X 30			// 10 ~ 38 설정 기본값:30
#define INPUT_SIZE_Y 20			// 10 ~ 22 설정 기본값:20

//초기화면이 이상하게 표시될때 바꿔보세요 
//WIN7 = "%c%c" 
//WIN10 = "%c%c "
#define SPECIAL_CHARCTERS "%c%c "

//게임 속도 설정
#define SPEED 100
//사용자 옵션 끝

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
	int direction = SNAKE_DIRECTION_RIGHT; //방향초기화
	int new_direction;
	int board[BOARD_SIZE_Y][BOARD_SIZE_X] = { 0 }; //게임판
	int gameover = 0;
	_POINT head, tail;

	srand(time(0));	//시드값 초기화

	setup_start_location(&head, &tail);
	setup_wall(board);
	flagged_board(board, head, SNAKE_HEAD);

	hidecursor();	//커서 삭제

//	system("mode con cols=80 lines=25");

	print_wall();	//벽출력

	setColor(RED, DARK_GREEN);
	print_location("●", generate_food(board, head, direction));	//음식 하나 생성 //음식 출력

	while (1) {
		if (kbhit()) {	//키입력있으면 방향키 설정
			key = _getch();
			if (key == -32)
				key = _getch();
			new_direction = get_direction(key);

			if (abs(direction - new_direction) != 2)  //방향키가 진행해온 방향의 반대방향이면 입력값 무시
				direction = new_direction;
		}


		front = check_front(board, head, direction);
		if (front >= WALL_FLAG) {
			_POINT mid = { BOARD_SIZE_X / 2, BOARD_SIZE_Y / 2 };
			setColor(RED, DARK_GREEN);
			print_location("GAME OVER", mid);//플레이어 머리 출력
			_getch();
			return 0;
		}

		head_forward(board, &head, direction);//플레이어 머리 전진
		setColor(BLUE, DARK_GREEN);
		print_location("■", head);//플레이어 머리 출력

		if(front != FOOD_FLAG){
			print_location("  ", tail);	//꼬리 위치 문자 지우기
			tail_forward(board, &tail, body_length);
		}
		else {
			body_length++;	//몸길이 늘리기
			setColor(RED, DARK_GREEN);
			print_location("●", generate_food(board, head, direction));	//음식 하나 생성 //음식 출력
		}	

		Sleep(SPEED); //난이도 설정
	}

}
void gotoxy(int x, int y)
{
	COORD Pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
int get_direction(char key) {
	switch (key) {
	case 72:  //위쪽(상) 방향의 화살표 키 입력
		return SNAKE_DIRECTION_UP;
	case 75:  //왼쪽(좌) 방향의 화살표 키 입력
		return SNAKE_DIRECTION_LEFT;
	case 77:  //오른쪽(우) 방향의 화살표 키 입력
		return SNAKE_DIRECTION_RIGHT;
	case 80:  //아래쪽(하) 방향의 화살표 키 입력
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

	//첫번째줄

	printf(SPECIAL_CHARCTERS, a, b[3]);
	for (int i = 0; i < INPUT_SIZE_X; i++)
		printf(SPECIAL_CHARCTERS, a, b[1]);
	printf(SPECIAL_CHARCTERS, a, b[4]);
	printf("\n");

	//두번째줄부터 마지막줄 전까지
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

	//마지막줄
	printf(SPECIAL_CHARCTERS, a, b[6]);
	for (int i = 0; i < INPUT_SIZE_X; i++)
		printf(SPECIAL_CHARCTERS, a, b[1]);
	printf(SPECIAL_CHARCTERS, a, b[5]);
	printf("\n");
}
void setup_start_location(_POINT * head, _POINT * tail)
{
	//플레이어 초기위치 중간 설정
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

	flagged_board(board, old, EMPTY_FLAG);	//꼬리 지우기
}