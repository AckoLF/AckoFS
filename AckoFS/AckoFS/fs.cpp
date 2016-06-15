#include "fs.h"
#include "KernelFS.h"

KernelFS* FS::myImpl = new KernelFS();

char FS::mount(Partition *partition) {
	myImpl->lock();
	auto result = myImpl->mount(partition);
	myImpl->unlock();
	return result;
}

char FS::unmount(char part) {
	myImpl->lock();
	auto result = myImpl->unmount(part);
	myImpl->unlock();
	return result;
}

char FS::format(char part) {
	myImpl->lock();
	auto result = myImpl->format(part);
	myImpl->unlock();
	return result;
}
