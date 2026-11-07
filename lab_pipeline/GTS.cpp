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
        cout << "Error: CS not found.\n";
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
        cout << "Error: only 500, 700, 1000 or 1400.";
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
    cout << "Error: no free pipe with this diameter " << diam << " found.\n";
}
void GTS::deleteCS(int id)
{
    for (const auto& link : links)
    {
        if (link.cs_in == id || link.cs_out == id)
        {
            cout << "Error: disconnect CS.\n";
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
            cout << "Error: disconnect pipe.\n";
            return;
        }
    }
    pipes.erase(id);
    cout << "Pipe deleted.\n";
}
void GTS::topologicalSort()
{
    int n = css.size();
    if (n == 0)
    {
        cout << "No CS available for sorting.\n";
        return;
    }
    unordered_map<int, int> id_to_idx;
    unordered_map<int, int> idx_to_id;
    int idx = 0;
    for (const auto& [id, cs] : css)
    {
        id_to_idx[id] = idx;
        idx_to_id[idx] = id;
        idx++;
    }
    vector<vector<int>> adj(n, vector<int>(n, 0));
    vector<int> in_degree(n, 0);
    for (const auto& link : links)
    {
        int u = id_to_idx[link.cs_in];
        int v = id_to_idx[link.cs_out];
        adj[u][v] = 1;
        in_degree[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++)
    {
        if (in_degree[i] == 0)
        {
            q.push(i);
        }
    }
    vector<int> order;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v = 0; v < n; v++)
        {
            if (adj[u][v] == 1)
            {
                in_degree[v]--;
                if (in_degree[v] == 0)
                {
                    q.push(v);
                }
            }
        }
    }
    if (order.size() != n)
    {
        cout << "Error: graph contains a cycle.\n";
        return;
    }
    cout << "Topological order of CS: ";
    for (int i : order)
    {
        cout << idx_to_id[i] << " ";
    }
    cout << "\n";
}