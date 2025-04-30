#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
typedef struct Subject Subject;
typedef struct Student Student;

void gotoxy(int x, int y) {
    COORD coord = {0, 0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct Subject {
    char Name[20];
    int score;
};

struct Student {
    int ID;
    int NumSubject;
    float Average;
    char grade;
    char Name[20];
    Subject* sub;
};

char Grade_based_Avg(float average) {
    if (average >= 90)
        return 'A';
    else if (average >= 80)
        return 'B';
    else if (average >= 70)
        return 'C';
    else if (average >= 60)
        return 'D';
    else
        return 'F';
}

void calc_average(Student* st, int Students_number) {
    for (int i = 0; i < Students_number; i++) {
        float total = 0;
        for (int j = 0; j < st[i].NumSubject; j++) {
            total += st[i].sub[j].score;
        }
        st[i].Average = (st[i].NumSubject > 0) ? total / st[i].NumSubject : 0;
        st[i].grade = Grade_based_Avg(st[i].Average);
    }
}

void add_Student(Student **st, int *Students_number) {
    system("color 3F");
    int New_size, valid_ID;

    printf("Enter the number of students to add (or type '0' to return to the main menu): \n");
    scanf("%d", &New_size);

    if (New_size == 0) {
        return;
    }

    *st = realloc(*st, (*Students_number + New_size) * sizeof(Student));
    if (*st == NULL) {
        printf("Memory allocation for students failed!\n");
        exit(1);
    }

    for (int i = *Students_number; i < *Students_number + New_size; i++) {
        do {
            valid_ID = 0;
            printf("Enter Student ID:\n");
            scanf("%d", &(*st)[i].ID);

            for (int m = 0; m < i; m++) {
                if ((*st)[m].ID == (*st)[i].ID) {
                    printf("This ID already exists! Please enter a different ID.\n");
                    valid_ID = 1;
                    break;
                }
            }
        } while (valid_ID);

        printf("Enter Student Name:\n");
        scanf("%19s", (*st)[i].Name);

        printf("Enter Number of subjects:\n");
        scanf("%d", &(*st)[i].NumSubject);

        while ((*st)[i].NumSubject <= 0) {
            printf("Invalid number of subjects. Please enter a valid number:\n");
            scanf("%d", &(*st)[i].NumSubject);
        }

        (*st)[i].sub = malloc((*st)[i].NumSubject * sizeof(Subject));
        if ((*st)[i].sub == NULL) {
            printf("Memory allocation for subjects failed!\n");

            for (int k = *Students_number; k < i; k++) {
                if ((*st)[k].sub != NULL) {
                    free((*st)[k].sub);
                }
            }
            free(*st);
            exit(1);
        }

        for (int j = 0; j < (*st)[i].NumSubject; j++) {
            int valid = 0;

            printf("Enter the name of subject %d (max 19 characters):\n", j + 1);
            scanf("%19s", (*st)[i].sub[j].Name);

            while (!valid) {
                char input[20];
                printf("Enter The Score Of Subject Between 0 and 100:\n");
                scanf("%19s", input);

                int is_numeric = 1;
                for (int k = 0; input[k] != '\0'; k++) {
                    if (!isdigit(input[k])) {
                        is_numeric = 0;
                        break;
                    }
                }

                if (is_numeric) {
                    int score = atoi(input);
                    if (score >= 0 && score <= 100) {
                        (*st)[i].sub[j].score = score;
                        printf("\nScore Saved Successfully\n");
                        valid = 1;
                    } else {
                        printf("Enter a valid score between 0 and 100.\n");
                    }
                } else {
                    printf("Invalid input. Please enter a numeric score.\n");
                }
            }
        }
    }

    *Students_number += New_size;
}

void update_students(Student* st, int Students_number) {
    system("color 3F");
    int id, found = 0;
    if (Students_number > 0) {
        calc_average(st, Students_number);
        for (int i = 0; i < Students_number; i++) {
            printf("Student's Data: \n\n");
            printf("\nStudent ID             : %d\n", st[i].ID);
            printf("Student Name           : %s\n", st[i].Name);
            printf("Student Average score  : %.3f\n", st[i].Average);
            printf("Student Grade is       : %c\n", st[i].grade);
        }

        printf("\nEnter Student ID You want to update: ");
        scanf("%d", &id);

        for (int i = 0; i < Students_number; i++) {
            if (st[i].ID == id) {
                found = 1;
                printf("\nStudent details before update: \n");
                printf("Student ID : %d , Name : %s\n\n", st[i].ID, st[i].Name);

                printf("Subjects for %s :\n", st[i].Name);
                for (int j = 0; j < st[i].NumSubject; j++) {
                    printf("Subject %d : %s , Score : %d\n\n", j + 1, st[i].sub[j].Name, st[i].sub[j].score);
                }

                int subject_index;
                printf("Enter the subject number you want to update (1 to %d): ", st[i].NumSubject);
                scanf("%d", &subject_index);

                if (subject_index > 0 && subject_index <= st[i].NumSubject) {
                    int new_score;
                    printf("Enter the new score for %s (between 0 and 100): ", st[i].sub[subject_index - 1].Name);
                    scanf("%d", &new_score);

                    if (new_score >= 0 && new_score <= 100) {
                        st[i].sub[subject_index - 1].score = new_score;
                        printf("Grade for %s updated to %d\n", st[i].sub[subject_index - 1].Name, new_score);
                    } else {
                        printf("Invalid score entered. Please enter a value between 0 and 100.\n");
                    }
                } else {
                    printf("Invalid subject number.\n");
                }

                calc_average(st, Students_number);
                break;
            }
        }

        if (!found) {
            printf("Student ID %d not found.\n", id);
        }
    } else {
        printf("\nStudents List Is Empty....\n\n");
    }
}

void Display_Result(Student* st, int Students_number) {
    system("color 3F");
    if (Students_number > 0) {
        float heights = 0;
        char St_Name[20];
        for (int i = 0; i < Students_number; i++) {
            printf("Student ID            : %d\n", st[i].ID);
            printf("Student Name          : %s\n", st[i].Name);
            printf("Student Average score : %.2f\n", st[i].Average);
            printf("Student Grade is      : %c\n\n", st[i].grade);
        }

        for (int i = 0; i < Students_number; i++) {
            if (st[i].Average > heights) {
                heights = st[i].Average;
                strncpy(St_Name, st[i].Name, sizeof(St_Name));
                St_Name[sizeof(St_Name) - 1] = '\0';
            }
        }
        printf("The highest average is : %.2f\n", heights);
        printf("The Name of student is : %s\n\n", St_Name);

        float Average_all = 0;
        printf("Average score for all students:\n");
        for (int i = 0; i < Students_number; i++) {
            printf("Average score of student %d : %s\n", i + 1, st[i].Name);
            printf("%.2f\n", st[i].Average);
            Average_all += st[i].Average;
        }
        printf("The average of all students is : %.2f\n", Average_all / Students_number);

        printf("Students Who Failed:\n");
        for (int i = 0; i < Students_number; i++) {
            if (st[i].grade == 'F') {
                printf("Student Name : %s\n", st[i].Name);
            }
        }

        printf("Students Who Passed:\n");
        for (int i = 0; i < Students_number; i++) {
            if (st[i].grade != 'F') {
                printf("Student Name : %s\n", st[i].Name);
            }
        }
    } else {
        printf("\nStudents List Is Empty....\n\n");
    }
}

void print_frame() {
    HANDLE set_c = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(set_c, 3);
    gotoxy(60, 13);
    printf("%c", 201);
    for (int i = 0; i < 30; i++) printf("%c", 205);
    printf("%c", 187);

    for (int i = 14; i < 20; i++) {
        gotoxy(60, i);
        printf("%c", 186);
        for (int j = 0; j < 30; j++) printf("%c", 32);
        printf("%c", 186);
    }

    gotoxy(60, 20);
    printf("%c", 200);
    for (int i = 0; i < 30; i++) printf("%c", 205);
    printf("%c", 188);
}

void save_to_file(Student* st, int Students_number) {
    system("color 3F");
    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for saving.\n");
        return;
    }

    fprintf(file, "%d\n", Students_number);
    for (int i = 0; i < Students_number; i++) {
        fprintf(file, "%d\n", st[i].ID);
        fprintf(file, "%s\n", st[i].Name);
        fprintf(file, "%d\n", st[i].NumSubject);
        fprintf(file, "%.2f\n", st[i].Average);
        fprintf(file, "%c\n", st[i].grade);

        for (int j = 0; j < st[i].NumSubject; j++) {
            fprintf(file, "%s %d\n", st[i].sub[j].Name, st[i].sub[j].score);
        }
    }

    fclose(file);
    printf("Data saved successfully to 'students.txt'.\n");
}

void load_from_file(Student** st, int* Students_number) {
    system("color 3F");

    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file for loading.\n");
        return;
    }

    fscanf(file, "%d\n", Students_number);

    *st = realloc(*st, (*Students_number) * sizeof(Student));
    if (*st == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < *Students_number; i++) {
        fscanf(file, "%d\n", &(*st)[i].ID);
        fgets((*st)[i].Name, sizeof((*st)[i].Name), file);
        (*st)[i].Name[strcspn((*st)[i].Name, "\n")] = '\0';
        fscanf(file, "%d\n", &(*st)[i].NumSubject);
        fscanf(file, "%f\n", &(*st)[i].Average);
        fscanf(file, " %c\n", &(*st)[i].grade);

        if ((*st)[i].NumSubject > 0) {
            (*st)[i].sub = malloc((*st)[i].NumSubject * sizeof(Subject));
            if ((*st)[i].sub == NULL) {
                printf("Memory allocation failed!\n");
                fclose(file);
                exit(1);
            }

            for (int j = 0; j < (*st)[i].NumSubject; j++) {
                fscanf(file, "%s %d\n", (*st)[i].sub[j].Name, &(*st)[i].sub[j].score);
            }
        } else {
            (*st)[i].sub = NULL;
        }
    }

    fclose(file);
    printf("Data loaded successfully from 'students.txt'.\n");
}

void save_load_menu(Student** st, int* Students_number) {
    HANDLE set_c = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice = 1;
    char ch;
    do {
        system("color 3F");
        system("cls");
        print_frame();

        SetConsoleTextAttribute(set_c, 2);
        gotoxy(56, 12);
        printf("(.. SAVE/LOAD MENU ..)");

        SetConsoleTextAttribute(set_c, 11);
        gotoxy(65, 15);
        printf("[1] - Save Data.");
        gotoxy(65, 16);
        printf("[2] - Load Data.");
        gotoxy(65, 17);
        printf("[3] - Back to Main Menu.");

        switch (choice) {
            case 1:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 15);
                printf("[1] - Save Data.");
                break;
            case 2:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 16);
                printf("[2] - Load Data.");
                break;
            case 3:

                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 17);
                printf("[3] - Back to Main Menu.");
                break;
        }

        ch = getch();
        if (ch == -32) {
            ch = getch();
            if (ch == 72) {
                choice--;
                if (choice == 0) choice = 3;
            } else if (ch == 80) {
                choice++;
                if (choice == 4) choice = 1;
            }
        } else if (ch == '\r') {
            system("cls");
            switch (choice) {
                case 1:
                    calc_average(*st, *Students_number);
                    save_to_file(*st, *Students_number);
                    printf("\nPress any key to return to the save/load menu...");
                    getch();
                    break;
                case 2:
                    load_from_file(st, Students_number);
                    printf("\nPress any key to return to the save/load menu...");
                    getch();
                    break;
                case 3:
                    system("color 3F");
                    return;
            }
        }
    } while (1);
}

int main() {
    HANDLE set_c = GetStdHandle(STD_OUTPUT_HANDLE);
    int Students_number = 0;
    Student* st = NULL;
    int poss = 1;
    char ch;
    do {
        system("color 3F");
        system("cls");
        print_frame();

        SetConsoleTextAttribute(set_c, 2);
        gotoxy(56, 12);
        printf("(.. STUDENT GRADE MANAGEMENT SYSTEM ..)");

        SetConsoleTextAttribute(set_c, 11);
        gotoxy(65, 15);
        printf("[1] - Add student.");
        gotoxy(65, 16);
        printf("[2] - Update Student.");
        gotoxy(65, 17);
        printf("[3] - Display Results.");
        gotoxy(65, 18);
        printf("[4] - Save/load File.");
        gotoxy(65, 19);
        printf("[5] - Exit");

        switch (poss) {
            case 1:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 15);
                printf("[1] - Add student.");
                break;
            case 2:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 16);
                printf("[2] - Update Student.");
                break;
            case 3:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 17);
                printf("[3] - Display Results.");
                break;
            case 4:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 18);
                printf("[4] - Save/load File.");
                break;
            case 5:
                SetConsoleTextAttribute(set_c, 15);
                gotoxy(65, 19);
                printf("[5] - Exit");
                break;
        }

        ch = getch();
        if (ch == -32) {
            ch = getch();
            if (ch == 72) {
                poss--;
                if (poss == 0) poss = 5;
            } else if (ch == 80) {
                poss++;
                if (poss == 6) poss = 1;
            }
        } else if (ch == '\r') {
            system("cls");
            switch (poss) {
                case 1:
                    add_Student(&st, &Students_number);
                    break;
                case 2:
                    update_students(st, Students_number);
                    break;
                case 3:
                    calc_average(st, Students_number);
                    Display_Result(st, Students_number);
                    break;
                case 4:
                    save_load_menu(&st, &Students_number);
                    break;
                case 5:
                    system("color 3F");
                    printf("Exiting... Goodbye!\n");
                    exit(0);
            }
            printf("\nPress any key to return to the main menu...");
            getch();
        }
    } while (1);

    return 0;
}
