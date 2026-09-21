#include <iostream>
#include <string>
#include <fstream>
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
    //cin >> p.d >> p.l >> p.n >> p.isWorking;
    while (true)
    {
        if (cin >> p.d && cin.peek() == '\n' && p.d > 0 && p.d < 100000)
        {
            break;
        }
        cerr << "Error: input a positive value.";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        if (cin >> p.l && cin.peek() == '\n' && p.l > 0 && p.l < 100000)
        {
            break;
        }
        cerr << "Error: input a positive value.";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        getline(cin >> ws, p.n);
        break;
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        if (cin >> p.isWorking && cin.peek() == '\n' && p.isWorking == 0 || p.isWorking == 1)
        {
            break;
        }
        cerr << "Error: input 0 or 1.";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cout << "New pipe added.\n";
}
void NewCS(CS& cs)
{
    cout << "Input name of CS, workshops, actworkshops, class of station" << endl;
    //cin >> cs.n2 >> cs.workshop;
    while (true)
    {
        getline(cin >> ws, cs.n2);
        break;
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        if (cin >> cs.workshop && cin.peek() == '\n' && cs.workshop > 0 && cs.workshop < 1000)
        {
            break;
        }
        cerr << "Error: input a positive value.";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        if (cin >> cs.actworkshop && cin.peek() == '\n' && cs.actworkshop >= 0 && cs.actworkshop <= cs.workshop)
        {
            break;
        }
        cerr << "Error: input a value from 0 to " << cs.workshop << "\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    while (true)
    {
        if (cin >> cs.station && cin.peek() == '\n')
        {
            break;
        }
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cout << "New CS added.\n";
}
void PrintPipe(const Pipe& p, bool PipeExist)
{
    if (!PipeExist)
    {
        cout << "Pipe doesn`t exist.\n";
        return;
    }
    cout << "d = " << p.d << endl;
    cout << "l = " << p.l << endl;
    cout << "n = " << p.n << endl;
    cout << (p.isWorking ? "" : "Don't ") << "work";
}
void PrintCS(const CS& cs, bool CsExist)
{
    if (!CsExist)
    {
        cout << "CS doesn`t exist.\n";
        return;
    }
    cout << "name of CS = " << cs.n2 << endl;
    cout << "workshops = " << cs.workshop << endl;
    cout << "actworkshops = " << cs.actworkshop << endl;
    cout << "class of station = " << cs.station << endl;
}
void EditPipe(Pipe& p, bool PipeExist)
{
    if (!PipeExist)
    {
        cout << "Pipe doesn`t exist.\n";
        return;
    }
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
void EditCs(CS& cs, bool CsExist)
{
    if (!CsExist)
    {
        cout << "CS doesn`t exist.\n";
        return;
    }
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
void SavePipe(const Pipe& p, bool PipeExist)
{
    ofstream file("pipe.txt");
    if (!PipeExist)
    {
        file << "Pipe doesn`t exist.";
        file.close();
        return;
    }
    file << p.d << endl;
    file << p.l << endl;
    file << p.n << endl;
    file << p.isWorking << endl;
    file.close();
    cout << "Pipe saved.\n";
}
void SaveCs(const CS& cs, bool CsExist)
{
    ofstream file("cs.txt");
    if (!CsExist)
    {
        file << "CS doesn`t exist.";
        file.close();
        return;
    }
    file << cs.n2 << endl;
    file << cs.workshop << endl;
    file << cs.actworkshop << endl;
    file << cs.station << endl;
    file.close();
    cout << "CS saved.\n";
}
void LoadPipe(Pipe& p, bool& PipeExist)
{
    ifstream file("pipe.txt");
    file >> p.d;
    file >> p.l;
    getline(file >> ws, p.n);
    file >> p.isWorking;
    PipeExist = true;
    file.close();
    cout << "Pipe loaded.\n";
}
void LoadCs(CS& cs, bool& CsExist)
{
    ifstream file("cs.txt");
    getline(file >> ws, cs.n2);
    file >> cs.workshop;
    file >> cs.actworkshop;
    file >> cs.station;
    CsExist = true;
    file.close();
    cout << "Cs loaded.\n";
}
int main()
{
    Pipe p = {};
    CS cs = {};
    bool PipeExist = false;
    bool CsExist = false;
    int choice;
    do
    {
        cout << "\n           MENU           \n";
        cout << "1. Add pipe\n";
        cout << "2. Add CS\n";
        cout << "3. View all objects\n";
        cout << "4. Edit pipe\n";
        cout << "5. Edit CS\n";
        cout << "6. Save\n";
        cout << "7. Load\n";
        cout << "0. Exit\n";
        if (cin >> choice && cin.peek() == '\n' && choice >= 0 && choice <= 7)
        {
            switch (choice)
            {
            case 1:
                if (PipeExist)
                {
                    cout << "Pipe has already existed.\n";
                }
                else
                {
                    NewPipe(p);
                    PipeExist = true;
                }
                break;
            case 2:
                if (CsExist)
                {
                    cout << "CS has already existed.\n";
                }
                else
                {
                    NewCS(cs);
                    CsExist = true;
                }
                break;
            case 3:
                PrintPipe(p, PipeExist);
                PrintCS(cs, CsExist);
                break;
            case 4:
                EditPipe(p, PipeExist);
                break;
            case 5:
                EditCs(cs, CsExist);
                break;
            case 6:
                SavePipe(p, PipeExist);
                SaveCs(cs, CsExist);
                break;
            case 7:
                LoadPipe(p, PipeExist);
                LoadCs(cs, CsExist);
                break;
            case 0:
                cout << "Program finished.\n";
                break;
            }
        }
        else
        {
            cout << "Error: enter a number from 0 to 7.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    } while (choice != 0);
    return 0;
}