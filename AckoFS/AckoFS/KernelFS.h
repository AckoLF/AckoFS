#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <Windows.h>

#include "KernelPartition.h"

class KernelFS
{
public:
	KernelFS();
	~KernelFS();
	void lock();
	void unlock();
	char mount(Partition *partition);
	char unmount(char partition);
	char format(char partition);
private:
	HANDLE mutex;
	std::vector<std::unique_ptr<KernelPartition>> mountedPartitions;
};

