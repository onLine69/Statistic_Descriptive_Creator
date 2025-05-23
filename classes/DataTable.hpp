#ifndef DATATABLE
#define DATATABLE

class DataTable{
    public:
        /*
            Constructor.
        */
        DataTable(double[], int, int);

        /*
            Destructor also deallocates the memory used.
        */
        ~DataTable();

        /*
            Display the FDT Table (might not align, depends on the input data).
        */
        void displayTable();

        /*
            Display Data sorted
        */
        void displayData();

        double* getArrPtr();

        int getArrSize();

        int getIntervals();

        double getClassWidth();

        double* getBoundaries();

        double* getClassMarks();

        int* getFrequencies();

        int* getLessFreq();

    private:
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
            Calculate the frequency for each interval passed
        */
        int freqCount (double[], int, double, double);

        //using bubble sort
        void sorter(double[], int);

        /*
            Calculate and list every interval for each class.
        */
        double* classInterval();

        /*
            Calculate and list every frequency for each class.
        */
        int* frequencies();

        /*
            Calculate and list every class marks for each class.
        */
        double* classMarks();

        /*
            Calculate and list every class boundaries for each class.
        */
        double* classBoundaries();

        /*
            Calculate and list every less than cumulative frequency for each class.
        */
        int* lessCumulativeFrequencies();
};

#endif // DATATABLE
