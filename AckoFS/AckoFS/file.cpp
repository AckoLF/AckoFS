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
	auto result = myImpl->read(count, buffer);
	return result;
}

char File::seek(BytesCnt position) {
	auto result = myImpl->seek(position);
	return result;
}

BytesCnt File::filePos() {
	auto result = myImpl->filePos();
	return result;
}

char File::eof() {
	auto result = myImpl->eof();
	return result;
}

BytesCnt File::getFileSize() {
	auto result = myImpl->getFileSize();
	return result;
}

char File::truncate() {
	myImpl->lock();
	auto result = myImpl->truncate();
	myImpl->unlock();
	return result;
}
