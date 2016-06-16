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

char FS::readRootDir(char part, EntryNum n, Directory & d) {
	myImpl->lock();
	auto result = myImpl->readRootDir(part, n, d);
	myImpl->unlock();
	return result;
}

char FS::doesExist(char * fname) {
	myImpl->lock();
	auto result = myImpl->doesExist(fname);
	myImpl->unlock();
	return result;
}

File * FS::open(char * fname, char mode) {
	myImpl->lock();
	auto result = myImpl->open(fname, mode);
	myImpl->unlock();
	return result;
}

char FS::deleteFile(char * fname) {
	myImpl->lock();
	auto result = myImpl->deleteFile(fname);
	myImpl->unlock();
	return result;
}
