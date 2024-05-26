#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
struct Expense {
    float amount;
    char jeepCode[50];
};

struct BudgetTracker {
    float balance;
    struct Expense expenses[100];
    int num_expenses;
};

//declaration of function
void add_income(struct BudgetTracker *tracker, float amount);
void add_expense(struct BudgetTracker *tracker, float amount, char *jeepCode);
void view_expenses(struct BudgetTracker *tracker);


//main function
int main() {
    struct BudgetTracker tracker;
    tracker.balance = 0;
    tracker.num_expenses = 0;

    while (1) {
        printf("\nBudget Tracker Menu:\n");
        printf("1. Add Income\n");
        printf("2. Add Expense\n");
        printf("3. View Expenses\n");
        printf("4. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter income amount Php: ");
                float income;
                scanf("%f", &income);
                add_income(&tracker, income);
                break;
            case 2:
                printf("Enter expense amount Php: ");
                float expense_amount;
                scanf("%f", &expense_amount);
                char jeepCode[50];
                printf("Enter jeepCode: ");
                scanf("%s", jeepCode);
                add_expense(&tracker, expense_amount, jeepCode);
                break;
            case 3:
                view_expenses(&tracker);
                break;
            case 4:
                printf("Thank You!");
                exit(0);
            default:
                printf("Please try again.\n");
        }
    }

    return 0;
}


// Function Prototypes
void add_income(struct BudgetTracker *tracker, float amount) {
    tracker->balance += amount;
    printf("Income of Php: %.2f added.\nCurrent balance Php: %.2f\n", amount, tracker->balance);
}

void add_expense(struct BudgetTracker *tracker, float amount, char *jeepCode) {
    if (amount <= tracker->balance) {
        tracker->balance -= amount;
        struct Expense expense = {amount};
        strcpy(expense.jeepCode, jeepCode);
        tracker->expenses[tracker->num_expenses++] = expense;
        printf("Expense of Php: %.2f in %s added.\n", amount, jeepCode);
    } else {
        printf("Insufficient funds!\n");
    }
}

void view_expenses(struct BudgetTracker *tracker) {
    if (tracker->num_expenses == 0) {
        printf("No expenses recorded yet.\n");
    } else {
        printf("Expenses:\n");
        for (int i = 0; i < tracker->num_expenses; i++) {
            printf("$%.2f in %s\n", tracker->expenses[i].amount, tracker->expenses[i].jeepCode);
        }
        printf("Current balance Php: %.2f\n", tracker->balance);
    }
}


