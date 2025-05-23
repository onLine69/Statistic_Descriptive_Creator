#include <iostream>
#include <cmath>
#include "DataTable.hpp"
#include "GroupedMeasurements.hpp"

using std::cout;
using std::endl;
using std::string;

    int dataSize;   //size of the arg arr
    int intervalsGroup; //number of class in the FDT
    double classWidthGroup; //class width of each group
    double *boundaries; //class boundaries
    double *classMarks; //class marks
    int *frequencies;   //class frequencies
    int *lessFreq;  //less cum. freq.

    /*
        Class Constructor.
    */
    GroupedMeasurements::GroupedMeasurements(DataTable* data){
        this->dataSize = data->getArrSize();
        this->intervalsGroup = data->getIntervals();
        this->classWidthGroup = data->getClassWidth();
        this->boundaries = data->getBoundaries();
        this->classMarks = data->getClassMarks();
        this->frequencies = data->getFrequencies();
        this->lessFreq = data->getLessFreq();
    }

    /*
        Class Destructor.
    */
    GroupedMeasurements::~GroupedMeasurements(){
        cout << "Grouped Measurements Deleted..." << endl;
    }

    /*
        Display the measurements based on the FDT.

        For parameters:
            int percentile = from 1 - 100 and 0 if not measured.
            int decile = from 1 - 10 and 0 if not measured.
            int quartile = from 1 - 4 and 0 if not measured.
    */
    void GroupedMeasurements::displayMeasurements(int percentile, int decile, int quartile){
        cout << "Grouped Data Measurements: " << endl;
        cout << "\tGroup Mean: " << groupedMean() << endl;
        cout << "\tGroup Median: " << groupedMedian() << endl;
        cout << "\tGroup Mode: " << groupedMode() << endl;
        cout << "\tGroup Variance: " << groupedVariance() << endl;
        cout << "\tGroup Std. Dev.: " << groupedStdDev() << endl;
        cout << "\tCV: " << coeffVar() << endl;

        string sf;  //suffix like 'st', 'nd' etc idk

        //for the fractiles
        //skip if the arg is 0
        if (percentile > 0 && percentile <= 100) {
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
            cout << "\t" << percentile << sf << " Percentile = " << groupedPercentile(percentile) << endl;
        }
        if (decile > 0 && decile <= 10) {
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
            cout << "\t" << decile << sf << " Decile = " << groupedDecile(decile) << endl;
        }
        if (quartile > 0 && quartile <= 4) {
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

            cout << "\t" << quartile << sf << " Quartile = " << groupedQuartile(quartile) << endl;
        }
        cout << endl;
    }

    /*
        Calculates the Group Mean based on the formula.
    */
    double GroupedMeasurements::groupedMean(){
        double sum = 0;

        //add the products of the class frequency and class marks
        for (int place = 0; place < this->intervalsGroup; place++){
            sum += *(this->frequencies + place) * *(this->classMarks + place);
        }
        
        double mean = sum / this->dataSize; //divide by the total number of data

        return mean;
    }

    /*
        Calculates the Group Median based on the formula.
    */
    double GroupedMeasurements::groupedMedian(){
        double medLoc = this->dataSize / 2.0;  //calculate the location of the median
        int classLoc;
        
        //identify where the median is located based on the less cum. freq.
        for (int count = 0; count < this->intervalsGroup; count++){
            if (*(lessFreq + count) >= medLoc){
                classLoc = count;
                break;
            }
        }

        double lowerBound = *(this->boundaries + (classLoc*2)); //L_m
        int preLessFreq = *(this->lessFreq + classLoc - 1); //F_(m-1)
        int medFreq = *(this->frequencies + classLoc);  //f_m

        if (classLoc == 0){
            preLessFreq = 0;    //if the median class is in the first class
        }

        //formula for group median
        double median = lowerBound + (this->classWidthGroup*(medLoc - preLessFreq) / medFreq);

        return median;
    }

    /*
        Calculates the Group Mode based on the formula.
    */
    double GroupedMeasurements::groupedMode(){
        int modeLoc = *(this->frequencies); //set the mode class to the first class
        int classLoc;

        //identify what class is the modal class
        for (int count = 0; count < this->intervalsGroup; count++){
            if (*(this->frequencies + count) >= modeLoc){
                classLoc = count;
                modeLoc = *(this->frequencies + count);
            }
        }

        double lowerBound = *(this->boundaries + (classLoc*2)); //L_m
        int modeFreq = *(this->frequencies + classLoc); //f_m
        int beforeFreq = *(this->frequencies + classLoc - 1);   //f_1
        int afterFreq = *(this->frequencies + classLoc + 1);    //f_2

        if (classLoc == 0){
            beforeFreq = 0; //if the modal class is at the first class
        }
        if (classLoc == this->intervalsGroup){
            afterFreq = 0;  //if the modal class is at the last class
        }

        //formula for the modal class
        double mode = lowerBound + (this->classWidthGroup*(modeFreq - beforeFreq)/(2*modeFreq - beforeFreq - afterFreq));
        return mode;

    }

    /*
        Calculates the Group Variance based on the formula.
    */
    double GroupedMeasurements::groupedVariance(){
        double sumSquares = 0;  
        double sum = 0;

        //calculate the sum of the frequencies multiplied by class width square
        for (int count = 0; count < this->intervalsGroup; count++){
            sumSquares += *(this->frequencies + count) * pow(*(this->classMarks + count),2);
        }

        //calculate the sum of the frequencies multiplied by class width
        for (int count = 0; count < this->intervalsGroup; count++){
            sum += *(this->frequencies + count) * *(this->classMarks + count);
        }

        //formula for the group variance
        double variance = ((this->dataSize*sumSquares) - pow(sum,2))/(this->dataSize * (this->dataSize - 1));

        return variance;
    }

    /*
        Calculates the Group Standard Deviation based on the formula.
    */
    double GroupedMeasurements::groupedStdDev(){
        return sqrt(groupedVariance()); //sqrt of the group variance
    }

    /*
        Calculates the Group Coefficient of Variation based on the formula.
    */
    double GroupedMeasurements::coeffVar(){
        return groupedStdDev()/groupedMean() * 100;    //std. dev. / mean
    }

    /*
        Calculates the Group Percentile based on the formula.
    */
    double GroupedMeasurements::groupedPercentile(int percentile){
        double percLoc = this->dataSize*(percentile) / 100.0;   //(mn/100)th location 
        int classLoc;

        //identify where the (mn/100)th is located based on the less cum. freq.
        for (int count = 0; count < this->intervalsGroup; count++){
            if (*(lessFreq + count) >= percLoc){
                classLoc = count;
                break;
            }
        }

        double lowerBound = *(this->boundaries + (classLoc*2)); //L_m
        int preLessFreq = *(this->lessFreq + classLoc - 1); //F_(m-1)
        int medFreq = *(this->frequencies + classLoc);  //f_1

        if (classLoc == 0){
            preLessFreq = 0;    //if the (mn/100)th class is in the first class
        }

        //formula for group percentile
        double percentileF = lowerBound + (this->classWidthGroup*(percLoc - preLessFreq) / medFreq);

        return percentileF;
    }

    /*
        Calculates the Group Decile based on the formula.
    */
    double GroupedMeasurements::groupedDecile(int decile){
        double deciLoc = this->dataSize*(decile) / 10.0;    //(mn/10)th location
        int classLoc;

        //identify where the (mn/10)th is located based on the less cum. freq.
        for (int count = 0; count < this->intervalsGroup; count++){
            if (*(lessFreq + count) >= deciLoc){
                classLoc = count;
                break;
            }
        }

        double lowerBound = *(this->boundaries + (classLoc*2)); //L_m
        int preLessFreq = *(this->lessFreq + classLoc - 1); //F_(m-1)
        int medFreq = *(this->frequencies + classLoc);  //f_1

        if (classLoc == 0){
            preLessFreq = 0;    //if the (mn/10)th class is in the first class
        }

        //formula for group decile
        double decileF = lowerBound + (this->classWidthGroup*(deciLoc - preLessFreq) / medFreq);

        return decileF;
    }

    /*
        Calculates the Group Quartile based on the formula.
    */
    double GroupedMeasurements::groupedQuartile(int quartile){
        double quarLoc = this->dataSize*(quartile) / 4.0;   //(mn/4)th location
        int classLoc;

        //identify where the (mn/4)th is located based on the less cum. freq.
        for (int count = 0; count < this->intervalsGroup; count++){
            if (*(lessFreq + count) >= quarLoc){
                classLoc = count;
                break;
            }
        }

        double lowerBound = *(this->boundaries + (classLoc*2)); //L_m
        int preLessFreq = *(this->lessFreq + classLoc - 1); //F_(m-1)
        int medFreq = *(this->frequencies + classLoc);  //f_1

        if (classLoc == 0){
            preLessFreq = 0;    //if the (mn/10)th class is in the first class
        }

        //formula for group quartile
        double quartileF = lowerBound + (this->classWidthGroup*(quarLoc - preLessFreq) / medFreq);

        return quartileF;
    }
