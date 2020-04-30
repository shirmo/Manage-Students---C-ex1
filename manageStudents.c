#include <stdio.h>
#include <ctype.h>
#include <string.h>


#define VALID_ID_LEN 11
#define MAX_FIELD_LEN 41
#define MAX_LINE_LEN 150
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define FALSE 0
#define TRUE 1
#define LEGAL_NAME_COUNTRY_CITY_CHAR '-'
#define WHITESPACE ' '
#define MAX_STUDENTS 5000
#define LEGAL_INPUT_FORMAT "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t"
#define QUIT1 "q\n"
#define QUIT2 "q\r\n"
#define EQUALS 0
#define INITIAL_LINE -1
#define NO_STUDENTS 0
#define LOWEST_RATE 0.0
#define FIRST 0
#define LEGAL_ARGS_AMOUNT 2
#define OPEN_MSG "Enter student info. To exit press q, then enter\n"
#define ERR_ID "ERROR: ID must be 10 digits\n"
#define ERR_NAME "ERROR: Name must be english alphabet, the sign - and whitespaces\n"
#define ERR_GRADE "ERROR: Grade must be integer between 0 to 100\n"
#define ERR_AGE "ERROR: Age must be integer between 18 to 120\n"
#define ERR_COUNTRY "ERROR: Country must be english alphabet and the sign - \n"
#define ERR_CITY "ERROR: City must be english alphabet and the sign - \n"
#define ERR_LINE "in line "
#define BEST_MSG "best student info is: "
#define BEST "best"
#define GRADE_SORT "merge"
#define ABC_SORT "quick"
#define USAGE_ERR1 "USAGE: to use the program enter a valid input: ./manageStudents.c <app>"
#define USAGE_ERR2 "USAGE: please choose and insert one of the following: 'best', 'merge', 'quick'"
#define ERR_CODE 1
#define SUCCESS_CODE 0
#define APP 1


/**
 * A struct for students with necessary fields
 */
typedef struct
{
    char ID[VALID_ID_LEN];
    char name[MAX_FIELD_LEN];
    int grade;
    int age;
    char country[MAX_FIELD_LEN];
    char city[MAX_FIELD_LEN];
} Student;

Student gMyStudents[MAX_STUDENTS];


/**
 * Checks is a string is only numbers
 * @param num - the string, array of chars
 * @return 0 if not, 1 if yes
 */
int isOnlyNums(const char num[MAX_FIELD_LEN])
{
    for (unsigned long i = 0; i < strlen(num); ++i)
    {
        if (isdigit(num[i]))
        {
            continue;
        }
        return FALSE;
    }
    return TRUE;
}


/**
 * Checks if a string is a valid ID - in the correct length and only numbers
 * @param ID - string, array of chars
 * @return 0 if not, non-zero if the id is valid
 */
int idValidate(const char ID[MAX_FIELD_LEN])
{
    return ((strlen(ID) == VALID_ID_LEN - 1) && isOnlyNums(ID));
}


/**
 * Checks if a string is a valid name which includes only alphabetic characters or '-'
 * @param name string, array of chars can be either name or country or city
 * @param length of the string name - unsigned long
 * @return 0 if valid, non-zero if not
 */
int nameCountryCityValidate(const char name[MAX_FIELD_LEN], unsigned long length, int isName)
{
    for (unsigned long i = 0; i < length; ++i)
    {
        if (isName)
        {
            if (name[i] != LEGAL_NAME_COUNTRY_CITY_CHAR && !isalpha(name[i]) &&
                name[i] != WHITESPACE)
            {
                return FALSE;
            }
        }
        else
        {
            if (name[i] != LEGAL_NAME_COUNTRY_CITY_CHAR && !isalpha(name[i]))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}


/**
 * Checking if the string is a valid age or grade - all numbers and in the right range.
 * @param tested - the string to check. can be either grade and age
 * @param min - of the range
 * @param max - of the range
 * @return 0 if invalid, non-zero if valid.
 */
int gradeAndAgeValidate(const char tested[MAX_FIELD_LEN], int min, int max)
{
    int numberForTesting;
    sscanf(tested, "%d", &numberForTesting);
    return (isOnlyNums(tested) && (min <= numberForTesting) && (numberForTesting <= max));
}


/**
 * Checks the input of the user separated by fields
 * @param ID - string, should be 10 digits
 * @param name - string of the student's name
 * @param grade - string, student's grade. should be integer
 * @param age - string, student's age. should be integer
 * @param country - string
 * @param city - string
 * @param line - int of the line number
 * @return 0 if the input is invalid, 1 if valid.
 */
int validateInput(const char ID[MAX_FIELD_LEN], const char name[MAX_FIELD_LEN],
                  const char grade[MAX_FIELD_LEN], const char age[MAX_FIELD_LEN],
                  const char country[MAX_FIELD_LEN], const char city[MAX_FIELD_LEN], int line)
{
    if (!idValidate(ID))
    {
        printf("%s%s%d\n", ERR_ID, ERR_LINE, line);
        return FALSE;
    }
    if (!nameCountryCityValidate(name, strlen(name), TRUE))
    {
        printf("%s%s%d\n", ERR_NAME, ERR_LINE, line);
        return FALSE;
    }
    if (!gradeAndAgeValidate(grade, MIN_GRADE, MAX_GRADE))
    {
        printf("%s%s%d\n", ERR_GRADE, ERR_LINE, line);
        return FALSE;
    }
    if (!gradeAndAgeValidate(age, MIN_AGE, MAX_AGE))
    {
        printf("%s%s%d\n", ERR_AGE, ERR_LINE, line);
        return FALSE;
    }
    if (!nameCountryCityValidate(country, strlen(country), FALSE))
    {
        printf("%s%s%d\n", ERR_COUNTRY, ERR_LINE, line);
        return FALSE;
    }
    if (!nameCountryCityValidate(city, strlen(city), FALSE))
    {
        printf("%s%s%d\n", ERR_CITY, ERR_LINE, line);
        return FALSE;
    }
    return TRUE;
}


/**
 * Creating a new student and adding it to the global array of students
 * @param ID - string, should be 10 digits
 * @param name - string of the student's name
 * @param grade - string, student's grade. should be integer
 * @param age - string, student's age. should be integer
 * @param country - string
 * @param city - string
 * @param amount - int, how many students in the array so far
 */
void addStudent(const char ID[MAX_FIELD_LEN], const char name[MAX_FIELD_LEN],
                const char grade[MAX_FIELD_LEN], const char age[MAX_FIELD_LEN],
                const char country[MAX_FIELD_LEN], const char city[MAX_FIELD_LEN], int amount)
{
    Student newStudent;
    strcpy(newStudent.ID, ID);
    strcpy(newStudent.name, name);
    sscanf(grade, "%d", &(newStudent.grade));
    sscanf(age, "%d", &(newStudent.age));
    strcpy(newStudent.country, country);
    strcpy(newStudent.city, city);
    gMyStudents[amount] = newStudent;
}

/**
 * Gets and splitting to variables the user's input
 * @param input - the input line of the user
 * @param line - a pointer to the line number of user's input
 * @param ID - string, should be 10 digits
 * @param name - string of the student's name
 * @param grade - string, student's grade. should be integer
 * @param age - string, student's age. should be integer
 * @param country - string
 * @param city - string
 */
void getInput(char input[MAX_LINE_LEN], int *line, char ID[MAX_FIELD_LEN], char name[MAX_FIELD_LEN],
              char grade[MAX_FIELD_LEN], char age[MAX_FIELD_LEN], char country[MAX_FIELD_LEN],
              char city[MAX_FIELD_LEN])
{
    (*line)++;
    printf("%s", OPEN_MSG);
    fgets(input, MAX_LINE_LEN, stdin);
    sscanf(input, LEGAL_INPUT_FORMAT, ID, name, grade, age, country, city);
}


/**
 * Parsing the user's input and creating and adding students if input is valid.
 */
int parser()
{
    char input[MAX_LINE_LEN];
    int line = INITIAL_LINE, amount = NO_STUDENTS;
    char ID[MAX_FIELD_LEN], name[MAX_FIELD_LEN], grade[MAX_FIELD_LEN], age[MAX_FIELD_LEN],
        country[MAX_FIELD_LEN], city[MAX_FIELD_LEN];
    getInput(input, &line, ID, name, grade, age, country, city);

    while (strcmp(input, QUIT1) != EQUALS && strcmp(input, QUIT2) != EQUALS)
    {
        if (!validateInput(ID, name, grade, age, country, city, line))
        {
            getInput(input, &line, ID, name, grade, age, country, city);
            continue;
        }

        addStudent(ID, name, grade, age, country, city, amount++);
        getInput(input, &line, ID, name, grade, age, country, city);
    }
    return amount;
}


/**
 * Checks for a specific student what it rate, e.g. grade/age
 * @param index - int, index of the student in myStudents array
 * @return float of grade/age
 */
float getRate(int index)
{
    float grade = (float) gMyStudents[index].grade;
    float age = (float) gMyStudents[index].age;
    return grade / age;
}


/**
 * Checks and prints the best student's info in school
 */
void best(int amountOfStudents)
{
    float bestRate = LOWEST_RATE;
    int bestStudentIndex = FIRST;

    for (int i = 0; i < amountOfStudents; i++)
    {
        float rate = getRate(i);
        if (rate > bestRate)
        {
            bestStudentIndex = i;
            bestRate = rate;
        }
    }

    Student bestStudent = gMyStudents[bestStudentIndex];
    printf("%s%s\t%s\t%d\t%d\t%s\t%s\t\n", BEST_MSG, bestStudent.ID, bestStudent.name,
           bestStudent.grade, bestStudent.age, bestStudent.country, bestStudent.city);
}


/**
 * Merging 2 arrays to one sorted array by student's grade
 * @param students - the array to sort, array of Student
 * @param left - start index of left sub array, int
 * @param middle - finish index of left sub array, start index of right sub array, int
 * @param right - finish index of right sub array, int
 */
void merge(int left, int middle, int right)
{
    int leftSize = middle - left + 1;
    int rightSize = right - middle;
    Student leftArray[MAX_STUDENTS / 2], rightArray[MAX_STUDENTS / 2];

    /**
     * copy left half  and right half elements to temp arrays
     */
    for (int i = 0; i < leftSize; i++)
    {
        leftArray[i] = gMyStudents[left + i];
    }
    for (int j = 0; j < rightSize; j++)
    {
        rightArray[j] = gMyStudents[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize)
    {
        if (leftArray[i].grade <= rightArray[j].grade)
        {
            gMyStudents[k] = leftArray[i];
            i++;
        }
        else
        {
            gMyStudents[k] = rightArray[j];
            j++;
        }
        k++;
    }
    /*
     * copy remaining elements from leftArray if any left.
     */
    while (i < leftSize)
    {
        gMyStudents[k] = leftArray[i];
        i++;
        k++;
    }
    /*
     * copy remaining elements from rightArray if any left.
     */
    while (j < rightSize)
    {
        gMyStudents[k] = rightArray[j];
        j++;
        k++;
    }
}


/**
 * Sorting the students in school (gMyStudents) by grade, by using merge sort algorithm
 * @param left - left index to sort from, int
 * @param right - right index to sort to, int
 */
void mergeSort(int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        mergeSort(left, middle);
        mergeSort(middle + 1, right);
        merge(left, middle, right);
    }

}


/**
 * Printing all the students with their info by order
 * @param amountOfStudents - int, number of students in school (in the array)
 */
void printAllStudents(int amountOfStudents)
{
    for (int i = 0; i < amountOfStudents; ++i)
    {
        Student student = gMyStudents[i];
        printf("%s\t%s\t%d\t%d\t%s\t%s\t\n", student.ID, student.name, student.grade, student.age,
               student.country, student.city);
    }
}


/**
 * Sorting the students in school (gMyStudents) by grade, and then printing them by order
 * @param amountOfStudents - int, number of students in school (in the array)
 */
void sortByGrade(int amountOfStudents)
{
    mergeSort(FIRST, amountOfStudents - 1);
    printAllStudents(amountOfStudents);
}


/**
 * Swapping 2 students in the array
 * @param student1 - Student
 * @param student2 - Student
 */
void swap(Student *student1, Student *student2)
{
    Student temp = *student1;
    *student1 = *student2;
    *student2 = temp;
}


/**
 * Re-arranging gMyStudents in a way which alphabetically all students in the left of the pivot
 * are "smaller" than it, and in the right of the pivot are "bigger" than it
 * @param left - int, left index to re-arrange from
 * @param right - int, right index to re-arrange until it
 * @return int - index of the pivot
 */
int partition(int left, int right)
{
    Student pivot = gMyStudents[right];
    int i = left - 1;

    for (int j = left; j <= right; ++j)
    {
        if (strcmp(gMyStudents[j].name, pivot.name) < 0)
        {
            i++;
            swap(&gMyStudents[i], &gMyStudents[j]);
        }
    }

    swap(&gMyStudents[i + 1], &gMyStudents[right]);
    return i + 1;
}


/**
 * Sorting the students in school (gMyStudents) by grade, by using quick sort algorithm
 * @param left - left index to sort from, int
 * @param right - right index to sort to, int
 */
void quickSort(int left, int right)
{
    if (left < right)
    {
        int pivot = partition(left, right);
        quickSort(left, pivot - 1);
        quickSort(pivot + 1, right);
    }
}


/**
 * Sorting the students in school (gMyStudents) alphabetically, and then printing them by order
 * @param amountOfStudents - int, number of students in school (in the array)
 */
void sortAlphabetically(int amountOfStudents)
{
    quickSort(FIRST, amountOfStudents - 1);
    printAllStudents(amountOfStudents);
}


int main(int argc, char *argv[])
{
    if (argc != LEGAL_ARGS_AMOUNT)
    {
        printf("%s\n", USAGE_ERR1);
        return ERR_CODE;
    }
    if (strcmp(argv[APP], BEST) != EQUALS && strcmp(argv[APP], GRADE_SORT) != EQUALS &&
        strcmp(argv[APP], ABC_SORT) != EQUALS)
    {
        printf("%s\n", USAGE_ERR2);
        return ERR_CODE;
    }

    int amountOfStudents = parser();
    if (amountOfStudents == NO_STUDENTS)
    {
        return SUCCESS_CODE;
    }

    if (strcmp(argv[APP], BEST) == EQUALS)
    {
        best(amountOfStudents);
    }
    else if (strcmp(argv[APP], GRADE_SORT) == EQUALS)
    {
        sortByGrade(amountOfStudents);
    }
    else if (strcmp(argv[APP], ABC_SORT) == EQUALS)
    {
        sortAlphabetically(amountOfStudents);
    }

    return SUCCESS_CODE;
}
