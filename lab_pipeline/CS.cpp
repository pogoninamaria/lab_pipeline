#include "CS.h"
#include "utils.h"
using namespace std;

int CS::NextCsID = 1;

CS::CS()
{
    id = NextCsID++;
    name = "Unknown";
    workshop = 0;
    actworkshop = 0;
    station = '?';
}

ostream& operator << (ostream& out, const CS& c)
{
    PRINT_PARAM(out, c.id);
    PRINT_PARAM(out, c.name);
    PRINT_PARAM(out, c.workshop);
    PRINT_PARAM(out, c.actworkshop);
    PRINT_PARAM(out, c.station);
    return out;
}

istream& operator >> (istream& in, CS& c)
{
    cout << "Input name of CS: ";
    INPUT_LINE(in, c.name);
    cout << "Input workshops (1 - 1000): ";
    c.workshop = GetCorrectNumber(1, 1000);
    cout << "Input active workshops (0 - " << c.workshop << "): ";
    c.actworkshop = GetCorrectNumber(0, c.workshop);
    cout << "Input class of station: ";
    in >> c.station;//!!!
    return in;
}
void CS::save(std::ostream& file) const
{
    file << id << '\n'
        << name << '\n'
        << workshop << '\n'
        << actworkshop << '\n'
        << station << '\n';
}
void CS::load(std::istream& file)
{
    file >> id;
    INPUT_LINE(file, name);
    file >> workshop >> actworkshop >> station;
    if (id >= NextCsID)
    {
        NextCsID = id + 1;
    }
}