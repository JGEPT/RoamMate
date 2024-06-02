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
void Budget(struct BudgetTracker *tracker, float amount);
void expense(struct BudgetTracker *tracker, float amount, char *jeepCode);
void viewExpenses(struct BudgetTracker *tracker);
void saveData(struct BudgetTracker *tracker);
void loadData(struct BudgetTracker *tracker);
void editExpense(struct BudgetTracker *tracker, int index, float newAmount, char *newJeepCode);
void deleteExpense(struct BudgetTracker *tracker, int index);

int main() {
    struct BudgetTracker tracker;
    tracker.balance = 0;
    tracker.num_expenses = 0;

    loadData(&tracker);

    while (1) {
        printf("\nBudget Tracker Menu:\n");
        printf("1. Add Budget\n");
        printf("2. Add Expense\n");
        printf("3. View Expenses\n");
        printf("4. Edit Expense\n"); 
        printf("5. Delete Expense\n"); 
        printf("6. Save Data\n");
        printf("7. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Budget amount PHP: ");
                float budget;
                scanf("%f", &budget);
                Budget(&tracker, budget);
                break;
            case 2:
                printf("Enter expense amount PHP: ");
                float expense_amount;
                scanf("%f", &expense_amount);
                char jeepCode[50];
                printf("Enter jeep code or taxi name: ");
                scanf("%s", jeepCode);
                expense(&tracker, expense_amount, jeepCode);
                break;
            case 3:
                viewExpenses(&tracker);
                break;
            case 4:
                printf("Enter what expense to edit (use 0 as the first, 1 as the second and so on): ");
                int editIndex;
                scanf("%d", &editIndex);
                printf("Enter new expense amount PHP: ");
                float newAmount;
                scanf("%f", &newAmount);
                char newJeepCode[50];
                printf("Enter new jeep code or taxi name: ");
                scanf("%s", newJeepCode);
                editExpense(&tracker, editIndex, newAmount, newJeepCode);
                break;
            case 5:
                printf("Enter what expense to delete (use 0 as the first, 1 as the second and so on): ");
                int deleteIndex;
                scanf("%d", &deleteIndex);
                deleteExpense(&tracker, deleteIndex);
                break;
            case 6:
                saveData(&tracker);
                printf("Data saved successfully.\n");
                break;
            case 7:
                printf("Thank You!\n");
                saveData(&tracker);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void Budget(struct BudgetTracker *tracker, float amount) {
    tracker->balance += amount;
    printf("Budget of PHP %.2f added.\nCurrent balance PHP: %.2f\n", amount, tracker->balance);
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
        printf("Current balance PHP: %.2f\n",tracker->balance);
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

void editExpense(struct BudgetTracker *tracker, int index, float newAmount, char *newJeepCode) {
    if (index < 0 || index >= tracker->num_expenses) {
        printf("Invalid expense index.\n");
        return;
    }
    tracker->balance += tracker->expenses[index].amount; 
    tracker->balance -= newAmount; 
    tracker->expenses[index].amount = newAmount;
    strcpy(tracker->expenses[index].jeepCode, newJeepCode);
    printf("Expense edited successfully.\n");
}

void deleteExpense(struct BudgetTracker *tracker, int index) {
    if (index < 0 || index >= tracker->num_expenses) {
        printf("Invalid expense index.\n");
        return;
    }
    tracker->balance += tracker->expenses[index].amount; 
    for (int i = index; i < tracker->num_expenses - 1; i++) {
        tracker->expenses[i] = tracker->expenses[i + 1]; 
    }
    tracker->num_expenses--;
    printf("Expense deleted successfully.\n");
}