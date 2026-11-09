#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
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
public:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CS> css;
    std::vector<Link> links;

    void linkCS();
    void deleteCS(int id);
    void deletePipe(int id);
    void topologicalSort();
    void disconnectLink();
};

