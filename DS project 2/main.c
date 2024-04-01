#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//AVL tree node
struct course
{
    char courseName [150];
    int creditHours;
    char courseCode [50];
    char Department [150];
    char topics [10000];
    struct course *right;
    struct course *left;
    int height;//for balance use
};
typedef struct course* courseNode;


struct Stack
{
    char courseCode [100];
    struct Stack *Next;
};

//functions prototypes:
courseNode MakeEmpty( courseNode T );
int Height( courseNode T );
int Max( int Lhs, int Rhs );
courseNode SingleRotateWithLeft( courseNode T2 );
courseNode SingleRotateWithRight( courseNode T1 );
courseNode DoubleRotateWithLeft( courseNode T3 );
courseNode DoubleRotateWithRight( courseNode T1 );
courseNode Insert(char courseName[], int creditHours, char courseCode [], char Department [],char topics [], courseNode T);
void PrintInOrder( courseNode T);
courseNode readCourseData(courseNode T);
void printLine();
void menu();
courseNode InsertNewCourse(courseNode T);
courseNode Find(char courseCode [] , courseNode T );
void updateMenu();
courseNode updateCourse(courseNode T);
courseNode InsertLexi(courseNode lexiT,courseNode T);
void listTopics(courseNode T);
courseNode findMin(courseNode T);
courseNode deleteNode(char courseCode [],courseNode T);
int getBalance(courseNode T);
void printInFile(courseNode T , FILE *output);
void saveInFile(courseNode T);
void coursesBelongToSameDep(courseNode T , char department []);
courseNode deleteByLetter(courseNode T , char letter , struct Stack *S);
courseNode deleteByDep(courseNode T , char department [] , struct Stack* S);
int IsEmpty(struct Stack *S);
struct Stack* CreateStack();
void Push(char courseCode[],struct Stack *S);
char * Pop(struct Stack *S);
void FillStackWithCC(courseNode T , char letter , struct Stack *S);
void FillStackWithCC2(courseNode T , char department [] , struct Stack *S);

int main()
{
    int choice = 0;
    courseNode courses = NULL;
    courses = MakeEmpty(courses);//courses that saves in the tree by course Code
    courseNode lexiCourses = NULL;//courses that saves in the tree by course name
    printf("Welcome to my program...\n");
    printLine();
    printf("choose from the following menu:\n");
    printLine();
    menu();
    printLine();
    scanf("%d",&choice);
    printLine();
    while(choice!= 11)
    {
        if(choice==1)
        {
            courses = readCourseData(courses);//read courses from file
            PrintInOrder(courses);
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==2)
        {
            courses = InsertNewCourse(courses);//insert new course to the tree
            printLine();
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==3)
        {
           courses = updateCourse(courses);//update on the available courses
           PrintInOrder(courses);
           menu();
           printLine();
           scanf("%d",&choice);
           printLine();
        }
        else if(choice==4)
        {
            lexiCourses = MakeEmpty(lexiCourses);//make the AVL tree empty for each (4) call to avoid any inserting error
            if(courses==NULL)//if the main tree does not exist
            {
                printf("There is no courses in the list.\n");
                printLine();
            }
            else
            {
                lexiCourses = InsertLexi(lexiCourses , courses);
                PrintInOrder(lexiCourses);
            }
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==5)
        {
           listTopics(courses);//list all the topics of specific course
           printLine();
           menu();
           printLine();
           scanf("%d",&choice);
           printLine();
        }
        else if(choice==6)
        {
            char department [150];
            printf("Enter the department name:");
            getchar();
            scanf("%[^\n]",department);
            printLine();
            coursesBelongToSameDep(courses , department);//print all the courses that belong to specified department
            printLine();
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==7)
        {
            char courseCode [150];
            courseNode temp;
            printf("Enter the course code of the course you want to delete:");
            scanf("%s",courseCode);
            printLine();
            temp = Find(courseCode , courses);//find the node of the given course code and assign it to temp
            courses = deleteNode(courseCode , courses);//delete the course node
            if(temp!=NULL)//to make the user know if the course code exist or not
            {
                printf("The course has been deleted successfully.\n");
                printLine();
                PrintInOrder(courses);
            }
            else
            {
                printf("The course is not in the list.\n");
                printLine();

            }
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==8)
        {
            char letter;
            struct Stack* s;
            s = CreateStack();//stack is used to store in it the course codes that
                             //belongs to courses start with the specific letter
            printf("Enter the first letter of the courses you want to delete:");
            getchar();
            scanf("%c",&letter);
            printLine();
            courses = deleteByLetter(courses , letter , s);//delete all the courses that start with specific letter
            printLine();
            PrintInOrder(courses);
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==9)
        {
            char department [150];
            struct Stack* s;
            s = CreateStack();//stack is used to store in it the course codes that
                             //belongs to courses that belong to specific department
            printf("Enter the department name you want to delete all of it's courses:");
            getchar();
            scanf("%[^\n]",department);
            printLine();
            courses = deleteByDep(courses , department , s);//delete all the courses that belong to specific department
            PrintInOrder(courses);
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==10)
        {
            if(courses==NULL)
            {
                printf("There is no courses to save.\n");
            }
            else
            {
                saveInFile(courses);//save all the courses in a file
                printf("The courses have been saved successfully.\n");
            }
            printLine();
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
    }
    return 0;
}

void menu()
{
    printf("1. Load courses data from the file.\n"
           "2. Insert a new course.\n"
           "3. update course Information.\n"
           "4. List courses in lexicographic order.\n"
           "5. List all topics associated with a given course.\n"
           "6. List all courses in lexicographic order for specific department.\n"
           "7. Delete a course.\n"
           "8. Delete all courses that start with a specific letter.\n"
           "9. Delete all courses belong to a specific department.\n"
           "10. Save.\n"
           "11. Exit.\n");
}

void updateMenu()
{
     printf("choose what information you want to update on:\n"
            "1. Course code.\n"
            "2. Course name.\n"
            "3. Credit hours.\n"
            "4. Department.\n"
            "5. Topics.\n"
            "6. Done from updating.\n");
}

courseNode readCourseData(courseNode T)//this function is to read courses data from courses.txt file
                                      //and store the data in AVL tree (courses)
{
    char courseName [150];
    int creditHours = 0;
    char courseCode [50];
    char Department [150];
    char topics [10000];
    char line[10000];
    int length = 0;
    FILE *coursesFile;
    coursesFile = fopen("courses.txt","r");//open courses.txt file to read from it
    while(!feof(coursesFile))//while it's not the end of the file... keep reading
    {
        fgets(line,10000,coursesFile);//read line by line
        length = strlen(line);
        if(line[length-1]=='\n')//to avoid reading '\n' character
        {
            line[length-1] = '\0';
        }
        //spliting the line and store it in variables
        strcpy(courseName , strtok(line , ":"));
        creditHours = atoi(strtok(NULL,"#"));
        strcpy(courseCode , strtok(NULL , "#"));
        strcpy(Department , strtok(NULL , "/"));
        strcpy(topics , strtok(NULL , "\n"));
        //insert the data into the AVL tree
        T = Insert(courseName , creditHours , courseCode , Department ,topics , T);
    }
    fclose(coursesFile);//closing the file
    return T;
}

void saveInFile(courseNode T)//save courses in offered_courses.txt file
{
    FILE *output;
    output = fopen("offered_courses.txt","w");//open offered_courses.txt to write in it
    printInFile(T , output);
    fclose(output);//closing the file
}

void printInFile(courseNode T , FILE *output)//traverse the tree to print the courses info in the file
{
    if(T!=NULL)
    {
        printInFile(T->left , output);
        fprintf(output , "%s:%d#%s#%s/%s\n",T->courseName , T->creditHours , T->courseCode , T->Department , T->topics);
        printInFile(T->right , output);
    }
}

courseNode InsertNewCourse(courseNode T)//insert new course and store it in the AVL tree(courses)
{
    courseNode temp;
    char courseCode [150];
    char courseName [150];
    int creditHours = 0;
    char Department [150];
    char topics [10000];
    printf("Enter the code of the course:");
    scanf("%s",courseCode);
    temp = Find(courseCode , T);//find the course code to see if it exists
    if(temp!=NULL && strcmp(temp->courseCode , courseCode)==0)
    {
        printLine();
        printf("The course is already in the list.\n");
    }
    else if(temp==NULL)//if it is not in the tree add it
    {
        printf("Enter course name:");
        getchar();
        scanf("%[^\n]",courseName);
        printf("Enter the credit hours of the course:");
        scanf("%d",&creditHours);
        printf("Enter the department Name:");
        getchar();
        scanf("%[^\n]",Department);
        printf("Enter the topics (seperated with commas):");
        getchar();
        scanf("%[^\n]",topics);
        T = Insert(courseName , creditHours , courseCode , Department , topics , T);
        printLine();
        printf("The course has been added successfully.\n");
    }
    return T;
}

courseNode updateCourse(courseNode T)//update information of available courses
{
    char courseName [150];
    int creditHours = 0;
    char courseCode [50];
    char newCourseCode [50];
    char Department [150];
    char topics [10000];
    int choice = 0;
    printf("Enter the course code of the course you want to update:");
    scanf("%s",courseCode);
    printLine();
    courseNode temp = Find(courseCode , T);//find the the node of this course code
    if(temp!=NULL && strcmp(temp->courseCode , courseCode)==0)
    {
        printf("Course Name: %s , Course Code: %s , Credit Hours: %d , Department: %s\nTopics: %s\n",temp->courseName ,
                                                                                                     temp->courseCode ,
                                                                                                     temp->creditHours ,
                                                                                                     temp->Department ,
                                                                                                     temp->topics);
        printLine();
        updateMenu();
        printLine();
        scanf("%d",&choice);
        printLine();
        while(choice!=6)
        {
            if(choice==1)
            {
                //copying the data of the courses to temp variables
                strcpy(courseName , temp->courseName);
                creditHours = temp->creditHours;
                strcpy(Department , temp->Department);
                strcpy(topics , temp->topics);
                T = deleteNode(courseCode , T);//delete the node of the old course code
                printf("Enter the new course code:");
                scanf("%s",newCourseCode);
                T = Insert(courseName , creditHours , newCourseCode, Department , topics , T);//insert the the new course code
                                                                                             //node to keep the balance
                temp = Find(newCourseCode , T);//make the temp point on the new course node
                printf("The course code has been updated successfully.\n");
                printLine();
                updateMenu();
                printLine();
                scanf("%d",&choice);
                printLine();
            }
            else if(choice==2)
            {
                printf("Enter the new course name:");
                getchar();
                scanf("%[^\n]",courseName);
                strcpy(temp->courseName , courseName);//overwrite on the course name
                printf("The course name has been updated successfully.\n");
                printLine();
                updateMenu();
                printLine();
                scanf("%d",&choice);
                printLine();
            }
            else if(choice==3)
            {
                printf("Enter the new number of credit hours:");
                scanf("%d",&creditHours);
                temp->creditHours = creditHours; //overwrite on the credit hours
                printf("The credit hours has been updated successfully.\n");
                printLine();
                updateMenu();
                printLine();
                scanf("%d",&choice);
                printLine();
            }
            else if(choice==4)
            {
                printf("Enter the new department name:");
                getchar();
                scanf("%[^\n]",Department);
                strcpy(temp->Department , Department);//overwrite on the department name
                printf("The department name has been updated successfully.\n");
                printLine();
                updateMenu();
                printLine();
                scanf("%d",&choice);
                printLine();
            }
            else if(choice==5)
            {
               printf("Enter the new topics (seperated with commas):");
               getchar();
               scanf("%[^\n]",topics);
               strcpy(temp->topics , topics);//overwrite on the course topics
               printf("The topics have been updated successfully.\n");
               printLine();
               updateMenu();
               printLine();
               scanf("%d",&choice);
               printLine();
            }
            else
            {
                printf("you have to enter a number from the list only.\n");
                printLine();
                updateMenu();
                printLine();
                scanf("%d",&choice);
                printLine();
            }
        }
    }
    else//the course that the user want is not in the list
    {
        printf("The course you want is not in the list.\n");
        printLine();
    }
    return T;
}

void listTopics(courseNode T)//list all topics of a specific course
{
    char courseCode [150];
    printf("Enter the course code of the course you want to view it's topic list:");
    scanf("%s",courseCode);
    printLine();
    courseNode temp = Find(courseCode , T);//find the node that has the entered course code
    if(temp!=NULL)//if it exists... print the topics of it
    {
        printf("The topics list of %s are:\n",temp->courseName);
        printLine();
        printf("%s\n",temp->topics);
    }
    else
    {
        printf("The course you want is not in the list.\n");
    }
}

courseNode deleteNode(char courseCode [],courseNode T)//this function is used to delete nodes from the AVL tree depending on the course code
{
    courseNode temp;
    if(T==NULL)//if there is no created AVL tree
    {
        return T;
    }
    //if the entered course code is less than the course code of the node then go left
    else if(strcmp(courseCode , T->courseCode)<0)
    {
        T->left = deleteNode(courseCode , T->left);
    }
    //if the entered course code is greater than the course code of the node then go right
    else if(strcmp(courseCode , T->courseCode)>0)
    {
        T->right = deleteNode(courseCode , T->right);
    }
    //found the element we want to delete
    else if(T->left && T->right)//if the node we want to delete has 2 children
    {
        temp = findMin(T->right);
        strcpy(T->courseCode , temp->courseCode);
        strcpy(T->courseName , temp->courseName);
        T->creditHours = temp->creditHours;
        strcpy(T->Department , temp->Department);
        strcpy(T->topics , temp->topics);
        T->right = deleteNode(T->courseCode , T->right);
    }
    else//has one child or it's just a leaf node
    {
        temp = T;
        if(T->left==NULL)
        {
            T = T->right;
        }
        else if(T->right==NULL)
        {
            T = T->left;
        }
        free(temp);
    }

    if(T==NULL)//this means if the tree(subtree) has one node
    {
        return T;
    }

    T->height = 1 + Max(Height(T->left) , Height(T->left));//update on the height of the tree after deletion

    int balance = getBalance(T);

    //left left case:
    if(balance > 1 && getBalance(T->left)>=0)
    {
        return SingleRotateWithRight(T);
    }
    //right right case:
    else if(balance < -1 && getBalance(T->right)<=0)
    {
        return SingleRotateWithLeft(T);
    }
    //left right case:
    else if(balance > 1 && getBalance(T->left) < 0)
    {
        T->left = SingleRotateWithLeft(T->left);
        return SingleRotateWithRight(T);
    }
    //right left case:
    else if(balance < -1 && getBalance(T->right) > 0)
    {
        T->right = SingleRotateWithRight(T->right);
        return SingleRotateWithLeft(T);
    }
    return T;
}

void FillStackWithCC(courseNode T , char letter , struct Stack *S)//traverse the tree and fill the stack with the
                                                                 //code of the courses that starts with a specific letter
{
   if(T!=NULL)//traverse all the tree
   {
       FillStackWithCC(T->left , letter , S);
       if(T->courseName[0]==letter)//if it's the same letter then push the code of the course into the stack
       {
           Push(T->courseCode , S);
       }
       FillStackWithCC(T->right , letter , S);
   }
}

courseNode deleteByLetter(courseNode T , char letter , struct Stack* S)//delete all the courses the starts with a specific letter
{
    //traverse the tree
    FillStackWithCC(T , letter , S);
    int flag = 0;
    while(!IsEmpty(S))//while the stack is not empty.. keep poping from it and delete the courses of the poped codes
    {
        T = deleteNode(Pop(S) , T);
        flag = 1;
    }
    if(flag)
    {
        printf("The courses that start with '%c' have been deleted successfully.\n",letter);
    }
    else
    {
        printf("There is no courses start with '%c'.\n",letter);
    }
    return T;
}

void FillStackWithCC2(courseNode T , char department [] , struct Stack *S)//traverse the tree and fill the stack with the
                                                                         //code of the courses that belongs to a specific department
{
   //traverse the tree
   if(T!=NULL)
   {
       FillStackWithCC2(T->left , department , S);
       if(strcmp(T->Department , department)==0)//if it's the same department then push the code of the course into the stack
       {
           Push(T->courseCode , S);
       }
       FillStackWithCC2(T->right , department , S);
   }
}

courseNode deleteByDep(courseNode T , char department [] , struct Stack* S)//delete all the courses that belongs to a specific department
{
    FillStackWithCC2(T , department , S);
    int flag = 0;
    while(!IsEmpty(S))//while the stack is not empty.. keep poping from it and delete the courses of the poped codes
    {
        T = deleteNode(Pop(S) , T);
        flag = 1;
    }
    if(flag)
    {
        printf("The courses that belongs to %s have been deleted successfully.\n",department);
    }
    else
    {
        printf("%s department is not in the list.\n",department);
    }
    printLine();
    return T;
}

int getBalance(courseNode T)//get the balance of the given node
{
    if(T==NULL)
    {
        return 0;
    }
    return (Height(T->left) - Height(T->right));
}

courseNode findMin(courseNode T)//find the minimum node of the given tree
{
    if(T==NULL)
    {
        return NULL;
    }
    else if(T->left==NULL)
    {
        return T;
    }
    else
        return findMin(T->left);
}

courseNode MakeEmpty( courseNode T )//make the tree empty
{
    if( T != NULL )
    {
        MakeEmpty( T->left );
        MakeEmpty( T->right );
        free( T );
    }
    return NULL;
}

int Height( courseNode T )//return the height of the given tree
{
    if( T == NULL )
        return -1;
    else
        return T->height;
}

int Max( int Lhs, int Rhs )//find the max between the two given heights
{
    return Lhs > Rhs ? Lhs : Rhs;
}

courseNode SingleRotateWithLeft( courseNode T2 )//single rotate to left
{
    courseNode  T1;

    T1 = T2->left;
    T2->left = T1->right;
    T1->right = T2;

    T2->height = Max( Height( T2->left ), Height( T2->right ) ) + 1;//find the bigger height between the left subtree and
                                                                   //the right subtree then add 1
    T1->height = Max( Height( T1->left ), T2->height ) + 1;

    return T1;
}

courseNode SingleRotateWithRight( courseNode T1 )//single rotate to right
{
    courseNode T2;

    T2 = T1->right;
    T1->right = T2->left;
    T2->left = T1;

    T1->height = Max( Height( T1->left ), Height( T1->right ) ) + 1;//find the bigger height between the left subtree and
                                                                   //the right subtree then add 1
    T2->height = Max( Height( T2->right ), T1->height ) + 1;

    return T2;
}

courseNode DoubleRotateWithLeft( courseNode T3 )//double rotate with left case
{
    /* Rotate between T1 and T2 */
    T3->left = SingleRotateWithRight( T3->left );

    /* Rotate between T3 and T2 */
    return SingleRotateWithLeft( T3 );
}

courseNode DoubleRotateWithRight( courseNode T1 )//double rotate with right case
{
    /* Rotate between T3 and T2 */
    T1->right = SingleRotateWithLeft( T1->right );

    /* Rotate between T1 and T2 */
    return SingleRotateWithRight( T1 );
}

//insert new node into the AVL tree depending on the course code
courseNode Insert(char courseName[], int creditHours, char courseCode [], char Department [], char topics [], courseNode T )
{
    if( T == NULL )
    {
        /* Create and return a one-node tree */
        T = (courseNode)malloc(sizeof(struct course));
        if( T == NULL )
            printf("Out of space.\n");
        else
        {
            //copy the data into the node
            strcpy(T->courseName , courseName);
            T->creditHours = creditHours;
            strcpy(T->courseCode , courseCode);
            strcpy(T->Department , Department);
            strcpy(T->topics , topics);
            T->height = 0;
            T->left = T->right = NULL;
        }
    }
    else if(strcmp(courseCode , T->courseCode)<0)//if the entered course is less than the code of the node then go left
    {
        T->left = Insert(courseName , creditHours , courseCode , Department , topics , T->left );
        if( Height( T->left ) - Height( T->right ) == 2 )//adjust the balance
            if( strcmp(courseCode,T->left->courseCode)<0)//left left case
                T = SingleRotateWithLeft( T );
            else//right left case
                T = DoubleRotateWithLeft( T );
    }
    else if(strcmp(courseCode , T->courseCode)>0)//if the entered course is greater than the code of the node then go right
    {
        T->right = Insert(courseName , creditHours , courseCode , Department ,topics, T->right );
        if( Height( T->right ) - Height( T->left ) == 2 )//adjust the balance
            if(strcmp(courseCode ,T->right->courseCode)>0)//right right case
                T = SingleRotateWithRight( T );
            else//left right case
                T = DoubleRotateWithRight( T );
    }
    T->height = Max( Height( T->left ), Height( T->right ) ) + 1;//adjust the height of the tree after inserting new node on it
    return T;
}

courseNode InsertLexi(courseNode lexiT,courseNode T)//copying and inserting from the courses tree into
                                                   //lexiCourses tree depending on the course name
{
    if( lexiT == NULL )
    {
        /* Create and return a one-node tree */
        lexiT = (courseNode)malloc(sizeof(struct course));
        if( lexiT == NULL )
            printf("Out of space.\n");
        else
        {
            //copy the data from the node of courses tree into the node of lexiCourses
            strcpy(lexiT->courseName , T->courseName);
            lexiT->creditHours = T->creditHours;
            strcpy(lexiT->courseCode , T->courseCode);
            strcpy(lexiT->Department , T->Department);
            strcpy(lexiT->topics , T->topics);
            lexiT->height = 0;
            lexiT->left = lexiT->right = NULL;
        }
    }
    else if(strcmp(T->courseName , lexiT->courseName)<0)//if the entered course name is less than the name of the node then go left
    {
        lexiT->left = InsertLexi(lexiT->left, T);
        if( Height( lexiT->left ) - Height( lexiT->right ) == 2 )//adjust the balance
            if( strcmp(T->courseName,lexiT->left->courseName)<0)//left left case
                lexiT = SingleRotateWithLeft( lexiT );
            else//right left case
                lexiT = DoubleRotateWithLeft( lexiT );
    }
    else if(strcmp(T->courseName , lexiT->courseName)>0)//if the entered course name is greater than the name of the node then go right
    {
        lexiT->right = InsertLexi(lexiT->right, T);
        if( Height( lexiT->right ) - Height( lexiT->left ) == 2 )//adjust the balance
            if(strcmp(T->courseName ,lexiT->right->courseName)>0)//right right case
                lexiT = SingleRotateWithRight( lexiT );
            else//left right case
                lexiT = DoubleRotateWithRight( lexiT );
    }
    lexiT->height = Max( Height( lexiT->left ), Height( lexiT->right ) ) + 1;//adjust the height of the tree after inserting new node
    //traverse the tree
    if(T->left!=NULL)
    {
        lexiT = InsertLexi(lexiT , T->left);
    }
    if(T->right!=NULL)
    {
        lexiT = InsertLexi(lexiT , T->right);
    }
    return lexiT;
}

void coursesBelongToSameDep(courseNode T , char department [])//traverse the tree and print all the courses that
                                                             //belongs to the same department
{
    //traverse the tree
    if(T!=NULL)
    {
        coursesBelongToSameDep(T->left , department);
        if(strcmp(T->Department , department)==0)
        {
            printf("%s\n",T->courseName);
        }
        coursesBelongToSameDep(T->right , department);
    }
}

courseNode Find(char courseCode [], courseNode T )//find the node that has the entered course code
{
    if( T == NULL )//if the node is not found
    {
        return NULL;
    }
    if(strcmp(courseCode , T->courseCode)<0)//if the entered course code is less than the code of the node then go left
    {
        return Find(courseCode ,T->left);
    }
    else if(strcmp(courseCode , T->courseCode)>0)//if the entered course code is greater than the code of the node then go right
    {
        return Find(courseCode , T->right );
    }
    else//else the node is found
        return T;
}

void PrintInOrder(courseNode T)//traverse the tree inorder and print all the info of the nodes
{
    if( T != NULL)
    {
        PrintInOrder( T->left );
        printf("Course Code: %s \t Course Name: %s \t Credit Hours: %d \t Department: %s"
               "\nTopics: %s\n",T->courseCode,T->courseName ,T->creditHours, T->Department ,T->topics);
        printLine();
        PrintInOrder( T->right );
    }
}

int IsEmpty(struct Stack *S)//to know if the stack is empty
{
    return S->Next == NULL;
}

struct Stack* CreateStack()//create stack
{
    struct Stack *S;
    S = (struct Stack*)malloc(sizeof( struct Stack));
    if(S == NULL)
    printf("Out of space.\n");
    S->Next =NULL;
    return S;
}

void Push(char courseCode[],struct Stack *S)//push courses codes to the stack
{
    struct Stack* temp;
    temp = (struct Stack*)malloc(sizeof(struct Stack));
    if( temp == NULL)
        printf("Out of space.\n");
    else
    {
        strcpy(temp->courseCode , courseCode);
        temp->Next = S->Next;
        S->Next = temp;
    }
}

char * Pop(struct Stack *S)//return a pointer that points on the array of characters and delete the occupied cell
{
    struct Stack* firstCell;
    char *str;
    if(IsEmpty(S))
    {
        printf("Empty stack.\n");
    }
    else
    {
        str = malloc(strlen(S->Next->courseCode));//create a cell it's size is the length of the array of characters
        firstCell= S->Next;
        strcpy(str,S->Next->courseCode);
        S->Next = S->Next->Next;
        free( firstCell);//deleting the cell after poping the elements from it
    }
    return str;
}

void printLine()
{
    printf("-----------------------------------------------------------------"
           "--------------------------------------------------------------------\n");
}
