#include "GTS.h"
#include "utils.h"

using namespace std;

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
            cout << "Successfully linked using pipe id=" << id << "\n";
            return;
        }
    }
    cout << "No free pipe with this diameter " << diam << " found.\n";
}
void GTS::deleteCS(int id)
{
    for (const auto& link : links)
    {
        if (link.cs_in == id || link.cs_out == id)
        {
            cout << "First disconnect CS.\n";
            return;
        }
    }
    css.erase(id);
    cout << "CS deleted.\n";
}
void GTS::deletePipe(int id)
{
    for (const auto& link : links)
    {
        if (link.pipe_id == id)
        {
            cout << "First disconnect pipe.\n";
            return;
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
    unordered_map<int, int> in_degree;
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