#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <format>
#include "Pipe.h"
#include "CS.h"
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
        << "0. Exit" << endl
        << "Choose action: ";
}
bool filter_pipe_name(const Pipe& p, const string& x) {
    return p.get_name().find(x) != string::npos;
}
bool filter_pipe_repair(const Pipe& p, const bool& x) {
    return p.get_repair() == x;
}
bool filter_cs_name(const CS& c, const string& x) {
    return c.get_name().find(x) != string::npos;
}
bool filter_cs_idle(const CS& c, const double& x) {
    if (c.get_workshop() == 0) return false;
    double idle = 100.0 * (c.get_workshop() - c.get_actworkshop()) / c.get_workshop();
    return idle >= x;
}
template <typename TObj, typename T>
using Filter = bool(*)(const TObj& o, const T& x);

template <typename TObj, typename T>
vector<int> Search(const unordered_map<int, TObj*>& objs, T x, Filter<TObj, T> f)
{
    vector<int> subset;
    for (const auto& [id, obj] : objs)
    {
        if (f(*obj, x)) subset.push_back(id);
    }
    return subset;
}
vector<int> select_ids(const vector<int>& found)
{
    cout << "1 - All found, 2 - Specific IDs: ";
    if (GetCorrectNumber(1, 2) == 1) return found;
    vector<int> chosen;
    cout << "Enter ID (0 to finish):\n";
    while (true) 
    {
        cout << "Enter ID: ";
        int id = GetCorrectNumber(0, 100000);
        if (id == 0) break;
        if (find(found.begin(), found.end(), id) != found.end()) {
            chosen.push_back(id);
        }
        else {
            cout << "Not found in the list, try again.\n";
        }
    }
    return chosen;
}
void SearchEditPipes(unordered_map<int, Pipe*>& pipes)
{
    if (pipes.empty()) { cout << "No pipes available.\n"; return; }
    cout << "1 - by name, 2 - in work: ";
    vector<int> found;
    if (GetCorrectNumber(1, 2) == 1) {
        string name; cout << "Name: "; INPUT_LINE(cin, name);
            found = Search(pipes, name, filter_pipe_name);
    }
    else {
        found = Search(pipes, true, filter_pipe_repair);
    }

    if (found.empty()) { cout << "Nothing found.\n"; return; }
    for (int id : found) cout << id << " ";
    cout << "\n";

    vector<int> chosen = select_ids(found);
    cout << "1 - work, 2 - repair, 3 - delete: ";
    int act = GetCorrectNumber(1, 3);

    for (int id : chosen) {
        if (act == 1) pipes[id]->setWorking(true);
        else if (act == 2) pipes[id]->setWorking(false);
        else { delete pipes[id]; pipes.erase(id); }
    }
}
void SearchEditCS(unordered_map<int, CS*>& css)
{
    if (css.empty()) { cout << "No CS available.\n"; return; }
    cout << "1 - by name, 2 - by idle %: ";
    vector<int> found;
    if (GetCorrectNumber(1, 2) == 1) {
        string name; cout << "Name: "; INPUT_LINE(cin, name);
            found = Search(css, name, filter_cs_name);
    }
    else {
        cout << "Min %: "; double p = GetCorrectNumber<double>(0.0, 100.0);
        found = Search(css, p, filter_cs_idle);
    }

    if (found.empty()) { cout << "Nothing found.\n"; return; }
    for (int id : found) cout << id << " ";
    cout << "\n";

    vector<int> chosen = select_ids(found);
    cout << "1 - start, 2 - stop, 3 - delete: ";
    int act = GetCorrectNumber(1, 3);

    for (int id : chosen) {
        if (act == 1) css[id]->start_workshop();
        else if (act == 2) css[id]->stop_workshop();
        else { delete css[id]; css.erase(id); }
    }
}
void SaveAll(const unordered_map<int, Pipe*>& pipes, const unordered_map<int, CS*>& css)
{
    string filename;
    cout << "Enter filename to save: ";
    INPUT_LINE(cin, filename);
    ofstream fout(filename);
    if (!fout) { cout << "Error opening file.\n"; return; }
    fout << pipes.size() << '\n';
    for (const auto& [id, p] : pipes) p->save(fout);
    fout << css.size() << '\n';
    for (const auto& [id, c] : css) c->save(fout);
    cout << "All objects saved.\n";
}
void LoadAll(unordered_map<int, Pipe*>& pipes, unordered_map<int, CS*>& css)
{
    string filename;
    cout << "Enter filename to load: ";
    INPUT_LINE(cin, filename);
    ifstream fin(filename);
    if (!fin) { cout << "Error opening file.\n"; return; }
    for (const auto& [id, p] : pipes) delete p;
    pipes.clear();
    for (const auto& [id, c] : css) delete c;
    css.clear();
    size_t count = 0;
    fin >> count;
    for (size_t i = 0; i < count; i++) {
        Pipe* p = new Pipe();
        p->load(fin);
        pipes[p->get_id()] = p;
    }
    fin >> count;
    for (size_t i = 0; i < count; i++) {
        CS* c = new CS();
        c->load(fin);
        css[c->get_id()] = c;
    }
    cout << "All objects loaded.\n";
    cerr << "LOG: Loaded from " << filename << endl;
}
int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string time = format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now());
    ofstream logfile("log_" + time + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);
    unordered_map <int, Pipe*> pipes;
    unordered_map<int, CS*> css;
    while(1)
    {
        PrintMenu();
        switch (GetCorrectNumber(0, 7))
        {
        case 1:
        {
            Pipe * p = new Pipe();
            cin >> *p;
            int id = p->get_id();
            pipes[id] = p;
            cerr << "LOG: Added pipe id=" << id << endl;
            break;
        }
        case 2:
        {
            CS * cs = new CS();
            cin >> *cs;
            int id = cs->get_id();
            css[id] = cs;
            cerr << "LOG: Added CS id=" << id << endl;
            break;
        }
        case 3:
        {
            cout << "Available Pipes IDs: ";
            if (pipes.empty()) cout << "No pipes available.";
            else for (const auto& [id, p] : pipes) cout << id << " ";
            cout << "\n\n";
            for (const auto& [id, p] : pipes) cout << *p;
            cout << "Available CS IDs: ";
            if (css.empty()) cout << "No css available.";
            else for (const auto& [id, cs] : css) cout << id << " ";
            cout << "\n\n";
            for (const auto& [id, cs] : css) cout << *cs;
            break;
        }
        case 4:
        {
            SearchEditPipes(pipes);
            break;
        }
        case 5:
        {
            SearchEditCS(css);
            break;
        }
        case 6:
        {
            SaveAll(pipes, css);
            break;
        }
        case 7:
        {
            LoadAll(pipes, css);
            break;
        }
        case 0:
        {
            for (const auto& [id, p] : pipes) delete p;
            for (const auto& [id, cs] : css) delete cs;
            return 0;
        }
        default:
            cout << "Wrong action" << endl;
        }
    }
    return 0;
}