#include "header.h" //side note: double quotes tells the compiler to check local folder for header
#include <pthread.h>

//mutex creation for logging of user1
pthread_mutex_t user_log_mutex = PTHREAD_MUTEX_INITIALIZER;

user_log user1[50];
int user1_index = 0;

void log_time(void){ //adds time to user1 log at current index
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    strftime(user1[user1_index].time, sizeof(user1[user1_index].time), "%Y-%m-%d %H:%M:%S", lt);
}
 
void* log_integer(void* arg) {
    LogData_int* data = (LogData_int*) arg;
    pthread_mutex_lock(&user_log_mutex);  //prevents other threads from writing
    user1[user1_index].type = data->type;
    char str[50];
    snprintf(str, sizeof(str), "%d", data->num);
    strncpy(user1[user1_index].text, str, sizeof(user1[user1_index].text) - 1);
    log_time();
    user1_index += 1;
    pthread_mutex_unlock(&user_log_mutex);  //unlocks other threads once writing is done
    if (user1_index >= 50) user1_index = 0;
    free(data);
    pthread_exit(NULL);
}

void helper_log_integer(char type, int x) { //helper function for thread handling
    pthread_t log_int_thread;
    LogData_int* data = malloc(sizeof(LogData_int)); //allocates memory for data
    data->type = type;
    data->num = x;
    pthread_create(&log_int_thread, NULL, log_integer, data);
    pthread_join(log_int_thread, NULL);
}

void* log_string(void* arg) {
    LogData_string* data = (LogData_string*) arg;
    pthread_mutex_lock(&user_log_mutex);  //prevents other threads from writing
    user1[user1_index].type = data->type;
    strncpy(user1[user1_index].text, data->text, sizeof(user1[user1_index].text) - 1);
    user1[user1_index].text[sizeof(user1[user1_index].text) - 1] = '\0';
    log_time();
    user1_index = (user1_index + 1) % 50;
    pthread_mutex_unlock(&user_log_mutex);  //unlocks other threads once writing is done
    free(data);
    pthread_exit(NULL);
}


void helper_log_string(char type, const char *text) { //helper function for thread handling
    pthread_t log_string_thread; //thread handle
    LogData_string *data = malloc(sizeof(LogData_string)); //allocates memory for data
    data->type = type;
    strncpy(data->text, text, sizeof(data->text) - 1);
    data->text[sizeof(data->text) - 1] = '\0';

    pthread_create(&log_string_thread, NULL, log_string, data);
    pthread_join(log_string_thread, NULL);
}

void print_log(void){ //prints the log into a text file and terminal
    FILE *fp;
    fp = fopen("output.txt", "w"); //opens file in write mode "w"
    for (int i =0; i < 50; i++){ //writes entire log to file
        printf("Time: %s Type: %c Data: %s\n", user1[i].time, user1[i].type, user1[i].text);
        fprintf(fp, "Time: %s Type: %c Data: %s\n", user1[i].time, user1[i].type, user1[i].text);
        if (user1[i+1].type == '\0') i = 50;
    }
    fclose(fp); //closes file
}

int input_int(void) {//handles integer inputs
    int a;
    printf("Enter number: ");
    
    if (scanf("%d", &a) != 1) {
        printf("Invalid Input!\n");
       while (getchar() != '\n'); //clears user input 
        helper_log_string('O', "Invalid operation.");
        return input_int(); //tries again
    }
    while (getchar() != '\n');
    helper_log_integer('I', a);
    return a;
}


char* input_string(void) { //handles string inputs
    char str[50];
    printf("Enter a string(%ld charaters max): ",sizeof(str));
    if (fgets(str, sizeof(str), stdin) == NULL) return NULL; //retrns NULL if an error occurs
    str[strcspn(str, "\n")] = '\0';
    helper_log_string('I', str);
    return strdup(str); 
} 

void* F_Arithmetic(void* arg) { //function for add (1), subtract (2) and multiply (3) operations
    int x = *(int*) arg;
    int result;
    if (x == 1){
        result = input_int() + input_int();
    } else if (x == 2){
        result = input_int() - input_int();
    } else if (x == 3){
        result = input_int() * input_int();
    }
    char str_out[50]; //char arary for output and log
    snprintf(str_out, sizeof(str_out), "The result is: %d", result); //makes the output with result as pure string
    printf("%s", str_out);
    helper_log_string('O', str_out);
    free(arg); //release arg's memory
    pthread_exit(NULL);
}

void helper_Arithmetic(int x) { // helper function to run F_Arithmetic in a thread
    pthread_t Arithmetic; // thread handle
    int *type = malloc(sizeof(int)); // allocate memory for the calculation type
    *type = x; 

    pthread_create(&Arithmetic, NULL, F_Arithmetic, type); // use F_Arithmetic as the thread function
    pthread_join(Arithmetic, NULL); // wait for thread to finish
}

void* F_special(void* arg) { //handles triangle number and factorial operation
    int x = *(int*) arg;
    int a;
    do { //in case of invalid number, ask user to try again
        a = input_int();
        if (a < 0) {
            printf("Invalid Input, try again!\n");
            helper_log_string('O', "Invalid operation.");
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
    helper_log_string('O', str_out); //logs pure string
    free(arg); //release arg's memory
    pthread_exit(NULL);
}

void helper_special(int x) { // helper function to run F_Arithmetic in a thread
    pthread_t special; // thread handle
    int *type = malloc(sizeof(int)); // allocate memory for the calculation type
    *type = x; 

    pthread_create(&special, NULL, F_special, type); // use F_Arithmetic as the thread function
    pthread_join(special, NULL); // wait for thread to finish
}

void* F_join_2_string(void* arg) {//prints two user input string together
    char *str1 = input_string(); //gets user input for first strig
    char *str2 = input_string();
    char str_out[50]; //string of output text for log
    snprintf(str_out, sizeof(str_out), "Here is what you entered: %s %s", str1, str2); //converts string with result to pure string
    printf("%s", str_out); //outputs pure string to user
    helper_log_string('O', str_out); //logs pure string
    pthread_exit(NULL);
}

void start_join_2_string() { // helper function to run F_Arithmetic in a thread
    pthread_t join_2_string; // thread handle
    pthread_create(&join_2_string, NULL, F_join_2_string, NULL); // use F_Arithmetic as the thread function
    pthread_join(join_2_string, NULL); // wait for thread to finish
}

int main(void) {

    while(1){
        
        printf("\nLab 5 Prelab\n");
        printf("\nHere are the available operations:\n");
        helper_log_string('O', "Chose operation:");

        for (int i = 0; i < 8; i++) {//Prints out the availible opperations (stored in header file)
            printf("- %s\n", operations[i]);
        }

        char *str = input_string(); //this functions returns a pointer to the string from the user

        if (strcmp(str, operations[0]) == 0) { //strcmp returns 0 if strings are equal
            helper_Arithmetic(1);
        } else if (strcmp(str, operations[1]) == 0) {
            helper_Arithmetic(2);
        } else if (strcmp(str, operations[2]) == 0) {
            helper_Arithmetic(3);
        } else if (strcmp(str, operations[3]) == 0) {
            helper_special(1);
        } else if (strcmp(str, operations[4]) == 0) {
            helper_special(2);
        } else if (strcmp(str, operations[5]) == 0) {
            start_join_2_string();
        } else if (strcmp(str, operations[6]) == 0) {
            print_log();
            return 0;
        } else {
            printf("Invalid operation.\n");
            helper_log_string('O', "Invalid operation.");
        }     
    }
}
