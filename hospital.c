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
int patientBed[MAX];
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

                // find a free bed
        int b;
        int ward = patientWard[i] - 1;
        patientBed[i] = 0;
        for (b = 0; b < wardBeds[ward]; b++) {
            if (beds[ward][b] == 0) {
                beds[ward][b] = 1;
                patientBed[i] = b + 1;
                break;
            }
        }
        if (patientBed[i] == 0) {
            printf("No beds available in this ward\n");
        }
    } else {
        patientWard[i] = 0;
        patientDays[i] = 0;
        patientBed[i] = 0;
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
        printf("Assigned Ward           : %s (Bed #%02d)\n", wardName[patientWard[i] - 1], patientBed[i]);
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


void sortByPriority() {
    int i, j;
    int tempAge, tempUrg, tempSpec, tempAdm, tempWard, tempDays;
    double tempBill;
    char tempName[50];

    if (patientCount == 0) {
        printf("No patients to sort\n");
        return;
    }

    // bubble sort by urgency (3 first, then 2, then 1)
    for (i = 0; i < patientCount - 1; i++) {
        for (j = 0; j < patientCount - 1 - i; j++) {
            if (patientUrgency[j] < patientUrgency[j + 1]) {
                // swap all patient data
                strcpy(tempName, patientName[j]);
                strcpy(patientName[j], patientName[j + 1]);
                strcpy(patientName[j + 1], tempName);

                tempAge = patientAge[j];
                patientAge[j] = patientAge[j + 1];
                patientAge[j + 1] = tempAge;

                tempUrg = patientUrgency[j];
                patientUrgency[j] = patientUrgency[j + 1];
                patientUrgency[j + 1] = tempUrg;

                tempSpec = patientSpec[j];
                patientSpec[j] = patientSpec[j + 1];
                patientSpec[j + 1] = tempSpec;

                tempAdm = patientAdmitted[j];
                patientAdmitted[j] = patientAdmitted[j + 1];
                patientAdmitted[j + 1] = tempAdm;

                tempWard = patientWard[j];
                patientWard[j] = patientWard[j + 1];
                patientWard[j + 1] = tempWard;

                tempDays = patientDays[j];
                patientDays[j] = patientDays[j + 1];
                patientDays[j + 1] = tempDays;

                tempBill = patientBill[j];
                patientBill[j] = patientBill[j + 1];
                patientBill[j + 1] = tempBill;
            }
        }
    }

    printf("\nPatients sorted by priority:\n");
    printf("----------------------------------------------------\n");
    for (i = 0; i < patientCount; i++) {
        printf("PAT-%d | %s | Age: %d | Urgency: %d | %s\n",
            1001 + i, patientName[i], patientAge[i],
            patientUrgency[i], specName[patientSpec[i] - 1]);
    }
    printf("----------------------------------------------------\n");
}


void summaryReport() {
    int i;
    int normal = 0, urgent = 0, critical = 0;
    double totalRevenue = 0;
    double totalDiscount = 0;
    int highestIndex = 0;

    if (patientCount == 0) {
        printf("No patients registered\n");
        return;
    }

    // count patients by urgency and find totals
    for (i = 0; i < patientCount; i++) {
        calculateBill(i);

        if (patientUrgency[i] == 1) {
            normal++;
        } else if (patientUrgency[i] == 2) {
            urgent++;
        } else if (patientUrgency[i] == 3) {
            critical++;
        }

        totalRevenue = totalRevenue + patientBill[i];

        // check age discount
        if (patientAge[i] < 5 || patientAge[i] > 65) {
            double gross = patientBill[i] / 0.85;
            totalDiscount = totalDiscount + (gross * 0.15);
        }

        // find highest paying patient
        if (patientBill[i] > patientBill[highestIndex]) {
            highestIndex = i;
        }
    }

    printf("\n====================================================\n");
    printf("         SUMMARY REPORT\n");
    printf("====================================================\n");
    printf("Total Patients : %d\n", patientCount);
    printf("  Normal       : %d\n", normal);
    printf("  Urgent       : %d\n", urgent);
    printf("  Critical     : %d\n", critical);
    printf("----------------------------------------------------\n");
    printf("Total Revenue  : LKR  %.2f\n", totalRevenue);
    printf("Total Discounts: LKR  %.2f\n", totalDiscount);
    printf("----------------------------------------------------\n");
    printf("Highest Paying Patient:\n");
    printf("  %s - LKR %.2f\n", patientName[highestIndex], patientBill[highestIndex]);
    printf("====================================================\n");
}


int main() {
    int choice;

    while (1) {
        printf("\n====================================================\n");
        printf("     SMART HOSPITAL MANAGEMENT SYSTEM\n");
        printf("====================================================\n");
        printf("1. Register Patient\n");
        printf("2. View Patient Bill\n");
        printf("3. Sort by Priority\n");
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
                sortByPriority();
                break;
            case 4:
                summaryReport();
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
