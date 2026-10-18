#pragma once
#include <iostream>
#include <string>

class CS
{
    int id;
    std::string n2;
    int workshop;
    int actworkshop;
    char station;

    static int NextCsID;

public:
    CS();
    int get_id() const { return id; }
    const std::string& get_name() const { return n2; }
    int get_workshop() const { return workshop; }
    int get_actworkshop() const { return actworkshop; }
    char get_station() const { return station; }
    static int get_next_id() { return NextCsID; }
    void start_workshop() { if (actworkshop < workshop) actworkshop++; }
    void stop_workshop() { if (actworkshop > 0) actworkshop--; }
    void save(std::ostream& file) const;
    void load(std::istream& file);
    friend std::ostream& operator << (std::ostream& out, const CS& c);
    friend std::istream& operator >> (std::istream& in, CS& c);
};

