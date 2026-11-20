
#include <stdio.h>
#include <string.h>

#define MAX 50

struct Vehicle {
    int slot;
    char number[20];
    char owner[50];
};

struct Vehicle vehicles[MAX];
int count = 0;

void loadData() {
    FILE *fp = fopen("parking.txt", "r");

    if (fp == NULL)
        return;

    while (count < MAX &&
           fscanf(fp, "%d %19s %49s",
                  &vehicles[count].slot,
                  vehicles[count].number,
                  vehicles[count].owner) == 3) {
        count++;
    }

    fclose(fp);
}

void saveData() {
    FILE *fp = fopen("parking.txt", "w");

    if (fp == NULL) {
        printf("Unable to save records!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %s\n",
                vehicles[i].slot,
                vehicles[i].number,
                vehicles[i].owner);
    }

    fclose(fp);
}

int findVehicle(char number[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(vehicles[i].number, number) == 0)
            return i;
    }

    return -1;
}

int findFreeSlot() {
    for (int slot = 1; slot <= MAX; slot++) {
        int occupied = 0;

        for (int i = 0; i < count; i++) {
            if (vehicles[i].slot == slot) {
                occupied = 1;
                break;
            }
        }

        if (!occupied)
            return slot;
    }

    return -1;
}

void addVehicle() {
    if (count >= MAX) {
        printf("Parking lot is full!\n");
        return;
    }

    struct Vehicle v;

    printf("Enter vehicle number (no spaces): ");
    scanf("%19s", v.number);

    if (findVehicle(v.number) != -1) {
        printf("Vehicle is already parked!\n");
        return;
    }

    printf("Enter owner name (one word): ");
    scanf("%49s", v.owner);

    v.slot = findFreeSlot();

    if (v.slot == -1) {
        printf("No parking slot available!\n");
        return;
    }

    vehicles[count] = v;
    count++;

    saveData();

    printf("Vehicle parked successfully!\n");
    printf("Allocated Slot: %d\n", v.slot);
}

void viewVehicles() {
    if (count == 0) {
        printf("Parking lot is empty!\n");
        return;
    }

    printf("\n===== PARKED VEHICLES =====\n");

    for (int i = 0; i < count; i++) {
        printf("Slot: %d | Number: %s | Owner: %s\n",
               vehicles[i].slot,
               vehicles[i].number,
               vehicles[i].owner);
    }
}

void searchVehicle() {
    char number[20];

    printf("Enter vehicle number: ");
    scanf("%19s", number);

    int index = findVehicle(number);

    if (index == -1) {
        printf("Vehicle not found!\n");
    } else {
        printf("Vehicle Found!\n");
        printf("Slot: %d\n", vehicles[index].slot);
        printf("Owner: %s\n", vehicles[index].owner);
    }
}

void exitVehicle() {
    char number[20];
    int hours;

    printf("Enter vehicle number: ");
    scanf("%19s", number);

    int index = findVehicle(number);

    if (index == -1) {
        printf("Vehicle not found!\n");
        return;
    }

    printf("Enter parking hours: ");

    if (scanf("%d", &hours) != 1 || hours <= 0) {
        printf("Invalid parking hours!\n");
        return;
    }

    int fee = hours * 50;

    printf("\n===== PARKING BILL =====\n");
    printf("Vehicle: %s\n", vehicles[index].number);
    printf("Owner: %s\n", vehicles[index].owner);
    printf("Parking Hours: %d\n", hours);
    printf("Total Fee: Rs. %d\n", fee);

    for (int i = index; i < count - 1; i++) {
        vehicles[i] = vehicles[i + 1];
    }

    count--;

    saveData();

    printf("Vehicle exit completed!\n");
}

int main() {
    int choice;

    loadData();

    do {
        printf("\n===== PARKING LOT MANAGEMENT =====\n");
        printf("1. Park Vehicle\n");
        printf("2. View Parked Vehicles\n");
        printf("3. Search Vehicle\n");
        printf("4. Vehicle Exit and Bill\n");
        printf("5. Available Slots\n");
        printf("6. Exit Program\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }

        switch (choice) {
            case 1:
                addVehicle();
                break;

            case 2:
                viewVehicles();
                break;

            case 3:
                searchVehicle();
                break;

            case 4:
                exitVehicle();
                break;

            case 5:
                printf("Available Slots: %d\n", MAX - count);
                break;

            case 6:
                saveData();
                printf("Parking records saved. Goodbye!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}
