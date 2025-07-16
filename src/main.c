#include "header.h" //side note: double quotes tells the compiler to check local folder for header

//creates a user to log inputs/outpus (global)
user_log user1[50];
int user1_index = 0; //starts at begining

void log_time(void){ //adds time to user1 log at current index
    time_t now = time(NULL);
    struct tm *lt = localtime(&now); //formats now to human readable time
    strftime(user1[user1_index].time, sizeof(user1[user1_index].time), "%Y-%m-%d %H:%M:%S", lt); //copies human readable string to user log
}
 
void log_integer(char type, int output) { //stores integer input into the log (converts from int>string)
    user1[user1_index].type = type; //adds type of data to log (in or out)
    char str[50];//string that will take converted integer
    snprintf(str, sizeof(str), "%d", output); //converts the int to string
    strncpy(user1[user1_index].text, str, sizeof(user1[user1_index].text) - 1); //copies data from function input argument into log
    log_time(); //logs the time of action
    user1_index += 1; //increments index
    if (user1_index >= 50) user1_index = 0; //rolls over back to start at the end of array
}

void log_string(char type, char *output) { //stores string data to the log
    user1[user1_index].type = type; //adds type of data to log
    strncpy(user1[user1_index].text, output, sizeof(user1[user1_index].text) - 1); //copies data from function input argument into log
    user1[user1_index].text[sizeof(user1[user1_index].text) - 1] = '\0'; // Ensure null termination
    log_time(); //log time of action
    user1_index += 1;
    if (user1_index >= 50) user1_index = 0; //rolls over back to start at the end of array
}

void print_log(void){ //prints the log into a text file and terminal
    FILE *fp; //initializes file opperation fp
    fp = fopen("output.txt", "w"); //opens file in write mode "w"
    for (int i =0; i < 50; i++){ //writes entire log to file
        printf("Time: %s Type: %c Data: %s\n", user1[i].time, user1[i].type, user1[i].text);
        fprintf(fp, "Time: %s Type: %c Data: %s\n", user1[i].time, user1[i].type, user1[i].text);
        if (user1[i+1].type == '\0') i = 50; //stops writing when data ends (next type is empty)
    }
    fclose(fp); //closes file
}

int input_int(void) {//handles integer inputs
    int a;
    printf("Enter number: ");
    //When the scanf function successfully reads a correct input it adds 1 to what it returns
    if (scanf("%d", &a) != 1) { //info about scanf(): https://www.ibm.com/docs/en/i/7.4.0?topic=functions-scanf-read-data
        printf("Invalid Input!\n");
       while (getchar() != '\n'); //clears user input 
        log_string('O', "Invalid operation.");
        return input_int(); //tries again
    }
    while (getchar() != '\n'); //clears user input
    log_integer('I', a); //stores the record of input
    return a;
}

char* input_string(void) { //handles string inputs
    char str[50]; //creates char array for input from user
    printf("Enter a string(%ld charaters max): ",sizeof(str));
    if (fgets(str, sizeof(str), stdin) == NULL) return NULL; //retrns NULL if an error occurs
    str[strcspn(str, "\n")] = '\0'; //replaces \n in array to null, strcspn() returns location of char in array its looking for. https://www.geeksforgeeks.org/strcspn-in-c/
    log_string('I', str); //stores the record of the input
    return strdup(str);  //returns a pointer to a copy of string str, https://www.geeksforgeeks.org/strdup-strdndup-functions-c/
} 

void F_Arithmetic(int x) { //function for add,subtract and multiply operations
    int result;
    if (x == 1){ //1 for addition
        result = input_int() + input_int();
    } else if (x == 2){ //2 for subtraction
        result = input_int() - input_int();
    } else if (x == 3){ //3 for multiplication
        result = input_int() * input_int();
    }
    char str_out[50]; //char arary for output and log
    snprintf(str_out, sizeof(str_out), "The result is: %d", result); //makes the output with result as pure string
    printf("%s", str_out); //outputs pure string to user
    log_string('O', str_out); //logs pure string
}

void F_special(int x) { //handles triangle number and factorial operation
    int a;
    do { //in case of invalid number, ask user to try again
        a = input_int();
        if (a < 0) {
            printf("Invalid Input, try again!\n");
            log_string('O', "Invalid operation.");
        }
    } while (a < 0);

    long int result;
    if (x == 1){ //triangle number calculation
        result = 0;
        for (int i = 1; i <= a; i++) {
            result += i;
        }
    }else if (x == 2){ //factorial calculation
        result = 1;
        for (int i = 1; i <= a; i++) {
            result *= i;
        }
    }
    char str_out[50]; //char arary for output and log
    snprintf(str_out, sizeof(str_out), "The result is: %ld", result); //makes the output with result as pure string
    printf("%s", str_out); //outputs pure string to user
    log_string('O', str_out); //logs pure string
}

int F_magic_8_ball(void) {//gives user random advice
    EightBall ball; //creates variable of structure eightball
    initialize_eightball(&ball); //sends the address of 'ball ' to the function to insert each option
    srand(time(NULL)); // sets the seed for a random number using curent time for the rand() function
    int random_index = rand() % 10;  // get a random index between 0 and 9
    printf("%s", ball.magic8ball[random_index]); //uses the random number as the array index to print
    log_string('O', ball.magic8ball[random_index]);
}

int F_join_2_string(void) {//prints two user input string together
    char *str1 = input_string(); //gets user input for first strig
    char *str2 = input_string();
    char str_out[50]; //string of output text for log
    snprintf(str_out, sizeof(str_out), "Here is what you entered: %s %s", str1, str2); //converts string with result to pure string
    printf("%s", str_out); //outputs pure string to user
    log_string('O', str_out); //logs pure string
}

int main(void) { //main loop of program 
    while(1){

        printf("\nHere are the available operations:\n");
        log_string('O', "Chose operation:");
        for (int i = 0; i < 8; i++) {//Prints out the availible opperations (stored in header file)
            printf("- %s\n", operations[i]);
        }

        //Gets input from user
        char *str = input_string(); //this functions returns a pointer to the string from the user

        if (strcmp(str, operations[0]) == 0) { //strcmp returns 0 if strings are equal
            F_Arithmetic(1); //calls add function
        } else if (strcmp(str, operations[1]) == 0) {
            F_Arithmetic(2);
        } else if (strcmp(str, operations[2]) == 0) {
            F_Arithmetic(3);
        } else if (strcmp(str, operations[3]) == 0) {
            F_special(1);
        } else if (strcmp(str, operations[4]) == 0) {
            F_special(2);
        } else if (strcmp(str, operations[5]) == 0) {
            F_magic_8_ball();
        } else if (strcmp(str, operations[6]) == 0) {
            F_join_2_string();
        } else if (strcmp(str, operations[7]) == 0) {
            print_log();
            return 0;
        } else {
            printf("Invalid operation.\n");
            log_string('O', "Invalid operation.");
        }     
    }
}
