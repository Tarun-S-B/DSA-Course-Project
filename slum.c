#include "settings.h"
#include "slum.h"


void clear_screen()
{
    printf("\033[2J");  // ANSI escape code to clear screen
    printf("\033[H");   // ANSI escape code to move cursor to the top-left corner
    landing_page();
    //printf("User: %s\n",username);
}

void landing_page()
{

    printf("\t $$$$$$\\  $$\\       $$\\   $$\\ $$\\      $$\\  $$$$$$\\      \n");
    printf("\t$$  __$$\\ $$ |      $$ |  $$ |$$$\\    $$$ |$$  __$$\\       \n");
    printf("\t$$ /  \\__|$$ |      $$ |  $$ |$$$$\\  $$$$ |$$ /  \\__|      \n");
    printf("\t\\$$$$$$\\  $$ |      $$ |  $$ |$$\\$$\\$$ $$ |\\$$$$$$\\        \n");
    printf("\t \\____$$\\ $$ |      $$ |  $$ |$$ \\$$$  $$ | \\____$$\\       \n");
    printf("\t$$\\   $$ |$$ |      $$ |  $$ |$$ |\\$  /$$ |$$\\   $$ |      \n");
    printf("\t\\$$$$$$  |$$$$$$$$\\ \\$$$$$$  |$$ | \\_/ $$ |\\$$$$$$  |      \n");
    printf("\t \\______/ \\________| \\______/ \\__|     \\__| \\______/       \n");

    if(strlen(username)!=0)
    {
        printf("\n\t\t\tUSER: %s\n",username);
    }

    printf("\n-----------------------------------------------------------------------------\n");
}


/**
Function:     give_date_time
Description:  gives the current date and time
Input param:  the address of string where the date and time will be stored
Return Type:  NULL
**/
void give_date_time(char *str)
{
    time(&current_time);
    local_time = localtime(&current_time);

    strftime(time_now, sizeof(time_now), "%Y-%m-%d %H:%M:%S", local_time);

    strcpy(str,time_now);
}


/**
Function:       open_log
Description:    opens a file "slum_log" in append+ mode to store the activities done in the program
Input param:    NULL
Return Type:    NULL
*/
void open_log()
{
    log_file = fopen("slum_log.txt","a+");

    if(log_file)
        fprintf(log_file,"%s\n","_________________________START OF ITERATION________________________");
    else
        printf("Unable to open log file, no actions will be recorded\n");

    fprintf(log_file,"%s","\n\n");
}


/**
Function:       close_log
Description:    it closes the file opened by the open_log and marks the end of the execution of the current instance of program
Input param:    NULL
Return Type:    NULL
*/
void close_log()
{
    if(log_file)
        fprintf(log_file,"%s\n","_________________________END OF ITERATION________________________");
    else
        printf("Unable to fetch log file, no actions will be recorded\n");

    fprintf(log_file,"%s","\n\n");
    fclose(log_file);
}


/**
Function:       write_slum_log
Description:    it appends the record of the processes taken place in the program
Input param:    string  func_name-  name of the function under consideration
                int     code     -  the result code after the executionof the program
                string  result   -  a message describing the instance of the function execution
Return Type:    NULL
*/
void write_slum_log(char *func_name, int code, char *result)
{
    char time[50];
    give_date_time(time);
    fprintf(log_file,"\n%s\nFunction: %s\nStatus Code: %d -> %s\n\n",time,func_name,code,result);
}


/**
Function:       load_residents
Description:    Loads resident data from a file into the resident_data array
Input param:    NULL
Return Type:    int-    SUCCESS if data is loaded successfully
                        FAILURE otherwise
*/
int load_residents()
{
    fp = fopen("residents.txt","r");
    if(fp == NULL)
    {
        printf("File not found\n");
        write_slum_log("load_residents",FAILURE,"Resident Data Not Loaded");
        return FAILURE;
    }

    while(!feof(fp))
    {
        fscanf(fp,"%d%s%s%d%*c%c%s%s%d",
               &resident_data[resident_count].aadhaar,
               resident_data[resident_count].fname,
               resident_data[resident_count].lname,
               &resident_data[resident_count].age,
               &resident_data[resident_count].gender,
               resident_data[resident_count].qualification,
               resident_data[resident_count].occupation,
               &resident_data[resident_count].income);


//        printf("%d %s %s %d %c %s %s %d\n",
//               resident_data[resident_count].aadhaar,
//               resident_data[resident_count].fname,
//               resident_data[resident_count].lname,
//               resident_data[resident_count].age,
//               resident_data[resident_count].gender,
//               resident_data[resident_count].qualification,
//               resident_data[resident_count].occupation,
//               resident_data[resident_count].income);

        resident_count++;
    }

    fclose(fp);
    write_slum_log("load_residents",SUCCESS,"Resident Data Loaded");
    return SUCCESS;
}


/**
Function:       load_scheme
Description:    Loads scheme data from a file into the SCHEME array
Input param:    NULL
Return Type:    int-    SUCCESS if data is loaded successfully
                        FAILURE otherwise
*/
int load_scheme()
{
    fp = fopen("scheme.txt","r");

    if(fp==NULL)
    {
        write_slum_log("load_scheme",FAILURE,"Scheme Data Not Loaded");
        return FAILURE;
    }


    while(!feof(fp))
    {
        fscanf(fp,"%s %d %d %[^\n]s",
               SCHEME[scheme_count].scheme_name,
               &SCHEME[scheme_count].amount,
               &SCHEME[scheme_count].slots,
               SCHEME[scheme_count].detail);

//        printf("%s %d %d \n%s\n",
//               SCHEME[scheme_count].scheme_name,
//               SCHEME[scheme_count].amount,
//               SCHEME[scheme_count].slots,
//               SCHEME[scheme_count].detail);

        scheme_count++;
    }
    write_slum_log("load_scheme",SUCCESS,"Scheme Data Loaded");
    return SUCCESS;
}


/**
Function:       load
Description:    it opens the log_file which amrks the start of a instance of the program
Input param:    NULL
Return Type:    NULL
*/
int load()
{

    open_log();

    response = load_residents();

    if(response == 0)
    {
        printf("Residents' Data not loaded\n");
        return FAILED_TO_LOAD_FILE;
    }

    response = load_scheme();
    if(response == 0)
    {
        printf("Scheme's Data not loaded\n");
        return FAILED_TO_LOAD_FILE;
    }

}


/**
Function:       get_digits
Description:    Counts the number of digits in a given unsigned long long integer
Input param:    unsigned long long int n-   the integer for which digits are counted
Return Type:    int-                        the number of digits in the given unsigned long long int
*/
int get_digits(unsigned long long int n)
{
    return (int)log10(n) + 1;
}


/**
Function:       read_password
Description:    Reads a password from the user input without displaying characters on the screen
Input param:    char *password - a character array to store the entered password
Return Type:    NULL
*/
void read_password(char *password)
{
    printf("Enter password: ");
    int i;
    char ch;

    while (1)
    {
        ch = _getch();
        if(ch == 13)
        {
            password[i] = '\0';
            break;
        }
        else if(ch == 8 && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else
        {
            password[i] = ch;
            i++;
            printf("\033[1;32m*\033[0m");
        }
    }
}


/**
Function:       hash_password
Description:    Hashes a password using a custom hashing algorithm
Input param:    char *password-             a null-terminated string representing the password.
Return Type:    unsigned long long int-     the hashed value of the password.
*/
unsigned long long int hash_password(char *password)
{
    for(int i=0; password[i]; i++)
        password[i] += 13;

    hash = 5381;

    for(int i=0; password[i]; i++)
    {
        hash += ((unsigned long long int)(password[i] * 33) * hash ) + 9011  ;
    }
    return hash;
}


/**
Function:       validate_credentials
Description:    Validates user credentials by checking against stored usernames and hashed passwords
Input param:    char *user-     the username to be validated
                char *password- the password to be validated
Return Type:    int-            SUCCESS if credentials are valid,
                                FAILURE otherwise
*/
int validate_credentials(char *user, char *password)
{
    fp = fopen("credentials.txt","r");
    if(fp==NULL)
    {
        printf("file open failed\n");
        return 0;
    }
    int admin_count = 0;
    while(!feof(fp))
    {
        fscanf(fp,"%s %s",admin[admin_count].username,admin[admin_count].password);
        admin_count++;
    }

    fclose(fp);

    unsigned long long int pw = hash_password(password);
    int len = get_digits(pw);
    char code[len+1];

    int rem;
    code[len] = '\0';
    for(int i=len-1; i>=0; i--)
    {
        rem = pw % 10;
        code[i] = rem + 48;
        pw = pw/10;
    }


    for(int j=0; j<admin_count; j++)
    {
        if(strcmp(admin[j].username,user)==0)
        {
            if(strcmp(admin[j].password,code)==0)
            {
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}


void to_hash_str(char *pass)
{
    unsigned long long int pw = hash_password(pass);
    int len = get_digits(pw);
    char code[len+1];

    int rem;
    code[len] = '\0';
    for(int i=len-1; i>=0; i--)
    {
        rem = pw % 10;
        code[i] = rem + 48;
        pw = pw/10;
    }
    strcpy(pass,code);
    return;
}

void loading_animation()
{

    printf("Loading: __________\t0%%\r");
    usleep(200000);
    printf("\033[K");
    printf("Loading: *________\t3%%\r");
    usleep(400000);
    printf("\033[K");
    printf("Loading: ***______\t30%%\r");
    usleep(200000);
    printf("\033[K");
    printf("Loading: ***______\t33%%\r");
    usleep(400000);
    printf("\033[K");
    printf("Loading: *********\t99%%\r");
    usleep(200000);
    printf("\033[K");
    printf("Loading: **********\t100%%\r");
    usleep(1000000);
    printf("\033[K");

}

/**
Function:       log_in
Description:    Logs in a user by prompting for username and password, and validating credentials
Input param:    NULL
Return Type:    int-    SUCCESS if login is successful
                        FAILURE otherwise
*/
int log_in()
{
    printf("Enter username: \033[1;32m");
    scanf("%s",username);
    printf("\033[0m");
    read_password(pass);
    response = validate_credentials(username,pass);

    if(response == 1)
    {
        printf("\n");
        loading_animation();
        char message[50] = "Logged in as user: ";
        strcat(message,username);
        printf("\n%s\n",message);
        write_slum_log("log_in",SUCCESS,message);
        return SUCCESS;
    }
    else
    {
        char message[50] = "Log in failed as user: ";
        strcat(message,username);
        printf("\n\033[1;31m\%s\033[0m\n",message);
        write_slum_log("log_in",FAILURE,message);
        return FAILURE;
    }
}


/**
Function:       give_random_int
Description:    it returns a random integer which will be used to construct the slum area
Input param:    int max-    the upper limit of the random integer that will be generated
Return Type:    int-        the random number generated that is less than the parameter passed
*/
int give_random_int(int max)
{
    return rand() % max;
}


/**
Function:       generate_map
Description:    it constructs the plan of the slum, the distances between the houses
Input param:    int v-      the number of houses in the slum
Return Type:    int
                    -MEMORY_ALLOCATION_FAILED   if there is failure to allocate memory
                    -SUCCESS                    if map of slum is done successfully
*/
int generate_map(int v)
{
    map_generated = 1;
    floyd_applied = 0;

    //dynamical allocation of 2D array of integers
    map = (int **) malloc (sizeof(int *) * v);
    if(map == NULL)
    {
        printf("Failed\n");
        write_slum_log("generate_map",MEMORY_ALLOCATION_FAILED,"Failed to allocate memory for slum construction");
        return MEMORY_ALLOCATION_FAILED;
    }

    for(i=0; i<v; i++)
        map[i] = (int *) malloc (sizeof(int) * v);

    //setting a seed to generate different number in each iteration of loop
    srand((unsigned int)time(NULL));

    //to store the random number generated which will be the distance between the houses
    dist_house;
    for(i=0; i<v; i++)
    {
        for(j=i; j<v; j++)
        {
            if(i==j)
            {
                //when i=j it forms the self -loop
                map[i][j] = 0;
            }
            else
            {
                //adding 1 at the end to to avoid the distance between the houses as 0
                dist_house = give_random_int(MAX_DIST) + 1;

                //performing dist%3 != 0, so to avoid making connection between each house to every other house
                if(dist_house % 3 != 0)
                {
                    //since it is a undirectional graph, the matrix must be symmetrical about the major diagonal
                    map[i][j] = dist_house;
                    map[j][i] = dist_house;
                }
                else
                {
                    //if there is no connection between the houses we indicate it by taking distance as 10000
                    map[i][j] = 10000;
                    map[j][i] = 10000;
                }
            }
        }
    }
    write_slum_log("generate_map",SUCCESS,"Successfully completed the generation of slum mapping");
    return SUCCESS;
}


/**
Function:       display_slum
Description:    it displays the distances between the houses in a well structured and formatted manner
Input param:    NULL
Return Type:    NULL
*/
void display_slum()
{
    printf("Slum:\n");
    printf(" * |");
    for(i=0; i<v; i++)
    {
        if(i<=9)
            printf("  %d",i);
        else
            printf(" %d",i);
    }

    printf("\n");
    for(i=0; i<v; i++)
        printf("----");
    printf("\n");

    for(int i=0; i<v; i++)
    {
        printf(" %d | ",i);
        for(int j=0; j<v; j++)
        {
            if(map[i][j] == 10000)
                printf(" ~ ");
            else
            {
                if(map[i][j]>9)
                    printf("%d ",map[i][j]);
                else
                    printf(" %d ",map[i][j]);
            }

        }
        printf("\n");
    }
    write_slum_log("display_slum",SUCCESS,"Displayed the structure of slum");
}


/**
Function:       floyd
Description:    it is a standard floyd algorithm to find the distance shortest distance between the houses
Input param:    NULL
Return Type:    NULL
*/
void floyd()
{
    shortest_dist = (int **) malloc (sizeof(int *) * v);
    for(i=0; i<v; i++)
        shortest_dist[i] = (int *) malloc (sizeof(int) * v);

    for(i=0; i<v; i++)
    {
        for(j=0; j<v; j++)
        {
            shortest_dist[i][j] = map[i][j];
        }
    }

    for(k=0; k<v; k++)
    {
        for(i=0; i<v; i++)
        {
            for(j=0; j<v; j++)
                shortest_dist[i][j] = min_between(shortest_dist[i][j], (shortest_dist[i][k] + shortest_dist[k][j]));
        }
    }
    floyd_applied = 1;
}


/**
Function:       dist_between
Description:    it gives the distance between the houses in constant time after it has computed a lookup table using floyd algorithm
Input param:    int house_a-    the index of house(number of house which will be the source)
                int house_b-    the index of house(number of house which will be the destination)
Return Type:    int-            the distance between the house_a and house_b
*/
int dist_between(int house_a, int house_b)
{
    if(!floyd_applied)
        floyd();

    if(shortest_dist[house_a][house_b] == 10000)
        return PATH_NOT_FOUND;

    write_slum_log("dist_between",SUCCESS,"Got the distance between the two houses passed as arguments");
    return shortest_dist[house_a][house_b];
}


/**
 * Function:        warshall
 * Description:     Applies Warshall's algorithm to compute the transitive closure of the house graph.
 * Input param:     NULL
 * Return Type:     NULL
 */
void warshall()
{
    trans_house = (int **) malloc (sizeof(int *) * v);
    for(i=0; i<v; i++)
        trans_house[i] = (int *) malloc (sizeof(int) * v);

    for(i=0; i<v; i++)
    {
        for(j=0; j<v; j++)
        {
            if(map[i][j]!=0 && map[i][j]!=10000)
                trans_house[i][j] = 1;
            else
                trans_house[i][j] = 0;
        }
    }
}


/**
Function:       identify_isolated
Description:    Identifies isolated houses in the house graph using Warshall's algorithm.
Input param:    NULL
Return Type:    NULL
*/
void identify_isolated()
{
    warshall();
    for(k=0; k<v; k++)
    {
        for(i=0; i<v; i++)
        {
            for(j=0; j<v; j++)
            {
                trans_house[i][j] = trans_house[i][j] || ( trans_house[i][k] &&trans_house[k][j] );
            }
        }
    }
//
//    printf("\n\nWARSHALL GRAPH");
//    for(i=0; i<v; i++)
//    {
//        for(j=0;j<v; j++)
//        {
//            printf("%d ",trans_house[i][j]);
//        }
//        printf("\n");
//    }


    int flag = 0;
    for(i=0;i<v;i++)
    {
        flag = 0;
        for(j=0; j<v; j++)
        {
            if(trans_house[i][j]!=0)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
                printf("House %d is isolated\n",i);
    }
    write_slum_log("identify_isolated",SUCCESS,"Identified isolated houses if any");
}


/**
Function:       display_dist_houses
Description:    Displays the shortest distances from a given house to all other houses using Floyd's algorithm.
Input param:    int h_no-   the house number
Return Type:    NULL
*/
void display_dist_houses(int h_no)
{
    floyd();

    for(i=0; i<v; i++)
    {
        if(i != h_no)
            printf("%d->%d = %d\n",h_no,i,shortest_dist[h_no][i]);
    }
}


/**
Function:       find
Description:    Checks if two elements in an array have the same value.
Input params:   int a[]-    the array containing elements
                int u-      index of the first element to compare
                int v-      index of the second element to compare
Return Type:    int-        1 if the elements at indices u and v are equal
                            0 otherwise
*/
int find(int a[], int u, int v)
{
    return (a[u]==a[v]);
}


/**
Function:       Union
Description:    Performs the union operation on two elements in an array representing disjoint sets.
Input params:   int a-      the array representing disjoint sets
                int n-      the number of elements in the array
                int u-      index of the first element to be unioned
                int v-      index of the second element to be unioned
Return Type:    NULL
*/
void Union(int a[], int n, int u, int v)
{
    int temp = a[u];
    for(int i=0; i<n; i++)
    {
        if(a[i]==temp)
            a[i] = a[v];
    }
}


/**
Function:       insertion_sort
Description:    Sorts an array of structures using the insertion sort algorithm based on a specified field.
Input params:   NODE a[]-       the array of structures to be sorted
                int n-          the number of elements in the array
Return Type: void
*/
void insertion_sort(NODE a[], int n)
{
    NODE temp_node;

    for(i=1; i<=n-1; i++)
    {
        temp_node = a[i];
        j = i-1;
        while(j>=0 && a[j].wt>temp_node.wt)
        {
            a[j+1] = a[j];
            j = j - 1;
        }
        a[j+1] = temp_node;
    }
}


/**
Function:       shortest_path_houses
Description:    Finds the shortest path between houses in a graph using Kruskal's algorithm.
Input params:   NULL
Return Type:    int-        the total weight of the shortest path
                            MEMORY_ALLOCATION_FAILED if failed to allocate memory
*/
int shortest_path_houses()
{
    edges = (NODE *) malloc (sizeof(NODE)*v*v);

    if(edges == NULL)
    {
        printf("Memory not allocated");
        write_slum_log("shortest_path_houses",MEMORY_ALLOCATION_FAILED,"failed to allocate memory to the edg nodes");
        return MEMORY_ALLOCATION_FAILED;
    }

    final_weight = 0;
    output_edge = 0;

    edge = 0;
    for(i=0; i<v; i++)
    {
        for(j=i; j<v; j++)
        {
            if(map[i][j] != 0 && map[i][j]!=10000)
            {
                u_no = i;
                v_no = j;
                edges[edge].wt = map[i][j];

                if(u_no>v_no)
                {
                    edges[edge].u = v_no;
                    edges[edge].v = u_no;
                }
                else
                {
                    edges[edge].v = v_no;
                    edges[edge].u = u_no;
                }
                edge++;
            }
        }
    }

    insertion_sort(edges,edge-1);

    int points[v];
    for(i=0; i<v; i++)
        points[i] = i;

    NODE curr;
    for(i=0; i<edge; i++)
    {
        curr = edges[i];

        if(!find(points,points[curr.u],points[curr.v]))
        {
            printf("(%d,%d) \t-> \t%d\n",curr.u,curr.v,curr.wt);
            final_weight += curr.wt;
            output_edge++;
        }
        Union(points,v,curr.u,curr.v);
    }
    free(edges);
    return final_weight;
}


void display_neighbours(int src)
{
    printf("Neighbours:\t");
    for(i=0; i<v; i++)
    {
        if(map[src][i]!=0 && map[src][i]!=10000)
            printf("%d\t",i);
    }
    printf("\n");
}

/**
Function:       min_between
Description:    Finds the minimum value between two integers
Input param:    int a-      the first integer
                int b-      the second integer
Return Type:    int-        the minimum value between the two integers
*/
int min_between(int a, int b)
{
    if(a<b)
        return a;
    return b;
}


/**
Function:       min_dist
Description:    Finds the index of the vertex with the minimum distance value in an array
Input param:    int v-      the number of vertices
Return Type:    int-        the index of the vertex with the minimum distance value
*/
int min_dist(int v)
{
    int min = -1;
    for(int i=0; i<v; i++)
    {
        if(!visited[i])
            if((dist[min] > dist[i]) || min==(-1))
                min = i;
    }
    return min;
}


/**
Function:       djikstras
Description:    Computes the shortest distance from a source vertex to all other vertices using Dijkstra's algorithm
Input param:    int src-    the source vertex
Return Type:    NULL
*/
void djikstras(int src)
{
    for(i=0; i<v; i++)
    {
        visited[i] = 0;
        dist[i] = map[src][i];
        path[i] = src;
    }

    visited[src] = 1;
    for(int loop=0; loop<v; loop++)
    {
        u = min_dist(v);
        distance = dist[u];
        visited[u] = 1;

        for(int x=0; x<v; x++)
        {
            if(!visited[x])
            {
                min = min_between(dist[x],distance+map[u][x]);

                if(min!=dist[x])
                {
                    dist[x] = min;
                    path[x] = u;
                }
            }
        }
    }
    write_slum_log("djikstras",SUCCESS,"Distance between Water Source and houses calculated");
}



/**
Function:       swap_int
Description:    Swaps the values of two integers
Input param:    int *a-     pointer to the first integer
                int *b-     pointer to the second integer
Return Type:    int-        indicating successful swap
*/
int swap_int(int *a,int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;

    return SUCCESS;
}


/**
Function:       partition_array
Description:    Partitions an array based on age values of residents
Input param:    int A[]-    the array to be partitioned
                int start-  the starting index of the partition
                int end-    the ending index of the partition
Return Type:    int-        the position of the pivot after partitioning
*/
int partition_array( int A[],int start ,int end)
{
    i = start + 1;
    piv = resident_data[A[start]].age ;            //make the first element as pivot element.
    for(j =start + 1; j <= end ; j++ )
    {
    /*rearrange the array by putting elements which are less than pivot
       on one side and which are greater that on other. */

        if (resident_data[A[j]].age<piv)
        {
            swap_int(&A[i],&A[j]);
            i++;
        }
   }
   swap_int(&A[start],&A[i-1]) ;  //put the pivot element in its proper place.
   return i-1;                      //return the position of the pivot
}


/**
Function:       quick_sort
Description:    Performs quicksort on an array of indices based on the age values of residents
Input param:    int a[]-    the array of indices to be sorted
                int l-      the leftmost index of the array
                int r-      the rightmost index of the array
Return Type:    NULL
*/
void quick_sort(int a[], int l, int r)
{
    if(l<r)
    {
        s = partition_array(a,l,r);
        quick_sort(a,l,s-1);
        quick_sort(a,s+1,r);
    }
}


/**
Function:       display_youngest
Description:    Displays information about the youngest individual in the slum
Input param:    int *age_sorted-    an array containing indices of residents sorted by age
Return Type:    NULL
*/
void display_youngest(int *age_sorted)
{
    if(!sorted_for_age)
    {
        for(i=0; i<resident_count; i++)
            age_sorted[i] = i;
        quick_sort(age_sorted,0,resident_count-1);
        write_slum_log("quick_sort",SUCCESS,"Sorted in aascending order of age using quick sort");
    }
    printf("Youngest\nName: %s %s Age: %d\n",
                                           resident_data[age_sorted[1]].fname,
                                           resident_data[age_sorted[1]].lname,
                                           resident_data[age_sorted[1]].age);
    write_slum_log("display_youngest",SUCCESS,"Displayed Youngest individual of slum");
}



/**
Function:       display_oldest
Description:    Displays information about the oldest individual in the slum
Input param:    int *age_sorted-    an array containing indices of residents sorted by age
Return Type:    NULL
*/
void display_oldest(int *age_sorted)
{
    if(!sorted_for_age)
    {
        for(i=0; i<resident_count; i++)
            age_sorted[i] = i;
        quick_sort(age_sorted,0,resident_count-1);
        write_slum_log("quick_sort",SUCCESS,"Sorted in ascending order of age using quick sort");
    }
    printf("Oldest\nName: %s %s Age: %d\n",
                                           resident_data[age_sorted[resident_count-1]].fname,
                                           resident_data[age_sorted[resident_count-1]].lname,
                                           resident_data[age_sorted[resident_count-1]].age);
    write_slum_log("display_oldest",SUCCESS,"Displayed Oldest individual of slum");
}


/**
Function:       display_age_group
Description:    Displays the population distribution according to age groups in the slum
Input param:    int *age_index-     an array containing indices of residents sorted by age
Return Type:    NULL
*/
void display_age_group(int *age_index)
{
//    printf("age group data:\n");
//    for(int i=0; i<resident_count; i++)
//    printf("%d ",resident_data[age_index[i]].age / 10);
//    printf("\n\n");

    for(i=0;i<10;i++)
        group[i] = 0;

    for(i=0; i<resident_count; i++)
    {
        group[((resident_data[i].age)/10)]++; //= group[ ((resident_data[i].age)/10) ] + 1;
    }

    start = 0;
    end = 10;

    printf("\nAge group: Population\n");
    for(i=0; i<10; i++)
    {
        printf("(%d,%d)\t->\t%d\n",start,end,group[i]);
        start+=10;
        end+=10;
    }
    printf("\n");
    write_slum_log("display_age_group",SUCCESS,"Displayed Population According to age group");
    return;
}


/**
Function:       word_to_lower
Description:    Converts a word to lowercase and stores the result in another array
Input param:    char constant[]-    the word to be converted to lowercase
                char result[]-      the array to store the lowercase result
Return Type:    NULL
*/
void word_to_lower(char constant[],char result[])
{
    strcpy(result,constant);

    for(int i=0; result[i]; i++)
        result[i] = tolower(result[i]);
}


/**
Function:       rabin_karp_search
Description:    Performs the Rabin-Karp substring search algorithm
Input param:    char text[]-    the text in which to search for the pattern
                char pattern[]- the pattern to search for
                int q-          a prime number for hashing
Return Type:    int-            SUCCESS if the pattern is found in the text
                                FAILURE otherwise
*/
int rabin_karp_search(char pattern[], char text[], int q)
{
    char pat[100];
    char txt[100];
    word_to_lower(pattern,pat);
    word_to_lower(text,txt);

    len_pat = strlen(pat);
    len_txt = strlen(txt);

    hash_pat = 0; // hash value for pattern
    hash_txt = 0; // hash value for txt
    h = 1;

    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < len_pat - 1; i++)
        h = (h * alpha_size) % q;

    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < len_pat; i++) {
        hash_pat = (alpha_size * hash_pat + pat[i]) % q;
        hash_txt = (alpha_size * hash_txt + txt[i]) % q;
    }

    // Slide the pattern over text one by one
    for (i = 0; i <= len_txt - len_pat; i++) {

        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        if (hash_pat == hash_txt) {
            /* Check for characters one by one */
            for (j = 0; j < len_pat; j++) {
                if (txt[i + j] != pat[j])
                    break;
            }

            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1]
            if (j == len_pat)
                return SUCCESS;
        }

        // Calculate hash value for next window of text: Remove
        // leading digit, add trailing digit
        if (i < len_txt - len_pat) {
            hash_txt = (alpha_size * (hash_txt - txt[i] * h) + txt[i + len_pat]) % q;

            // We might get negative value of t, converting it
            // to positive
            if (hash_txt < 0)
                hash_txt = (hash_txt + q);
        }
    }
    return FAILURE;
}


/**
Function:       query_name
Description:    Searches for residents by name using the Rabin-Karp algorithm
Input param:    NULL
Return Type:    int-    DATA_FOUND if residents are found
                        DATA_NOT_FOUND otherwise
*/
int query_name()
{
    printf("Enter Name: ");
    scanf("%s",query);
    found = 0;
    for(int i=0; i<resident_count; i++)
    {
        strcpy(full_name,resident_data[i].fname);
        strcat(full_name,resident_data[i].lname);
        if(rabin_karp_search(query,full_name,101))
        {
            display_resident(resident_data[i]);
            found++;
        }
    }
    if(found==0)
    {
        printf("No Data Found\n");
        write_slum_log("query_name",DATA_NOT_FOUND,"No residents Found");
        return DATA_NOT_FOUND;
    }
    else
    {
        write_slum_log("query_name",DATA_FOUND,"Residents Found");
        return DATA_FOUND;
    }
}


/**
Function:       query_aadhaar
Description:    Searches for a resident by Aadhaar Number
Input param:    NULL
Return Type:    int-    The index of the resident if found
                        DATA_NOT_FOUND otherwise
*/
int query_aadhaar()
{
    printf("Enter Aadhaar Number: ");
    scanf("%d",&aadhaar_num);

    for(i=0; i<resident_count; i++)
    {
        if(resident_data[i].aadhaar == aadhaar_num)
        {
            write_slum_log("query_aadhaar",SUCCESS,"Resident Found");
            return i;
        }
    }
    write_slum_log("query_aadhaar",DATA_NOT_FOUND,"Resident Not Found");
    return DATA_NOT_FOUND;
}


/**
Function:       query_age
Description:    Searches for residents by age
Input param:    int search_age-     the age to search for
Return Type:    int-                DATA_FOUND if residents are found
                                    DATA_NOT_FOUND otherwise
*/
int query_age(int search_age)
{
    printf("Enter age: ");
    scanf("%d",&search_age);
    found = 0;

    for(i=0; i<resident_count; i++)
    {
        if(resident_data[i].age == search_age)
        {
            display_resident(resident_data[i]);
            found++;
        }
    }
    if(found==0)
    {
        write_slum_log("query_age",DATA_NOT_FOUND,"No residents Found");
        return DATA_NOT_FOUND;
    }
    else
    {
        write_slum_log("query_age",DATA_FOUND,"Residents Found");
        return DATA_FOUND;
    }
}


/**
Function:       max_between
Description:    Returns the maximum value between two integers
Input param:    int a-  the first integer
                int b-  the second integer
Return Type:    int-    the maximum value between a and b
*/
int max_between (int a, int b)
{
    if(a>b)
        return a;
    return b;
}


/**
Function:       badCharHeuristic
Description:    Preprocesses the bad character heuristic array for the Boyer-Moore string search algorithm
Input param:    char *str-                     the pattern for preprocessing
                int size-                   the size of the pattern
                int badchar[alpha_size]-    the bad character heuristic array
Return Type:    NULL
*/
void bad_char_heuristic( char *str, int size, int badchar[alpha_size])
{
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < alpha_size; i++)
         badchar[i] = -1;

    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
         badchar[(int) str[i]] = i;
}


/**
Function:       boyer_moore_bad
Description:    Implementation of the Boyer-Moore string search algorithm with bad character heuristic
Input param:    char *text-     the text to be searched
                char *pattern-  the pattern to search for
Return Type:    int-            SUCCESS if the pattern is found
                                FAILURE otherwise
*/
int boyer_moore_bad( char *text,  char *pattern)
{
    char pat[100];
    char txt[100];

    word_to_lower(text,txt);
    word_to_lower(pattern,pat);

    int m = strlen(pat);
    int n = strlen(txt);

    int badchar[alpha_size];

    /* Fill the bad character array by calling
       the preprocessing function badCharHeuristic()
       for given pattern */
    bad_char_heuristic(pat, m, badchar);


    int s = 0;  // s is shift of the pattern with
                // respect to text
    while(s <= (n - m))
    {
        int j = m-1;

        /* Keep reducing index j of pattern while
           characters of pattern and text are
           matching at this shift s */
        while(j >= 0 && pat[j] == txt[s+j])
            j--;

        /* If the pattern is present at current
           shift, then index j will become -1 after
           the above loop */
        if (j < 0)
        {
//            printf("\n pattern occurs at shift = %d", s);
            return SUCCESS;
            /* Shift the pattern so that the next
               character in text aligns with the last
               occurrence of it in pattern.
               The condition s+m < n is necessary for
               the case when pattern occurs at the end
               of text */
            //s +=
            if(s+m < n)
                s += m-badchar[txt[s+m]];
            else
                s += 1;
            //(s+m < n)? m-badchar[txt[s+m]] : 1;

        }

        else
            /* Shift the pattern so that the bad character
               in text aligns with the last occurrence of
               it in pattern. The max function is used to
               make sure that we get a positive shift.
               We may get a negative shift if the last
               occurrence  of bad character in pattern
               is on the right side of the current
               character. */
            s += max_between(1, j - badchar[txt[s+j]]);
    }

    return FAILURE;
}


/**
Function:       display_resident
Description:    Displays information about a resident
Input param:    RES resident-   a structure containing information about a resident.
Return Type:    NULL
*/
void display_resident(RES resident)
{
    printf("\nAadhaar Num: %d\nName: %s %s\nAge: %d\nGender: %c\nQualification: %s\nOccupation: %s\nIncome: %d\n",
               resident.aadhaar,
               resident.fname,
               resident.lname,
               resident.age,
               resident.gender,
               resident.qualification,
               resident.occupation,
               resident.income);
}


/**
Function:       load_occu_stat
Description:    Loads occupation statistics into a binary search tree
Input param:    char occupation[]-  the occupation to be loaded
                int count-          the count associated with the occupation
Return Type:    int-                SUCCESS if the occupation is loaded successfully,
                                    MEMORY_ALLOCATION_FAILED if memory allocation fails,
                                    DATA_ALREADY_EXISTS if the occupation already exists in the tree
*/
int load_occu_stat(char occupation[], int count)
{
    OCC * p = (OCC *) malloc(sizeof(OCC));

    if(p == NULL)
    {
        write_slum_log("load_occu_stat",MEMORY_ALLOCATION_FAILED,"Failed to allocate memory");
        return MEMORY_ALLOCATION_FAILED;
    }

    p->left = NULL;
    p->right = NULL;
    p->number = count;
    strcpy(p->occupation,occupation);

    if(occu_root == NULL)
    {
        occu_root = p;
        write_slum_log("load_occu_stat",SUCCESS,"Occupation data entered successfully");
        return SUCCESS;
    }
    else
    {
        OCC *curr = occu_root;
        OCC *par = NULL;

        while(curr)
        {
            par = curr;
            if(strcasecmp(occupation,curr->occupation) >= 1 )
                curr = curr->right;
            else if(strcasecmp(occupation,curr->occupation) <= -1 )
                curr = curr->left;
            else if(strcasecmp(occupation,curr->occupation) == 0)
            {
                free(p);
                write_slum_log("load_occu_stat",DATA_ALREADY_EXISTS,"Occupation data already exists");
                return DATA_ALREADY_EXISTS;
            }
        }

        if(strcasecmp(occupation,par->occupation)<=-1)
            par->left = p;
        else if(strcasecmp(occupation,par->occupation)>=1)
            par->right = p;

    }
    write_slum_log("load_occu_stat",SUCCESS,"Occupation data entered successfully");
    return SUCCESS;
}


/**
Function:       search_occ
Description:    Searches for an occupation in a binary search tree
Input param:    OCC* root-          the root of the binary search tree
                char *occupation-   the occupation to be searched
Return Type:    int-                SUCCESS if the occupation is found
                                    FAILURE otherwise
*/
int search_occ(OCC *root, char *occupation)
{
    OCC *curr = root;

    while(curr)
    {
        if(strcasecmp(root->occupation,occupation) >= 1)
            curr = curr->left;
        else if(strcasecmp(root->occupation,occupation) >= -1)
            curr = curr->right;
        else if(strcasecmp(root->occupation,occupation) == 0)
            return SUCCESS;
    }
    return FAILURE;
}


/**
Function:       query_occupation
Description:    Queries residents based on occupation and updates occupation statistics
Input param:    NULL
Return Type:    NULL
*/
void query_occupation()
{
    printf("Enter Occupation: ");
    scanf("%s",search_occupation);

    found = 0;
    stored = 0;

    for(int i=0; i<resident_count; i++)
    {
        if(boyer_moore_bad(resident_data[i].occupation,search_occupation))
        {
            found++;
            display_resident(resident_data[i]);
            if(!stored)
                strcpy(store_occupation,resident_data[i].occupation);
        }
    }

    if(found != 0)
    {
        load_occu_stat(store_occupation,found);
        write_slum_log("query_name",DATA_FOUND,"Residents Found");
    }
    else
        write_slum_log("query_name",DATA_NOT_FOUND,"No residents Found");
}


/**
Function:       display_occupation_stat
Description:    Displays occupation statistics stored in a binary search tree
Input param:    OCC* root - the root of the binary search tree
Return Type:    NULL
*/
void display_occupation_stat(OCC* root)
{
    if(root == NULL)
        return;
    printf("%s: %d\n",root->occupation,root->number);
    display_occupation_stat(root->left);
    display_occupation_stat(root->right);
}


/**
Function:       display_schemes
Description:    Displays information about various governments' schemes
Input param:    NULL
Return Type:    NULL
*/
void display_schemes()
{
    printf("\nDisplaying Government Schemes:\n");
    for(int i=0; i<scheme_count;  i++)
    {
        printf("%d: %s\nDesc: %s\nAmount: %d\tSlots:%d\n\n",
               i+1,SCHEME[i].scheme_name,
               SCHEME[i].detail,
               SCHEME[i].amount,SCHEME[i].slots);
    }
}


/**
Function:       initQueue
Description:    initalise the data structure queue
Input param:    the address of the queue
Return Type:    NULL
*/
void initQueue(struct Queue *q)
{
    q->fron = 0;
    q->rear = -1;
}


/**
Function:       EnQueue
Description:    Adds a element to the start of the end od the queue
Input param:    struct Queue *  q-  the address of the queue
                int             e-  the integer that will be added to the end of queue
Return Type:    NULL
*/
void EnQueue(struct Queue *q, int e)
{
    q->rear ++;
    q->elements[q->rear] = e;
}


/**
Function:       is_full
Description:    determines if the queue is full of not
Input param:    struct Queue *  q-  the address of the queue
Return Type:    int-    1 if queue is full
                        0 if queue is not full
*/
int is_full(struct Queue *q)
{
    return q->rear == QUEUE_SIZE - 1;
}


/**
Function:       DeQueue
Description:    Removes the element of the start of the queue
Input param:    struct Queue *  q-  the address of the queue
Return Type:    int-    returns the element which is in the start of the queue
*/
int DeQueue(struct Queue *q)
{
    int e = q->elements[q->fron];
    q->fron++;
    return e;
}


/**
Function:       isEmpty
Description:    determines if the queue is empty of not
Input param:    struct Queue *  q-  the address of the queue
Return Type:    int-    1 if queue is empty
                        0 if queue is not empty
*/
int isEmpty(struct Queue *q)
{
    if (q->fron > q->rear)
        return 1;
    else
        return 0;
}


/**
Function:       getFront
Description:    gives the element which is in the beginning of the queue
Input param:    struct Queue *  q-  the address of the queue
Return Type:    int-    the element that is in front of the queue
*/
int getFront(const struct Queue *q)
{
    return (q->elements[q->fron]);
}

/**
Function:       sizeQ
Description:    determines the size of the queue, or the numbe rof elements present in the queue
Input param:    struct Queue *  q-  the address of the queue
Return Type:    int-    number of elements in the queue
*/
int sizeQ(const struct Queue *q)
{
    return ((q->rear)-(q->fron)+1);
}


/**
Function:       display_name
Description:    Displays the name of the resident
Input param:    RES resident-   a structure containing information about a resident.
Return Type:    NULL
*/
void display_name(RES resident)
{
    printf("Name: %s %s\n",
            resident.fname,
            resident.lname);
}


/**
Function:       swap_resident
Description:    Swaps two RES structures.
Input param:    RES *a-     pointer to the first RES structure.
                RES *b-     pointer to the second RES structure.
Return Type:    NULL
 */
void swap_resident(RES *a, RES *b)
{
    RES temp;
    temp = *a;
    *a = *b;
    *b = temp;
}


/**
 Function:      max_heapify_resident
 Description:   Performs max heapify operation on the resident_data array based on age.
 Input param:   NULL
 Return Type:   NULL
 */
void max_heapify_resident()
{
    sorted_for_age = 0;
    int i,k,good,j;
    RES v;
    int n = heap_items;
    for(i=ceil(n/2)-1; i>=0; i--)
    {
        k = i;
        v = resident_data[k];
        good = 0;

        while(!good && 2*k<=n)
        {
            j = 2*k+1;
            if(j<n)
                if(resident_data[j+1].age>resident_data[j].age)
                    j = j+1;

            if(v.age>=resident_data[j].age)
                good = 1;
            else
            {
                resident_data[k] = resident_data[j];
                k = j;
            }
        }
        resident_data[k] = v;
    }
}


/**
Function:       give_aged_funds
Description:    Displays the name of the resident with the highest age and updates the heap.
Input param:    int *heap_items-    pointer to the variable representing the number of items in the heap.
Return Type:    NULL
*/
void give_aged_funds(int *heap_items)
{
    if(resident_data[0].age > 60)
    {
        display_name(resident_data[0]);
        swap_resident(&resident_data[0],&resident_data[*heap_items]);
        (*heap_items)--;
        max_heapify_resident();
    }
}


/**
Function:       min_heapify_resident
Description:    Performs min heapify operation on the resident_data array based on age.
Input param:    NULL
Return Type:    NULL
*/
void min_heapify_resident()
{
    sorted_for_age = 0;
    int i,k,good,j;
    RES v;
    int n = heap_items;
    for(i=ceil(n/2)-1; i>=0; i--)
    {
        if(resident_data[i].age == 0)
            printf("%d",i);
        k = i;
        v = resident_data[k];
        good = 0;

        while(!good && 2*k<=n)
        {
            j = 2*k+1;
            if(j<n)
                if(resident_data[j+1].age<resident_data[j].age)
                    j = j+1;

            if(v.age<=resident_data[j].age)
                good = 1;
            else
            {
                resident_data[k] = resident_data[j];
                k = j;
            }
        }
        resident_data[k] = v;
    }
}


/**
Function:       give_benefits
Description:    Allocates benefits based on the selected scheme.
Input param:    int- n:     An integer representing the selected scheme
                            1 for Student Scholarship Fund
                            2 for Old Age Fund
Return Type:    NULL
*/
void give_benefits(int n)
{
    switch(n)
    {
        case 1: printf("\n\nAllocating Student Scholarship Fund\n");
                QUEUE q;

                initQueue(&q);

                for(int i=0; i<resident_count; i++)
                {
                    if(boyer_moore_bad("Student",resident_data[i].occupation))
                        EnQueue(&q,i);
                }

                printf("\nStudents Who Got Scholarship:\n");
                for(int i=0; i<SCHEME[n-1].slots;i++)
                {
                    display_name(resident_data[DeQueue(&q)]);
                }

                printf("\nStudents Who didn't get Scholarship:\n");
                for(int i=0; i<SCHEME[n-1].slots;i++)
                {
                    display_name(resident_data[DeQueue(&q)]);
                }
                write_slum_log("give_benefits",SUCCESS,"Government funds distributed to children");
                break;

        case 2: printf("\n\nAllocating Old Age Fund\n");
                heap_items = resident_count;
                max_heapify_resident();


                printf("Paid funds to:\n");
                for(i=0; i<SCHEME[n-1].slots; i++)
                    give_aged_funds(&heap_items);
                write_slum_log("give_benefits",SUCCESS,"Government funds distributed to aged residents");
                break;

    }
}




