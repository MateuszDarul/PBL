#include "ListComponent.h"

ListComponent::ListComponent()
    :Component(2)
{
    list.push_back(0.25);
    list.push_back(1.50);
    list.push_back(2.75);
}

double ListComponent::Get(uint32_t index)
{
    if(index >= list.size() || index < 0)
        return 0.0;

    return list[index];
}

bool ListComponent::Remove(uint32_t index)
{
    if(index >= list.size() || index < 0)
        return false;

    list.erase(list.begin() + index);
    return true;
}

void ListComponent::Add(double v)
{
    list.push_back(v);
}