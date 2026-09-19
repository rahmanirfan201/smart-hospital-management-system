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

void registerPatient() {
    if (patientCount >= MAX) {
        printf("Hospital full\n");
        return;
    }

    int i = patientCount;

    printf("Enter patient name: ");
    getchar();
    fgets(patientName[i], 50, stdin);
    patientName[i][strlen(patientName[i]) - 1] = '\0';

    printf("Enter age: ");
    scanf("%d", &patientAge[i]);

    printf("Enter urgency (1=Normal, 2=Urgent, 3=Critical): ");
    scanf("%d", &patientUrgency[i]);

    printf("Select specialty (1=OPD, 2=Paediatrics, 3=Cardiology, 4=Neurology): ");
    scanf("%d", &patientSpec[i]);

    printf("Admitted to ward? (1=Yes, 0=No): ");
    scanf("%d", &patientAdmitted[i]);

    if (patientAdmitted[i] == 1) {
        printf("Enter ward (1=General, 2=Paediatric, 3=Surgical, 4=ICU): ");
        scanf("%d", &patientWard[i]);

        printf("Days admitted: ");
        scanf("%d", &patientDays[i]);
    } else {
        patientWard[i] = 0;
        patientDays[i] = 0;
    }

    queue[patientSpec[i] - 1]++;
    patientCount++;
    printf("Patient registered\n");
}

void calculateBill(int i) {
    int spec = patientSpec[i] - 1;
    double fee = baseFee[spec];
    double surcharge = 0;
    double wardCost = 0;
    double gross, discount, total;

    // emergency surcharge
    if (patientUrgency[i] == 2) {
        surcharge = fee * 0.20;
    } else if (patientUrgency[i] == 3) {
        surcharge = fee * 0.50;
    }

    // ward cost
    if (patientAdmitted[i] == 1) {
        wardCost = patientDays[i] * wardRate[patientWard[i] - 1];
    }

    // gross total
    gross = fee + surcharge + wardCost;

    // age discount
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        discount = gross * 0.15;
    } else {
        discount = 0;
    }

    // final
    total = gross - discount;
    patientBill[i] = total;
}


void printBill(int i) {
    int spec = patientSpec[i] - 1;

    calculateBill(i);

    int waitTime = queue[spec] * timePerPatient[spec];

    printf("\n=====================================================\n");
    printf("       SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID              : PAT-%d\n", 1001 + i);
    printf("Patient Name            : %s\n", patientName[i]);
    printf("Age                     : %d Years", patientAge[i]);
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        printf(" (15%% Subsidy Eligible)");
    }
    printf("\n");
    printf("Specialty               : %s\n", specName[spec]);
    if (patientAdmitted[i] == 1) {
        printf("Assigned Ward           : %s\n", wardName[patientWard[i] - 1]);
    }
    printf("Urgency Level           : Level %d", patientUrgency[i]);
    if (patientUrgency[i] == 1) printf(" (Normal)");
    else if (patientUrgency[i] == 2) printf(" (Urgent)");
    else if (patientUrgency[i] == 3) printf(" (Critical)");
    printf("\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR  %.2f\n", baseFee[spec]);
    if (patientUrgency[i] == 2) {
        printf("Emergency Surcharge     : LKR  %.2f (20%%)\n", baseFee[spec] * 0.20);
    } else if (patientUrgency[i] == 3) {
        printf("Emergency Surcharge     : LKR  %.2f (50%%)\n", baseFee[spec] * 0.50);
    }
    if (patientAdmitted[i] == 1) {
        printf("Ward Stay Cost (%d Days) : LKR  %.2f\n", patientDays[i], patientDays[i] * wardRate[patientWard[i] - 1]);
    }
    printf("----------------------------------------------------------------------------------------\n");

    double gross = patientBill[i];
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        gross = patientBill[i] / 0.85;
    }

    printf("Gross Total Bill        : LKR  %.2f\n", gross);
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        printf("Age Subsidy Discount    : LKR  -%.2f (15%%)\n", gross * 0.15);
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount    : LKR  %.2f\n", patientBill[i]);
    printf("Estimated Waiting Time  : ");
    if (patientUrgency[i] == 3) {
        printf("0.00 mins (Immediate Attention)\n");
    } else {
        printf("%.2f mins\n", (double)waitTime);
    }
    printf("=====================================================\n");
}


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
                registerPatient();
                break;
            case 2:
                if (patientCount == 0) {
                    printf("No patients registered\n");
                } else {
                    int id;
                    printf("Enter patient number (1 to %d): ", patientCount);
                    scanf("%d", &id);
                    if (id >= 1 && id <= patientCount) {
                        printBill(id - 1);
                    } else {
                        printf("Invalid patient number\n");
                    }
                }
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
