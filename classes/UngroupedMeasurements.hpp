#ifndef UNGROUPEDMEASUREMENTS
#define UNGROUPEDMEASUREMENTS

class UngroupedMeasurements{
public:
    UngroupedMeasurements(DataTable*);
    ~UngroupedMeasurements();
    void displayMeasurements(char, int, int, int);

private:
    double *dataPtr;
    int dataSizeUngrouped;
    int numDecimalPlace;

    double ungroupedMean();
    double ungroupedMedian();
    double* ungroupedMode();
    double ungroupedSVariance();
    double ungroupedPVariance();
    double sStandardDev();
    double pStandardDev();
    double sCoefficientVariation();
    double pCoefficientVariation();
    double getPercentile(int);
    double getDecile(int);
    double getQuartile(int);
};

#endif // UNGROUPEDMEASUREMENTS
