#include "Pipe.h"
#include "utils.h" 
using namespace std;

int Pipe::NextPipeID = 1;

Pipe::Pipe()
{
    id = NextPipeID++;
    d = 0;
    l = 0.0;
    name = "Unknown";
    isWorking = true;
}

ostream& operator << (ostream& out, const Pipe& p)
{
    PRINT_PARAM(out, p.id);
    PRINT_PARAM(out, p.d);
    PRINT_PARAM(out, p.l);
    PRINT_PARAM(out, p.name);
    PRINT_PARAM(out, p.isWorking);
    return out;
}
istream& operator >> (istream& in, Pipe& p)
{
    cout << "Input diametr (mm): ";
    p.d = GetCorrectNumber(1, 100000);
    cout << "Input length (km): ";
    p.l = GetCorrectNumber<double>(0.001, 100000.0);
    cout << "Input name: ";
    INPUT_LINE(in, p.name);
    cout << "Input status (1 - work, 0 - repair): ";
    p.isWorking = GetCorrectNumber(0, 1) == 1;
    return in;
}
void Pipe::save(std::ostream& file) const
{
    file << id << '\n'
        << d << '\n'
        << l << '\n'
        << name << '\n'
        << isWorking << '\n';
}
void Pipe::load(std::istream& file)
{
    file >> id >> d >> l;
    INPUT_LINE(file, name);
    file >> isWorking;
    if (id >= NextPipeID)
    {
        NextPipeID = id + 1;
    }
}