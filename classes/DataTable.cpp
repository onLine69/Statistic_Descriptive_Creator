#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "DataTable.hpp"

using std::cout;
using std::endl;
using std::string;
using std::left;
using std::setw;

double *arrPtr;
int arrSize;
int numDecimalPlace;
double range;
int intervals;
double classWidth;

double *data;
double *intervalPtr;
int *freqPtr;
double *marksPtr;
double *boundariesPtr;
int *lcfPrt;

/*
    Constructor.
*/
DataTable::DataTable(double arrData[], int numDecimalPlace, int arrSize){
    this->arrPtr = arrData;
    this->arrSize = arrSize;
    this->numDecimalPlace = numDecimalPlace;

    sorter(arrData, arrSize);       //sort the data

    this->range = *(this->arrPtr + arrSize - 1) - *(this->arrPtr);
    this->intervals = ceil(sqrt(this->arrSize));
    this->classWidth = round((this->range / this->intervals)*pow(10,this->numDecimalPlace)) / pow(10,this->numDecimalPlace);

    if (*(arrPtr + arrSize - 1) >= *arrPtr + (classWidth * intervals)) intervals++;

    data = arrPtr;
    intervalPtr = classInterval();
    freqPtr = frequencies();
    marksPtr = classMarks();
    boundariesPtr = classBoundaries();
    lcfPrt = lessCumulativeFrequencies();
}

/*
    Destructor also deallocates the memory used.
*/
DataTable::~DataTable(){
    cout << "Data Table Deleted..." << endl;
}

/*
    Getters
*/
double* DataTable::getArrPtr(){
    return this->arrPtr;
}

int DataTable::getArrSize(){
    return this->arrSize;
}

int DataTable::getIntervals(){
    return this->intervals;
}

double DataTable::getClassWidth(){
    return this->classWidth;
}

double* DataTable::getBoundaries(){
    return this->boundariesPtr;
}

double* DataTable::getClassMarks(){
    return this->marksPtr;
}

int* DataTable::getFrequencies(){
    return this->freqPtr;
}

int* DataTable::getLessFreq(){
    return this->lcfPrt;
}

/*
    Display the FDT Table (might not align, depends on the input data).
*/
void DataTable::displayTable(){
    cout << "Frequency Distribution Table..." << endl;
    cout << "\tClass Interval\t | \tFrequency\t | \tClass Marks\t | \tClass Boundaries\t | \tLess Cum. Freq.\t" << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------------" << endl;

    for (int row = 0; row < this->intervals; row++) {
        std::stringstream upInterval;
        upInterval << *(intervalPtr + row*2);
        std::stringstream lowInterval;
        lowInterval << *(intervalPtr + row*2 + 1);
        string interval = upInterval.str() + " - " + lowInterval.str();

        std::stringstream upBoundary;
        upBoundary << *(boundariesPtr + row*2);
        std::stringstream lowBoundary;
        lowBoundary << *(boundariesPtr + row*2 + 1);
        string boundary = upBoundary.str() + " - " + lowBoundary.str();
        
        cout << "\t " << left << setw(14) << interval;

        cout << "\t | \t " << left << setw(9) << *(freqPtr + row);

        cout << "\t | \t " << left << setw(11) << *(marksPtr + row);

        cout << "\t | \t " << left << setw(16) << boundary;

        cout << "\t | \t " << left << setw(15) << *(lcfPrt + row);

        cout << endl;
    }

    cout << "Table End...." << endl << endl;
}

/*
    Display Data sorted.
*/
void DataTable::displayData(){
    cout << "Data (Sorted): [";
    for (int data = 0; data < this->arrSize; data++){
        cout << *(arrPtr + data) << ", ";
    }
    cout << "\b\b]" << endl << endl;
}

/*
    Calculate the frequency for each interval passed
*/
int DataTable::freqCount (double dataArr[], int sizeArr, double lowBound, double upBound){
    int frequency = 0;

    for (int data = 0; data < sizeArr; data++){
        if (dataArr[data] < lowBound || dataArr[data] > upBound){
            return frequency;
        }

        frequency ++;
    }

    return frequency;
}

//using bubble sort
void DataTable::sorter(double dataArr[], int sizeArr){
    int i, j;
    bool swapped;
    for (i = 0; i < sizeArr - 1; i++){
        swapped = false;
        for (j = 0; j < sizeArr - i - 1; j++){
            if (dataArr[j] > dataArr[j + 1]){
                std::swap(dataArr[j], dataArr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}

/*
    Calculate and list every interval for each class.
*/
double* DataTable::classInterval(){
    //Class Intervals
    double *intervalBounds = new double[2*(this->intervals)];

    double intervalGap = pow(10, -this->numDecimalPlace);
    //for the lower bounds
    intervalBounds[0] = *(this->arrPtr);
    for (int dataLoc = 2; dataLoc < 2*this->intervals - 1; dataLoc += 2){
        intervalBounds[dataLoc] = intervalBounds[dataLoc - 2] + this->classWidth;
    }

    //for the upper bounds
    intervalBounds[1] = intervalBounds[0] + this->classWidth - intervalGap;
    for (int dataLoc = 3; dataLoc < 2*this->intervals; dataLoc += 2){
        intervalBounds[dataLoc] = intervalBounds[dataLoc - 2] + this->classWidth;
    }

    return intervalBounds;
}

/*
    Calculate and list every frequency for each class.
*/
int* DataTable::frequencies(){
    //Frequency
    int *frequencyList = new int[intervals];

    //copying of the data
    double forFreqData[arrSize];
    for (int dataCount = 0; dataCount < arrSize; dataCount++){
        forFreqData[dataCount] = *(arrPtr + dataCount);
    }
    int newArrSize = arrSize;

    for (int intervClass = 0; intervClass < intervals; intervClass++){
        frequencyList[intervClass] = freqCount(forFreqData, newArrSize, *(classInterval() + intervClass*2), *(classInterval() + intervClass*2 + 1));

        int oldArrSize = newArrSize;
        double oldFreqArr[oldArrSize];
        for (int dataCount = 0; dataCount < arrSize; dataCount++){
            oldFreqArr[dataCount] = forFreqData[dataCount];     //transfer data
        }


        newArrSize -= frequencyList[intervClass];
        forFreqData[newArrSize];
        for (int dataCount = frequencyList[intervClass], i = 0; dataCount < oldArrSize; dataCount++){
            forFreqData[i] = oldFreqArr[dataCount];
            i++;
        }
    }

    return frequencyList;
}

/*
    Calculate and list every class marks for each class.
*/
double* DataTable::classMarks(){
    //Class Marks
    double *classMarks = new double[this->intervals];
    for (int dataCount = 0; dataCount < this->intervals; dataCount++){
        classMarks[dataCount] = ((*(classInterval() + dataCount*2) + *(classInterval() + dataCount*2 + 1))/2);
    }

    return classMarks;
}

/*
    Calculate and list every class boundaries for each class.
*/
double* DataTable::classBoundaries(){
    //Class Boundaries
    double *classBoundaries = new double[this->intervals*2];
    double correctionFactor = pow(10, -this->numDecimalPlace) * 0.5;
    for (int bound = 0; bound < intervals*2; bound++){
        int loc = pow(-1, bound - 1);   //-1 if lowBound && 1 if upBound
        classBoundaries[bound] = *(classInterval() + bound) + (loc * correctionFactor);
    }

    return classBoundaries;
}

/*
    Calculate and list every less than cumulative frequency for each class.
*/
int* DataTable::lessCumulativeFrequencies(){
    //< Cumulative Frequencies
    int *lessCumFreq = new int[intervals];
    lessCumFreq[0] = *frequencies();
    for (int lessCumCount = 1; lessCumCount < intervals; lessCumCount++){
        lessCumFreq[lessCumCount] = *(frequencies() + lessCumCount) + lessCumFreq[lessCumCount - 1];
    }

    return lessCumFreq;
}

