/*************************
*                        *
*     Lugovskih Danil    *
*        PI - 221        *
*       Snake game       *
*                        *
*************************/

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
using namespace std;

void colorize(int colorNumber) {
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorNumber);
}

/* добавить: поле, кнопки управления, генерацию еды на поле, взаимодействия змейки с едой и границами поля,
   таблицу рекордов, ввод имени пользователя, систему баллов, базирующуюся на длине змейки при помощи вычитания
   начальной длины змейки от её итоговой длины, условия победы, скорость змейки, UI для игры, главное меню с
   началом, рейтингом и выходом.
*/

string userName = "-";
int score = 0;

struct coordinates {
  int columnIndex, rowIndex;
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

      cout << "\n\n\n"
           "\t NAME:" << userName << "\n"
           "\t CURRENT SCORE:" << score << "\n"
           "\t HIGHEST SCORE: \n";
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

    void spawn(int columnIndex, int rowIndex, char character) {
      currentField[columnIndex][rowIndex] = character;
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

    void setPosition(int columnIndex, int rowIndex) {
      position.rowIndex = rowIndex;
      position.columnIndex = columnIndex;
    }

    void reposition(const field& currentField) {
      position.rowIndex = srand() % (currentField.getHeight());
      position.columnIndex = srand() % (currentField.getWidth());
      if (position.rowIndex == 0 || position.columnIndex == 0 ||
          position.rowIndex == currentField.getHeight() - 1 ||
          position.columnIndex == currentField.getWidth() - 1) {
        void(reposition(currentField));
      }
    }

    int getColumnIndex() const {
      return position.columnIndex;
    }
    int getRowIndex() const {
      return position.rowIndex;
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
          currentField.spawn(position[bodyPartIndex].columnIndex,
                             position[bodyPartIndex].rowIndex, headSymbol);
        } else {
          currentField.spawn(position[bodyPartIndex].columnIndex,
                             position[bodyPartIndex].rowIndex, bodySymbol);
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

      if (head.columnIndex < 1 ||
          head.rowIndex < 1 ||
          head.columnIndex >= currentField.getHeight() - 1||
          head.rowIndex >= currentField.getWidth() - 1) {
        throw "dead";

      }

      for (int bodyPartIndex = snakeSize - 1; bodyPartIndex > 0; --bodyPartIndex) {
        if (position[0].rowIndex == position[bodyPartIndex].rowIndex &&
            position[0].columnIndex == position[bodyPartIndex].columnIndex) {
          throw "dead";
        }
      }
    }

    bool checkFood(const food& currentFood) {
      if(currentFood.getRowIndex() == head.columnIndex &&
          currentFood.getColumnIndex() == head.rowIndex) {
        snakeSize += 1;
        score += 10;
        return true;
      }
      return false;
    }

} gameSnake(gameField);

void firstScreen() {
  colorize(250);
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
       "                                              press any key to continue...                                               \n"
       "                                                                                                                         \n";
  _getch();
  system("cls");
}

struct leaderboardLine {
  int place;
  string name = "-";
  int score;
} players[10];

void createLeaderboard(string currentPlayerName, int currentPlayerScore) {
  ofstream outfile ("leaderboard.txt");
  players[0].name = currentPlayerName;
  players[0].score = currentPlayerScore;

  outfile << players[0].place;
  outfile << ' ';
  outfile << players[0].name;
  outfile << ' ';
  outfile << players[0].score;
  outfile << '\n';
  outfile.close();
}

int fileParsing(fstream& file, string currentPlayerName, int currentPlayerScore ) {
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    file >> players[playerIndex].place;
    file >> players[playerIndex].name;
    file >> players[playerIndex].score;
  }

  int worsePlayerScore;
  string worsePlayerName;
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    if (currentPlayerScore > players[playerIndex].score) {
      for (int worsePlayerIndex = playerIndex; worsePlayerIndex < 10; ++worsePlayerIndex) {
        worsePlayerScore = players[worsePlayerIndex].score;
        worsePlayerName = players[worsePlayerIndex].name;
        players[worsePlayerIndex].score = currentPlayerScore;
        players[worsePlayerIndex].name = currentPlayerName;
        currentPlayerScore = worsePlayerScore;
        currentPlayerName = worsePlayerName;
      }
      return 1;
    }
  }
  return 0;
}

void leaderboardOutput() {
  fstream output;
  output.open("leaderboard.txt", fstream::out | fstream::trunc);
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    output << players[playerIndex].place;
    output << ' ';
    output << players[playerIndex].name;
    output << ' ';
    output << players[playerIndex].score;
    output << '\n';
  }
  output.close();;
}

void leaderboard(string name, int score) {
  fstream readingFile;
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    players[playerIndex].place = playerIndex + 1;
  }
  readingFile.open("leaderboard.txt");
  if (readingFile.is_open()) {
    fileParsing(readingFile, name, score);
    readingFile.close();
  } else {
    createLeaderboard(name, score);
  }
  leaderboardOutput();
  readingFile.open("leaderboard.txt");
  cout << readingFile.rdbuf();
}

int choice() {
  int input;
  cout << "Choose the action: press 1 to start the game, 2 to show the leaderboard or 3 to exit. ";
  cin >> input;
  return input;
}

void gameOver () {
  system("cls");
  cout <<"   /^^^^         /^       /^^       /^^/^^^^^^^^\n"
       " /^    /^^      /^ ^^     /^ /^^   /^^^/^^      \n"
       "/^^            /^  /^^    /^^ /^^ / /^^/^^      \n"
       "/^^           /^^   /^^   /^^  /^^  /^^/^^^^^^  \n"
       "/^^   /^^^^  /^^^^^^ /^^  /^^   /^  /^^/^^      \n"
       " /^^    /^  /^^       /^^ /^^       /^^/^^      \n"
       "  /^^^^^   /^^         /^^/^^       /^^/^^^^^^^^\n"
       "                                                \n"
       "    /^^^^     /^^         /^^/^^^^^^^^/^^^^^^^  \n"
       "  /^^    /^^   /^^       /^^ /^^      /^^    /^^  \n"
       "/^^        /^^  /^^     /^^  /^^      /^^    /^^  \n"
       "/^^        /^^   /^^   /^^   /^^^^^^  /^ /^^      \n"
       "/^^        /^^    /^^ /^^    /^^      /^^  /^^    \n"
       "  /^^     /^^      /^^^^     /^^      /^^    /^^  \n"
       "    /^^^^           /^^      /^^^^^^^^/^^      /^^\n";
  leaderboard(userName, score);
  cout << "\nThanks for coming, goodbye!\n";
  system("pause");
}

const int field::height = 10;
const int field::width = 15;

int main() {
  firstScreen();

  gameFood.setPosition(8, 5);

  switch (choice()) {
    case 1:
      cout << "Enter your name: ";
      cin >> userName;
      system("cls");

      while (true) {
        gameField.clear();

        gameSnake.getInput(gameField);
        try {
          gameSnake.move(gameField);
        } catch (const char * err) {
          gameField.clear();
          gameOver();
          return -1;
        }
        gameSnake.snakeSpawn(gameField);
        gameField.spawn(gameFood.getRowIndex(), gameFood.getColumnIndex(), gameFood.getSymbol());

        if(gameSnake.checkFood(gameFood)) {
          gameFood.reposition(gameField);
        }

        gameField.print();

        Sleep(100);
        system("cls");
      }
      system("pause");
      return 0;
    case 2:
      leaderboard(userName, score);
      system("pause");
      system("cls");
      main();
    case 3:
      cout << "Thanks for coming, goodbye!\n";
      system("pause");
      return 0;
    default:
      cout << "wrong input\n";
      system("pause");
      system("cls");
      main();
  }
  return 0;
}
