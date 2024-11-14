#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Declaration
int playRound(int mode);
void displayHowToPlay();
void playGame();
void normalizeString(char *str);
int jackpotRound();

// Structure to store question data
typedef struct {
    char question[200];
    char answers[10][50];   // 10 answers per question
    int points[10];         // Points for each answer
} Question;

// Function to display "How to Play" instructions
void displayHowToPlay() {
    printf("\n-- HOW TO PLAY --\n");
    printf("1. The game consists of 5 questions in the main round.\n");
    printf("2. Players take turns, but after every incorrect answer, the turn switches to the other player.\n");
    printf("3. Each question has 3 possible answers, and each answer has points associated with it.\n");
    printf("4. The player with the highest score after 5 questions will win the game.\n");
    printf("\nPress any key to return to the main menu...\n");
    getchar();
    getchar();
}

// Normalize the string: convert to lowercase and remove extra spaces
void normalizeString(char *str) {
    int i = 0, j = 0;
    char temp[100];

    // Convert to lowercase and remove leading/trailing spaces
    while (str[i]) {
        if (!isspace((unsigned char)str[i])) {
            temp[j++] = tolower((unsigned char)str[i]);
        }
        i++;
    }
    temp[j] = '\0';

    // Copy the normalized string back
    strcpy(str, temp);
}

// Function to play the game
void playGame() {
    int modeChoice;
    printf("\n-- CHOOSE YOUR PREFERRED MODE TO BEGIN THE GAME --\n");
    printf("1] One Player\n");
    printf("2] Two Players\n");
    printf("Select an option (1 or 2): ");
    scanf("%d", &modeChoice);

    if (modeChoice == 1) {
        printf("\nStarting the game in One Player mode...\n");
        int score = 0;
        score = playRound(1);  // Play the round for one player

        printf("\nYour final score is: %d\n", score);

        // Check score and display appropriate message
        if (score < 150) {
            printf("\nYou have Lost.\n");
        } else if (score >= 150 && score <= 349) {
            printf("\nCongratulations, thank you for playing!\n");
            char choice;
            printf("Do you want to return to the main menu? (Y/N): ");
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                return;
            } else {
                printf("\nGoodbye!\n");
                exit(0);
            }
        } else if (score >= 350) {
            printf("\nCongratulations! You've scored enough points to move on to the Jackpot Round!\n");
            jackpotRound(); // Proceed to Jackpot round
        }

    } else if (modeChoice == 2) {
        printf("\nStarting the game in Two Player mode...\n");
        int score1 = 0, score2 = 0;
        score1 = playRound(2);  // Play the round for player 1
        score2 = playRound(2);  // Play the round for player 2

        if (score1 > score2) {
            printf("\nPlayer 1 wins with a score of %d!\n", score1);
        } else if (score2 > score1) {
            printf("\nPlayer 2 wins with a score of %d!\n", score2);
        } else {
            printf("\nIt's a tie!\n");
        }
    } else {
        printf("\nInvalid choice! Returning to the main menu...\n");
    }

    printf("\nPress any key to return to the main menu...\n");
    getchar(); // To consume newline character
    getchar(); // Wait for the user to press a key
}

// Function to play the main round
int playRound(int mode) {
    Question questions[] = {
        {"An article from CNN covered famous tourist destinations, What is the most famous tourist destination in the world?",
         {"Paris", "Amsterdam", "Japan", "Berlin", "Italy", "Los Angeles", "New York", "Singapore", "South Korea", "Ireland"},
         {100, 75, 70, 50, 35, 25, 25, 20, 20, 5}},
        
        {"From a study by the National Safety Council, What age of drivers are involved in the most crashes?",
         {"25-34", "35-44", "65-75", "16-21"},
         {100, 75, 50, 25}},
        
        {"What is the most watched sport in the world?",
         {"Soccer", "Tennis", "Volleyball", "Golf", "Baseball", "Basketball", "American Football", "Badminton", "F1"},
         {100, 75, 60, 50, 35, 25, 20, 10, 5}},
        
        {"From a study by the Social Security Administration, What is a common name for men?",
         {"James", "John", "Michael", "Anthony", "Daniel", "Mark", "Jerry", "Adam", "Kyle"},
         {100, 75, 70, 50, 35, 25, 20, 25, 20}},
        
        {"What are common pets in the world?",
         {"Dogs", "Cats", "Fish", "Snake", "Birds", "Horse", "Hamster", "Rabbits", "Lizard"},
         {75, 50, 50, 25, 25, 20, 15, 10, 5}}
    };

    int totalScore = 0;
    char answer[50];
    int i, j, correct;

    for (i = 0; i < 5; i++) {
        printf("\nQuestion %d: %s\n", i + 1, questions[i].question);

        // Display only the answers, no points
        for (j = 0; j < 10 && questions[i].answers[j][0] != '\0'; j++) {
            printf("%s\n", questions[i].answers[j]);
        }

        printf("Enter your guess: ");
        

        fflush(stdin);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;  // Remove newline character from input

        // Normalize the answer to lowercase before comparing
        normalizeString(answer);

        // Check for the special case in question 2 for the age range 25-34 or 35-44
        if (i == 1) {
            int age;
            if (sscanf(answer, "%d", &age) == 1) {
                if (age >= 25 && age <= 34) {
                    totalScore += 100;  // Give 100 points
                    printf("You chose: %d\n", age);
                    printf("You earned 100 points.\n");
                } else if (age >= 35 && age <= 44) {
                    totalScore += 75;  // Give 75 points
                    printf("You chose: %d\n", age);
                    printf("You earned 75 points.\n");
                } else if (age >= 65 && age <= 75){
                	totalScore += 50; // Give 50 points
                    printf("You chose: %d\n", age);
                    printf("You earned 50 points.\n");
				} else if (age >= 16 && age <= 21){
				    totalScore += 25; // Give 25 points
                    printf("You chose: %d\n", age);
                    printf("You earned 25 points.\n");	
				} else {
                    // Specific answers 0 points will get
                    for (j = 0; j < 4; j++) {
                        if (strcmp(answer, questions[i].answers[j]) == 0) {
                            totalScore += questions[i].points[j];
                            printf("You chose: %s\n", questions[i].answers[j]);
                            printf("You earned %d points.\n", questions[i].points[j]);
                            correct = 1;
                            break;
                        }
                    }
                    if (!correct) {
                        printf("Invalid choice. Please choose one of the available options.\n");
                    }
                }
            } else {
                printf("Invalid input. Please enter a valid age.\n");
            }
        } else {
            // Handle all other questions as usual
            correct = 0;
            for (j = 0; j < 10; j++) {
                char correctAnswer[50];
                strcpy(correctAnswer, questions[i].answers[j]);
                normalizeString(correctAnswer);

                if (strcmp(answer, correctAnswer) == 0) {
                    totalScore += questions[i].points[j];
                    printf("You chose: %s\n", questions[i].answers[j]);
                    printf("You earned %d points.\n", questions[i].points[j]);
                    correct = 1;
                    break;
                }
            }

            if (!correct) {
                printf("Invalid choice. Please choose one of the available options.\n");
            }
        }
    }

    printf("\nTotal score for this round: %d\n", totalScore);
    return totalScore;
}

// Jackpot Round Function
int jackpotRound() {
    Question jackpotQuestions[] = {
        {"What is the largest planet in our solar system?", 
         {"Jupiter", "Saturn", "Mars", "Earth", "Venus"},
         {200, 150, 100, 75, 50}},
        
        {"Which country is known as the Land of the Rising Sun?", 
         {"Japan", "China", "India", "South Korea", "Thailand"},
         {200, 150, 100, 50, 25}},
        
        {"What is the tallest mountain in the world?", 
         {"Mount Everest", "K2", "Kangchenjunga", "Mount Fuji", "Mount Kilimanjaro"},
         {200, 150, 100, 75, 50}}
    };

    int jackpotScore = 0;
    char answer[50];
    int i, j, correct;

    for (i = 0; i < 3; i++) {
        printf("\nJackpot Question %d: %s\n", i + 1, jackpotQuestions[i].question);

        // Display only the answers
        for (j = 0; j < 5 && jackpotQuestions[i].answers[j][0] != '\0'; j++) {
            printf("%s\n", jackpotQuestions[i].answers[j]);
        }

        printf("Enter your guess: ");
        
        fflush(stdin);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;  // Remove newline character from input

        // Normalize the answer to lowercase before comparing
        normalizeString(answer);

        correct = 0;
        for (j = 0; j < 5; j++) {
            char correctAnswer[50];
            strcpy(correctAnswer, jackpotQuestions[i].answers[j]);
            normalizeString(correctAnswer);

            if (strcmp(answer, correctAnswer) == 0) {
                jackpotScore += jackpotQuestions[i].points[j];
                printf("You chose: %s\n", jackpotQuestions[i].answers[j]);
                printf("You earned %d points.\n", jackpotQuestions[i].points[j]);
                correct = 1;
                break;
            }
        }

        if (!correct) {
            printf("Invalid choice. Please choose one of the available options.\n");
        }
    }

    printf("\nTotal Jackpot score: %d\n", jackpotScore);
    if (jackpotScore >= 500) {
        printf("\nCongratulations! You've won the Jackpot!\n");
    } else {
        printf("\nSorry, you did not win the Jackpot this time.\n");
    }

    return jackpotScore;
}

int main() {
    char choice;

    while (1) {
        printf("\n-- MAIN MENU --\n");
        printf("H] How to Play\n");
        printf("P] Play Game\n");
        printf("Q] Quit\n");
        printf("\nSelect an option (H, P, Q): ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'H':
            case 'h':
                displayHowToPlay();
                break;
            case 'P':
            case 'p':
                playGame();
                break;
            case 'Q':
            case 'q':
                printf("\nExiting the game. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please select a valid option.\n");
        }
    }

    return 0;
}

