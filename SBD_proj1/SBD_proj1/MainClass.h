#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <regex>
#include <vector>
#include <queue>
#include <cstdio>

class BufferClass;


class MainClass {
private:
	const std::string RECORD_FILE_NAME = "records.txt";
	const std::string RESULT_FILE_NAME = "recordsRunOut.txt";
	const int MAX_LICENSE_PLATE_LENGHT = 8;

	bool PRINT_FILE_AFTER_MODIFICATION = false;

	int NUMBER_OF_RECORDS_N;
	int BUFFER_SIZE_b = 10;
	int NUMBER_OF_BUFFERS_n = 11;
	int DISK_OPERATIONS = 0;
	int DISK_READ = 0;
	int DISK_WRITE = 0;
	int MERGE_PHASES = 0;
	BufferClass* buffers;
	std::vector<int> runOffsets;
	std::vector<int> nextRunOffsets;

	std::vector<std::string> debugString;
	std::priority_queue<
		std::pair<std::string, int>,
		std::vector<std::pair<std::string, int>>,
		std::greater<std::pair<std::string, int>>
	> minQue;
public:
	MainClass();
	~MainClass();
	int countRecords();
	std::string generateString(size_t length, bool isRegText);
	bool isOnlyLetters(const std::string& s);
	bool isOnlyLettersAndNumbers(const std::string& s);
	void addRecord();
	void generateRecords(int records_number);
	void printFile();
	bool readToBuffers(int* offset);
	void sortBuffers(int* offset);
	void firstStageLargeBuffersSort();
	void mergeRuns();
	void secondStageLargeBuffersSort();
	void experiment();
	int main();

	void addDiskRead();
	void addDiskWrite();
	void addDiskOperation();
	void addMergePhase();
	void prepNextSort();
	std::string getRecordFileName();
	std::string getResultFileName();
};