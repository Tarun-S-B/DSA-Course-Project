#include "slum.c"
#include "locals.h"


void give_choices()
{
    printf("\033[1;34m\n<------------ MAIN MENU ---------->\033[0m\n\n");

    printf("0. Slum Design\n");
    printf("1. Place a water Source\n");
    printf("2. Length of pipeline to be laid if\n");
    printf("3. Age Related Query\n");
    printf("4. Details of resident\n");
    printf("5. Government Schemes\n");
    printf("9. Show Menu\n");
}


int main()
{
    landing_page();
    load();

    while(1)
    {
        response = log_in();
        if(response)
            break;
        else
        {
            printf("\nRetry(1) Exit(0): ");
            scanf("%d",&sub_ch);

            if(sub_ch == 0)
            {
                close_log();
                exit(0);
            }
        }
    }


    while(1)
    {
        //clear_screen();
        give_choices();
        printf("\033[1;32mEnter Choice: \033[0m");
        scanf("%d",&choice);

        switch(choice)
        {
            case 0: sub_ch = -1;
                    while(sub_ch!=8)
                    {
                        printf("\033[1;34m\n<------------ Sub - Menu ---------->\033[0m\n\n");
                        printf("1. Generate Slum\n");
                        printf("2. Display Slum\n");
                        printf("3. Shortest Distance b/w houses\n");
                        printf("4. Identify Isolated/DeadEnded Houses \n");
                        printf("5. Shortest Distance to visit all Houses\n");
                        printf("6. Path to travel all houses in shortest distance\n");
                        printf("7. Neighbours \n");
                        printf("8. Back\n");

                        printf("\033[1;32mEnter Option: \033[0m");
                        scanf("%d",&sub_ch);

                        switch(sub_ch)
                        {
                            case 1: gen_slum:
                                        printf("Enter no of houses: ");
                                        scanf("%d",&v);
                                        generate_map(v);
                                        break;

                            case 2: display_slum();
                                    break;

                            case 3: printf("Enter House '1': ");
                                    scanf("%d",&h_i);

                                    if(h_i>=v)
                                    {
                                        printf("Invalid house number\n");
                                        break;
                                    }

                                    printf("Enter House '2': ");
                                    scanf("%d",&h_ii);

                                    if(h_ii>=v)
                                    {
                                        printf("Invalid house number\n");
                                        break;
                                    }
                                    response = dist_between(h_i,h_ii);
                                    if(response == PATH_NOT_FOUND)
                                        printf("No Path %d -> %d\n",h_i,h_ii);
                                    else
                                        printf("Shortest distance b/w %d -> %d = %d\n",h_i,h_ii,response);
                                    break;

                            case 4: identify_isolated();
                                    break;

                            case 5: printf("Enter house_no: ");
                                    scanf("%d",&h_no);
                                    if(h_no<v)
                                        display_dist_houses(h_no);
                                    else
                                        printf("Invalid house number");
                                    break;

                            case 6: response = shortest_path_houses();
                                    if(response != MEMORY_ALLOCATION_FAILED)
                                        printf("Total distance to be travelled = %d\n",response);
                                    break;

                            case 7: printf("Enter House No: ");
                                    scanf("%d",&h_no);
                                    if(h_no<v)
                                        display_neighbours(h_no);
                                    else
                                        printf("Invalid House Number\n");
                                    break;

                            case 8: printf("\033[1;34m\n<------------Back to main menu---------->\033[0m\n\n");
                                    break;
                        }
                    }
                    break;


            case 1: house_num:
                    if(!map_generated)
                    {
                        printf("\033[1;31mPlease Generate Slum: \033[0m\n");
                        goto gen_slum;
                    }


                        printf("Enter the house no: ");
                        scanf("%d",&h_no);
                        if(h_no>=v)
                        {
                            printf("Invaild House No!\n");
                            goto house_num;
                        }
                        printf("Enter Radius of Water Source: ");
                        scanf("%d",&radius);

                    djikstras(h_no);
                    printf("The List of Houses within the radius %d of Water source at %d:\n",radius,h_no);

                    n = 0;
                    for(int i=0; i<v; i++)
                    {
                        if(dist[i] <= radius && dist[i]!=0)
                        {
                            printf("House: %d Dist: %d\n",i,dist[i]);
                            n++;
                        }
                    }
                    if(n==0)
                        printf("No house within the range of water source!\n");

                    water_source_placed = 1;
                    break;


            case 2: if(water_source_placed)
                    {
                        printf("Use existing water source? yes(1) no(0): ");
                        scanf("%d",&sub_ch);

                        if(sub_ch == 1)
                        {
                            length = 0;
                            for(int i=0; i<v; i++)
                            {
                                if(dist[i] <= radius)
                                    length += dist[i];
                            }
                            printf("Total Length of Pipeline to be laid = %d units\n",length);
                        }
                        else
                            goto place_water_source;
                    }
                    else
                    {
                        if(!map_generated)
                        {
                            printf("\033[1;31mPlease Generate Slum!\033[0m\n");
                            goto gen_slum;
                        }
                        place_water_source:

                        printf("Enter the house no: ");
                        scanf("%d",&h_no);
                        printf("Enter Radius of Water Source: ");
                        scanf("%d",&radius);

                        djikstras(h_no);
                        n = 0;
                        length = 0;
                        for(int i=0; i<v; i++)
                        {
                            if(dist[i] <= radius)
                                length += dist[i];
                        }
                        printf("Total Length of Pipeline to be laid = %d units\n",length);

                        water_source_placed = 1;
                    }
                    break;

            case 3: sub_ch = -1;
                    while(sub_ch!=4)
                    {
                        printf("\033[1;34m\n<------------ Sub - Menu ---------->\033[0m\n\n");
                        printf("1. Display Name of The Youngest Resident in slum: \n");
                        printf("2. Display Name of The Oldest Resident in slum: \n");
                        printf("3. Display no of residents in age group: \n");
                        printf("4. Back\n");

                        printf("\n\033[1;32mEnter Option: \033[0m");
                        scanf("%d",&sub_ch);

                        switch(sub_ch)
                        {
                            case 1: display_youngest(age_sorted);
                                    break;

                            case 2: display_oldest(age_sorted);
                                    break;

                            case 3: display_age_group(age_sorted);
                                    break;

                            case 4: printf("\033[1;34m\n<------------Back to main menu---------->\033[0m\n\n");
                                    break;

                            default: break;
                        }

                    }
                    break;

            case 4: sub_ch = -1;
                    while(sub_ch != 6)
                    {
                        printf("\033[1;34m\n<------------ Sub - Menu ---------->\033[0m\n\n");
                        printf("Enter Parameter to search for:\n");
                        printf("1. Name\n");
                        printf("2. Aadhaar Number\n");
                        printf("3. Age\n");
                        printf("4. Occupation\n");
                        printf("5. Category of Occupation(precomputed only)\n");
                        printf("6. Back\n");

                        printf("\n\033[1;32mEnter Option: \033[0m");
                        scanf("%d",&sub_ch);

                        switch(sub_ch)
                        {
                            case 1: query_name();
                                    break;

                            case 2: response = query_aadhaar();
                                    if(response != DATA_NOT_FOUND)
                                        display_resident(resident_data[i]);
                                    else
                                        printf("No Resident Found\n");
                                    break;

                            case 3: response = query_age(search_age);
                                    if(response == DATA_NOT_FOUND)
                                        printf("No Resident Found\n");
                                    break;

                            case 4: query_occupation();
                                    break;

                            case 5: //compute_occupation_stat(occu_root);
                                    display_occupation_stat(occu_root);
                                    break;

                            case 6: printf("\033[1;34m<------------Back to main menu---------->\033[0m\n");
                                    break;

                            default: break;
                        }
                    }
                    break; // case 4

            case 5: sub_ch = -1;
                    while(sub_ch!=3)
                    {
                        printf("\033[1;34m\n<------------ Sub - Menu ---------->\033[0m\n\n");
                        printf("1. Display All Goverment Schemes\n");
                        printf("2. Allot Scheme Funds\n");
                        printf("3. Back\n");

                        printf("\n\033[1;32mEnter Option: \033[0m");
                        scanf("%d",&sub_ch);


                        switch(sub_ch)
                        {
                            case 1: display_schemes();
                                    break;

                            case 2: display_schemes();
                                    printf("Enter Scheme Number: ");
                                    scanf("%d",&scheme_no);
                                    give_benefits(scheme_no);
                                    break;

                            case 3: printf("\033[1;34m\n<------------Back to main menu---------->\033[0m\n\n");
                                    break;
                        }
                        break; // inner switch case
                    }
                    break; //case 5

        case 9: give_choices();
                break;
        default: close_log();
                free(map);
                exit(0);
        }//main outer switch
    }//main outer while loop
    return 0;
}//main


