#include "KernelFile.h"

KernelFile::KernelFile() {
}


KernelFile::~KernelFile() {
}

char KernelFile::seek(BytesCnt position) {
	// TODO(acko): Check if position is legal 
	this->position = position;
	return '1';
}

BytesCnt KernelFile::filePos() {
	return position;
}
