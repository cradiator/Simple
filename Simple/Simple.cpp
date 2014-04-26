#include "Common.h"
#include "DBG.h"
#include "MM.h"

int main(int argc, char** argv)
{
	struct MM_GCStorage* storage;
	storage = MM_NewGCStorage();
	for (int i = 1; i < 6; ++i)
	{
		MM_Malloc(storage, i * 20);
	}

	MM_DumpGCMemory(storage);
	MM_SweepGCMemory(storage);
	MM_DumpGCMemory(storage);
	return 0;
}

