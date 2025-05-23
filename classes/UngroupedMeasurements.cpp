#include <iostream>
#include <cmath>
#include "DataTable.hpp"
#include "UngroupedMeasurements.hpp"

using std::cout;
using std::endl;
using std::string;

    double *dataPtr;    //data to be measured
    int dataSizeUngrouped;  //size of the data

    /*
        Class Constructor.
    */
    UngroupedMeasurements::UngroupedMeasurements(DataTable* data){
        this->dataPtr = data->getArrPtr();
        this->dataSizeUngrouped = data->getArrSize();
    }

    /*
        Class Destructor.
    */
    UngroupedMeasurements::~UngroupedMeasurements(){
        cout << "Ungrouped Measurements Deleted..." << endl;
    }

    /*
        Display the measurements based on the data.

        For parameters:
            char types = 'p' for Population and 's' for Sample
            int percentile = from 1 - 100 and 0 if not measured.
            int decile = from 1 - 10 and 0 if not measured.
            int quartile = from 1 - 4 and 0 if not measured.
    */
    void UngroupedMeasurements::displayMeasurements(char type, int percentile, int decile, int quartile) {
        cout << "Ungrouped Data Measurements: " << endl;
        cout << "\tMean = " << ungroupedMean() << endl;
        cout << "\tMedian = " << ungroupedMedian() << endl;
        
        double *modePtr = ungroupedMode();

        //if there's a mode
        if ((int)*modePtr > 0){
            cout << "\tMode = {";
            for (int i = 1; i < (int)*modePtr; i++){
                cout << *(modePtr + i) << ", ";
            }
            cout << "\b\b}" << endl;
        }
        
        //display values if it is from population or from sample
        if (type == 'p'){
            cout << "\tPop. Variance = " << ungroupedPVariance() << endl;
            cout << "\tPop. Std. Dev. = " << pStandardDev() << endl;
            cout << "\tPop. CV = " << pCoefficientVariation() << endl;
        }
        else if (type == 's'){
            cout << "\tSample Variance = " << ungroupedSVariance() << endl;
            cout << "\tSample Std. dev. = " << sStandardDev() << endl;
            cout << "\tSample CV = " << sCoefficientVariation() << endl;
        }

        string sf;  //suffix like 'st', 'nd' etc idk

        //for the fractiles
        //skip if the arg is 0
        if (percentile > 0) {
            switch (percentile%10) {
            case 1:
                if (percentile/10 == 1) sf = "th";
                else sf = "st";
                break;
            case 2:
                if (percentile/10 == 1) sf = "th";
                else sf = "nd";
                break;
            case 3:
                if (percentile/10 == 1) sf = "th";
                else sf = "rd";
                break;
            default:
                sf = "th";
                break;
            }
            cout << "\t" << percentile << sf << " Percentile = " << getPercentile(percentile) << endl;
        }
        if (decile > 0) {
            switch (decile%10) {
            case 1:
                sf = "st";
                break;
            case 2:
                sf = "nd";
                break;
            case 3:
                sf = "rd";
                break;
            default:
                sf = "th";
                break;
            }
            cout << "\t" << decile << sf << " Decile = " << getDecile(decile) << endl;
        } 
        if (quartile > 0) {
            switch (quartile%10) {
            case 1:
                sf = "st";
                break;
            case 2:
                sf = "nd";
                break;
            case 3:
                sf = "rd";
                break;
            default:
                sf = "th";
                break;
            }

            cout << "\t" << quartile << sf << " Quartile = " << getQuartile(quartile) << endl;
        }   
        cout << endl;
    }

    /*
        Calculates the Ungroup Mean based on the formula.
    */
    double UngroupedMeasurements::ungroupedMean() {
        double sum = 0;

        //sum of all the data
        for (int data = 0; data < this->dataSizeUngrouped; data++)
            sum += *(this->dataPtr + data);

        //divide by the number of data
        double mean = sum / this->dataSizeUngrouped;

        return mean;
    }

    /*
        Calculates the Ungroup Median based on the formula.
    */
    double UngroupedMeasurements::ungroupedMedian() {
        double median;

        if (dataSizeUngrouped % 2 == 0) {
            //if the number of data is even
            //get the average (or mean) of the two middle datus
            median = (*(this->dataPtr + (this->dataSizeUngrouped / 2) - 1) + *(this->dataPtr + (this->dataSizeUngrouped / 2))) / 2;
        } else {
            //if the number of data is odd
            //get the middle data
            median = *(this->dataPtr + (int)std::ceil(this->dataSizeUngrouped / 2));
        }

        return median;
    }

    /*
        Get the data that have the highest frequency (can have multiple data or none).
    */
    double* UngroupedMeasurements::ungroupedMode() {
        //count how many unique data are there
        int countUnique = 1;
        for (int i = 0; i < this->dataSizeUngrouped - 1; i++){
            if (*(this->dataPtr + i) != *(this->dataPtr + i + 1)){
                countUnique++;
            }
        }

        //create a 2d array that will hold the pair of the data and how frequent they occur
        double groups[countUnique][2];
        int base = 0;
        int place = 0;
        int counter = 0;
        while (place < this->dataSizeUngrouped){
            if (*(this->dataPtr + base) == *(this->dataPtr + place)) place++;
            else {
                groups[counter][0] = *(this->dataPtr + base);
                groups[counter][1] = place - base;
                base = place;
                counter++;
            }

            //for the last data
            if ((place == this->dataSizeUngrouped - 1) && (*(this->dataPtr + place) != *(this->dataPtr + place -1))){
                groups[counter][0] = *(this->dataPtr + place);
                groups[counter][1] = 1;
            }
            if ((place == this->dataSizeUngrouped - 1) && (*(this->dataPtr + place) == *(this->dataPtr + place -1))){
                groups[countUnique - 1][0] = *(this->dataPtr + base);
                groups[countUnique - 1][1] += place - base + 1;
            }
        }

        //finding the max amount of repetition
        int maxCount = groups[0][1];
        for (int i = 1; i < countUnique; i++) {
            if (groups[i][1] > maxCount) maxCount = groups[i][1];
        }

        //counting how many max there is
        int maxMany = 0;
        for (int i = 0; i < countUnique; i++) {
            if (maxCount == groups[i][1]) maxMany++;
        }

        double* modes = new double[maxMany + 1];    //array of all the modes

        //if all the data occur with the same frequency
        if (maxMany == 0 || maxMany == countUnique) {
            cout << "No Mode" << endl;
            modes[0] = 0;
            return modes;
        }

        //set the first element to the number of modes + the amount
        modes[0] = (double) maxMany + 1;

        //insert the modes in the array
        int modeCount = 1;
        for (int i = 0; i < countUnique; i++) {
            if (maxCount == groups[i][1]){

                modes[modeCount] = groups[i][0];
                modeCount++;
            }
        }

        return modes;
    }

    /*
        Calculates the Sample Variance based on the formula.
    */
    double UngroupedMeasurements::ungroupedSVariance() {
        double sumSquares = 0;

        //calculate the sum of all the data squared
        for (int data = 0; data < dataSizeUngrouped; data++) {
            sumSquares += pow(*(this->dataPtr + data), 2);
        }

        //formula for the sample variance
        double variance = (sumSquares - (this->dataSizeUngrouped * (pow(ungroupedMean(), 2)))) / (this->dataSizeUngrouped - 1);

        return variance;
    }

    /*
        Calculates the Population Variance based on the formula.
    */
    double UngroupedMeasurements::ungroupedPVariance() {
        double sumSquares = 0;

        //calculate the sum of all the data squared
        for (int data = 0; data < dataSizeUngrouped; data++) {
            sumSquares += pow(*(this->dataPtr + data), 2);
        }

        //formula for the sample variance
        double variance = (sumSquares - (this->dataSizeUngrouped * (pow(ungroupedMean(), 2)))) / (this->dataSizeUngrouped);

        return variance;
    }

    /*
        Calculates the Sample Standard Deviation based on the formula.
    */
    double UngroupedMeasurements::sStandardDev() {
        return sqrt(ungroupedSVariance());  //sqrt of the sample variance
    }

    /*
        Calculates the Population Standard Deviation based on the formula.
    */
    double UngroupedMeasurements::pStandardDev() {
        return sqrt(ungroupedPVariance());  //sqrt of the population variance
    }

    /*
        Calculates the Sample Coefficient of Variation based on the formula.
    */
    double UngroupedMeasurements::sCoefficientVariation() {
        return (sStandardDev() / ungroupedMean()) * 100;    //std. dev. / mean
    }

    /*
        Calculates the Population Coefficient of Variation based on the formula.
    */
    double UngroupedMeasurements::pCoefficientVariation() {
        return (pStandardDev() / ungroupedMean()) * 100;    //std. dev. / mean
    }

    /*
        Calculates the Ungrouped Percentile based on the formula.
    */
    double UngroupedMeasurements::getPercentile(int percentile) {
        double location = percentile*(this->dataSizeUngrouped)/100.0;   //(mn/100)th location 
        double percentileData;

        if (location/(int)location == 1){
            //if (mn/100)th is an integer
            //get average (or mean) the (mn/100)th data and the (mn/100 + 1)th data
            percentileData = (*(dataPtr + (int)floor(location) - 1) + *(dataPtr + (int)floor(location)))/2;
        } else {
            //else get the (mn/100)th data
            percentileData = *(dataPtr + (int)ceil(location) - 1);
        }
        return percentileData;
    }

    /*
        Calculates the Ungrouped Decile based on the formula.
    */
    double UngroupedMeasurements::getDecile(int decile){
        double location = decile*(this->dataSizeUngrouped)/10.0;    //(mn/10)th location 
        double decileData;
        
        if (location/(int)location == 1){
            //if (mn/10)th is an integer
            //get average (or mean) the (mn/10)th data and the (mn/10 + 1)th data
            decileData = (*(dataPtr + (int)floor(location) - 1) + *(dataPtr + (int)floor(location)))/2;
        } else {
            //else get the (mn/10)th data
            decileData = *(dataPtr + (int)ceil(location) - 1);
        }
        return decileData;
    }

    /*
        Calculates the Ungrouped Quartile based on the formula.
    */
    double UngroupedMeasurements::getQuartile(int quartile){
        double location = quartile*(this->dataSizeUngrouped)/4.0;   //(mn/4)th location 
        double quartileData;

        if (location/(int)location == 1){
            //if (mn/4)th is an integer
            //get average (or mean) the (mn/4)th data and the (mn/4 + 1)th data
            quartileData = (*(dataPtr + (int)floor(location) - 1) + *(dataPtr + (int)floor(location)))/2;
        } else {
            //else get the (mn/4)th data
            quartileData = *(dataPtr + (int)ceil(location) - 1);
        }
        return quartileData;
    }