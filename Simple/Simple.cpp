#include "Common.h"
#include "DBG.h"
#include "MM.h"
#include "ERR.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        ERR_Print(ERR_LEVEL_ERROR, "%s filename", argv[0]);
        exit(-1);
    }


	return 0;
}

