#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100

typedef struct {
    int id;
    char title[100];
    char dueDate[20];
    int priority;
    int isCompleted;
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;

void loadTasks();
void saveTasks();
void addTask();
void viewTasks();
void markCompleted();
void editTask();
void deleteTask();
void exportCompletedTasks();
int calculateDaysLeft(char *dueDate);
void menu();
int login();

int main() {
    if (!login()) {
        printf("Access Denied!\n");
        return 1;
    }
    loadTasks();
    menu();
    saveTasks();
    return 0;
}

int login() {
    char password[20];
    printf("Enter password to access To-Do List: ");
    scanf("%s", password);
    return strcmp(password, "admin123") == 0;
}

void menu() {
    int choice;
    do {
        printf("\n===== TO-DO LIST MENU =====\n");
        printf("1. Add Task\n");
        printf("2. View All Tasks\n");
        printf("3. Mark Task as Completed\n");
        printf("4. Edit Task\n");
        printf("5. Delete Task\n");
        printf("6. Export Completed Tasks\n");
        printf("7. Save and Exit\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: addTask(); saveTasks(); break;
            case 2: viewTasks(); break;
            case 3: markCompleted(); saveTasks(); break;
            case 4: editTask(); saveTasks(); break;
            case 5: deleteTask(); saveTasks(); break;
            case 6: exportCompletedTasks(); break;
            case 7: printf("Saving and exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while(choice != 7);
}

int calculateDaysLeft(char *dueDate) {
    int day, month, year;
    sscanf(dueDate, "%d-%d-%d", &day, &month, &year);
    struct tm due = {0};
    due.tm_mday = day;
    due.tm_mon = month - 1;
    due.tm_year = year - 1900;
    time_t now = time(NULL);
    double seconds = difftime(mktime(&due), now);
    return (int)(seconds / (60 * 60 * 24));
}

void loadTasks() {
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) return;

    while (fscanf(file, "%d\n%[^\n]\n%[^\n]\n%d\n%d\n",
                  &tasks[taskCount].id,
                  tasks[taskCount].title,
                  tasks[taskCount].dueDate,
                  &tasks[taskCount].priority,
                  &tasks[taskCount].isCompleted) == 5) {
        taskCount++;
    }

    fclose(file);
}

void saveTasks() {
    FILE *file = fopen("tasks.txt", "w");
    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d\n%s\n%s\n%d\n%d\n",
                tasks[i].id,
                tasks[i].title,
                tasks[i].dueDate,
                tasks[i].priority,
                tasks[i].isCompleted);
    }
    fclose(file);
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task limit reached!\n");
        return;
    }
    Task newTask;
    newTask.id = taskCount + 1;

    printf("Enter task title: ");
    fgets(newTask.title, sizeof(newTask.title), stdin);
    newTask.title[strcspn(newTask.title, "\n")] = 0;

    printf("Enter due date (dd-mm-yyyy): ");
    fgets(newTask.dueDate, sizeof(newTask.dueDate), stdin);
    newTask.dueDate[strcspn(newTask.dueDate, "\n")] = 0;

    printf("Enter priority (1=High, 2=Medium, 3=Low): ");
    scanf("%d", &newTask.priority);
    getchar();

    newTask.isCompleted = 0;
    tasks[taskCount++] = newTask;
    printf("Task added successfully!\n");
}

void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks to show.\n");
        return;
    }
    printf("\n%-5s %-30s %-15s %-10s %-10s %-10s\n", "ID", "Title", "Due Date", "Priority", "Status", "Days Left");
    for (int i = 0; i < taskCount; i++) {
        int daysLeft = calculateDaysLeft(tasks[i].dueDate);
        printf("%-5d %-30s %-15s %-10s %-10s %d\n",
               tasks[i].id,
               tasks[i].title,
               tasks[i].dueDate,
               (tasks[i].priority == 1) ? "High" :
               (tasks[i].priority == 2) ? "Medium" : "Low",
               (tasks[i].isCompleted == 1) ? "Done" : "Pending",
               daysLeft);
    }
}

void markCompleted() {
    int id;
    printf("Enter task ID to mark as completed: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            tasks[i].isCompleted = 1;
            printf("Task marked as completed!\n");
            return;
        }
    }
    printf("Task ID not found!\n");
}

void editTask() {
    int id;
    printf("Enter task ID to edit: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            printf("Enter new title: ");
            fgets(tasks[i].title, sizeof(tasks[i].title), stdin);
            tasks[i].title[strcspn(tasks[i].title, "\n")] = 0;

            printf("Enter new due date: ");
            fgets(tasks[i].dueDate, sizeof(tasks[i].dueDate), stdin);
            tasks[i].dueDate[strcspn(tasks[i].dueDate, "\n")] = 0;

            printf("Enter new priority (1=High, 2=Medium, 3=Low): ");
            scanf("%d", &tasks[i].priority);
            getchar();

            printf("Task updated successfully!\n");
            return;
        }
    }
    printf("Task ID not found!\n");
}

void deleteTask() {
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];
                tasks[j].id = j + 1;
            }
            taskCount--;
            printf("Task deleted successfully!\n");
            return;
        }
    }
    printf("Task ID not found!\n");
}

void exportCompletedTasks() {
    FILE *file = fopen("completed_tasks.txt", "w");
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].isCompleted == 1) {
            fprintf(file, "%d\n%s\n%s\n%d\n%d\n",
                    tasks[i].id,
                    tasks[i].title,
                    tasks[i].dueDate,
                    tasks[i].priority,
                    tasks[i].isCompleted);
        }
    }
    fclose(file);
    printf("Completed tasks exported to 'completed_tasks.txt'.\n");
}


