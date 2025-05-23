#include <iostream>
#include "classes/DataTable.hpp"
#include "classes/UngroupedMeasurements.hpp"
#include "classes/GroupedMeasurements.hpp"

using std::cout;
using std::endl;

int main(void){
    //Input Data here
    double arr[] = {18, 31, 37, 21, 43, 16, 48, 20, 18, 47, 28, 32, 49, 24, 12, 42, 36, 63, 30, 19, 15, 29, 26, 28, 23,
                        40, 39, 54, 23, 25, 10, 58, 54, 32, 29, 32, 22, 30, 26, 38, 47, 48, 56, 61, 28, 26, 40, 17, 36, 35};

    double arr2[] = {20,45,66,96,25,48,68,100,30,50,70,120,36,53,72,130,38,56,74,135,40,58,80,140,40,60,88,150,42,64,90,152};

    double firstQuiz[] = {11.9,12,13.1,13.3,13.4,13.5,14.8,14.8,15,15.2,15.4,15.8,15.8,15.9,16.1,16.2,16.2,16.3,16.7,16.9,17.1,17.1,17.2,17.2,17.5,17.6,17.7,18.5,18.6,18.7,18.8,19,19.3,19.4,
                            19.5,19.7,19.8,19.9,20.2,20.7,21.6,21.7,21.7,21.9,22.1,22.2,22.8,23.9,23.9,24.1};

    double topic1PPT2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};

    double num6[] = {-1.5, 5.4, 3.6, 6.9, 5.5};
    
    int decimalPlaces = 1;  //based on the element width the highest decimal place

    int arrSize = sizeof(num6)/sizeof(double);    //calculate the number of data

    cout << "Start...." << endl;
    DataTable* data = new DataTable(num6, decimalPlaces, arrSize);

    data->displayData();
    data->displayTable();

    UngroupedMeasurements* central = new UngroupedMeasurements(data);
    central->displayMeasurements('s', 85,7,1);

    GroupedMeasurements* gCentral = new GroupedMeasurements(data);
    gCentral->displayMeasurements(85,7,1);

    cout << "Program Finished...." << endl;
    return 0;
}
