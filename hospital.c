#include <stdio.h>
#include <string.h>

#define MAX 100

// doctor data
char specName[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
double baseFee[4] = {1500.00, 2500.00, 4500.00, 5000.00};
int timePerPatient[4] = {15, 20, 30, 30};
int patientCap[4] = {30, 20, 12, 10};

// ward data
char wardName[4][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
double wardRate[4] = {3000.00, 6000.00, 12000.00, 25000.00};
int wardBeds[4] = {20, 10, 10, 5};

// bed status 0=free 1=taken
int beds[4][20];

// queue for each specialty
int queue[4] = {0, 0, 0, 0};

int main() {
    printf("Smart Hospital Management System\n");
    return 0;
}
