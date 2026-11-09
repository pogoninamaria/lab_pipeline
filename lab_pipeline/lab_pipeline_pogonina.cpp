#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <format>
#include "Pipe.h"
#include "CS.h"
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
bool filter_pipe_name(const Pipe& p, string x)
{
    return p.get_name().find(x) != string::npos;
}
bool filter_pipe_repair(const Pipe& p, bool x)
{
    return p.get_repair() == x;
}
bool filter_cs_name(const CS& c, string x)
{
    return c.get_name().find(x) != string::npos;
}
bool filter_cs_idle(const CS& c, double x)
{
    return c.get_idle() >= x;
}
template <typename TObj, typename T>
using Filter = bool(*)(const TObj& o, T x);

template <typename TObj, typename T>
set<int> Search(const unordered_map<int, TObj>& objs, T x, Filter<TObj, T> f)
{
    set<int> subset;
    for (const auto& [id, obj] : objs)
    {
        if (f(obj, x)) 
            subset.emplace(id);
    }
    return subset;
}
set<int> select_ids(const set<int>& found)
{
    cout << "1 - All found, 2 - Specific IDs: ";
    if (GetCorrectNumber(1, 2) == 1)
        return set<int>(found.begin(), found.end());//
    set<int> chosen;
    cout << "Enter ID (0 to finish):\n";
    while (true) 
    {
        cout << "Enter ID: ";
        int id = GetCorrectNumber(0, 100000);
        if (id == 0)
            break;
        if (find(found.begin(), found.end(), id) != found.end()) {//!!!!
            chosen.insert(id);
        }
        else
        {
            cout << "Not found in the list, try again.\n";
        }
    }
    return chosen;
}
void SearchEditPipes(GTS& gts)
{
    if (gts.pipes.empty()) 
    {
        cout << "No pipes available.\n";
        return;
    }

    cout << "1 - by name, 2 - in work, 3 - in repair: ";
    set<int> found;
    int choice = GetCorrectNumber(1, 3);
    if (choice == 1) 
    {
        string name;
        cout << "Name: "; 
        INPUT_LINE(cin, name);
       found = Search(gts.pipes, name, filter_pipe_name);
    }
    else if(choice == 2)
    {
       found = Search(gts.pipes, true, filter_pipe_repair);
    }
    else
    {
       found = Search(gts.pipes, false, filter_pipe_repair);
    }
    if (found.empty())
    {
        cout << "Nothing found.\n"; 
        return; 
    }

    for (int id : found) 
        cout << id << " ";
    cout << "\n";

    set<int> chosen = select_ids(found);
    cout << "1 - work, 2 - repair, 3 - delete: ";
    int act = GetCorrectNumber(1, 3);

    for (int id : chosen)
    {
        if (act == 1) 
            gts.pipes[id].setWorking(true);
        else if (act == 2) 
            gts.pipes[id].setWorking(false);
        else 
        { 
           gts.deletePipe(id);
        }
    }
}
void SearchEditCS(GTS& gts)
{
    if (gts.css.empty())
    { 
        cout << "No CS available.\n";
        return;
    }
    cout << "1 - by name, 2 - by idle %: ";
    set<int> found;
    if (GetCorrectNumber(1, 2) == 1)
    {
        string name;
        cout << "Name: ";
        INPUT_LINE(cin, name);
        found = Search(gts.css, name, filter_cs_name);
    }
    else
    {
        cout << "Min %: ";
        double p = GetCorrectNumber<double>(0.0, 100.0);
        found = Search(gts.css, p, filter_cs_idle);
    }

    if (found.empty())
    { 
        cout << "Nothing found.\n";
        return;
    }
    for (int id : found)
        cout << id << " ";
    cout << "\n";

    auto chosen = select_ids(found);
    cout << "1 - start, 2 - stop, 3 - delete: ";
    int act = GetCorrectNumber(1, 3);

    for (int id : chosen)
    {
        if (act == 1)
            gts.css[id].start_workshop();
        else if (act == 2)
            gts.css[id].stop_workshop();
        else
        {
            gts.deleteCS(id);
        }
    }
}
void SaveAll(const GTS& gts)
{
    string filename;
    cout << "Enter filename to save: ";
    INPUT_LINE(cin, filename);
    ofstream fout(filename);
    if (!fout)
    { 
        cout << "Error opening file.\n";
        return;
    }
    fout << gts.pipes.size() << '\n';
    for (const auto& [id, p] : gts.pipes)
        p.save(fout);
    fout << gts.css.size() << '\n';
    for (const auto& [id, c] : gts.css)
        c.save(fout);
    fout << gts.links.size() << '\n';
    for (const auto& link : gts.links)
    {
        fout << link.cs_in << ' ' << link.cs_out << ' ' << link.pipe_id << '\n';
    }
    cout << "All objects saved.\n";
}
void LoadAll(GTS& gts)
{
    string filename;
    cout << "Enter filename to load: ";
    INPUT_LINE(cin, filename);
    ifstream fin(filename);
    if (!fin) 
    { 
        cout << "Error opening file.\n"; 
        return; 
    }
    gts.pipes.clear();
    gts.css.clear();
    gts.links.clear();
    Pipe::reset_id();
    CS::reset_id();
    size_t count = 0;
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        Pipe p;
        p.load(fin);
        gts.pipes.emplace(p.get_id(), p);
    }
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        CS c;
        c.load(fin);
        gts.css.emplace(c.get_id(), c);
    }
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        Link link;
        fin >> link.cs_in >> link.cs_out >> link.pipe_id;
        gts.links.push_back(link);
    }
    cout << "All objects loaded.\n";
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
            gts.pipes.emplace(p.get_id(), p);
            break;
        }
        case 2:
        {
            CS cs;
            cin >> cs;
            gts.css.emplace(cs.get_id(), cs);
            break;
        }
        case 3:
        {
            cout << "Available Pipes IDs: ";
            if (gts.pipes.empty())
                cout << "No pipes available.";
            else for (const auto& [id, p] : gts.pipes)
                cout << id << " ";
            cout << "\n";
            for (const auto& [id, p] : gts.pipes)
                cout << p;
            cout << "\n\n";
            cout << "Available CS IDs: ";
            if (gts.css.empty())
                cout << "No css available.";
            else for (const auto& [id, cs] : gts.css) 
                cout << id << " ";
            cout << "\n";
            for (const auto& [id, cs] : gts.css) 
                cout << cs;
            break;
        }
        case 4:
        {
            SearchEditPipes(gts);
            break;
        }
        case 5:
        {
            SearchEditCS(gts);
            break;
        }
        case 6:
        {
            SaveAll(gts);
            break;
        }
        case 7:
        {
            LoadAll(gts);
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