#include <iostream>
#include "classes/DataTable.hpp"
#include "classes/UngroupedMeasurements.hpp"
#include "classes/GroupedMeasurements.hpp"

using std::cout;
using std::endl;

int main(void){
    //Input Data here
    double arr[] = {18, 31, 37, 21, 43, 16, 48, 20, 18, 47, 28, 32, 49, 24, 12, 42, 36, 63, 30, 19, 15, 29, 26, 28, 23, 40, 39, 54, 23, 25, 10, 58, 54, 32, 29, 32, 22, 30, 26, 38, 47, 48, 56, 61, 28, 26, 40, 17, 36, 35};
    
    int decimalPlaces = 1;  //based on the element width the highest decimal place

    int arrSize = sizeof(arr)/sizeof(double);    //calculate the number of data

    cout << "Start...." << endl;
    cout << "Original Data: [";
    for (int d = 0; d < arrSize; d++) cout << arr[d] << ", ";
    cout << "\b\b]\n\n";
    
    DataTable* data = new DataTable(arr, decimalPlaces, arrSize);

    data->displayData();
    data->displayTable();

    UngroupedMeasurements* central = new UngroupedMeasurements(data);
    central->displayMeasurements('s', 85,7,1);

    GroupedMeasurements* gCentral = new GroupedMeasurements(data);
    gCentral->displayMeasurements(85,7,1);

    cout << "Program Finished...." << endl;
    return 0;
}
