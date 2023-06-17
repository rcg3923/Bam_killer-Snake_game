#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define FALSE   0
#define TRUE    1

//#define speed 1000000
//#define speed 500000
//#define speed 250000
#define speed 100000
class item{
public:

  int condition; // 10 = growth, -10 = poison
  int Is; //1 true 0 false

};


//--------------------------------------SNAKE CLASS---------------------------------------------------
class snake {
  int x;
	int y;
	int len;

public:
  snake* node;

	void set(int a, int b) {
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
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	int getLen() {
		return len;
	}
	void add_body() {
		len++;

		int temp_x=0, temp_y=0;

		snake* temp,*temp2;
		temp = new snake;
		temp2 = new snake;
		temp = this;
		while(1) {
			if (temp->node == NULL) {
				break;
			}
			temp_x = temp->x;
			temp_y = temp->y;
			temp = temp->node;
		}
		//2 case
		if (temp_x - temp->x == 1) {
			temp2->x = temp->x - 1;
			temp2->y = temp->y;
		}
		else if (temp_x - temp->x == -1) { // 1 case
			temp2->x = temp->x + 1;
			temp2->y = temp->y;
		}
		else if (temp_y - temp->y == 1) { // 4 case
			temp2->x = temp->x;
			temp2->y = temp->y - 1;
		}
		else {//3case
			temp2->x = temp->x;
			temp2->y = temp->y + 1;
		}
		temp2->node = NULL;
		temp->node = temp2;
	}
	void delete_body() {
		len--;
		snake* temp;
		temp = new snake;
		temp = this;
		while (1) {
			if ((temp->node)->node == NULL) break;

			temp = temp->node;
		}
		delete temp->node;
		temp->node = NULL;
	}

  void move(int input){
    int i;
    int last_x=0, last_y=0;
    int temp_x=0, temp_y=0;
    snake* temp;
    temp = new snake;
    temp = this;

    i = len-1;
    last_x = x;
    last_y = y;
    if(input == KEY_UP){
      x-=1;
    }else if(input == KEY_DOWN){
      x+=1;
    }else if(input == KEY_LEFT){
      y-=1;
    }else if(input == KEY_RIGHT){
      y+=1;
    }

    for(; i>0 ; i--){
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

//-----------------------------------------FUNCTION-------------------------------------------------------

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


int printarr(int (*arr)[25], snake* f ,WINDOW *win_score,WINDOW *win_mission,int grow, int poi){

  snake* x;
  x= new snake;
  x = f;
  int temp[25][25]={0};

  init_pair(1,COLOR_WHITE,COLOR_WHITE);
  init_pair(2,COLOR_YELLOW, COLOR_YELLOW);
  init_pair(3,COLOR_BLUE, COLOR_BLUE);
  init_pair(4,COLOR_RED, COLOR_RED);
  init_pair(5,COLOR_CYAN, COLOR_CYAN);
  init_pair(6,COLOR_WHITE, COLOR_BLACK);


  for(int i=0;i<25;i++){
    for(int j=0;j<25;j++){
      temp[i][j] = arr[i][j];
    }
  }

  int len = x->getLen();
  temp[x->getX()][x->getY()] = 3;
  for(int i=1; i<len; i++){
    x = x->node;
    temp[x->getX()][x->getY()] = 4;
  }



  for(int i=0;i<25;i++){
    for(int j=0; j<25;j++){
      if(temp[i][j] == 0){
        attron(COLOR_PAIR(1));
        mvprintw(i,j*3,"000");
        attroff(COLOR_PAIR(1));
      }else if(temp[i][j] == 1){
        attron(COLOR_PAIR(2));
        mvprintw(i,j*3,"000");
        attroff(COLOR_PAIR(2));
      }else if(temp[i][j] == 2){
        attron(COLOR_PAIR(3));
        mvprintw(i,j*3,"000");
        attroff(COLOR_PAIR(3));
      }else if(temp[i][j] == 3){
        attron(COLOR_PAIR(4));
        mvprintw(i,j*3,"000");
        attroff(COLOR_PAIR(4));
    }else if(temp[i][j] == 4){
      attron(COLOR_PAIR(5));
      mvprintw(i,j*3,"000");
      attroff(COLOR_PAIR(5));
    }else if(temp[i][j] == 10){
      attron(COLOR_PAIR(6));
      mvprintw(i,j*3,"-|-");
      attroff(COLOR_PAIR(6));
    }else if(temp[i][j] == -10){
      attron(COLOR_PAIR(6));
      mvprintw(i,j*3,"---");
      attroff(COLOR_PAIR(6));
    }

}
printw("\n");
  refresh();
  }


  init_pair(10,COLOR_BLACK, COLOR_WHITE);
  int goal_B=5, goal_P=1, goal_M=1, goal_G=3, G =3;
  char chk_B=' ', chk_P=' ', chk_M=' ', chk_G =' ';

  if(f->getLen() >= goal_B) chk_B='v';
  if(grow >= goal_P) chk_P='v';
  if(poi >= goal_M) chk_M='v';
  if(G >=goal_G) chk_G ='v';

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

if(chk_B=='v'&& chk_P=='v'&& chk_M=='v'&& chk_G =='v') return 1;

return 0;
}

int input_key(int x){

    switch (x){
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

int check(int (*arr)[25], snake* x, int input){
  int check_map[25][25]={0};
  int head_x=0,head_y=0;

  if((x->getLen()) <3){
    return 0;
  }
  for(int i=0;i<25;i++){
    for(int j=0;j<25;j++){
      check_map[i][j] = arr[i][j];
    }
  }
  head_x = x->getX();
  head_y = x->getY();
  int len = x->getLen();

  for(int i=1; i<len-1; i++){
    x = x->node;
    check_map[x->getX()][x->getY()] = 4;
  }
  switch (input){
    case KEY_UP:
    if(check_map[head_x-1][head_y] == 1 || check_map[head_x-1][head_y] == 4) return 0;
    else if(check_map[head_x-1][head_y] == 10) return 2;
    else if(check_map[head_x-1][head_y] == -10) return 3;
      break;
    case KEY_DOWN:
    if(check_map[head_x+1][head_y] == 1 || check_map[head_x+1][head_y] == 4) return 0;
    else if(check_map[head_x+1][head_y] == 10) return 2;
    else if(check_map[head_x+1][head_y] == -10) return 3;
    break;
    case KEY_LEFT:
    if(check_map[head_x][head_y-1] == 1 || check_map[head_x][head_y-1] == 4) return 0;
    else if(check_map[head_x][head_y-1] == 10) return 2;
    else if(check_map[head_x][head_y-1] == -10) return 3;
    break;
    case KEY_RIGHT:
    if(check_map[head_x][head_y+1] == 1 || check_map[head_x][head_y+1] == 4) return 0;
    else if(check_map[head_x][head_y+1] == 10) return 2;
    else if(check_map[head_x][head_y+1] == -10) return 3;
    break;
  }
  return 1;
}


void creat_item(int (*arr)[25], snake* a, item* z){
  {
    int temp[25][25]={0};
    snake* t;
    t = new snake;
    t = a;

    int x,y,p=0;
    p = rand() % 2;
    if(p==0) z->condition = 10;
    else z -> condition = -10;

    for(int i=0;i<25;i++){
      for(int j=0;j<25;j++){
        temp[i][j] = arr[i][j];
      }
    }

    int len = t->getLen();
    temp[t->getX()][t->getY()] = 3;
    for(int i=1; i<len; i++){
      t = t->node;
      temp[t->getX()][t->getY()] = 4;
    }

    while(1){
      x = rand() % 25;
      y = rand()% 25;
      if(temp[x][y] == 0) break;
    }
    arr[x][y] = z->condition;
  }

}

void delete_item(int (*arr)[25]){
  {
    for(int i=0; i<25;i++){
      for(int j=0; j<25; j++){
        if(arr[i][j] == 10 || arr[i][j] == -10){
          arr[i][j] =0;
          return;
        }
      }
    }

}
}

void change_map(int (*map)[25], int x){

  //map2
  if(x ==1){
    for(int i=8; i<16; i++) map[i][12] = 1;
    return;
  }
  //map3
  else if(x ==2){
    for(int i=8; i<16; i++) map[i][12] = 0;
    for(int i=8;i<16;i++) map[i][17] =1;
    for(int i=8; i<17;i++) map[15][i] =1;
    return;
  }
  //map4
  else{
    int j=0,k=0;
    for(int i=8;i<16;i++) map[i][17] =0;
    for(int i=8; i<17;i++) map[15][i] =0;
    for(int i=5; i<13; i++) map[i][12] = 1;
    for(int i=0; i<6; i++) map[13+ j++][12 + i]= 1;
    for(int i=0;i<6;i++) map[13+ k++][12 -i] = 1;
    return;
  }
}

//---------------------------------------MAIN--------------------------------------------------

int main(){
  int map[25][25] = {0, };

  for(int i = 0; i < 25; i++){
    map[0][i] = 1;
    map[24][i] = 1;
    map[i][0] = 1;
    map[i][24] = 1;
  }
  map[0][0] = 2;
  map[0][24] = 2;
  map[24][0] = 2;
  map[24][24] = 2;

int pressed_key;
int cnt_sec=0;
int item_grow=0;
int item_poison=0;
int map_num=0;
snake user;
item box;

user.set(3, 4);
box.Is =0;
box.condition =0;
srand(time(NULL));




  initscr();
  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();




  start_color();


  mvprintw(12,12,"game start!");
  getch();

  WINDOW *win_score, *win_mission;





user.add_body();


printarr(map, &user, win_score, win_mission,item_grow,item_poison);


pressed_key = KEY_RIGHT;


  while(1){
    while (!kbhit()){

      if(check(map,&user,pressed_key) == 0){
        mvprintw(10,10,"oh no!!!!!!");
        getch();
        getch();
        endwin();
        return 0;
      }
      else if(check(map,&user,pressed_key) == 2){
        box.Is =0;
        delete_item(map);
        cnt_sec =0;
        user.add_body();
        item_grow++;
      }
      else if(check(map,&user,pressed_key) == 3){
        box.Is =0;
        delete_item(map);
        cnt_sec =0;
        user.delete_body();
        item_poison++;
      }

      user.move(pressed_key);
      if(printarr(map, &user, win_score, win_mission,item_grow,item_poison)){
        if(map_num ==3){
          mvprintw(10,10,"you win!!!");
          getch();
          getch();
          endwin();
          return 0;
        }
        user.set(3, 4);
        user.add_body();
        change_map(map, ++map_num);
        item_grow=0;
        item_poison=0;

        printarr(map, &user, win_score, win_mission,item_grow,item_poison);
        mvprintw(10,10,"round %d", map_num +1);
        getch();
        getch();
        pressed_key = KEY_RIGHT;
      }

      cnt_sec++;
      if(cnt_sec == 50){
        if(box.Is == 0){
          box.Is =1;
          creat_item(map, &user, &box);
          cnt_sec =0;
        }
        else{
          box.Is =0;
          delete_item(map);
          cnt_sec =0;
        }
      }
    }

    if(getchar()==27){
      if(getchar()==79){
          pressed_key = input_key((int)getchar());
      }
    }
    cnt_sec++;
    if(cnt_sec == 50){
      if(box.Is == 0){
        box.Is =1;
        creat_item(map, &user, &box);
        cnt_sec =0;
      }
      else{
        box.Is =0;
        delete_item(map);
        cnt_sec =0;
      }
    }
    }




  mvprintw(12,12,"END");


  refresh();
  getch();
  getch();
  endwin();
  return 0;
}
