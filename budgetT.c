#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "budgetTracker.txt"

struct Expense {
    float amount;
    char jeepCode[50];
};

struct BudgetTracker {
    float balance;
    struct Expense expenses[100];
    int num_expenses;
};

// function declaration
void Income(struct BudgetTracker *tracker, float amount);
void expense(struct BudgetTracker *tracker, float amount, char *jeepCode);
void viewExpenses(struct BudgetTracker *tracker);
void saveData(struct BudgetTracker *tracker);
void loadData(struct BudgetTracker *tracker);

int main() {
    struct BudgetTracker tracker;
    tracker.balance = 0;
    tracker.num_expenses = 0;

    loadData(&tracker);

    while (1) {
        printf("\nBudget Tracker Menu:\n");
        printf("1. Add Income\n");
        printf("2. Add Expense\n");
        printf("3. View Expenses\n");
        printf("4. Save Data\n");
        printf("5. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter income amount PHP: ");
                float income;
                scanf("%f", &income);
                Income(&tracker, income);
                break;
            case 2:
                printf("Enter expense amount PHP: ");
                float expense_amount;
                scanf("%f", &expense_amount);
                char jeepCode[50];
                printf("Enter expense jeep code: ");
                scanf("%s", jeepCode);
                expense(&tracker, expense_amount, jeepCode);
                break;
            case 3:
                viewExpenses(&tracker);
                break;
            case 4:
                saveData(&tracker);
                printf("Data saved successfully.\n");
                break;
            case 5:
                printf("Thank You!\n");
                saveData(&tracker);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void Income(struct BudgetTracker *tracker, float amount) {
    tracker->balance += amount;
    printf("Income of PHP %.2f added.\nCurrent balance: PHP %.2f\n", amount, tracker->balance);
}

void expense(struct BudgetTracker *tracker, float amount, char *jeepCode) {
    if (amount <= tracker->balance) {
        tracker->balance -= amount;
        struct Expense expense = {amount};
        strcpy(expense.jeepCode, jeepCode);
        tracker->expenses[tracker->num_expenses++] = expense;
        printf("Expense of PHP %.2f in %s added.\nCurrent balance PHP: %.2f\n", amount, jeepCode, tracker->balance);
    } else {
        printf("Insufficient funds!\n");
    }
}

void viewExpenses(struct BudgetTracker *tracker) {
    if (tracker->num_expenses == 0) {
        printf("No expenses recorded yet.\n");
    } else {
        printf("Expenses:\n");
        for (int i = 0; i < tracker->num_expenses; i++) {
            printf("PHP %.2f in %s\n", tracker->expenses[i].amount, tracker->expenses[i].jeepCode);
        }
    }
}

void saveData(struct BudgetTracker *tracker) {
    FILE *file = fopen(FILENAME, "w");
    if (file != NULL) {
        fprintf(file, "%.2f\n", tracker->balance);
        fprintf(file, "%d\n", tracker->num_expenses);
        for (int i = 0; i < tracker->num_expenses; i++) {
            fprintf(file, "%.2f %s\n", tracker->expenses[i].amount, tracker->expenses[i].jeepCode);
        }
        fclose(file);
    } else {
        printf("Error: Unable to save data to file.\n");
    }
}

void loadData(struct BudgetTracker *tracker) {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        fscanf(file, "%f", &tracker->balance);
        fscanf(file, "%d", &tracker->num_expenses);
        for (int i = 0; i < tracker->num_expenses; i++) {
            fscanf(file, "%f %s", &tracker->expenses[i].amount, tracker->expenses[i].jeepCode);
        }
        fclose(file);
    } else {
        printf("No previous data found.\n");
    }
}


