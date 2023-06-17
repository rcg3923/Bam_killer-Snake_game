# Snake Game

> Ncurses를 사용하여 C++로 Snake Game 제작.

팀명 : 뱀 사냥꾼
팀원 : (김찬혁), (위승주), (이재석)

![](./Image/start.png)

## 실행 방법

snake.cpp, snake_game.h, ./snake 다운로드 받은 후 터미널 상에서 <br>
./snake 명령어 입력 후 아무키나 입력하면 초기 화면이 아래 사진 처럼 나온다. <br>

컴파일 방법
g++ snake.cpp -o snake -lncurses

![](./Image/ingame.png)

## 게임 룰

키보드의 조이스틱을 사용하여 조작한다. <br>
↑ (위) , ← (왼쪽), → (오른쪽), ↓ (아래쪽). <br>
머리가 꼬리가 있는 방향으로 이동하면 Lose. <br>
스네이크 크기가 2개 이하 Lose. <br>
벽에 박으면 Lose.

## 개발 환경 설정

터미널 혹은 WSL 사용

```sh
wget https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.2.tar.gz -P ~/Downloads
```

- 압축을 해제한 후 디렉터리가 있는 곳으로 이동 후 환경 설정
- 설치 시 make가 필요
  - 설치가 안되어 있는 경우 "sudo apt install make" 사용하여 설치 or "brew install make"
  - 설치확인은 make -v

```sh
./configure

make

sudo make install

```

## 업데이트 내역

- 1.0
  - 개발환경 구축 및 요구되는 기술 공부
- 1.1
  - 상수값 정의 및 클래스 구현
- 1.2
  - map 구현
- 1.3
  - snake와 map, item 연관 함수 구현
- 1.4
  - 인터페이스 수정
