#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for patient record
struct Patient {
    char name[50];
    int age;
    char disease[50];
    char contact[20];
};

// Function prototypes
void addPatient();
void viewPatients();
void searchPatient();
void deletePatient();

int main() {
    int choice;
    while (1) {
        printf("\n==== Patient Management System ====\n");
        printf("1. Add Patient\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: deletePatient(); break;
            case 5: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to add a new patient record
void addPatient() {
    FILE *file = fopen("patients.dat", "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Patient p;
    printf("Enter patient name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0'; // Remove newline character

    printf("Enter patient age: ");
    scanf("%d", &p.age);
    getchar(); // Clear input buffer

    printf("Enter disease: ");
    fgets(p.disease, sizeof(p.disease), stdin);
    p.disease[strcspn(p.disease, "\n")] = '\0';

    printf("Enter contact number: ");
    fgets(p.contact, sizeof(p.contact), stdin);
    p.contact[strcspn(p.contact, "\n")] = '\0';

    fwrite(&p, sizeof(struct Patient), 1, file);
    fclose(file);
    printf("Patient record added successfully!\n");
}

// Function to view all patient records
void viewPatients() {
    FILE *file = fopen("patients.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Patient p;
    printf("\n--- Patient Records ---\n");
    while (fread(&p, sizeof(struct Patient), 1, file)) {
        printf("Name: %s\nAge: %d\nDisease: %s\nContact: %s\n", 
                p.name, p.age, p.disease, p.contact);
        printf("-------------------------\n");
    }
    fclose(file);
}

// Function to search for a patient by name
void searchPatient() {
    FILE *file = fopen("patients.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    char searchName[50];
    struct Patient p;
    int found = 0;

    printf("Enter patient name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    while (fread(&p, sizeof(struct Patient), 1, file)) {
        if (strcmp(p.name, searchName) == 0) {
            printf("Record Found!\nName: %s\nAge: %d\nDisease: %s\nContact: %s\n", 
                   p.name, p.age, p.disease, p.contact);
            found = 1;
            break;
        }
    }
    if (!found) printf("Patient not found!\n");
    fclose(file);
}

// Function to delete a patient record
void deletePatient() {
    FILE *file = fopen("patients.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    char deleteName[50];
    struct Patient p;
    int found = 0;

    printf("Enter patient name to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = '\0';

    while (fread(&p, sizeof(struct Patient), 1, file)) {
        if (strcmp(p.name, deleteName) != 0) {
            fwrite(&p, sizeof(struct Patient), 1, tempFile);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);
    remove("patients.dat");
    rename("temp.dat", "patients.dat");

    if (found) {
        printf("Patient record deleted successfully!\n");
    } else {
        printf("Patient not found!\n");
    }
}
