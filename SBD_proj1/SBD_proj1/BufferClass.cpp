#include "BufferClass.h"
#include "MainClass.h"

BufferClass::BufferClass(int bufferSize, MainClass* mainClass, int id_number) {
	buffInit(bufferSize,mainClass,id_number);
};

BufferClass::~BufferClass() {
	delete[] buffer;
}

BufferClass::BufferClass() {
}

void BufferClass::buffInit(int bufferSize, MainClass* mainClass, int id_number) {
	this->mainClass = mainClass;
	this->bufferSize_b = bufferSize;
	this->id_number = id_number;
	buffer = new std::string[bufferSize_b];
	for (int i = 0; i < bufferSize_b; i++) buffer[i] = "";
};

bool BufferClass::readBuffer(std::fstream* file) {
	std::string line;
	for (int i = 0; i < bufferSize_b; i++) {
		if (!std::getline(*file, line)) {
			mainClass->addDiskOperation();
			mainClass->addDiskRead();
			return false;
		}
		else {

			buffer[i] = line;
		}
	}
	mainClass->addDiskOperation();
	mainClass->addDiskRead();
	return true;
};

void BufferClass::sortBuffer() {
	std::stable_sort(buffer, buffer + bufferSize_b,
		[](const std::string& a, const std::string& b) {
			return BufferClass::compareRecords(a, b) < 0;
		});
};

void BufferClass::printBuffer() {
	std::cout << "Buffer contents:\n";
	for (int i = 0; i < bufferSize_b; i++) {
		if (buffer[i] != "") {
			std::cout << buffer[i] << "\n";
		}
	}
};

int BufferClass::compareRecords(const std::string& record1, const std::string& record2) {
	if (record1 != record2) {
		return record1 < record2 ? -1 : 1;
	}
	return 0;
}



void BufferClass::saveBufferToMainFile(std::ofstream* outputFile, int* offset, int number_of_records) {
	bool actuallyWritten = false;
	for (int i = 0; i < bufferSize_b && buffer[i] != "" && (*offset)<number_of_records; i++, (*offset)++) {
		(*outputFile) << buffer[i] << "\n";
		actuallyWritten = true;
	}
	if (actuallyWritten) {
		mainClass->addDiskOperation();
		mainClass->addDiskWrite();
	}
}



std::string* BufferClass::getBuffer() {
	return buffer;
}

void BufferClass::setRunOffset(int offset,int endOffset) {
	this->startOffset = offset;
	this->runOffset = offset;
	this->endOffset = endOffset;
}

void BufferClass::readRunToBuffer() {
	bool actuallyRead = false;
	bufferRunIndex = 0;
	std::ifstream* inputFile = new std::ifstream(mainClass->getRecordFileName());
	if (!inputFile->is_open()) {
		std::cerr << "Error opening file: " << mainClass->getRecordFileName() << std::endl;
		return;
	}
	inputFile->seekg(runOffset * (MAX_LICENSE_PLATE_LENGHT + 2)); // +1 for newline character

	std::string line;
	for (int i = 0; i < bufferSize_b && runOffset < endOffset; i++, runOffset++) {
		if (!std::getline(*inputFile, line)) {
			buffer[i] = "";
		}
		else {
			actuallyRead = true;
			buffer[i] = line;
		}
	}
	inputFile->close();
	if (actuallyRead) {
		mainClass->addDiskOperation();
		mainClass->addDiskRead();
	}
}

void BufferClass::writeRunToFile() {
	bool actuallyWritten = false;
	std::ofstream outputFile = std::ofstream(mainClass->getResultFileName(), std::ios::app);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file: " << mainClass->getResultFileName() << std::endl;
		return;
	}
	for (int i = 0; i < bufferSize_b; i++) {
		if (!buffer[i].empty()) {
			actuallyWritten = true;
			outputFile << buffer[i] << "\n";
		}
	}
	outputFile.close();
	clearBuffer();
	if (actuallyWritten) {
		mainClass->addDiskOperation();
		mainClass->addDiskWrite();
	}
}

std::string BufferClass::getLowestRecord() {
	if (startOffset + bufferRunIndex >= endOffset) {
		return "";
	}
	std::string record = buffer[bufferRunIndex];
	bufferRunIndex++;
	if (bufferRunIndex >= bufferSize_b) {
		startOffset += bufferRunIndex;
		readRunToBuffer();
	}
	return record;
}

void BufferClass::clearBuffer() {
	for (int i = 0; i < bufferSize_b; i++) {
		buffer[i] = "";
	}

}

void BufferClass::putRecordToBuffer(const std::string& record) {
	for (int i = 0; i < bufferSize_b; i++) {
		if (buffer[i].empty()) {
			buffer[i] = record;
			return;
		}
	}
	writeRunToFile();
	buffer[0] = record;
}