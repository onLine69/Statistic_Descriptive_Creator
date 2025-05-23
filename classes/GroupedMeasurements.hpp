#ifndef GROUPEDMEASUREMENTS
#define GROUPEDMEASUREMENTS

class GroupedMeasurements{
public:
    GroupedMeasurements(DataTable*);
    ~GroupedMeasurements();
    void displayMeasurements(int,int,int);

private:
    int dataSize;
    int intervalsGroup;
    double classWidthGroup;
    double *boundaries;
    double *classMarks;
    int *frequencies;
    int *lessFreq;

    double groupedMean();
    double groupedMedian();
    double groupedMode();
    double groupedVariance();
    double groupedStdDev();
    double coeffVar();
    double groupedPercentile(int);
    double groupedDecile(int);
    double groupedQuartile(int);
};

#endif  //GROUPEDMEASUREMENTS