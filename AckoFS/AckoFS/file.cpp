#include "file.h"
#include "KernelFile.h"

File::~File() {
}

// TODO (acko): Add synchronization

char File::write(BytesCnt count, char * buffer) {
	return myImpl->write(count, buffer);
}

BytesCnt File::read(BytesCnt count, char * buffer) {
	return myImpl->read(count, buffer);
}

char File::seek(BytesCnt position) {
	return myImpl->seek(position);
}

BytesCnt File::filePos() {
	return myImpl->filePos();
}

char File::eof() {
	return myImpl->eof();
}

BytesCnt File::getFileSize() {
	return myImpl->getFileSize();
}

char File::truncate() {
	return myImpl->truncate();
}
