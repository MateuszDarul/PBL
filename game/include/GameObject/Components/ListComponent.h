#ifndef __LIST_COMPONENT_H__
#define __LIST_COMPONENT_H__

#include "Component.h"

class ListComponent :public Component
{
private:
    std::vector<double> list;

public:
    ListComponent();

    double Get(int index);

    bool Remove(int index);
    
    void Add(double v);
};

#endif // __LIST_COMPONENT_H__