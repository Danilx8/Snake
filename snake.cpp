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
#include <time.h>
using namespace std;

const int SNAKE_HEAD_AND_LOGO_COLOR = 2;
const int FAILURE_COLOR = 4;
const int FOOD_COLOR = 6;
const int SNAKE_AND_TEXT_COLOR = 10;
const int FRIENDLY_TEXT_COLOR = 14;
const int WALLS_COLOR = 15;
const int USER_NAME_COLOR = 47;


void colorize(int colorNumber) {
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorNumber);
}

void setCursorPosition(int x, int y) {
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  COORD coord = { (SHORT)x, (SHORT)y };
  SetConsoleCursorPosition(hOut, coord);
}

void hideCursor() {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
}

string userName = "-";
int score = 0;

struct coordinates {
  int columnIndex, rowIndex;
};

struct leaderboardLine {
  int place;
  string name = "-";
  int score;
} players[10];

void createLeaderboard(string currentPlayerName, int currentPlayerScore) {
  ofstream outfile ("leaderboard.txt");
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    players[playerIndex].place = playerIndex + 1;
  }
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

void leaderboardBuilder() {
  fstream output;
  output.open("leaderboard.txt", fstream::out | fstream::trunc);
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    output << '\t';
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
  readingFile.open("leaderboard.txt");
  if (readingFile.is_open()) {
    fileParsing(readingFile, name, score);
    readingFile.close();
  } else {
    createLeaderboard(name, score);
  }

  leaderboardBuilder();
  readingFile.open("leaderboard.txt");
  colorize(FRIENDLY_TEXT_COLOR);
  cout << "\n\tBEST SCORES\n";
  cout << readingFile.rdbuf();
  readingFile.close();
}

void getHighestScore() {
  fstream file;
  file.open("leaderboard.txt");
  fileParsing(file, "-", 0);
  file.close();
}

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
      setCursorPosition(0,0);

      for (int rowIndex = 0; rowIndex < height; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < width; ++columnIndex) {
          switch(currentField[rowIndex][columnIndex]) {
            case '#':
              colorize(WALLS_COLOR);
              break;
            case '@':
              colorize(SNAKE_HEAD_AND_LOGO_COLOR);
              break;
            case '*':
              colorize(SNAKE_AND_TEXT_COLOR);
              break;
            case '$':
              colorize(FOOD_COLOR);
              break;
            default:
              break;
          }
          cout << currentField[rowIndex][columnIndex];
        }
        cout << endl;
      }

      colorize(FRIENDLY_TEXT_COLOR);
      cout << "\n\n\n"
           "\t NAME:" << userName << "\n"
           "\t CURRENT SCORE:" << score << "\n"
           "\t HIGHEST SCORE: " << players[0].score << "\n";
      cout.flush();
    }

    void clear() {
      setCursorPosition(0, 0);
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

    void reposition(field& currentField) {
      position.rowIndex = rand() % (currentField.getHeight());
      position.columnIndex = rand() % (currentField.getWidth());
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
    enum {STOP, UP, DOWN, LEFT, RIGHT, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT} direction;
    char headSymbol = '@';
    char bodySymbol = '*';
    int speed = 1;
    int snakeSize;
    coordinates& head;

  public:
    snake (field& fieldData):
      position(),
      snakeSize(1),
      direction(STOP),
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

    void getInput() {
      if (_kbhit()) {
        switch (_getch()) {
          case '4':
            if (direction != RIGHT) {
              direction = LEFT;
            }
            break;
          case '6':
            if (direction != LEFT) {
              direction = RIGHT;
            }
            break;
          case '8':
            if (direction != DOWN) {
              direction = UP;
            }
            break;
          case '5':
            if (direction != UP) {
              direction = DOWN;
            }
            break;
          case '7':
            if (direction != DOWNRIGHT) {
              direction = UPLEFT;
            }
            break;
          case '9':
            if (direction != DOWNLEFT) {
              direction = UPRIGHT;
            }
            break;
          case '1':
            if (direction != UPRIGHT) {
              direction = DOWNLEFT;
            }
            break;
          case '3':
            if (direction != UPLEFT) {
              direction = DOWNRIGHT;
            }
            break;
          default:
            break;
        }
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
          break;
        case UPRIGHT:
          nextPosition.rowIndex += speed;
          nextPosition.columnIndex -= speed;
          break;
        case UPLEFT:
          nextPosition.rowIndex -= speed;
          nextPosition.columnIndex -= speed;
          break;
        case DOWNRIGHT:
          nextPosition.rowIndex += speed;
          nextPosition.columnIndex += speed;
          break;
        case DOWNLEFT:
          nextPosition.rowIndex -= speed;
          nextPosition.columnIndex += speed;
          break;
        default:
          break;
      }

      if (direction != STOP) {
        for (int bodyPartIndex = snakeSize - 1; bodyPartIndex > 0; --bodyPartIndex) {
          position[bodyPartIndex] = position[bodyPartIndex - 1];
        }
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
  colorize(SNAKE_HEAD_AND_LOGO_COLOR);
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

int choice() {
  int input;
  colorize(SNAKE_AND_TEXT_COLOR);
  cout << "Choose the action: press 1 to start the game, 2 to show instructions, 3 to show the leaderboard, 4 to reset the leaderboard or 5 to exit. " << endl;
  cin >> input;
  return input;
}

void instructions() {
  system("cls");
  colorize(FRIENDLY_TEXT_COLOR);
  cout << "HOW TO PLAY (use numpad):\n789 - up and left, up, up and right\n456 - left, down, right\n1 3 - down and left, down and right\n";
}

void gameOver () {
  system("cls");
  colorize(FAILURE_COLOR);
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

const int field::height = 40;
const int field::width = 60;

int main() {
  hideCursor();
  getHighestScore();
  srand(time(0));
  firstScreen();
  enum choices {GAME = 1, TUTORIAL, LEADERBOARD, CLEAR_LEADERBOARD, EXIT};
  gameFood.setPosition((rand() % (gameField.getWidth() - 2)) + 1,
                       (rand() % (gameField.getHeight() - 2)) + 1);

  switch (choice()) {
    case GAME:
      colorize(SNAKE_AND_TEXT_COLOR);
      cout << "Enter your name without spaces: ";
      colorize(USER_NAME_COLOR);
      cin >> userName;
      system("cls");

      while (true) {
        gameSnake.getInput();
        try {
          gameSnake.move(gameField);
        } catch (const char * err) {
          system("cls");
          gameOver();
          return 0;
        }
        gameSnake.snakeSpawn(gameField);
        gameField.spawn(gameFood.getRowIndex(), gameFood.getColumnIndex(), gameFood.getSymbol());

        if(gameSnake.checkFood(gameFood)) {
          gameFood.reposition(gameField);
        }

        gameField.print();

        Sleep(10);
        gameField.clear();
      }
      system("pause");
      return 0;
    case TUTORIAL:
      instructions();
      system("pause");
      system("cls");
      main();
      return 0;
    case LEADERBOARD:
      system("cls");
      leaderboard(userName, score);
      system("pause");
      system("cls");
      main();
      return 0;
    case CLEAR_LEADERBOARD:
      remove("leaderboard.txt");
      for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
        players[playerIndex].score = 0;
        players[playerIndex].name = "-";
      }
      system("cls");
      colorize(FAILURE_COLOR);
      cout << "leaderboard reset successfully\n";
      system("pause");
      system("cls");
      main();
      return 0;
    case EXIT:
      system("cls");
      colorize(FRIENDLY_TEXT_COLOR);
      cout << "Thanks for playing, goodbye!\n";
      system("pause");
      return 0;
    default:
      system("cls");
      while(cin.fail()) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        colorize(FAILURE_COLOR);
        cout << "wrong input\n";
      }
      system("pause");
      system("cls");
      main();
  }
  return 0;
}
