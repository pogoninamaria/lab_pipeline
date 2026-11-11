#include "GTS.h"
#include "utils.h"
#include <fstream>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

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
        if (find(found.begin(), found.end(), id) != found.end()) {
            chosen.insert(id);
        }
        else
        {
            cout << "Not found in the list, try again.\n";
        }
    }
    return chosen;
}
void GTS::AddPipe(const Pipe& p)
{
    pipes.emplace(p.get_id(), p);
}
void GTS::AddCS(const CS& c)
{
    css.emplace(c.get_id(), c); 
}
void GTS::PrintAll() const
{
    cout << "Available Pipes IDs: ";
    if (pipes.empty())
        cout << "No pipes available.";
    else
        for (const auto& [id, p] : pipes) cout << id << " ";
    cout << "\n";
    for (const auto& [id, p] : pipes) cout << p;
    cout << "\n\n";
    cout << "Available CS IDs: ";
    if (css.empty())
        cout << "No css available.";
    else
        for (const auto& [id, cs] : css) cout << id << " ";
    cout << "\n";
    for (const auto& [id, cs] : css) cout << cs;
}
void GTS::SearchEditPipes()
{
    if (pipes.empty())
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
        found = Search(pipes, name, filter_pipe_name);
    }
    else if (choice == 2)
    {
        found = Search(pipes, true, filter_pipe_repair);
    }
    else
    {
        found = Search(pipes, false, filter_pipe_repair);
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
            pipes[id].setWorking(true);
        else if (act == 2)
            pipes[id].setWorking(false);
        else
        {
            deletePipe(id);
        }
    }
}
void GTS::SearchEditCS()
{
    if (css.empty())
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
        found = Search(css, name, filter_cs_name);
    }
    else
    {
        cout << "Min %: ";
        double p = GetCorrectNumber<double>(0.0, 100.0);
        found = Search(css, p, filter_cs_idle);
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
            css[id].start_workshop();
        else if (act == 2)
            css[id].stop_workshop();
        else
        {
            deleteCS(id);
        }
    }
}
bool GTS::isPathExist(int start, int end)
{
    if (start == end)
    {
        return true;
    }
    queue<int> q;
    set<int> visited;
    q.push(start);
    visited.insert(start);
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        for (const auto& link : links)
        {
            if (link.cs_in == current)
            {
                if (link.cs_out == end)
                {
                    return true;
                }
                if (visited.find(link.cs_out) == visited.end())
                {
                    visited.insert(link.cs_out);
                    q.push(link.cs_out);
                }
            }
        }
    }
    return false;
}
void GTS::linkCS() {
    cout << "Enter input CS id: ";
    int in_id = GetCorrectNumber(1, 10000);
    cout << "Enter output CS id: ";
    int out_id = GetCorrectNumber(1, 10000);
    if (css.find(in_id) == css.end() || css.find(out_id) == css.end())
    {
        cout << "CS not found.\n";
        return;
    }
    if (isPathExist(out_id, in_id)) {
        cout << "Link has a cycle.\n";
        return;
    }
    cout << "Enter diameter (500, 700, 1000, 1400): ";
    int diam;
    while (true)
    {
        diam = GetCorrectNumber(500, 1400);
        if (diam == 500 || diam == 700 || diam == 1000 || diam == 1400)
        {
            break;
        }
        cout << "Input only 500, 700, 1000 or 1400.";
    }
    for (auto& [id, p] : pipes)
    {
        if (!p.get_connect() && p.get_diameter() == diam) {
            p.connect();
            links.push_back({ in_id, out_id, id });
            cout << "Linked using pipe id=" << id << "\n";
            return;
        }
    }
    cout << "No free pipe with this diameter " << diam << " found.\n";
}
void GTS::deleteCS(int id)
{
    bool has_links = false;
    for (const auto& link : links) {
        if (link.cs_in == id || link.cs_out == id) {
            has_links = true;
            break;
        }
    }
    if (has_links) {
        cout << "CS is in link. Do you want to disconnect it and delete? (1 - yes, 0 - no): ";
        if (GetCorrectNumber(0, 1) == 0) {
            cout << "Deletion cancelled.\n";
            return;
        }
    }
    for (int i = 0; i < links.size(); i++) {
        if (links[i].cs_in == id || links[i].cs_out == id) {
            pipes[links[i].pipe_id].disconnect();
            links.erase(links.begin() + i);
            i--;
        }
    }
    css.erase(id);
    cout << "CS deleted.\n";
}
void GTS::deletePipe(int id)
{
    for (int i = 0; i < links.size(); i++)
    {
        if (links[i].pipe_id == id)
        {
            cout << "Pipe is in link. Do you want to disconnect it and delete? (1 - yes, 0 - no): ";
            if (GetCorrectNumber(0, 1) == 1)
            {
                links.erase(links.begin() + i);
                i--;
            }
            else
            {
                cout << "Deletion cancelled.\n";
                return;
            }
        }
    }
    pipes.erase(id);
    cout << "Pipe deleted.\n";
}
void GTS::topologicalSort()
{
    if (css.empty())
    {
        cout << "No CS available.\n";
        return;
    }
    map<int, int> in_degree;
    for (const auto& [id, cs] : css)
    {
        in_degree[id] = 0;
    }
    for (const auto& link : links)
    {
        in_degree[link.cs_out]++;
    }
    queue<int> q;
    for (const auto& [id, deg] : in_degree)
    {
        if (deg == 0) q.push(id);
    }
    vector<int> order;
    while (!q.empty())
    {
        int current_cs = q.front();
        q.pop();
        order.push_back(current_cs);
        for (const auto& link : links)
        {
            if (link.cs_in == current_cs)
            {
                int next_cs = link.cs_out;
                in_degree[next_cs]--;
                if (in_degree[next_cs] == 0)
                {
                    q.push(next_cs);
                }
            }
        }
    }
    if (order.size() != css.size())
    {
        cout << "Graph contains a cycle.\n";
        return;
    }
    cout << "Topological order of CS: ";
    for (int id : order)
    {
        cout << id << " ";
    }
    cout << "\n";
}
void GTS::disconnectLink()
{
    if (links.empty())
    {
        cout << "No connections to disconnect.\n";
        return;
    }
    cout << "Current connections:\n";
    for (int i = 0; i < links.size(); i++)
    {
        cout << i + 1 << ". CS " << links[i].cs_in << " -> CS " << links[i].cs_out
            << " (Pipe id=" << links[i].pipe_id << ")\n";
    }
    cout << "Select connection number to remove (0 - cancel): ";
    int choice = GetCorrectNumber(0, 100);
    if (choice > 0)
    {
        int pipe_id = links[choice - 1].pipe_id;
        pipes[pipe_id].disconnect();
        links.erase(links.begin() + choice - 1);
        cout << "Connection removed. Pipe " << pipe_id << " is free.\n";
    }
}
void GTS::SaveAll(const std::string& filename) const
{
    std::ofstream fout(filename);
    if (!fout) { std::cout << "Error opening file.\n"; return; }
    fout << pipes.size() << '\n';
    for (const auto& [id, p] : pipes) p.save(fout);
    fout << css.size() << '\n';
    for (const auto& [id, c] : css) c.save(fout);
    fout << links.size() << '\n';
    for (const auto& link : links)
    {
        fout << link.cs_in << ' ' << link.cs_out << ' ' << link.pipe_id << '\n';
    }
    std::cout << "All objects saved.\n";
}
void GTS::LoadAll(const std::string& filename)
{
    std::ifstream fin(filename);
    if (!fin) { std::cout << "Error opening file.\n"; return; }
    pipes.clear();
    css.clear();
    links.clear();
    Pipe::reset_id();
    CS::reset_id();
    size_t count = 0;
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        Pipe p; p.load(fin);
        pipes.emplace(p.get_id(), p);
    }
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        CS c; c.load(fin);
        css.emplace(c.get_id(), c);
    }
    fin >> count;
    for (size_t i = 0; i < count; i++)
    {
        Link link;
        fin >> link.cs_in >> link.cs_out >> link.pipe_id;
        links.push_back(link);
    }
    std::cout << "All objects loaded.\n";
}