#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <regex>
#include "MainClass.h"
#include "BufferClass.h"


MainClass::MainClass() {
	NUMBER_OF_RECORDS_N=countRecords();
    buffers = new BufferClass[NUMBER_OF_BUFFERS_n];
    for (int i = 0; i < NUMBER_OF_BUFFERS_n; ++i)
        buffers[i].buffInit(BUFFER_SIZE_b, this,i);
}

MainClass::~MainClass() {
    delete[] buffers;
}

int MainClass::countRecords() {
    std::ifstream inputFile(RECORD_FILE_NAME);
    int lineCount = 0;
    if (inputFile.is_open()) {
        std::string line;


        while (std::getline(inputFile, line)) {
            lineCount++;
        }

        inputFile.close();
    }
    return lineCount;
}

std::string MainClass::generateString(size_t length, bool isRegText) {
    static const std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const std::string numbers = "1234567890";

    std::string result;
    result.reserve(length);
    if (isRegText) {
        for (size_t i = 0; i < length; ++i) {
            result += letters[rand() % letters.length()];
        }
    }
    else
    {
        for (size_t i = 0; i < length; ++i) {
            if (rand() % 5 == 1)
                result += letters[rand() % letters.length()];
            else
                result += numbers[rand() % numbers.length()];
        }
    }
    return result;
}

bool MainClass::isOnlyLetters(const std::string& s) {
    static const std::regex pattern("^[A-Z]+$");
    return std::regex_match(s, pattern);
}

bool MainClass::isOnlyLettersAndNumbers(const std::string& s) {
    static const std::regex pattern("^[A-Z0-9]+$");
    return std::regex_match(s, pattern);
}

void MainClass::addRecord() {
    std::ofstream file(RECORD_FILE_NAME, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << RECORD_FILE_NAME << std::endl;
        return;
    }
    std::string wojPart, indPart;
    std::cout << "Type e to exit\n";
    while (true)
    {
        std::cout << "Write first part of license plate\n";
        std::cin >> wojPart;
        if (wojPart == "e" || wojPart == "E")
            return;
        else if (wojPart.length() < 2 || wojPart.length() > 3 || !isOnlyLetters(wojPart)) {
            std::cout << "First part of license plate must be 2 or 3 letters long, and should contain only capital letters\n";
            continue;
        }
        std::cout << "Write second part of license plate\n";
        std::cin >> indPart;
        if (indPart == "e" || wojPart == "E")
            return;
        else if (indPart.length() != 5 || !isOnlyLettersAndNumbers(indPart)) {
            std::cout << "It must be 5 characters long, and should contain only capital letters and numbers\n";
            continue;
        }
        if (wojPart.length() == 2) {
			indPart += " "; // padd
        }
        NUMBER_OF_RECORDS_N += 1;
        file << wojPart + indPart << "\n";
    }
    file.close();
}

void MainClass::generateRecords(int records_number) {
	NUMBER_OF_RECORDS_N += records_number;
    std::ofstream file(RECORD_FILE_NAME, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << RECORD_FILE_NAME << std::endl;
        return;
    }

    int randPartLength;
    std::string genRecord;
    for (int i = 0; i < records_number; ++i) {
        randPartLength = rand() % 2 + 2;
        genRecord = generateString(randPartLength, true) + generateString(5, false);
        if (randPartLength == 2) {
            genRecord += " "; // padd
        }
        genRecord += "\n";
        file << genRecord;
    }
    file.close();
    
}

void MainClass::printFile() {
    std::ifstream file(RECORD_FILE_NAME);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << RECORD_FILE_NAME << std::endl;
        return;
    }
    std::string line;
    std::cout << "Records in file:\n";
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
    file.close();
}


int MainClass::main()
{
    int choice;
    bool exitCondition = false;
    int recordsNumber;
    while (!exitCondition)
    {
        std::cout << "Choose action.\n 1. To generate random records\n 2. Write your own records.\n 3. Delete records in a file.\n "
            << "4. Print all records in a file.\n 5. Sort all records using large buffers.\n 6. Experiment.\n 7. Change print flag.\n";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
        {
            
            std::cout << "How many records do you want to generate?\n";
            std::cin >> recordsNumber;
            generateRecords(recordsNumber);
            printFile();
            break;
        }
        case 2:
        {
            addRecord();
            printFile();
            break;
        }
        case 3:
        {
            std::ofstream file(RECORD_FILE_NAME, std::ios::trunc);
            NUMBER_OF_RECORDS_N = 0;
            file.close();
            std::ofstream file2(RESULT_FILE_NAME, std::ios::trunc);
            file2.close();
            std::cout << "Records deleted.\n";
            break;
        }
        case 4:
        {
            printFile();
            break;
        }
        case 5:
        {
            firstStageLargeBuffersSort();
            //std::cout << "Large buffer sorting first stage completed.\n" <<
            //    "Number of disk operations: " << DISK_OPERATIONS << "\n" <<
           //     "Number of disk read: " << DISK_READ << "\n" <<
            //    "Number of disk write: " << DISK_WRITE << "\n" <<
            //    "Number of merge phases: " << MERGE_PHASES << "\n";
            if(PRINT_FILE_AFTER_MODIFICATION)
                printFile();
            secondStageLargeBuffersSort();
            std::cout << "Large buffer sorting second stage completed.\n" 
                "Number of records: " << NUMBER_OF_RECORDS_N << "\n" <<
                "Number of disk operations: " << DISK_OPERATIONS << "\n" <<
                "Number of disk read: " << DISK_READ << "\n" <<
                "Number of disk write: " << DISK_WRITE << "\n" <<
                "Number of merge phases: " << MERGE_PHASES << "\n";
            if (PRINT_FILE_AFTER_MODIFICATION)
                printFile();
            prepNextSort();
            break;
		}
        case 6:
        {
			experiment();
            break;
        }
        case 7:
        {
			PRINT_FILE_AFTER_MODIFICATION = !PRINT_FILE_AFTER_MODIFICATION;
			std::cout << "Print file after modification set to: " << PRINT_FILE_AFTER_MODIFICATION << "\n";
            break;
        }
        default:
        {
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

        }
    }
	return 0;
}

void MainClass::prepNextSort() {
    DISK_OPERATIONS = 0;
    DISK_READ = 0;
    DISK_WRITE = 0;
    MERGE_PHASES = 0;
    runOffsets.clear();
    nextRunOffsets.clear();
}

void MainClass::experiment() {
    std::ofstream file("exp.csv", std::ios::app);
    for (int recordsNumber=10; recordsNumber <= 1000; recordsNumber+=10) {
        std::ofstream file2(RECORD_FILE_NAME, std::ios::trunc);
        file2.close();
        std::ofstream file3(RESULT_FILE_NAME, std::ios::trunc);
        file3.close();
        NUMBER_OF_RECORDS_N = 0;
        prepNextSort();
        generateRecords(recordsNumber);
        firstStageLargeBuffersSort();
        secondStageLargeBuffersSort();
        file << DISK_OPERATIONS << "," << recordsNumber << "\n";
    }
    file.close();

}

std::string MainClass::getRecordFileName() {
    return RECORD_FILE_NAME;
}


bool MainClass::readToBuffers(int* offset) {
    std::fstream file(RECORD_FILE_NAME);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << RECORD_FILE_NAME << std::endl;
        return false;
    }

    file.seekp((*offset) * (MAX_LICENSE_PLATE_LENGHT + 2));// +2 for newline character

    for (int i = 0; i < NUMBER_OF_BUFFERS_n;i++) {
        if (!buffers[i].readBuffer(&file)) {
            file.close();
            return false;
        }
	}
    return true;
}

void MainClass::sortBuffers(int* offset) {
    std::vector<std::string> merged;
    for(int i=0, locOff = (*offset);i< NUMBER_OF_BUFFERS_n;i++){
        for(int j=0;j<BUFFER_SIZE_b && locOff < NUMBER_OF_RECORDS_N;j++, locOff++){
            merged.push_back(buffers[i].getBuffer()[j]);
        }
	}

    std::stable_sort(merged.begin(), merged.end());

    int index = 0;
    for (int i = 0, locOff = (*offset); i < NUMBER_OF_BUFFERS_n; i++) {
        for (int j = 0; j < BUFFER_SIZE_b && locOff < NUMBER_OF_RECORDS_N; j++, index++, locOff++) {
            buffers[i].getBuffer()[j]= merged[index];
        }
    }

	std::ofstream outputFile(RECORD_FILE_NAME, std::ios::in | std::ios::out);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << RECORD_FILE_NAME << std::endl;
        return;
    }

    outputFile.seekp((*offset) * (MAX_LICENSE_PLATE_LENGHT + 2));// +1 for newline character

    for (int i = 0; i < NUMBER_OF_BUFFERS_n; i++) {
       buffers[i].saveBufferToMainFile(&outputFile,offset, NUMBER_OF_RECORDS_N);
    }
}


void MainClass::mergeRuns() {
    std::string lowRecord;
    int indexOfBuffer;
	int startingRunOffsets;
    while (runOffsets.size() > 0) {
		startingRunOffsets = 0;
        if(nextRunOffsets.size()>0)
			startingRunOffsets = nextRunOffsets.back();
        buffers[0].setRunOffset(startingRunOffsets, runOffsets[0]);
        buffers[0].readRunToBuffer();
        lowRecord = buffers[0].getLowestRecord();
        minQue.push(std::make_pair(lowRecord, 0));
        for (int i = 1; i < runOffsets.size() && i < NUMBER_OF_BUFFERS_n - 1; i++) {
            buffers[i].setRunOffset(runOffsets[i - 1], runOffsets[i]);
            buffers[i].readRunToBuffer();
            lowRecord = buffers[i].getLowestRecord();
            minQue.push(std::make_pair(lowRecord, i));
        }
        int resultBufferIndex;
        if (runOffsets.size() < NUMBER_OF_BUFFERS_n) {
            buffers[runOffsets.size()].clearBuffer();
            resultBufferIndex = runOffsets.size();

        }
        else {
            buffers[NUMBER_OF_BUFFERS_n - 1].clearBuffer();
            resultBufferIndex = NUMBER_OF_BUFFERS_n - 1;
            
        }
        nextRunOffsets.push_back(runOffsets[resultBufferIndex-1]);

        while (!minQue.empty()) {
            lowRecord = minQue.top().first;
            indexOfBuffer = minQue.top().second;
            minQue.pop();
            buffers[resultBufferIndex].putRecordToBuffer(lowRecord);
			debugString.push_back(lowRecord);
            lowRecord = buffers[indexOfBuffer].getLowestRecord();
            if (lowRecord != "") {
                minQue.push(std::make_pair(lowRecord, indexOfBuffer));
            }
        }
		buffers[resultBufferIndex].writeRunToFile();
		debugString.clear();
        for (int i = 0; i < resultBufferIndex; i++) {
            runOffsets.erase(runOffsets.begin());
        }
    }
}

void MainClass::secondStageLargeBuffersSort() {
    if(runOffsets.size()<=1)
		return;
    do
    {
        mergeRuns();
        addMergePhase();
		runOffsets = nextRunOffsets;
		nextRunOffsets.clear();

        int result = std::remove("records.txt");
        if (result != 0) {
            perror("Del err");
        }

        result = std::rename("recordsRunOut.txt", "records.txt");
        if (result != 0) {
            perror("Renaming err");
        }

    } while (runOffsets.size() > 1);
}

void MainClass::firstStageLargeBuffersSort() {
	int offset = 0;
    while (offset < NUMBER_OF_RECORDS_N) {
        readToBuffers(&offset);
        sortBuffers(&offset);

		runOffsets.push_back(offset);
    }
}

std::string MainClass::getResultFileName() {
    return RESULT_FILE_NAME;
}


void MainClass::addDiskOperation() {
    DISK_OPERATIONS += 1;
}

void MainClass::addDiskRead() {
    DISK_READ += 1;
}

void MainClass::addDiskWrite() {
    DISK_WRITE += 1;
}

void MainClass::addMergePhase() {
    MERGE_PHASES += 1;
}