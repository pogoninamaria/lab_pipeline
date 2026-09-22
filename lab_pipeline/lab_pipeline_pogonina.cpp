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
    }
    while (true)
    {
        if (cin >> p.isWorking && cin.peek() == '\n' && (p.isWorking == 0 || p.isWorking == 1))
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
    while (true)
    {
        getline(cin >> ws, cs.n2);
        break;
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
    cout << "Pipe:\n";
    cout << "d = " << p.d << endl;
    cout << "l = " << p.l << endl;
    cout << "n = " << p.n << endl;
    cout << "status: " << (p.isWorking ? "" : "don't ") << "work.\n";
}
void PrintCS(const CS& cs, bool CsExist)
{
    if (!CsExist)
    {
        cout << "CS doesn`t exist.\n";
        return;
    }
    cout << "CS:\n";
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
    cout << "Pipe is " << (p.isWorking ? "" : "not ") << "working now.\n";//!!
    cout << "1 - Pipe is working" << endl;
    cout << "2 - Pipe isn`t working." << endl;
    int choice;
    cin >> choice;
    while (choice != 1 && choice != 2)
    {
        cerr << "Error: enter 1 or 2" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> choice;
    }
    p.isWorking = (choice == 1);
    cout << "Pipe is changed.\n";
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
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> choice2;
    }
    if (choice2 == 1)
    {
        if (cs.actworkshop < cs.workshop)
        {
            cs.actworkshop++;
            cout << "Workshop started.\n";
        }
        else
            cout << "All workshops are active.\n";
    }
    else
    {
        if (cs.actworkshop > 0)
        {
            cs.actworkshop--;
            cout << "Workshop stopped.\n";
        }
        else
            cout << "There are not active workshops.\n";
    }
}
void SavePipe(const Pipe& p, bool PipeExist)
{
    ofstream fout("file.txt");//!!
    if (fout)
    {
        fout << PipeExist << endl;
        if (PipeExist)
        {
            fout << p.d << endl;
            fout << p.l << endl;
            fout << p.n << endl;
            fout << p.isWorking << endl;
        }
        fout.close();
        cout << "Pipe saved.\n";
    }
    else
    {
        cout << "File isn`t found.\n";
    }
}
void SaveCs(const CS & cs, bool CsExist)
{
    ofstream fout("file.txt", ios::app);//!!
    if (fout)
    {
        fout << CsExist << endl;
        if (CsExist)
        {
            fout << cs.n2 << endl;
            fout << cs.workshop << endl;
            fout << cs.actworkshop << endl;
            fout << cs.station << endl;
        }
        fout.close();
        cout << "CS saved.\n";
    }
    else
    {
        cout << "File isn`t found.\n";
    }
}
void LoadPipe(Pipe& p, bool& PipeExist)
{
    ifstream fin("file.txt");
    if (fin)
    {
        fin >> PipeExist;
        if (PipeExist)
        {
            fin >> p.d;
            fin >> p.l;
            getline(fin >> ws, p.n);
            fin >> p.isWorking;
        }
        fin.close();
        cout << "Pipe loaded.\n";
    }
    else
    {
        cout << "File isn`t found.\n";
    }
}
void LoadCs(CS& cs, bool& CsExist)
{
    ifstream fin("file.txt");
    if (fin)
    {
        bool PipeExists;
        fin >> PipeExists;
        if (PipeExists)
        {
            int d;
            double l;
            string n;
            bool isWorking;
            fin >> d;
            fin >> l;
            getline(fin >> ws, n);
            fin >> isWorking;
        }
        fin >> CsExist;
        if (CsExist)
        {
            getline(fin >> ws, cs.n2);
            fin >> cs.workshop;
            fin >> cs.actworkshop;
            fin >> cs.station;
        }
        fin.close();
        cout << "CS loaded.\n";
    }
    else
    {
        cout << "File isn`t found.\n";
    }
}
int main()
{
    Pipe p = {};
    CS cs = {};
    bool PipeExist = false;
    bool CsExist = false;
    int choice = -1;
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
        if (!(cin >> choice && cin.peek() == '\n' && choice >= 0 && choice <= 7))
        {
            cout << "Error: enter a number from 0 to 7.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }
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
    } while (choice != 0);
    return 0;
}