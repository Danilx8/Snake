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
using namespace std;

/* добавить: поле, кнопки управления, генерацию еды на поле, взаимодействия змейки с едой и границами поля, 
   таблицу рекордов, ввод имени пользователя, систему баллов, базирующуюся на длине змейки при помощи вычитания 
   начальной длины змейки от её итоговой длины, условия победы, скорость змейки, UI для игры, главное меню с 
   началом, рейтингом и выходом.
*/

string userName;

class field {
  public:
    int height, width;
    
    void setHeight(int inputHeight) {
      height = inputHeight;
    }
    
    void setWidth(int inputWidth) {
      width = inputWidth;
    }
    
    void initializeField() {
      system ("cls");
      
      for (int y = 0; y < height; ++y) {
        if (y == 0 || y == height - 1) {
          for (int x = 0; x < width; ++x) {
            cout << '#';
          }
        } else {
          cout << '#' << string(width - 2, ' ') << '#';
        }
        cout << endl;
      }
    }
};

class entity {
  public:
    int x, y;
    
};

class snake: public entity{
  public:
    void move (int direction) {
      cout << direction;
    }
};

class food: public entity {
  
};

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

int main() {
  firstScreen();
  
  bool choice;
  cout << "Choose the action: press 1 to start the game or 0 to exit. ";
  cin >> choice;
  
  if (choice != true) {
    cout << "Goodbye!" << endl;
    system("pause");
    return 0;
  }
  
  while (true) {
    cout << "right choice";
    field area;
    area.setHeight(8);
    area.setWidth(15);
    area.initializeField();
    break;
  }
  
  system("pause");
  return 0;
}
