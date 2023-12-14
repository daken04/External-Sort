#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <iomanip>
using namespace std;

const int MAX_MEMORY = 4;
const int WAYS = 5;

class MinHeap
{
public:
    MinHeap() {}

    void push(pair<int, int> element)
    {
        heap.push_back(element);
        push_heap(heap.begin(), heap.end(), greater<pair<int, int>>());
    }

    pair<int, int> pop()
    {
        pair<int, int> top = heap.front();
        pop_heap(heap.begin(), heap.end(), greater<pair<int, int>>());
        heap.pop_back();
        return top;
    }

    bool empty() const
    {
        return heap.empty();
    }

private:
    vector<pair<int, int>> heap;
};

void mergeSortedChunks(const vector<string> &tempFiles, const string &outputFilePath)
{
    vector<ifstream> tempFileStreams;
    for (const auto &tempFile : tempFiles)
    {
        tempFileStreams.emplace_back(tempFile);
    }

    ofstream outputFile(outputFilePath);

    MinHeap minHeap;
    for (int i = 0; i < tempFiles.size(); ++i)
    {
        int num;
        if (tempFileStreams[i] >> num)
        {
            minHeap.push(make_pair(num, i));
        }
    }

    while (!minHeap.empty())
    {
        pair<int, int> top = minHeap.pop();
        int smallest = top.first;
        int fileIndex = top.second;

        outputFile << smallest << '\n';

        int nextNum;
        if (tempFileStreams[fileIndex] >> nextNum)
        {
            minHeap.push(make_pair(nextNum, fileIndex));
        }
    }
    for (const auto &tempFile : tempFiles)
    {
        remove(tempFile.c_str());
    }
}

void externalMergeSort(const string &inputFilePath, const string &outputFilePath)
{
    const char *inp_str = inputFilePath.c_str();
    FILE *input = fopen(inp_str, "r");
    vector<int> chunk;
    vector<string> inputFiles;
    int chunkCount = 0;
    int totalIntegers = 0;
    int value;
    while (fscanf(input, "%d", &value) == 1)
    {
        totalIntegers++;

        chunk.push_back(value);

        if (chunk.size() == MAX_MEMORY)
        {
            sort(chunk.begin(), chunk.end());
            string tempInputFileName = "temp_input_" + to_string(chunkCount) + ".txt";
            ofstream tempInputFile(tempInputFileName);
            if (!tempInputFile.is_open())
            {
                cerr << "Error opening temporary input file " << tempInputFileName << ".\n";
                return;
            }
            for (const int &val : chunk)
            {
                tempInputFile << val << '\n';
            }
            tempInputFile.close();
            inputFiles.push_back(tempInputFileName);
            chunk.clear();
            chunkCount++;
        }
    }

    if (!chunk.empty())
    {
        sort(chunk.begin(), chunk.end(), less<int>());
        string tempInputFileName = "temp_input_" + to_string(chunkCount) + ".txt";
        ofstream tempInputFile(tempInputFileName);
        if (!tempInputFile.is_open())
        {
            cerr << "Error opening temporary input file " << tempInputFileName << ".\n";
            return;
        }
        for (const int &val : chunk)
        {
            tempInputFile << val << '\n';
        }
        tempInputFile.close();
        inputFiles.push_back(tempInputFileName);
    }

    int count = inputFiles.size();

    int j = 0;
    while (inputFiles.size() > 1)
    {
        vector<string> outputFiles;

        for (size_t i = 0; i < inputFiles.size(); i += WAYS)
        {
            vector<string> mergeFiles;
            for (int k = 0; k < WAYS && (i + k) < inputFiles.size(); ++k)
            {
                mergeFiles.push_back(inputFiles[i + k]);
            }

            string tempOutputFileName = "temp_output_" + to_string(j) + ".txt";
            j++;
            mergeSortedChunks(mergeFiles, tempOutputFileName);

            for (const auto &file : mergeFiles)
            {
                remove(file.c_str());
            }

            outputFiles.push_back(tempOutputFileName);
        }

        inputFiles = outputFiles;
        count += inputFiles.size();
    }

    if (!inputFiles.empty())
    {
        string output_file = "";
        int size = outputFilePath.size();
        int i = size - 1;
        while (i >= 0 && outputFilePath[i] != '/')
        {
            output_file = outputFilePath[i] + output_file;
            i--;
        }
        if (rename(inputFiles[0].c_str(), output_file.c_str()) != 0)
        {
            cerr << "Error renaming the final output file.\n";
            return;
        }
    }

    cout << "Number of integers in a temporary file: " << MAX_MEMORY << '\n';
    cout << "Number of temporary files created: " << chunkCount + 1 << '\n';
    cout << "Total time taken by the program: " << fixed << setprecision(2) << (double)clock() / CLOCKS_PER_SEC << " seconds\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];

    clock_t start = clock();

    externalMergeSort(inputFilePath, outputFilePath);

    clock_t end = clock();
    double timeTaken = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    return 0;
}
