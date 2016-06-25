#include "file.h"
#include "KernelFile.h"

File::~File() {
}

// TODO (acko): Add synchronization

char File::write(BytesCnt count, char * buffer) {
	return myImpl->write(count, buffer);
}

BytesCnt File::read(BytesCnt, char * buffer) {
	return BytesCnt();
}

char File::seek(BytesCnt) {
	return 0;
}

BytesCnt File::filePos() {
	return BytesCnt();
}

char File::eof() {
	return 0;
}

BytesCnt File::getFileSize() {
	return BytesCnt();
}

char File::truncate() {
	return 0;
}
