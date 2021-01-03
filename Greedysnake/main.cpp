#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<locale.h>
#include<conio.h>

#define WIDTH 40
#define HEIGHT 20
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define CLS system("cls")
#define SPACE(x) for(int i=0;i<x;i++)printf(" ")
#define STAR(x) for(int i=0;i<x;i++)printf("*")
#define NL printf("\n")
#define BUG_MAX_COUNT 5
#define GOLD_MAX_COUNT 5
#define REPLAY 1
#define EXIT 0
#define PLAY_CONTINUE 2


typedef struct location{
	int x;//相对游戏框左上角的纵向偏移
	int y; //相对游戏框左上角的横向偏移
	int direction;//运动方向
	struct location* next, * last;
}Location;

Location* snake_head, * snake_end;
Location* gold_head, * gold_end;
Location* bug_head, * bug_end;

/// <summary>
/// 刷新频率ms
/// </summary>
int frequency;
/// <summary>
/// 蛇的初始长度
/// </summary>
int initiallength = 3;
/// <summary>
/// 虫子的个数
/// </summary>
int bugcount = 0;
/// <summary>
/// 金币个数
/// </summary>
int goldcount = 0;
/// <summary>
/// 虫子刷新频率
/// </summary>
int bugfrequency = 5;
/// <summary>
/// 计数器
/// </summary>
int counter = 0;


char GameScreen[HEIGHT+2][WIDTH+2];

void initial();
void show();
void grow();
void freshscreen();
int move();
void follow();
void generatebug();
void generategold();

int bugcheck(int x, int y);

int goldcheck(int x, int y);

int snakecheck(int x, int y);

int main() {
	int input;
	int receive;
	initial();
	while (1) {
		CLS;
		generatebug();
		counter++;
		//counter = counter % bugfrequency;
		generategold();
		receive = move();
		if (receive == EXIT)exit(0);
		else if(receive==REPLAY)
			continue;
		follow();
		freshscreen();
		show();

		setlocale(LC_ALL, "chs");
		Sleep(frequency);
		if (_kbhit()) {
			input = _getch();
			if (input == 224) {
				input = _getch();
				switch (input) {
					case UP:
					case DOWN:
					case LEFT:
					case RIGHT:snake_head->direction = input; break;
					default:break;
				}
			}
		}
	}
}
/// <summary>
/// 初始化
/// </summary>
void initial() {
	frequency = 500;
	//生成蛇头
	snake_head = (Location*)malloc(sizeof(Location));
	if (snake_head != NULL) {
		snake_head->x = HEIGHT / 2;
		snake_head->y = initiallength;
		snake_head->direction = RIGHT;//让蛇从游戏框左侧中间开始向右运动
		snake_head->last = NULL;
		snake_head->next = NULL;
		snake_end = snake_head;
		for (int i = 0; i < initiallength - 1; i++) {
			grow();
		}
	}

	//虫子初始化
	bug_head = bug_end = NULL;
	counter = 0;
	//生成第一个金币
	gold_head = (Location*)malloc(sizeof(Location));
	if (gold_head != NULL) {
		int x, y;
		while (1) {
			x = rand() % HEIGHT + 1;
			y = rand() % WIDTH + 1;
			if (bugcheck(x, y) || snakecheck(x, y)) {
				continue;
			}
			else {
				gold_head->x = x;
				gold_head->y = y;
				gold_head->last = NULL;
				gold_head->next = NULL;
				gold_end = gold_head;
				goldcount = 1;
				break;
			}
		}
	}
}
/// <summary>
/// 游戏界面
/// </summary>
void show() {
	SPACE(WIDTH / 2 - 5); printf("☆贪吃蛇☆\n");
	NL;
	SPACE(1); STAR(WIDTH); NL;
	SPACE(1); STAR(1); SPACE(15); printf("游戏规则"); SPACE(15); STAR(1); NL;
	SPACE(1); STAR(1); SPACE(12); printf("吃金币可以变长"); SPACE(12); STAR(1); NL;
	SPACE(1); STAR(1); SPACE(12); printf("撞上虫子会变短"); SPACE(12); STAR(1); NL;
	SPACE(1); STAR(WIDTH); NL; NL;
	for (int i = 0; i < HEIGHT + 2; i++) {
		for (int j = 0; j < WIDTH + 2; j++) {
			printf("%c", GameScreen[i][j]);
		}
		NL;
	}
	NL;
}
/// <summary>
/// 蛇生长
/// </summary>
void grow() {
	Location* p;
	p = (Location*)malloc(sizeof(Location));
	if (p != NULL) {
		switch (snake_end->direction)
		{
			case UP:
				p->x = snake_end->x + 1;
				p->y = snake_end->y;
				break;
			case DOWN:
				p->x = snake_end->x - 1;
				p->y = snake_end->y;
				break;
			case LEFT:
				p->x = snake_end->x;
				p->y = snake_end->y + 1;
				break;
			case RIGHT:
				p->x = snake_end->x;
				p->y = snake_end->y - 1;
				break;
			default:
				break;
		}
		p->direction = snake_end->direction;
		snake_end->next = p;
		p->last = snake_end;
		p->next = NULL;
		snake_end = p;
	}
}
/// <summary>
/// 刷新游戏屏幕
/// </summary>
void freshscreen() {
	for (int i = 0; i < HEIGHT + 2; i++) {
		for (int j = 0; j < WIDTH + 2; j++) {
			if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
				GameScreen[i][j] = '#';
			}
			else {
				GameScreen[i][j] = ' ';
			}
		}
	}
	
	Location* p;
	//显示蛇
	p = snake_head;
	while (p != NULL) {
		if (p == snake_head) {
			GameScreen[p->x][p->y] = 'o';
		}
		else {
			GameScreen[p->x][p->y] = '*';
		}
		p = p->next;
	}
	//显示虫子
	p = bug_head;
	while (p != NULL) {
		GameScreen[p->x][p->y] = '&';
		p = p->next;
	}
	//显示金币
	p = gold_head;
	while (p != NULL) {
		GameScreen[p->x][p->y] = '$';
		p = p->next;
	}
}

int move() {
	Location* p;
	p = snake_head;
	while (p != NULL) {
		switch (p->direction) {
			case UP:
				p->x--; 
				if (p->x <1) {
					p->x += HEIGHT;
				}
				break;
			case DOWN:
				p->x++; 
				if (p->x > HEIGHT) {
					p->x -= HEIGHT;
				}
				break;
			case LEFT:
				p->y--; 
				if (p->y < 1) {
					p->y += WIDTH;
				}
				break;
			case RIGHT:
				p->y++;
				if (p->y > WIDTH) {
					p->y -= WIDTH;
				}
				break;
		}
		p = p->next;
	}
	if (snake_head != NULL) {
		if (goldcheck(snake_head->x, snake_head->y)) {
			//金币变少
			p = gold_head;
			while (p != NULL) {
				if (p->x == snake_head->x && p->y == snake_head->y) {
					if (p == gold_head) {//链首
						if (gold_head->next != NULL)
							gold_head = gold_head->next;
						gold_head->last = NULL;
						free(p);
						p = NULL;
					}
					else {
						p->last->next = p->next;
						if (p->next != NULL)//链中
							p->next->last = p->last;
						else {//链尾
							gold_end = gold_end->last;
							gold_end->next = NULL;
						}
						free(p);
						p = NULL;
					}
					break;
				}
				p = p->next;
			}
			goldcount--;
			//蛇变长
			grow();
		}
		if (bugcheck(snake_head->x, snake_head->y)) {
			//虫子变少
			p=bug_head;
			while (p != NULL) {
				if (p->x == snake_head->x && p->y == snake_head->y) {
					if (p == bug_head) {
						if (bug_head->next != NULL)
							bug_head = bug_head->next;
						bug_head->last = NULL;
						free(p);
						p = NULL;
					}
					else {
						p->last->next = p->next;
						if (p->next != NULL)
							p->next->last = p->last;
						else {
							bug_end = bug_end->last;
						}
						free(p);
						p = NULL;
					}
					break;
				}
				p = p->next;
			}
			bugcount--;
			//蛇变短
			p = snake_end;
			if (snake_end != snake_head) {
				snake_end = snake_end->last;
				snake_end->next = NULL;
				free(p);
				p = NULL;
			}
			else {
				free(p);
				p = NULL;
				strcpy_s(&GameScreen[10][15],10, "游戏结束!");
				strcpy_s(&GameScreen[11][12],17, "R.重玩  E.不玩了");
				CLS;
				show();

				int input;
				while (1) {
					input = _getch();
					if (input == 'e' || input == 'E') {
						return EXIT;
					}
					else if (input == 'R' || input == 'r') {
						initial();
						return REPLAY;
					}
				}
			}
		}
	}
	return PLAY_CONTINUE;
	
}
/// <summary>
/// 跟随蛇头的方向
/// </summary>
void follow() {
	Location* p;
	p = snake_end;
	while (p->last != NULL) {
		p->direction = p->last->direction;
		p = p->last;
	}
}
/// <summary>
/// 生成虫子
/// </summary>
void generatebug() {
	int x, y;
	Location* p, * temp;
	if (counter == 0) {
		//清除所有的虫子
		p = bug_head;
		while (p != NULL) {
			temp = p;
			p = p->next;
			free(temp);
			bugcount--;
		}
		//重新生成虫子
		bug_head = (Location*)malloc(sizeof(Location));
		if (bug_head != NULL) {
			int x, y;
			while (1) {
				x = rand() % HEIGHT + 1;
				y = rand() % WIDTH + 1;
				if (snakecheck(x, y)) {
					continue;
				}
				else {
					bug_head->x = x;
					bug_head->y = y;
					bug_head->last = NULL;
					bug_head->next = NULL;
					bug_end = bug_head;
					bugcount = 1;
					break;
				}
			}
		}

		while (bugcount < BUG_MAX_COUNT) {
			while (1) {
				x = rand() % HEIGHT + 1;
				y = rand() % WIDTH + 1;
				if (bugcheck(x, y) || goldcheck(x, y) || snakecheck(x, y)) {
					continue;
				}
				else {
					Location* p;
					p = (Location*)malloc(sizeof(Location));
					if (p != NULL) {
						p->x = x;
						p->y = y;
						bug_end->next = p;
						p->last = bug_end;
						p->next = NULL;
						bug_end = p;
						bugcount++;
						break;
					}
				}
			}
		}
	}
}

/// <summary>
/// 生成金币
/// </summary>
void generategold() {
	int x, y;
	while (goldcount < GOLD_MAX_COUNT) {
		while (1) {
			x = rand() % HEIGHT + 1;
			y = rand() % WIDTH + 1;
			if (goldcheck(x, y) || bugcheck(x, y) || snakecheck(x, y)) {
				continue;
			}
			else {
				Location* p;
				p = (Location*)malloc(sizeof(Location));
				if (p != NULL) {
					p->x = x;
					p->y = y;
					p->last = gold_end;
					p->next = NULL;
					gold_end->next = p;
					gold_end = p;
					goldcount++;
					break;
				}
			}
		}
	}
}
/// <summary>
/// 指定位置是否有虫子，是1，否0
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
int bugcheck(int x,int y) {
	Location* p;
	p = bug_head;
	while (p != NULL) {
		if (p->x == x && p->y == y) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}

/// <summary>
/// 指定位置是否有金币，是1，否0
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
int goldcheck(int x, int y) {
	Location* p;
	p = gold_head;
	while (p != NULL) {
		if (p->x == x && p->y == y) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}

/// <summary>
/// 指定位置是否有蛇，是1，否0
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
int snakecheck(int x, int y) {
	Location* p;
	p = snake_head;
	while (p != NULL) {
		if (p->x == x && p->y == y) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}