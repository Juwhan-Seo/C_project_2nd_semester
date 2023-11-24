#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_WORD_COUNT 50
#define MAX_WORD_LENGTH 20
#define TIME_LIMIT 120 // 2분
#define YELLOW_TEXT "\x1b[33m"
#define GREEN_TEXT "\x1b[32m"
#define BLUE_TEXT "\x1b[34m"
#define RED_TEXT "\x1b[31m"
#define RESET_TEXT "\x1b[0m"
#define BOX_WIDTH 40
#define POINTS_PER_WORD 10

const char *original_word_list[MAX_WORD_COUNT] = {
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon"
};

char *word_list[MAX_WORD_COUNT];
int word_count = 10;

char *custom_strdup(const char *s) {
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

void shuffle_words() {
    for (int i = 0; i < word_count; ++i) {
        word_list[i] = custom_strdup(original_word_list[i]);
    }

    for (int i = 0; i < word_count; ++i) {
        int j = rand() % word_count;
        char *temp = word_list[i];
        word_list[i] = word_list[j];
        word_list[j] = temp;
    }
}

int check_input(const char *input, const char *word) {
    return strcmp(input, word) == 0;
}

void draw_line() {
    printf(BLUE_TEXT);
    for (int i = 0; i < BOX_WIDTH; i++) {
        printf("-");
    }
    printf(RESET_TEXT "\n");
}

void display_ui(const char *word, int seconds_left) {
    system("cls");
    printf(BLUE_TEXT "Time left: %02d seconds\n" RESET_TEXT, seconds_left);
    printf(YELLOW_TEXT "Word to type:\n" RESET_TEXT);
    printf(GREEN_TEXT "%s\n\n" RESET_TEXT, word);
    printf(RED_TEXT "Type here: " RESET_TEXT);
}

void display_start_message() {
    system("cls");
    draw_line();
    printf(YELLOW_TEXT "Welcome to the Typing Game!\n" RESET_TEXT);
    printf("Each correct word is worth %d points.\n", POINTS_PER_WORD);
    printf("Try to score as much as possible in 2 minutes.\n");
    draw_line();
    printf("Press Enter to start...\n");
    draw_line();
    while(getchar() != '\n'); // Enter key to start
}

int main() {
    setlocale(LC_ALL, "");

    display_start_message();

    char input[MAX_WORD_LENGTH];
    int score = 0, words_attempted = 0;
    time_t start_time, current_time;

    srand(time(NULL));
    time(&start_time);
    shuffle_words();

    while (words_attempted < word_count) {
        time(&current_time);
        int time_spent = difftime(current_time, start_time);
        int time_left = TIME_LIMIT - time_spent;

        if (time_left <= 0) {
            printf(BLUE_TEXT "\nTime Over\n" RESET_TEXT);
            break;
        }

        display_ui(word_list[words_attempted], time_left);

        fgets(input, MAX_WORD_LENGTH, stdin);
        input[strcspn(input, "\n")] = 0;

        if (check_input(input, word_list[words_attempted])) {
            printf("Correct!\n");
            score += POINTS_PER_WORD;
        } else {
            printf(RED_TEXT "Incorrect!\n" RESET_TEXT);
            system("pause"); // Pause briefly on incorrect
        }
        words_attempted++;
    }

    printf(BLUE_TEXT "\nFinal Score: " RESET_TEXT);
    printf(YELLOW_TEXT "%d\n" RESET_TEXT, score);
    system("pause");

    for (int i = 0; i < word_count; ++i) {
        free(word_list[i]);
    }

    return 0;
}