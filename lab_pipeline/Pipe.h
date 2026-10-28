#pragma once
#include <iostream>
#include <string>

class Pipe
{
    int id;
    int d;
    double l;
    std::string name;
    bool isWorking;
    static int NextPipeID;
public:
    Pipe();
    Pipe(int d, int l, const std::string& n, bool isWorking);

    static int get_next_id() { return NextPipeID; }
    static void reset_id() { NextPipeID = 1;}

    int get_id() const { return id; }
    int get_diameter() const { return d; }
    double get_length() const { return l; }
    const std::string& get_name() const { return name; }
    bool get_repair() const { return isWorking; }

    void setWorking(bool value) { isWorking = value; }

    void save(std::ostream& file) const;
    void load(std::istream& file);

    friend std::ostream& operator << (std::ostream& out, const Pipe& p);
    friend std::istream& operator >> (std::istream& in, Pipe& p);
};

