#pragma once
#include <string>
#include <iostream>
#include <fstream>

class MainClass;


class BufferClass {
private:
	std::string* buffer;
	int id_number;
	int bufferRunIndex=0;
	int startOffset = 0;
	int runOffset=0;
	int endOffset = 0;
	int bufferSize_b;
	const int MAX_LICENSE_PLATE_LENGHT = 8;
	MainClass* mainClass;
public:
	BufferClass();
	void buffInit(int bufferSize, MainClass* mainClass, int id_number);
	BufferClass(int bufferSize, MainClass* mainClass, int id_number);
	~BufferClass();
	bool readBuffer(std::fstream* file);
	void sortBuffer();
	void printBuffer();
	static int compareRecords(const std::string& record1, const std::string& record2);
	void saveBufferToMainFile(std::ofstream* outputFile, int* offset, int number_of_records);

	std::string getLowestRecord();
	void writeRunToFile();
	void setRunOffset(int offset, int endOffset);
	void readRunToBuffer();
	void clearBuffer();
	void putRecordToBuffer(const std::string& record);

	std::string* getBuffer();
};