#include"slum.c"


int main()
{
    //Open the credentials file to load the credentials in to struct admin
    fp = fopen("credentials.txt","r+");

    //if the credentials.txt file doesn't exists
    if(fp == NULL)
    {
        printf("File not found\n");
        write_slum_log("add_user.c",FAILURE,"Resident Data Not Loaded");
        return FAILURE;
    }

    //Load the struct admin with data from credentials.txt
    int admin_count = 0;
    while(!feof(fp))
    {
        fscanf(fp,"%s%s",admin[admin_count].username,admin[admin_count].password);
        admin_count++;
    }

    //if the maximum limit of the users is reached
    //The limit can be change by navigating to settings.h and modifing the value of MAX_ADMIN
    if(admin_count > MAX_ADMIN)
    {
        printf("Cannot Add User as User limit Reached!\n");
        write_slum_log("add_user.c",FAILURE,"User Limit Reached");
        return;
    }


    //Take Username of the new user to be added
    printf("Enter username: ");
    scanf("%s",username);

    //Check if the username already exists
    for(i=0; i<admin_count; i++)
    {
        if(!strcasecmp(admin[i].username,username))
        {
            printf("username exists");
            write_slum_log("add_user.c",FAILURE,"Trying to add existing user");
            return FAILURE;
        }
    }
    //read password from user
    read_password(pass);
    to_hash_str(pass);

    //write the new user's username and hashed password to credientials
    fseek(fp,1,-1);
    fprintf(fp,"%s %s\n",username,pass);
    printf("\nUSER: %s Added Successfully!\n",username);
    write_slum_log("add_user.c",SUCCESS,"Added user successfully");

    fclose(fp);
    return 0;
}
