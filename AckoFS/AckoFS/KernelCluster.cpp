#include "KernelCluster.h"

#include <iostream>

using namespace std;

KernelCluster::KernelCluster(char * cluster) {
	this->cluster = cluster;
	this->position = 0U;
}

unsigned int KernelCluster::getPosition() {
	return position;
}

void KernelCluster::seek(unsigned int position) {
	this->position = position;
}

char KernelCluster::readByte() {
	if (position >= 2048U) {
		throw "KernelCluster::readByte() end of file!";
	}
	return cluster[position++];
}

void KernelCluster::writeByte(char byte) {
	cluster[position++] = byte;
}

unsigned long KernelCluster::readNumber() {
	if (position >= 2045U) {
		throw "KernelCluster::readNumber() end of file!";
	}
	unsigned long result;
	memcpy(&result, cluster + position, 4);
	position += 4;
	return result;
}

void KernelCluster::writeNumber(unsigned long number) {
	memcpy(cluster + position, &number, 4);
	position += 4;
}

char KernelCluster::peekByte() {
	return cluster[position];
}

unsigned long KernelCluster::peekNumber() {
	unsigned long result;
	memcpy(&result, cluster + position, 4);
	return result;
}

Entry KernelCluster::readClusterEntry() {
	if (position >= 2029U) {
		throw "KernelCluster::readClusterEntry() end of file!";
	}
	Entry clusterEntry;
	for (int i = 0; i < FNAMELEN; i++) {
		clusterEntry.name[i] = readByte();
	}
	for (int i = 0; i < FEXTLEN; i++) {
		clusterEntry.ext[i] = readByte();
	}
	clusterEntry.reserved = readByte();
	clusterEntry.indexCluster = readNumber();
	clusterEntry.size = readNumber();
	return clusterEntry;
}

void KernelCluster::writeClusterEntry(Entry clusterEntry) {
	for (int i = 0; i < FNAMELEN; i++) {
		writeByte(clusterEntry.name[i]);
	}
	for (int i = 0; i < FEXTLEN; i++) {
		writeByte(clusterEntry.ext[i]);
	}
	writeByte(clusterEntry.reserved);
	writeNumber(clusterEntry.indexCluster);
	writeNumber(clusterEntry.size);
}
