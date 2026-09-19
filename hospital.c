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

// patient data arrays
char patientName[MAX][50];
int patientAge[MAX];
int patientUrgency[MAX];
int patientSpec[MAX];
int patientAdmitted[MAX];
int patientWard[MAX];
int patientDays[MAX];
double patientBill[MAX];
int patientCount = 0;

int main() {
    int choice;

    while (1) {
        printf("\n====================================================\n");
        printf("     SMART HOSPITAL MANAGEMENT SYSTEM\n");
        printf("====================================================\n");
        printf("1. Register Patient\n");
        printf("2. View Patient Bill\n");
        printf("3. Sort by Triage\n");
        printf("4. Summary Report\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Register patient -\n");
                break;
            case 2:
                printf("View bill -\n");
                break;
            case 3:
                printf("Sort triage -\n");
                break;
            case 4:
                printf("Summary -\n");
                break;
            case 5:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Wrong choice\n");
        }
    }

    return 0;
}
