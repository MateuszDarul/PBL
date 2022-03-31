#include "GameApplication.h"

int main()
{
    int error = GameApplication::Init();
    if (error == 0)
    {
        GameApplication::Run();
    }

    return error;
}



