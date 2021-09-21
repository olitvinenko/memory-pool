#include <iostream>

#include "memory/Defines.hpp"
#include "memory/ObjectPtr.hpp"

class Noizy
{
    DECLARE_POOLED_ALLOCATION(Noizy);
public:
    Noizy()
    {
        std::cout << "Noizy::Noizy()";
    }
    
    ~Noizy()
    {
        std::cout << "Noizy::~Noizy()";
    }
};
IMPLEMENT_POOLED_ALLOCATION(Noizy);

int main()
{
    {
        object_ptr<Noizy> n1(new Noizy);
        Noizy* m = n1;
        //Noizy* m = new Noizy;
        delete m;
    }
    
    
    return 0;
}
