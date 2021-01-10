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
	int x;//�����Ϸ�����Ͻǵ�����ƫ��
	int y; //�����Ϸ�����Ͻǵĺ���ƫ��
	int direction;//�˶�����
	struct location* next, * last;
}Location;

Location* snake_head, * snake_end;
Location* gold_head, * gold_end;
Location* bug_head, * bug_end;

/// <summary>
/// ˢ��Ƶ��ms
/// </summary>
int frequency;
/// <summary>
/// �ߵĳ�ʼ����
/// </summary>
int initiallength = 3;
/// <summary>
/// ���ӵĸ���
/// </summary>
int bugcount = 0;
/// <summary>
/// ��Ҹ���
/// </summary>
int goldcount = 0;
/// <summary>
/// ����ˢ��Ƶ��
/// </summary>
int bugfrequency = 5;
/// <summary>
/// ������
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
/// ��ʼ��
/// </summary>
void initial() {
	frequency = 500;
	//������ͷ
	snake_head = (Location*)malloc(sizeof(Location));
	if (snake_head != NULL) {
		snake_head->x = HEIGHT / 2;
		snake_head->y = initiallength;
		snake_head->direction = RIGHT;//���ߴ���Ϸ������м俪ʼ�����˶�
		snake_head->last = NULL;
		snake_head->next = NULL;
		snake_end = snake_head;
		for (int i = 0; i < initiallength - 1; i++) {
			grow();
		}
	}

	//���ӳ�ʼ��
	bug_head = bug_end = NULL;
	counter = 0;
	//���ɵ�һ�����
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
/// ��Ϸ����
/// </summary>
void show() {
	SPACE(WIDTH / 2 - 5); printf("��̰���ߡ�\n");
	NL;
	SPACE(1); STAR(WIDTH); NL;
	SPACE(1); STAR(1); SPACE(15); printf("��Ϸ����"); SPACE(15); STAR(1); NL;
	SPACE(1); STAR(1); SPACE(12); printf("�Խ�ҿ��Ա䳤"); SPACE(12); STAR(1); NL;
	SPACE(1); STAR(1); SPACE(12); printf("ײ�ϳ��ӻ���"); SPACE(12); STAR(1); NL;
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
/// ������
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
/// ˢ����Ϸ��Ļ
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
	//��ʾ��
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
	//��ʾ����
	p = bug_head;
	while (p != NULL) {
		GameScreen[p->x][p->y] = '&';
		p = p->next;
	}
	//��ʾ���
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
			//��ұ���
			p = gold_head;
			while (p != NULL) {
				if (p->x == snake_head->x && p->y == snake_head->y) {
					if (p == gold_head) {//����
						if (gold_head->next != NULL)
							gold_head = gold_head->next;
						gold_head->last = NULL;
						free(p);
						p = NULL;
					}
					else {
						p->last->next = p->next;
						if (p->next != NULL)//����
							p->next->last = p->last;
						else {//��β
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
			//�߱䳤
			grow();
		}
		if (bugcheck(snake_head->x, snake_head->y)) {
			//���ӱ���
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
			//�߱��
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
				strcpy_s(&GameScreen[10][15],10, "��Ϸ����!");
				strcpy_s(&GameScreen[11][12],17, "R.����  E.������");
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
/// ������ͷ�ķ���
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
/// ���ɳ���
/// </summary>
void generatebug() {
	int x, y;
	Location* p, * temp;
	if (counter == 0) {
		//������еĳ���
		p = bug_head;
		while (p != NULL) {
			temp = p;
			p = p->next;
			free(temp);
			bugcount--;
		}
		//�������ɳ���
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
/// ���ɽ��
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
/// ָ��λ���Ƿ��г��ӣ���1����0
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
/// ָ��λ���Ƿ��н�ң���1����0
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
/// ָ��λ���Ƿ����ߣ���1����0
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