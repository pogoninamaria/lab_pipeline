#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include "Pipe.h"
#include "CS.h"

struct Link
{
	int cs_in = -1;
	int cs_out = -1;
	int pipe_id = -1;
};
class GTS
{
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CS> css;
    std::vector<Link> links;

public:
    void AddPipe(const Pipe& p);
    void AddCS(const CS& c);
    void PrintAll() const;
    void SearchEditPipes();
    void SearchEditCS();
    void linkCS();
    bool isPathExist(int start, int end);
    void deleteCS(int id);
    void deletePipe(int id);
    void topologicalSort();
    void disconnectLink();
    void SaveAll(const std::string& filename) const;
    void LoadAll(const std::string& filename);
};

