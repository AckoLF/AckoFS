#pragma once

#include "part.h"

class KernelPartition
{
public:
	KernelPartition(Partition *partition);
	~KernelPartition();
private:
	Partition *partition;
};

