#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

// #define speed 1000000
// #define speed 500000
// #define speed 250000
// #define speed 100000
#define speed 50000
class item
{
public:
  int condition; // 10 = growth, -10 = poison
  int Is;        // 1 true 0 false
};

// snake 클래스입니다.
class snake
{
  int x;
  int y;
  int len;

public:
  snake *node;

  void set(int a, int b)
  {
    x = a;
    y = b;
    len = 3;
    node = new snake;
    (this->node)->x = a + 1;
    (this->node)->y = b;
    (this->node)->node = new snake;
    ((this->node)->node)->x = a + 2;
    ((this->node)->node)->y = b;
    ((this->node)->node)->node = NULL;
  }
  int getX()
  {
    return x;
  }
  int getY()
  {
    return y;
  }
  int getLen()
  {
    return len;
  }
  void add_body()
  {
    len++;

    int temp_x = 0, temp_y = 0;

    snake *temp, *temp2;
    temp = new snake;
    temp2 = new snake;
    temp = this;
    while (1)
    {
      if (temp->node == NULL)
      {
        break;
      }
      temp_x = temp->x;
      temp_y = temp->y;
      temp = temp->node;
    }
    // 2 case
    if (temp_x - temp->x == 1)
    {
      temp2->x = temp->x - 1;
      temp2->y = temp->y;
    }
    else if (temp_x - temp->x == -1)
    { // 1 case
      temp2->x = temp->x + 1;
      temp2->y = temp->y;
    }
    else if (temp_y - temp->y == 1)
    { // 4 case
      temp2->x = temp->x;
      temp2->y = temp->y - 1;
    }
    else
    { // 3case
      temp2->x = temp->x;
      temp2->y = temp->y + 1;
    }
    temp2->node = NULL;
    temp->node = temp2;
  }
  void delete_body()
  {
    len--;
    snake *temp;
    temp = new snake;
    temp = this;
    while (1)
    {
      if ((temp->node)->node == NULL)
        break;

      temp = temp->node;
    }
    delete temp->node;
    temp->node = NULL;
  }

  void move(int input)
  {
    int i;
    int last_x = 0, last_y = 0;
    int temp_x = 0, temp_y = 0;
    snake *temp;
    temp = new snake;
    temp = this;

    i = len - 1;
    last_x = x;
    last_y = y;
    if (input == KEY_UP)
    {
      x -= 1;
    }
    else if (input == KEY_DOWN)
    {
      x += 1;
    }
    else if (input == KEY_LEFT)
    {
      y -= 1;
    }
    else if (input == KEY_RIGHT)
    {
      y += 1;
    }

    for (; i > 0; i--)
    {
      temp = temp->node;
      temp_x = temp->x;
      temp_y = temp->y;
      temp->x = last_x;
      temp->y = last_y;
      last_x = temp_x;
      last_y = temp_y;
    }
  }
};

// 여기부터 함수입니다.

int kbhit(void)
{
  struct termios pri_term, new_term;
  int st;
  int flag;

  // 현재 터미널의 속성을 저장합니다.
  tcgetattr(STDIN_FILENO, &pri_term);
  new_term = pri_term;

  // 터미널의 속성을 수정하여 입력을 즉시 읽을 수 있도록 설정합니다.
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  // 입력 스트림의 플래그를 변경하여 비차단(non-blocking) 모드로 설정합니다.
  flag = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK);

  // 사용자 입력을 읽어옵니다.
  st = getchar();

  // 터미널의 속성과 입력 스트림의 플래그를 복원합니다.
  tcsetattr(STDIN_FILENO, TCSANOW, &pri_term);
  fcntl(STDIN_FILENO, F_SETFL, flag);

  // 입력된 문자가 EOF가 아니라면 다시 입력 스트림으로 되돌리고 1을 반환합니다.
  if (st != EOF)
  {
    ungetc(st, stdin);
    return 1;
  }

  // 입력된 문자가 없을 경우 0을 반환합니다.
  return 0;
}

int printarr(int (*arr)[25], snake *f, WINDOW *win_score, WINDOW *win_mission, int grow, int poi)
{
  snake *x;
  x = new snake;
  x = f;
  int temp[25][25] = {0};

  // 색상 쌍을 초기화합니다.
  // 각 숫자에 해당하는 색상을 설정합니다.
  init_pair(1, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(3, COLOR_CYAN, COLOR_CYAN);
  init_pair(4, COLOR_RED, COLOR_RED);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_WHITE, COLOR_BLACK);

  // 임시 배열 temp에 arr의 값을 복사합니다.
  // temp 배열은 실제 출력할 맵을 저장하기 위해 사용됩니다.
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      temp[i][j] = arr[i][j];
    }
  }

  int len = x->getLen();
  temp[x->getX()][x->getY()] = 3;
  for (int i = 1; i < len; i++)
  {
    x = x->node;
    temp[x->getX()][x->getY()] = 4;
  }

  // 임시 배열 temp를 출력합니다.
  // 각 숫자에 해당하는 색상을 적용하여 맵을 출력합니다.
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      if (temp[i][j] == 0)
      {
        attron(COLOR_PAIR(1));
        mvprintw(i, j * 3, "000");
        attroff(COLOR_PAIR(1));
      }
      else if (temp[i][j] == 1)
      {
        attron(COLOR_PAIR(2));
        mvprintw(i, j * 3, "000");
        attroff(COLOR_PAIR(2));
      }
      else if (temp[i][j] == 2)
      {
        attron(COLOR_PAIR(3));
        mvprintw(i, j * 3, "000");
        attroff(COLOR_PAIR(3));
      }
      else if (temp[i][j] == 3)
      {
        attron(COLOR_PAIR(4));
        mvprintw(i, j * 3, "000");
        attroff(COLOR_PAIR(4));
      }
      else if (temp[i][j] == 4)
      {
        attron(COLOR_PAIR(5));
        mvprintw(i, j * 3, "000");
        attroff(COLOR_PAIR(5));
      }
      else if (temp[i][j] == 10)
      {
        attron(COLOR_PAIR(6));
        mvprintw(i, j * 3, "+++");
        attroff(COLOR_PAIR(6));
      }
      else if (temp[i][j] == -10)
      {
        attron(COLOR_PAIR(6));
        mvprintw(i, j * 3, "---");
        attroff(COLOR_PAIR(6));
      }
    }
    printw("\n");
    refresh();
  }

  // 스코어 보드를 출력합니다.
  // 게임의 스코어와 미션 달성 여부를 보여줍니다.
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
  int goal_B = 5, goal_P = 1, goal_M = 1, goal_G = 3, G = 3;
  char chk_B = ' ', chk_P = ' ', chk_M = ' ', chk_G = ' ';

  // 미션 달성 여부에 따라 체크 표시를 설정합니다.
  if (f->getLen() >= goal_B)
    chk_B = 'v';
  if (grow >= goal_P)
    chk_P = 'v';
  if (poi >= goal_M)
    chk_M = 'v';
  if (G >= goal_G)
    chk_G = 'v';

  // 스코어 보드 윈도우를 생성하고 내용을 출력합니다.
  win_score = newwin(9, 15, 1, 78);
  wbkgd(win_score, COLOR_PAIR(10));
  wattron(win_score, COLOR_PAIR(10));
  mvwprintw(win_score, 1, 1, " Score Board\n");
  mvwprintw(win_score, 2, 1, " \n");
  mvwprintw(win_score, 3, 1, " B : %d \n", f->getLen());
  mvwprintw(win_score, 4, 1, " + : %d\n", grow);
  mvwprintw(win_score, 5, 1, " - : %d\n", poi);
  mvwprintw(win_score, 6, 1, " G : %d", G);
  wborder(win_score, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(win_score);

  // 미션 창을 출력합니다.
  // 현재 미션과 달성 여부를 보여줍니다.
  win_mission = newwin(9, 15, 12, 78);
  wbkgd(win_mission, COLOR_PAIR(10));
  wattron(win_mission, COLOR_PAIR(10));
  mvwprintw(win_mission, 1, 1, "   Mission\n");
  mvwprintw(win_mission, 2, 1, " \n");
  mvwprintw(win_mission, 3, 1, " B : %d  (%c)\n", goal_B, chk_B);
  mvwprintw(win_mission, 4, 1, " + : %d  (%c)\n", goal_P, chk_P);
  mvwprintw(win_mission, 5, 1, " - : %d  (%c)\n", goal_M, chk_M);
  mvwprintw(win_mission, 6, 1, " G : %d  (%c)", goal_G, chk_G);
  wborder(win_mission, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(win_mission);

  usleep(speed);

  // 모든 미션을 달성했을 경우 1을 반환하여 게임을 종료합니다.
  if (chk_B == 'v' && chk_P == 'v' && chk_M == 'v' && chk_G == 'v')
    return 1;

  // 미션을 아직 달성하지 못했을 경우 0을 반환합니다.
  return 0;
}

int input_key(int x)
{

  switch (x)
  {
  case 65:
    x = KEY_UP;
    break;
  case 66:
    x = KEY_DOWN;
    break;
  case 68:
    x = KEY_LEFT;
    break;
  case 67:
    x = KEY_RIGHT;
    break;
  }
  return x;
}

int check(int (*arr)[25], snake *x, int input)
{
  // 맵 확인을 위한 배열
  int check_map[25][25] = {0};
  // 머리 위치 변수
  int head_x = 0, head_y = 0;

  // 뱀의 길이가 3 미만인 경우 이동 가능하지 않음
  if ((x->getLen()) < 3)
  {
    return 0;
  }

  // 게임 맵을 check_map 배열에 복사
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      check_map[i][j] = arr[i][j];
    }
  }

  // 뱀의 머리 위치 저장
  head_x = x->getX();
  head_y = x->getY();
  // 뱀의 길이 저장
  int len = x->getLen();

  // 뱀의 몸통 부분을 check_map에 표시
  for (int i = 1; i < len - 1; i++)
  {
    x = x->node;
    check_map[x->getX()][x->getY()] = 4;
  }

  // 입력된 방향에 따라 다음 이동이 가능한지 판단
  switch (input)
  {
  case KEY_UP:
    // 벽 또는 뱀의 몸통과 부딪힌 경우 이동 불가능
    if (check_map[head_x - 1][head_y] == 1 || check_map[head_x - 1][head_y] == 4)
      return 0;
    // 목표 지점에 도달한 경우
    else if (check_map[head_x - 1][head_y] == 10)
      return 2;
    // 적절한 이동인 경우
    else if (check_map[head_x - 1][head_y] == -10)
      return 3;
    break;

  case KEY_DOWN:
    // 벽 또는 뱀의 몸통과 부딪힌 경우 이동 불가능
    if (check_map[head_x + 1][head_y] == 1 || check_map[head_x + 1][head_y] == 4)
      return 0;
    // 목표 지점에 도달한 경우
    else if (check_map[head_x + 1][head_y] == 10)
      return 2;
    // 적절한 이동인 경우
    else if (check_map[head_x + 1][head_y] == -10)
      return 3;
    break;

  case KEY_LEFT:
    // 벽 또는 뱀의 몸통과 부딪힌 경우 이동 불가능
    if (check_map[head_x][head_y - 1] == 1 || check_map[head_x][head_y - 1] == 4)
      return 0;
    // 목표 지점에 도달한 경우
    else if (check_map[head_x][head_y - 1] == 10)
      return 2;
    // 적절한 이동인 경우
    else if (check_map[head_x][head_y - 1] == -10)
      return 3;
    break;

  case KEY_RIGHT:
    // 벽 또는 뱀의 몸통과 부딪힌 경우 이동 불가능
    if (check_map[head_x][head_y + 1] == 1 || check_map[head_x][head_y + 1] == 4)
      return 0;
    // 목표 지점에 도달한 경우
    else if (check_map[head_x][head_y + 1] == 10)
      return 2;
    // 적절한 이동인 경우
    else if (check_map[head_x][head_y + 1] == -10)
      return 3;
    break;
  }

  // 이동 가능한 경우
  return 1;
}

void creat_item(int (*arr)[25], snake *a, item *z)
{
  // 맵 상태를 확인하기 위한 임시 배열
  int temp[25][25] = {0};
  // 뱀 객체 복사
  snake *t;
  t = new snake;
  t = a;

  int x, y, p = 0;
  // 아이템 상태를 랜덤으로 설정
  p = rand() % 2;
  if (p == 0)
    z->condition = 10; // 10은 양수 아이템을 나타냄
  else
    z->condition = -10; // -10은 음수 아이템을 나타냄

  // 게임 맵을 temp 배열에 복사
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      temp[i][j] = arr[i][j];
    }
  }

  // 뱀의 위치를 temp 배열에 표시
  int len = t->getLen();
  temp[t->getX()][t->getY()] = 3;
  for (int i = 1; i < len; i++)
  {
    t = t->node;
    temp[t->getX()][t->getY()] = 4;
  }

  // 빈 공간에 아이템 위치 설정
  while (1)
  {
    x = rand() % 25;
    y = rand() % 25;
    if (temp[x][y] == 0)
      break; // 빈 공간인 경우 반복문 종료
  }
  arr[x][y] = z->condition; // 아이템 위치에 상태 설정
}

void delete_item(int (*arr)[25])
{
  // 맵을 순회하면서 아이템을 찾아 삭제
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      // 10 또는 -10인 경우, 아이템을 발견한 것이므로 삭제
      if (arr[i][j] == 10 || arr[i][j] == -10)
      {
        arr[i][j] = 0; // 아이템 삭제
        return;        // 아이템을 하나만 삭제하고 함수 종료
      }
    }
  }
}

void change_map(int (*map)[25], int x)
{
  // 맵 변경 함수

  // map2
  if (x == 1)
  {
    for (int i = 8; i < 16; i++)
      map[i][12] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    return;           // 함수 종료
  }
  // map3
  else if (x == 2)
  {
    for (int i = 8; i < 16; i++)
      map[i][12] = 0; // 지정된 범위 내의 좌표에 값을 0으로 변경
    for (int i = 8; i < 16; i++)
      map[i][17] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    for (int i = 8; i < 17; i++)
      map[15][i] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    return;           // 함수 종료
  }
  // map4
  else
  {
    int j = 0, k = 0;
    for (int i = 8; i < 16; i++)
      map[i][17] = 0; // 지정된 범위 내의 좌표에 값을 0으로 변경
    for (int i = 8; i < 17; i++)
      map[15][i] = 0; // 지정된 범위 내의 좌표에 값을 0으로 변경
    for (int i = 5; i < 13; i++)
      map[i][12] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    for (int i = 0; i < 6; i++)
      map[13 + j++][12 + i] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    for (int i = 0; i < 6; i++)
      map[13 + k++][12 - i] = 1; // 지정된 범위 내의 좌표에 값을 1로 변경
    return;                      // 함수 종료
  }
}

int main()
{
  int map[25][25] = {
      0,
  };

  // 테두리에 벽을 설정
  for (int i = 0; i < 25; i++)
  {
    map[0][i] = 1;
    map[24][i] = 1;
    map[i][0] = 1;
    map[i][24] = 1;
  }
  map[0][0] = 2; // 맵의 모서리에 특정한 값을 설정하여 구분
  map[0][24] = 2;
  map[24][0] = 2;
  map[24][24] = 2;

  int pressed_key;
  int cnt_sec = 0;
  int item_grow = 0;
  int item_poison = 0;
  int map_num = 0;
  snake user;
  item box;

  // 시작 위치를 랜덤으로 설정
  srand(time(NULL));
  user.set(rand() % 15 + 5, rand() % 15 + 5);
  box.Is = 0;
  box.condition = 0;
  srand(time(NULL));

  // ncurses 초기화
  initscr();
  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();

  start_color();
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x); // 터미널의 최대 크기 얻기

  int start_x = (max_x = 10) / 2; // 문구의 가로 중앙 위치 계산
  int start_y = max_y / 2;        // 문구의 세로 중앙 위치 계산
  mvprintw(start_y, start_x, "--- --- --- --- ---");
  mvprintw(start_y + 1, start_x, "|    |  | | | |  |");
  mvprintw(start_y + 2, start_x, "---  |  |-| |-|  |");
  mvprintw(start_y + 3, start_x, "  |  |  | | |\\   |");
  mvprintw(start_y + 4, start_x, "---  |  | | | \\  |");
  getch();

  WINDOW *win_score, *win_mission;

  user.add_body();

  printarr(map, &user, win_score, win_mission, item_grow, item_poison);

  pressed_key = KEY_RIGHT;

  while (1)
  {
    while (!kbhit())
    {
      // 입력된 키에 따라 게임 상태 체크
      if (check(map, &user, pressed_key) == 0)
      {
        start_y = max_y / 2;
        mvprintw(start_y, start_x, "========");
        mvprintw(start_y + 1, start_x, "you lose");
        mvprintw(start_y + 2, start_x, "========");

        getch();
        getch();
        endwin();
        return 0;
      }
      else if (check(map, &user, pressed_key) == 2)
      {
        box.Is = 0;
        delete_item(map);
        cnt_sec = 0;
        user.add_body();
        item_grow++;
      }
      else if (check(map, &user, pressed_key) == 3)
      {
        box.Is = 0;
        delete_item(map);
        cnt_sec = 0;
        user.delete_body();
        item_poison++;
      }

      user.move(pressed_key);

      // 맵을 출력하고 게임 종료 또는 다음 라운드로 이동
      if (printarr(map, &user, win_score, win_mission, item_grow, item_poison))
      {
        if (map_num == 3)
        {
          start_y = max_y / 2;
          mvprintw(start_y, start_x, "=======");
          mvprintw(start_y + 1, start_x, "you win");
          mvprintw(start_y + 2, start_x, "=======");

          getch();
          getch();
          endwin();
          return 0;
        }
        user.set(3, 4);
        user.add_body();
        change_map(map, ++map_num);
        item_grow = 0;
        item_poison = 0;

        printarr(map, &user, win_score, win_mission, item_grow, item_poison);
        mvprintw(10, 10, "round %d", map_num + 1);
        getch();
        getch();
        pressed_key = KEY_RIGHT;
      }

      cnt_sec++;
      // 아이템 생성 및 삭제
      if (cnt_sec == 50)
      {
        if (box.Is == 0)
        {
          box.Is = 1;
          creat_item(map, &user, &box);
          cnt_sec = 0;
        }
        else
        {
          box.Is = 0;
          delete_item(map);
          cnt_sec = 0;
        }
      }
    }

    // ESC 키 입력 처리
    if (getchar() == 27)
    {
      if (getchar() == 79)
      {
        pressed_key = input_key((int)getchar());
      }
    }

    cnt_sec++;
    // 아이템 생성 및 삭제
    if (cnt_sec == 50)
    {
      if (box.Is == 0)
      {
        box.Is = 1;
        creat_item(map, &user, &box);
        cnt_sec = 0;
      }
      else
      {
        box.Is = 0;
        delete_item(map);
        cnt_sec = 0;
      }
    }
  }

  mvprintw(12, 12, "END");

  refresh();
  getch();
  getch();
  endwin();
  return 0;
}