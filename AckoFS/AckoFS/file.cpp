#include "file.h"
#include "KernelFile.h"

File::~File() {
	delete myImpl;
}

char File::write(BytesCnt count, char * buffer) {
	myImpl->lock();
	auto result = myImpl->write(count, buffer);
	myImpl->unlock();
	return result;
}

BytesCnt File::read(BytesCnt count, char * buffer) {
	myImpl->lock();
	auto result = myImpl->read(count, buffer);
	myImpl->unlock();
	return result;
}

char File::seek(BytesCnt position) {
	myImpl->lock();
	auto result = myImpl->seek(position);
	myImpl->unlock();
	return result;
}

BytesCnt File::filePos() {
	myImpl->lock();
	auto result = myImpl->filePos();
	myImpl->unlock();
	return result;
}

char File::eof() {
	myImpl->lock();
	auto result = myImpl->eof();
	myImpl->unlock();
	return result;
}

BytesCnt File::getFileSize() {
	myImpl->lock();
	auto result = myImpl->getFileSize();
	myImpl->unlock();
	return result;
}

char File::truncate() {
	myImpl->lock();
	auto result = myImpl->truncate();
	myImpl->unlock();
	return result;
}
