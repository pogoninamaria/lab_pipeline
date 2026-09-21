#include <iostream>

using namespace std;

struct Pipe
{
    int d; //диаметр(мм)
    double l; //длина(км)
    std::string n; //километровая оценка
    bool isWorking; //ремонт трубы
};
struct CS
{
    std::string n2; //название
    int workshop; //количество цехов
    int actworkshop; //количество работающих цехов
    char station; //класс станции
};
void NewPipe(Pipe& p)
{
 cout << "Input diametr(mm), length(km), name, status (1-work, 0-don`t work)" << endl;
 cin >> p.d >> p.l >> p.n >> p.isWorking;
 cout << "New pipe added.\n";
}
void NewCS(CS& cs)
{
 cout << "Input name of CS, workshops, actworkshops, class of station" << endl;
 cin >> cs.n2 >> cs.workshop;
 do
 {
  cin >> cs.actworkshop;
 } while (cs.actworkshop > cs.workshop);
 cin >> cs.station;
 cout << "New CS added.\n";
}
void PrintPipe(const Pipe& p)
{
 cout << "d = " << p.d << endl;
 cout << "l = " << p.l << endl;
 cout << "n = " << p.n << endl;
 cout << (p.isWorking ? "" : "Don't ") << "work";
}
void PrintCS(const CS& cs)
{
 cout << "name of CS = " << cs.n2 << endl;
 cout << "workshops = " << cs.workshop << endl;
 cout << "actworkshops = " << cs.actworkshop << endl;
 cout << "class of station = " << cs.station << endl;
}
void EditPipe(Pipe& p)
{
 cout << "1 - Pipe is working" << endl;
 cout << "2 - Pipe is repairing." << endl;
 int choice;
 cin >> choice;
 while (choice != 1 && choice != 2)
 {
  cerr << "Error: enter 1 or 2" << endl;
  cin >> choice;
 }
 if (choice == 1)
 {
  p.isWorking = true;
 }
 else
 {
  p.isWorking = false;
 }
 cout << "Pipe is changed";
}
void EditCS(CS& cs)
{
 cout << "workshops = " << cs.workshop << endl;
 cout << "actworkshops = " << cs.actworkshop << endl;
 cout << "1 - Start workshop." << endl;
 cout << "2 - Stop workshop." << endl;
 int choice2;
 cin >> choice2;
 while (choice2 != 1 && choice2 != 2)
 {
  cerr << "Error: enter 1 or 2" << endl;
  cin >> choice2;
 }
 if (choice2 == 1)
 {
  if (cs.actworkshop < cs.workshop)
  {
   cs.actworkshop++;
   cout << "Workshop started.";
  }
  else
  {
   cout << "All workshops are active.";
  }
 }
 else
 {
  if (cs.actworkshop < 0)
  {
   cs.actworkshop--;
   cout << "Workshop stopped.";
  }
  else
  {
   cout << "There are not active workshops.";
  }
 }
}
int main()
{
 
}