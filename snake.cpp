/*************************
*                        *
*     Lugovskih Danil    *
*        PI - 221        *
*       Snake game       *
*                        *
*************************/

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
using namespace std;

/* добавить: поле, кнопки управления, генерацию еды на поле, взаимодействия змейки с едой и границами поля,
   таблицу рекордов, ввод имени пользователя, систему баллов, базирующуюся на длине змейки при помощи вычитания
   начальной длины змейки от её итоговой длины, условия победы, скорость змейки, UI для игры, главное меню с
   началом, рейтингом и выходом.
*/

string userName;

struct coordinates {
  int rowIndex, columnIndex;
};

class field {
  private:
    static const int height, width;
    char ** currentField;

  public:
    field() {
      currentField = new char* [height];
      for(int rowIndex = 0; rowIndex < height; ++rowIndex) {
        currentField[rowIndex] = new char[width];
      }

      for (int rowIndex = 0; rowIndex < height ; ++rowIndex) {
        if (rowIndex == 0 || rowIndex == height - 1) {
          for (int columnIndex = 0; columnIndex < width; ++columnIndex) {
            currentField[rowIndex][columnIndex] = '#';
          }
        } else {
          currentField[rowIndex][0] = '#';
          currentField[rowIndex][width - 1] = '#';
        }
      }
    }

    void print() {
      system ("cls");

      for (int rowIndex = 0; rowIndex < height; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < width; ++columnIndex) {
          cout << currentField[rowIndex][columnIndex];
        }
        cout << endl;
      }
    }

    void clear() {
      for(int rowIndex = 1; rowIndex < height - 1; ++rowIndex) {
        for(int columnIndex = 1; columnIndex < width - 1; ++columnIndex) {
          currentField[rowIndex][columnIndex] = ' ';
        }
      }
    }

    int getHeight() const {
      return height;
    }

    int getWidth() const {
      return width;
    }

    void spawn(int rowIndex, int columnIndex, char character) {
      currentField[rowIndex][columnIndex] = character;
    }
} gameField;

class food {
  private:
    coordinates position;
    char symbol = '$';

  public:
    food():
      position() {
      position.columnIndex = -1;
      position.rowIndex = -1;
    }

    void setPosition(int rowIndex, int columnIndex) {
      position.rowIndex = rowIndex;
      position.columnIndex = columnIndex;
    }

    void reposition(const field& currentField) {
      position.rowIndex = rand() % (currentField.getHeight());
      position.columnIndex = rand() % (currentField.getWidth());
      if (position.rowIndex == 0 || position.columnIndex == 0 || position.rowIndex == currentField.getHeight() - 1
       || position.columnIndex == currentField.getWidth() - 1) {
        void(reposition(currentField));
      }
    }

    int getRowIndex() const {
      return position.rowIndex;
    }
    int getColumnIndex() const {
      return position.columnIndex;
    }
    char getSymbol() const {
      return symbol;
    }
} gameFood;

class snake {
  private:
    coordinates position[100];
    enum {UP, DOWN, LEFT, RIGHT} direction;
    char headSymbol = '@';
    char bodySymbol = '*';
    int speed = 1;
    int snakeSize;
    coordinates& head;

  public:
    snake (field& fieldData):
      position(),
      snakeSize(1),
      direction(RIGHT),
      head(position[0]) {
      position[0].rowIndex = fieldData.getHeight() / 2;
      position[0].columnIndex = fieldData.getWidth() / 2;
    }

    void snakeSpawn(field& currentField) {
      for(int bodyPartIndex = 0; bodyPartIndex < snakeSize; ++bodyPartIndex) {
        if(bodyPartIndex == 0) {
          currentField.spawn(position[bodyPartIndex].rowIndex,
                             position[bodyPartIndex].columnIndex, headSymbol);
        } else {
          currentField.spawn(position[bodyPartIndex].rowIndex,
                             position[bodyPartIndex].columnIndex, bodySymbol);
        }
      }
    }

    void getInput(const field& currentField) {
      if (GetAsyncKeyState(VK_UP) && (direction != DOWN)) {
        direction = UP;
      }
      if (GetAsyncKeyState(VK_DOWN) && (direction != UP)) {
        direction = DOWN;
      }
      if (GetAsyncKeyState(VK_LEFT) && (direction != RIGHT)) {
        direction = LEFT;
      }
      if (GetAsyncKeyState(VK_RIGHT) && (direction != LEFT)) {
        direction = RIGHT;
      }
    }

    void move(const field& currentField) {
      coordinates nextPosition = {0, 0};
      switch(direction) {
        case UP:
          nextPosition.columnIndex -= speed;
          break;
        case DOWN:
          nextPosition.columnIndex += speed;
          break;
        case LEFT:
          nextPosition.rowIndex -= speed;
          break;
        case RIGHT:
          nextPosition.rowIndex += speed;
      }
      
      for (int bodyPartIndex = snakeSize - 1; bodyPartIndex > 0; --bodyPartIndex) {
        position[bodyPartIndex] = position[bodyPartIndex - 1];
      }
      head.rowIndex += nextPosition.rowIndex;
      head.columnIndex += nextPosition.columnIndex;

      if (head.columnIndex < 0 ||
          head.rowIndex < 0 ||
          head.columnIndex >= currentField.getWidth() - 1||
          head.rowIndex >= currentField.getHeight() - 1) {
        throw "DEADD!!!!";
        
      }

      for (int bodyPartIndex = snakeSize - 1; bodyPartIndex > 0; --bodyPartIndex) {
        if (position[0].rowIndex == position[bodyPartIndex].rowIndex &&
            position[0].columnIndex == position[bodyPartIndex].columnIndex) {
          throw "Dead";
        }
      }
    }
    
    void getHeadCoordinates() {
      cout << head.rowIndex << endl << head.columnIndex;
    }
    
    bool checkFood(const food& currentFood) {
      if(currentFood.getRowIndex() == head.rowIndex && currentFood.getColumnIndex() == head.columnIndex) {
        snakeSize += 1;
        return true;
      }
      return false;
    }

} gameSnake(gameField);

void firstScreen() {
  cout << "                                                                                                                         \n"
       "                                                                                                                         \n"
       "     SSSSSSSSSSSSSSS NNNNNNNN        NNNNNNNN               AAA               KKKKKKKKK    KKKKKKKEEEEEEEEEEEEEEEEEEEEEE \n"
       "   SS:::::::::::::::SN:::::::N       N::::::N              A:::A              K:::::::K    K:::::KE::::::::::::::::::::E \n"
       "  S:::::SSSSSS::::::SN::::::::N      N::::::N             A:::::A             K:::::::K    K:::::KE::::::::::::::::::::E \n"
       "  S:::::S     SSSSSSSN:::::::::N     N::::::N            A:::::::A            K:::::::K   K::::::KEE::::::EEEEEEEEE::::E \n"
       "  S:::::S            N::::::::::N    N::::::N           A:::::::::A           KK::::::K  K:::::KKK  E:::::E       EEEEEE \n"
       "  S:::::S            N:::::::::::N   N::::::N          A:::::A:::::A            K:::::K K:::::K     E:::::E              \n"
       "   S::::SSSS         N:::::::N::::N  N::::::N         A:::::A A:::::A           K::::::K:::::K      E::::::EEEEEEEEEE    \n"
       "    SS::::::SSSSS    N::::::N N::::N N::::::N        A:::::A   A:::::A          K:::::::::::K       E:::::::::::::::E    \n"
       "      SSS::::::::SS  N::::::N  N::::N:::::::N       A:::::A     A:::::A         K:::::::::::K       E:::::::::::::::E    \n"
       "         SSSSSS::::S N::::::N   N:::::::::::N      A:::::AAAAAAAAA:::::A        K::::::K:::::K      E::::::EEEEEEEEEE    \n"
       "              S:::::SN::::::N    N::::::::::N     A:::::::::::::::::::::A       K:::::K K:::::K     E:::::E              \n"
       "              S:::::SN::::::N     N:::::::::N    A:::::AAAAAAAAAAAAA:::::A    KK::::::K  K:::::KKK  E:::::E       EEEEEE \n"
       "  SSSSSSS     S:::::SN::::::N      N::::::::N   A:::::A             A:::::A   K:::::::K   K::::::KEE::::::EEEEEEEE:::::E \n"
       "  S::::::SSSSSS:::::SN::::::N       N:::::::N  A:::::A               A:::::A  K:::::::K    K:::::KE::::::::::::::::::::E \n"
       "  S:::::::::::::::SS N::::::N        N::::::N A:::::A                 A:::::A K:::::::K    K:::::KE::::::::::::::::::::E \n"
       "   SSSSSSSSSSSSSSS   NNNNNNNN         NNNNNNNAAAAAAA                   AAAAAAAKKKKKKKKK    KKKKKKKEEEEEEEEEEEEEEEEEEEEEE \n"
       "                                                                                                                         \n"
       "                                                                                                                         \n"
       "                                                                                                                         \n"
       "                                                                                                                         \n"
       "                                               press any key to continue...                                              \n"
       "                                                                                                                         \n";
  _getch();
  system("cls");
  cout << "Enter your name: ";
  cin >> userName;
  system("cls");
}

const int field::height = 25;
const int field::width = 15;

int main() {
  firstScreen();
  
  gameFood.setPosition(22, 13);

  bool choice;
  cout << "Choose the action: press 1 to start the game or 0 to exit. ";
  cin >> choice;

  if (choice != true) {
    cout << "Goodbye!" << endl;
    system("pause");
    return 0;
  }
  
  //gameSnake.getHeadCoordinates();
  //cout << endl;
  //cout << gameField.getHeight() << ' ' << gameField.getWidth();
  while (true) {
    gameField.clear();

    gameSnake.getInput(gameField);
    try {
      gameSnake.move(gameField);
    } catch (const char * err) {
      gameField.clear();
      cerr << err << endl;
      system("pause");
      return -1;
    }
    gameSnake.snakeSpawn(gameField);
    gameField.spawn(gameFood.getRowIndex(), gameFood.getColumnIndex(), gameFood.getSymbol());
    cout << endl << gameFood.getRowIndex() << gameFood.getColumnIndex();

    if(gameSnake.checkFood(gameFood)) {
      gameFood.reposition(gameField);
    }

    gameField.print();

    Sleep(0.1);
    system("cls");
  } 
  system("pause");
  return 0;
}

/*
// C program to build the complete
// snake game
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int i, j, height = 20, width = 20;
int gameover, score;
int x, y, fruitx, fruity, flag;

// Function to generate the fruit
// within the boundary
void setup()
{
	gameover = 0;

	// Stores height and width
	x = height / 2;
	y = width / 2;
label1:
	fruitx = rand() % 20;
	if (fruitx == 0)
		goto label1;
label2:
	fruity = rand() % 20;
	if (fruity == 0)
		goto label2;
	score = 0;
}

// Function to draw the boundaries
void draw()
{
	system("cls");
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (i == 0 || i == width - 1
				|| j == 0
				|| j == height - 1) {
				printf("#");
			}
			else {
				if (i == x && j == y)
					printf("0");
				else if (i == fruitx
						&& j == fruity)
					printf("*");
				else
					printf(" ");
			}
		}
		printf("\n");
	}

	// Print the score after the
	// game ends
	printf("score = %d", score);
	printf("\n");
	printf("press X to quit the game");
}

// Function to take the input
void input()
{
	if (kbhit()) {
		switch (getch()) {
		case 'a':
			flag = 1;
			break;
		case 's':
			flag = 2;
			break;
		case 'd':
			flag = 3;
			break;
		case 'w':
			flag = 4;
			break;
		case 'x':
			gameover = 1;
			break;
		}
	}
}

// Function for the logic behind
// each movement
void logic()
{
	sleep(0.01);
	switch (flag) {
	case 1:
		y--;
		break;
	case 2:
		x++;
		break;
	case 3:
		y++;
		break;
	case 4:
		x--;
		break;
	default:
		break;
	}

	// If the game is over
	if (x < 0 || x > height
		|| y < 0 || y > width)
		gameover = 1;

	// If snake reaches the fruit
	// then update the score
	if (x == fruitx && y == fruity) {
	label3:
		fruitx = rand() % 20;
		if (fruitx == 0)
			goto label3;

	// After eating the above fruit
	// generate new fruit
	label4:
		fruity = rand() % 20;
		if (fruity == 0)
			goto label4;
		score += 10;
	}
}

// Driver Code
int main()
{
	int m, n;

	// Generate boundary
	setup();

	// Until the game is over
	while (!gameover) {

		// Function Call
		draw();
		input();
		logic();
 }
}
*/
