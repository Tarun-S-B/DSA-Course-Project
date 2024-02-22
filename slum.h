//Including all the .h files required
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include "settings.h"


/**-- USER DEFINED STRUCTURES*/

/**
resident structure stores the data of the residents
*/
struct resident
{
    int aadhaar;
    char fname[30];
    char lname[30];
    int age;
    char gender;
    char qualification[30];
    char occupation[30];
    int income;
};
typedef struct resident RES;
RES resident_data[MAX_POPULATION];
int resident_count = 0;

/**
resident occupation_stat stores the name of the occupation and the no of people who are working
*/
struct occupation_stat
{
    char occupation[100];
    int number;
    struct occupation_stat * left;
    struct occupation_stat * right;
};
typedef struct occupation_stat OCC;
OCC * occu_root;


/**
admin structure stores the username and encrypted password of the admins
*/
struct admin
{
    char username[20];
    char password[20];
};
struct admin admin[MAX_ADMIN];


/**
govt_scheme structure stores the data of the government schemes
*/
struct govt_scheme
{
    char scheme_name[50];
    char detail[100];
    int amount;
    int slots;
    int *beneficiary;

};
typedef struct govt_scheme SCH;
SCH SCHEME[10];
int scheme_count = 0;


/**
The traditional Queue data structure
*/
struct Queue
{
    int elements[100];
    int fron, rear;
};
typedef struct Queue QUEUE;


/**
node structure used to store the details of edges of the slum graph
*/
struct node
{
    int u;
    int v;
    int wt;
};
typedef struct node NODE;
NODE *edges;
int edge = 0;
int u_no,v_no;
int final_weight = 0;
int output_edge = 0;


//FILE pointers which will be used to access data files and log file
FILE *fp;
FILE *log_file;


//variables declared which will be used during the admin log ins
char username[50];
char pass[50];
unsigned long long int hash;


//variables used during the process of writing in log file
time_t current_time;
struct tm *local_time;
char time_now[50];

//The response after a function is executed is stored in this
int response = 0;

//Variables used in the disrtibution of age goups
int group[10] = {0};
int start = 0;
int end = 10;

//Variables used in the generation and analysis of a map of slum
int map_generated = 0;
int **map;
int **shortest_dist;
int **trans_house;
int floyd_applied;

//the in variables i,j,k used to iterate
int i,j,k;

//variables used in the djikstras algorithm
int v; //vertices (no of houses)
int visited[16];
int dist[16];
int path[16];
int u;
int distance;
int min;



//age quick sort
int piv;
int s;
int age_sorted[50];
int sorted_for_age = 0;


//String matching algorithm variables
char query[50];
char full_name[50];
int found;
char search_occupation[100];
char store_occupation[100];
int stored = 0;
int aadhaar_num;
int search_age;
char pat[100];
char txt[100];
int len_pat ;
int len_txt;
int hash_pat = 0; // hash value for pattern
int hash_txt = 0; // hash value for txt
int h = 1;
char pat[100];
char txt[100];
int m;
int n;
int badchar[256];
char ch;

//storing of the occuaptions data
OCC * p;
OCC *curr;
OCC *par;



//boyer moore
int admin_count = 0;
unsigned long long int pw;
int len;
int rem;

//name of the queue
QUEUE q;

//scheme alot heap
int heap_items = 0;

//in generate function
int dist_house;



//FUNCTION DECALRATION
void landing_page();
void give_date_time(char *);
void open_log();
void close_log();
void write_slum_log(char *, int , char *);
int load_residents();
int load_scheme();
int load();
int get_digits(unsigned long long int);
void read_password(char *);
unsigned long long int hash_password(char *);
int validate_credentials(char *user, char *);
int log_in();
int give_random_int(int);
int generate_map(int);
void display_slum();
void floyd();
int dist_between(int, int);
void warshall();
void identify_isolated();
void display_dist_houses(int);
int find(int [], int, int);
void Union(int a[], int, int, int);
void bubble_sort(struct node [], int);
void insertion_sort(NODE a[], int);
int shortest_path_houses();
int min_between(int, int);
int min_dist(int);
void djikstras(int);
int swap_int(int *,int *);
int partition_array( int A[],int,int);
void quick_sort(int a[], int, int);
void display_youngest(int *);
void display_oldest(int *);
void display_age_group(int *);
void word_to_lower(char constant[],char result[]);
int rabin_karp_search(char pattern[], char text[], int);
int query_name();
int query_aadhaar();
int query_age(int);
int max_between (int, int);
void bad_char_heuristic( char *, int, int badchar[alpha_size]);
int boyer_moore_bad( char *,  char *);
void display_resident(RES resident);
int load_occu_stat(char occupation[], int);
int search_occ(OCC *, char *);
void query_occupation();
void display_occupation_stat(OCC*);
void display_schemes();
void initQueue(struct Queue *);
void EnQueue(struct Queue *, int);
int is_full(struct Queue *);
int DeQueue(struct Queue *);
int isEmpty(struct Queue *);
int getFront(const struct Queue *);
int sizeQ(const struct Queue *);
void display_name(RES resident);
void swap_resident(RES *, RES *);
void max_heapify_resident();
void give_aged_funds(int *);
void min_heapify_resident();
void give_benefits(int);

