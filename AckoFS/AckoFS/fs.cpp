#include "fs.h"
#include "KernelFS.h"

KernelFS* FS::myImpl = new KernelFS();

using namespace std;

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

char FS::doesExist(char *fname) {
	myImpl->lock();
	auto result = myImpl->doesExist(fname);
	myImpl->unlock();
	return result;
}

File * FS::open(char *fname, char mode) {
	myImpl->lock();
	auto result = myImpl->open(fname, mode);
	myImpl->unlock();
	return result;
}

char FS::deleteFile(char *fname) {
	myImpl->lock();
	auto result = myImpl->deleteFile(fname);
	myImpl->unlock();
	return result;
}

void Entry::splitRelativePath(std::string relativePath) {
	auto position = relativePath.find('.');
	memcpy(name, relativePath.data(), position);
	for (int i = position; i < FNAMELEN; i++) {
		name[i] = ' ';
	}
	memcpy(ext, relativePath.substr(position + 1).data(), relativePath.length() - position - 1);
	for (int i = relativePath.length() - position - 1; i < FEXTLEN; i++) {
		ext[i] = ' ';
	}
}

std::string Entry::getRelativePath() {
	// make this a function for real now
	char name0[FNAMELEN + 1];
	memcpy(name0, name, FNAMELEN);
	int length = FNAMELEN;
	while (name0[length - 1] == ' ') {
		length--;
	}
	name0[length] = '\0';
	char ext0[FEXTLEN + 1];
	memcpy(ext0, ext, FEXTLEN);
	length = FEXTLEN;
	while (ext0[length - 1] == ' ') {
		length--;
	}
	ext0[length] = '\0';
	return string(name0) + "." + string(ext0);
}

std::string Entry::toString() {
	// for now it should suffice .
	return getRelativePath() + " --- " + std::to_string(indexCluster);
}
