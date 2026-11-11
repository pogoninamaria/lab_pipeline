#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <format>
#include "GTS.h"
#include "utils.h"
using namespace std;
using namespace chrono;
void PrintMenu()
{
    cout << "\n           MENU           " << endl
        << "1. Add pipe" << endl
        << "2. Add CS" << endl
        << "3. View all objects" << endl
        << "4. Search and edit pipe" << endl
        << "5. Search and edit cs" << endl
        << "6. Save" << endl
        << "7. Load" << endl
        << "8. Link CS" << endl
        << "9. Disconnect Link" << endl
        << "10. Topological Sort" << endl
        << "0. Exit" << endl
        << "Choose action: ";
}
int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string time = format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now());
    ofstream logfile("log_" + time + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    GTS gts;

    while(1)
    {
        PrintMenu();
        switch (GetCorrectNumber(0, 10))
        {
        case 1:
        {
            Pipe p;
            cin >> p;
            gts.AddPipe(p);
            break;
        }
        case 2:
        {
            CS cs;
            cin >> cs;
            gts.AddCS(cs);
            break;
        }
        case 3:
        {
            gts.PrintAll();
            break;
        }
        case 4:
        {
            gts.SearchEditPipes();
            break;
        }
        case 5:
        {
            gts.SearchEditCS();
            break;
        }
        case 6:
        {
            string f;
            cout << "Enter filename to save: ";
            INPUT_LINE(cin, f);
            gts.SaveAll(f);
            break;
        }
        case 7:
        {
            string f;
            cout << "Enter filename to load: ";
            INPUT_LINE(cin, f);
            gts.LoadAll(f);
            break;
        }
        case 8:
        {
            gts.linkCS();
            break;
        }
        case 9:
        {
            gts.disconnectLink();
            break;
        }
        case 10:
        {
            gts.topologicalSort();
            break;
        }
        case 0:
        {
            return 0;
        }
        default:
            cout << "Wrong action" << endl;
        }
    }
    return 0;
}