#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_QUESTIONS 8
#define NUM_ANSWERS 15
#define NUM_CHOICES 4

// Declaration
int playRound(int mode);
void displayHowToPlay();
void playGame();
void normalizeString(char *str);
int jackpotRound();

// Define a Question struct
typedef struct {
    char question[200];
    char answers[NUM_ANSWERS][50];
    int points[NUM_ANSWERS];
} Question;

void shuffleArray(char arr[][50], int size) {
    int i, j;  // Declare i and j only once here
    for (i = size - 1; i > 0; i--) {  // Use the outer 'i' variable in the loop
        j = rand() % (i + 1);  // Random index to swap
        char temp[50];
        strcpy(temp, arr[i]);
        strcpy(arr[i], arr[j]);
        strcpy(arr[j], temp);
    }
}

// Function to shuffle the questions array
void shuffleQuestions(Question questions[], int size) {
    int i, j;  // Declare i and j only once here
    for (i = size - 1; i > 0; i--) { 
        j = rand() % (i + 1);  // Random index to swap
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

// Function to shufflt answer with its corresponding points
void shuffleAnswersAndPoints(char answers[][50], int points[], int size) {
    int i, j;
    for (i = size - 1; i > 0; i--) {
        j = rand() % (i + 1);  // Random index to swap
        // Swap answers
        char temp[50];
        strcpy(temp, answers[i]);
        strcpy(answers[i], answers[j]);
        strcpy(answers[j], temp);
        
        // Swap points
        int tempPoints = points[i];
        points[i] = points[j];
        points[j] = tempPoints;
    }
}

// Function to display "How to Play" instructions
void displayHowToPlay() {
    printf("\n-- HOW TO PLAY --\n");
    printf("1. The game consists of 5 questions in the main round.\n");
    printf("2. Players take turns, but after every incorrect answer, the turn switches to the other player.\n");
    printf("3. Each question has 3 possible answers, and each answer has points associated with it.\n");
    printf("4. The player with the highest score after 5 questions will move to the jackpot round.\n");
    printf("5. In the jackpot round, the winner will answer 5 random questions and must score 150 points to win the jackpot\n");
    printf("\nPress any key to return to the main menu...\n");
    getchar();
    getchar();
}

// Remove extra spaces
void normalizeString(char *str) {
    int i = 0, j = 0;
    char temp[100];

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
    printf("\nSelect an option [1 or 2]: ");
    scanf("%d", &modeChoice);

    if (modeChoice == 1) {
        printf("\n-- ONE PLAYER --\n");
        int score = playOnePlayer(1);  // Play the round for one player

        if (score < 150) {
            printf("\nYou have Lost.\n");
            char choice;
            printf("Do you want to return to the main menu? (Y/N): ");
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                return;
            } else {
                printf("\nGoodbye!\n");
                exit(0);
            }
        } else if (score >= 150 && score <= 249) {
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
        } else if (score >= 250) {
            printf("\n-- You got %d points --\n", score);
            printf("1] Move to Jackpot Round\n");
            printf("2] End the Game\n");

            int choice;
            printf("Select an option (1 or 2): ");
            scanf("%d", &choice);

            if (choice == 1) {
                jackpotRound(score);
            } else {
                printf("\nYour total points are: %d\n", score);
                printf("Returning to the main menu...\n");
            }
        }

    } else if (modeChoice == 2) {
        printf("\n-- TWO PLAYER --\n");
        int score1 = playTwoPlayer();
    } else {
        printf("\nInvalid choice! Returning to the main menu...\n");
    }
    
    printf("\nPress any key to return to the main menu...\n");
    getchar();
    getchar();
}



// Function to play the main round
int playOnePlayer(int mode) {
    // Array of questions
    Question questions[] = {
        {"An article from CNN covered famous tourist destinations, What is the most famous tourist destination in the world?",
         {"Paris", "Amsterdam", "Japan", "Berlin", "Italy", "Los Angeles", "New York", "Singapore", "South Korea", "Ireland"},
         {100, 75, 75, 75, 65, 55, 45, 35, 25, 20}},
        
        {"From a study by the National Safety Council, What age of drivers are involved in the most crashes?",
         {"25", "32", "35", "37", "42", "43", "65", "68", "72", "75", "16", "17", "18", "20", "21"},
         {100, 100, 75, 75, 75, 75, 50, 50, 50, 50, 25, 25, 25, 25, 25}},
        
        {"What is the most watched sport in the world?",
         {"Soccer", "Tennis", "Volleyball", "Golf", "Baseball", "Basketball", "American Football", "Badminton", "F1"},
         {100, 75, 60, 50, 45, 35, 30, 25, 20}},
        
        {"From a study by the Social Security Administration, What is a common name for men?",
         {"James", "John", "Michael", "Anthony", "Daniel", "Mark", "Jerry", "Adam", "Kyle"},
         {100, 75, 70, 55, 50, 45, 35, 25, 20}},
        
        {"What are common pets in the world?",
         {"Dogs", "Cats", "Fish", "Snake", "Birds", "Horse", "Hamster", "Rabbits", "Lizard"},
         {100, 100, 75, 75, 55, 55, 55, 35, 35}},
         
        {"Most common baby names in the Philippines?",
         {"Karlo", "Angela", "Sophia", "Miguel", "Nathan", "Andrea", "Princess", "Angela", "Sam", "Alex"},
         {100, 75, 75, 65, 55, 45, 35, 35, 25, 25}},
         
        {"Most used instrument in the world?",
         {"Piano", "Guitar", "Violin", "Drums", "Saxophone", "Trumpet", "Clarinet", "Harp"},
         {100, 75, 50, 50, 45, 35, 25, 15}},
         
        {"Most popular food in Asia?",
         {"Peking duck", "Sushi", "Kimchi", "Biryani", "Pho", "Tom Yam", "Nasi Goreng", "Tonkatsu", "Ramen"},
         {100, 75, 75, 55, 55, 50, 30, 25, 15}},
    };

    int totalScore = 0;
    char answer[50];
    int i, j, correct;

    // Seed the random number generator
    srand(time(NULL));

    // Shuffle the questions using the new NUM_QUESTIONS constant
    shuffleQuestions(questions, NUM_QUESTIONS);

    // Loop through the shuffled questions
	for (i = 0; i < 5; i++) {  // We are now displaying only 5 questions
        printf("\nQUESTION #%d\n", i + 1);
        printf("%s\n", questions[i].question);

        // Shuffle the answers and points together
        shuffleAnswersAndPoints(questions[i].answers, questions[i].points, 15);

        // Display the first 4 non-empty shuffled answers
        int displayedAnswers = 0;
        for (j = 0; j < 15 && displayedAnswers < 4; j++) {
            if (questions[i].answers[j][0] != '\0') {  // Check if the answer is non-empty
                printf("%d. %s\n", displayedAnswers + 1, questions[i].answers[j]);
                displayedAnswers++;
            }
        }

        printf("\nEnter your guess: ");
        
        // Normalize the input to lowercase
        fflush(stdin);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;  // Remove the newline character
        normalizeString(answer);

        // Check for correct answers
        correct = 0;
        for (j = 0; j < 15; j++) {
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
            printf("Wrong! 0 points\n");
        }
    }

    printf("\nTotal score for this round: %d points.\n", totalScore);
    return totalScore;
}

//Function to play two player
int playTwoPlayer() {
    Question questions[] = {
        {"An article from CNN covered famous tourist destinations, What is the most famous tourist destination in the world?",
         {"Paris", "Amsterdam", "Japan", "Berlin", "Italy", "Los Angeles", "New York", "Singapore", "South Korea", "Ireland"},
         {100, 75, 75, 75, 65, 55, 45, 35, 25, 20}},
        
        {"From a study by the National Safety Council, What age of drivers are involved in the most crashes?",
         {"25", "32", "35", "37", "42", "43", "65", "68", "72", "75", "16", "17", "18", "20", "21"},
         {100, 100, 75, 75, 75, 75, 50, 50, 50, 50, 25, 25, 25, 25, 25}},
        
        {"What is the most watched sport in the world?",
         {"Soccer", "Tennis", "Volleyball", "Golf", "Baseball", "Basketball", "American Football", "Badminton", "F1"},
         {100, 75, 60, 50, 45, 35, 30, 25, 20}},
        
        {"From a study by the Social Security Administration, What is a common name for men?",
         {"James", "John", "Michael", "Anthony", "Daniel", "Mark", "Jerry", "Adam", "Kyle"},
         {100, 75, 70, 55, 50, 45, 35, 25, 20}},
        
        {"What are common pets in the world?",
         {"Dogs", "Cats", "Fish", "Snake", "Birds", "Horse", "Hamster", "Rabbits", "Lizard"},
         {100, 100, 75, 75, 55, 55, 55, 35, 35}},
         
        {"Most common baby names in the Philippines?",
         {"Karlo", "Angela", "Sophia", "Miguel", "Nathan", "Andrea", "Princess", "Sam", "Alex"},
         {100, 75, 75, 65, 55, 45, 35, 35, 25}},
         
        {"Most used instrument in the world?",
         {"Piano", "Guitar", "Violin", "Drums", "Saxophone", "Trumpet", "Clarinet", "Harp"},
         {100, 75, 50, 50, 45, 35, 25, 15}},
         
        {"Most popular food in Asia?",
         {"Peking duck", "Sushi", "Kimchi", "Biryani", "Pho", "Tom Yam", "Nasi Goreng", "Tonkatsu", "Ramen"},
         {100, 75, 75, 55, 55, 50, 30, 25, 15}},
    };

    int totalScorePlayer1 = 0;
    int totalScorePlayer2 = 0;
    char answer[50];
    int i, j, correct;
    char player1Answer[50] = ""; // To store Player 1's answer
    
    // Seed the random number generator
    srand(time(NULL));

    // Shuffle the questions
    shuffleQuestions(questions, 8);

	for (i = 0; i < 5; i++) {
        printf("\nQUESTION #%d\n", i + 1);
        printf("%s\n", questions[i].question);

        // Shuffle the answers and points for the current question
        shuffleAnswersAndPoints(questions[i].answers, questions[i].points, 10);  // Shuffle the answers (size = 10)

		// Display the first 4 non-empty shuffled answers
		int displayedAnswers = 0;
		for (j = 0; j < 10 && displayedAnswers < 4; j++) {
		    if (questions[i].answers[j][0] != '\0') { 
		        printf("%d. %s\n", displayedAnswers + 1, questions[i].answers[j]);
		        displayedAnswers++;
		    }
		}

        // Player 1's turn
        printf("\nPlayer 1, enter your guess: ");
        fflush(stdin); // Clear the input buffer
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0; // Remove the newline character
        normalizeString(answer); // Normalize the answer (remove spaces, make lowercase)

        correct = 0;
        for (j = 0; j < 4; j++) {
            char correctAnswer[50];
            strcpy(correctAnswer, questions[i].answers[j]);
            normalizeString(correctAnswer);

            if (strcmp(answer, correctAnswer) == 0) {
                totalScorePlayer1 += questions[i].points[j];
                printf("Player 1 chose: %s\n", questions[i].answers[j]);
                strcpy(player1Answer, answer); // Store Player 1's answer
                correct = 1;
                break;
            }
        }

        if (!correct) {
            printf("Player 1: Wrong answer. 0 points.\n");
        }

        // Player 2's turn
        int validChoice = 0;
        while (!validChoice) {
            printf("\nPlayer 2, enter your guess: ");
            fflush(stdin); // Clear the input buffer
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = 0; // Remove the newline character
            normalizeString(answer); // Normalize the answer

            // Check if Player 2's answer matches Player 1's answer
            if (strcmp(answer, player1Answer) == 0) {
                printf("Already chosen, please guess another one.\n");
            } else {
                // Check if the answer is correct
                correct = 0;
                for (j = 0; j < 4; j++) {
                    char correctAnswer[50];
                    strcpy(correctAnswer, questions[i].answers[j]);
                    normalizeString(correctAnswer);

                    if (strcmp(answer, correctAnswer) == 0) {
                        totalScorePlayer2 += questions[i].points[j];
                        printf("Player 2 chose: %s\n", questions[i].answers[j]);
                        correct = 1;
                        validChoice = 1; // Valid answer selected
                        break;
                    }
                }

                if (!correct) {
                    printf("Player 2: Wrong answer. 0 points.\n");
                    validChoice = 1; // Even if it's wrong, we continue to the next question.
                }
            }
        }

        // Display the current scores after this round (for this question)
        printf("\nCurrent scores after Question #%d:\n", i + 1);
        printf("Player 1: %d points\n", totalScorePlayer1);
        printf("Player 2: %d points\n", totalScorePlayer2);
    }

    // Display the final total score after all questions
    printf("\nFinal Scores after all questions:\n");
		printf("Player 1: %d points\n", totalScorePlayer1);
		printf("Player 2: %d points\n", totalScorePlayer2);
		
		// Determine who has the higher score
		if (totalScorePlayer1 >= 250 || totalScorePlayer2 >= 250) {
		    if (totalScorePlayer1 > totalScorePlayer2) {
		        printf("\nCongratulations Player One with %d points! You will move onto the Jackpot Round.\n", totalScorePlayer1);
		        printf("Nice try Player Two with %d points!\n", totalScorePlayer2);
		        printf("\n-- Player One, you have the chance to move to the Jackpot Round --\n");
		        printf("1] Move to Jackpot Round\n");
		        printf("2] End the Game\n");
		
		        int choice;
		        printf("Select an option (1 or 2): ");
		        scanf("%d", &choice);
		
		        if (choice == 1) {
		            jackpotRound(totalScorePlayer1);
		        } else {
		            printf("\nPlayer 1's total points are: %d\n", totalScorePlayer1);
		            printf("Returning to the main menu...\n");
		            return 0;
		        }
		    } else {
		        printf("\nCongratulations Player Two with %d points! You will move onto the Jackpot Round.\n", totalScorePlayer2);
		        printf("Nice try Player One with %d points!\n", totalScorePlayer1);
		        printf("\n-- Player Two, you have the chance to move to the Jackpot Round --\n");
		        printf("1] Move to Jackpot Round\n");
		        printf("2] End the Game\n");
		
		        int choice;
		        printf("Select an option (1 or 2): ");
		        scanf("%d", &choice);
		
		        if (choice == 1) {
		            jackpotRound(totalScorePlayer2);
		        } else {
		            printf("\nPlayer 2's total points are: %d\n", totalScorePlayer2);
		            printf("Returning to the main menu...\n");
		            return 0;
		        }
		    }
		} else {
		    printf("\nNo player reached 250 points, both of you have lost.\n");
		    printf("Thank you for playing!\n");
		    char choice;
		    printf("Do you want to return to the main menu? (Y/N): ");
		    scanf(" %c", &choice);
		
		    if (choice == 'Y' || choice == 'y') {
		        return 0;
		    } else {
		        printf("\nGoodbye!\n");
		        exit(0);
		    }
		}

    return totalScorePlayer1 > totalScorePlayer2 ? totalScorePlayer1 : totalScorePlayer2;
}


// Jackpot Round Function
int jackpotRound(int mainRoundScore) {
	printf("\n-- JACKPOT ROUND --\n");
	
    Question jackpotQuestions[] = {
        {"Most Popular car brands in america?", 
         {"Toyota", "Ford", "Chevrolet", "Nissan", "Honda", "BMW", "Tesla", "Bugatti", "Ferrari", "Lamborghini"},
         {75, 75, 55, 55, 45, 45, 35, 25, 25, 20}},
        
        {"What month has the most birthdays?", 
         {"August", "September", "October", "June", "December", "March", "November", "April"},
         {75, 70, 55, 45, 45, 35, 35, 35}},
        
        {"Whats the most common color used for cars?", 
         {"White", "Black", "Gray", "Blue", "Silver", "Red", "Green", "Beige"},
         {75, 50, 45, 45, 40, 40, 35, 25}},
         
        {"Which country has the youngest drinking age in the world?", 
         {"Central African Republic", "Belgium", "Denmark", "Germany", "Argentina", "South Korea", "Japan", "Egypt", "USA", "Saudi Arabia", "Yemen"},
         {75, 55, 45, 35, 35, 25, 25, 25, 25, 5, 5}},

        {"Hardest course to take in college in the Philippines?", 
         {"Chemical Engineering", "Electrical Engineering", "Nursing", "Industrial Engineering", "Chemistry", "Physics", "Accounting", "Agriculture", "Secondary Education"},
         {75, 70, 50, 40, 35, 35, 25, 25, 25,}},

        {"Most used programming language?", 
         {"Python", "Java", "JavaScript", "C++", "SQL", "C#", "C", "HTML", "SHELL"},
         {100, 75, 70, 60, 55, 55, 45, 45, 45,}},

        {"Average number of cars per household in the world?", 
         {"2", "1", "0", "3", "4", "5", "6", "7", "8"},
         {100, 75, 75, 65, 45, 35, 25, 25, 25,}},

        {"Most watched 2010s cartoon shows in the world?", 
         {"Gravity Falls", "Steven Universe", "The Amazing World of Gumball", "Adventure Time", "Regular Show", "Clarence", "We Bare Bears", "Teen Titans Go", "Rabbids Invasion", "Bread Winners", "Fanboy and Chumchum"},
         {75, 75, 75, 55, 55, 55, 45, 45, 45, 45, 45}},
    };

    int jackpotScore = 0;
    char answer[50];
    int i, j, correct;

    // Seed the random number generator
    srand(time(NULL));

    // Shuffle the questions
    shuffleQuestions(jackpotQuestions, 8);
    
   for (i = 0; i < 5; i++) {
        printf("\nJACKPOT QUESTION #%d\n", i + 1);
        printf("%s\n", jackpotQuestions[i].question);
        
        // Shuffle the answers and points for the current question
        shuffleAnswersAndPoints(jackpotQuestions[i].answers, jackpotQuestions[i].points, 11);

        // Display the first 4 non-empty shuffled answers
        int displayedAnswers = 0;
        for (j = 0; j < 11 && displayedAnswers < 4; j++) {
            if (jackpotQuestions[i].answers[j][0] != '\0') {  // Check if the answer is non-empty
                printf("%d. %s\n", displayedAnswers + 1, jackpotQuestions[i].answers[j]);
                displayedAnswers++;
            }
        }

        printf("\nEnter your guess: ");
        
		// Lowercase        
        fflush(stdin);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0; 
        normalizeString(answer);

        correct = 0;
        for (j = 0; j < 11; j++) {
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
            printf("Wrong 0 points\n");
        }
    }

    if (jackpotScore >= 150) {
        printf("\n'JACKPOT', You got a total of %d points.\n", jackpotScore);
        printf("Congratulations!\n");
    } else {
        printf("\nWhat an amazing run, you got %d points.\n", jackpotScore);
        printf("You'll get the jackpot prize next time!\n");
    }

	printf("\nOverall score thankyou for playing!!\n");
	printf("Jackpot Round score: %d points.\n", jackpotScore);
	printf("Main Round Score: %d points\n", mainRoundScore);


    // Option to go back to the main menu
    printf("\n1] Go back to main menu\n");
    int choice;
    printf("Select an option [1]: ");
    scanf("%d", &choice);

    if (choice == 1) {
		return 0;
    } else {
        printf("\nInvalid choice. Returning to the main menu...\n");
        return 0;
    }

    return jackpotScore;
}



// MENU
int main() {
    char choice;

    while (1) {
        printf("\n-- MAIN MENU --\n");
        printf("H] How to Play\n");
        printf("P] Play Game\n");
        printf("Q] Quit\n");
        printf("\nSelect an option [H, P, Q]: ");
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

