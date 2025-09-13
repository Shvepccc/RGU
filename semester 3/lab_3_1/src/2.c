//make run TASK=2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char login[7];
    int pin;
    int blocked;
} user_struct;

user_struct* users = NULL;
int user_count = 0;
int user_capacity = 16;

int compare_users(const void* a, const void* b)
{
    return strcmp(((user_struct*)a)->login, ((user_struct*)b)->login);
}

// user_struct* find_user(const char* login)
// {
//     for(int i = 0; i < user_count; i++)
//     {
//         if(strcmp(users[i].login, login) == 0)
//             return &users[i];
//     }
//     return NULL;
// }

user_struct* find_user(const char* login)
{
    int left = 0, right = user_count - 1;
    while(left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = strcmp(login, users[mid].login);
        if(cmp == 0)
            return &users[mid];
        else if(cmp < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return NULL;
}

// int register_user()
// {
//     char login[7];
//     int pin, len;
//     user_struct* temp;
//     while(1)
//     {
//         printf("Enter new login (1-6 chars): ");
//         scanf("%s", login);
//         len = strlen(login);
//         if(len == 0 || len > 6)
//         {
//             printf("Invalid login. Try again.\n");
//             continue;
//         }
//         if(find_user(login))
//         {
//             printf("User already exists. Try another login.\n");
//             continue;
//         }
//         break;
//     }
//     while(1)
//     {
//         printf("Enter PIN (0-999999): ");
//         if(scanf("%d", &pin) != 1)
//         {
//             while(getchar() != '\n');
//             printf("Invalid input. Enter a number.\n");
//             continue;
//         }
//         if(pin < 0 || pin > 999999)
//         {
//             printf("PIN out of range. Try again.\n");
//             continue;
//         }
//         break;
//     }
//     if (user_count >= user_capacity || user_count == 0)
//     {
//         temp = realloc(users, sizeof(user_struct) * (user_capacity * 2));
//         if (temp == NULL)
//         {
//             return 1;
//         }
//         users = temp;
//         temp = NULL;
//     }
//     strcpy(users[user_count].login, login);
//     users[user_count].pin = pin;
//     users[user_count].blocked = 0;
//     user_count++;
//     printf("Registration successful!\n");
//     return 0;
// }

int register_user()
{
    char login[7];
    int pin, len;
    user_struct* temp;

    while(1)
    {
        printf("Enter new login (1-6 chars): ");
        scanf("%6s", login);

        len = 0;
        while(login[len] != '\0') len++;
        if(len == 0 || len > 6)
        {
            printf("Invalid login. Try again.\n");
            continue;
        }
        if(find_user(login))
        {
            printf("User already exists. Try another login.\n");
            continue;
        }
        break;
    }

    while(1)
    {
        printf("Enter PIN (0-999999): ");
        if(scanf("%d", &pin) != 1)
        {
            while(getchar() != '\n');
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        if(pin < 0 || pin > 999999)
        {
            printf("PIN out of range. Try again.\n");
            continue;
        }
        break;
    }

    if(user_count >= user_capacity || user_count == 0)
    {
        user_capacity *= 2;
        temp = realloc(users, sizeof(user_struct) * user_capacity);
        if(temp == NULL)
        {
            return 1;
        }
        users = temp;
    }

    strcpy(users[user_count].login, login);
    users[user_count].pin = pin;
    users[user_count].blocked = 0;
    user_count++;

    qsort(users, user_count, sizeof(user_struct), compare_users);

    printf("Registration successful!\n");
    return 0;
}

user_struct* login_user()
{
    char login[7];
    int pin;
    printf("Enter login: ");
    scanf("%6s", login);
    user_struct* user = find_user(login);
    if(!user)
    {
        printf("User not found =(\n");
        return NULL;
    }
    if(user->blocked)
    {
        printf("User is blocked =(\n");
        return NULL;
    }
    while (1) 
    {
        printf("Enter PIN: ");
        scanf("%d", &pin);
        if(user->pin != pin)
        {
            printf("Incorrect PIN. Try again.\n");
            continue;
        }
        break;
    }
    printf("Login successful!\n");
    return user;
}

void print_time()
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    printf("%02d:%02d:%02d\n",
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

void print_date()
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    printf("%02d:%02d:%04d\n",
        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

void howmuch(char* date_str, char* time_str, char* flag)
{
    time_t t1, t2;
    double diff;
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(tm_info));
    tm_info.tm_isdst = -1;
    if(sscanf(date_str, "%d:%d:%d",
        &tm_info.tm_mday, &tm_info.tm_mon, &tm_info.tm_year) != 3)
    {
        printf("Invalid date format\n");
        return;
    }
    if(sscanf(time_str, "%d:%d:%d",
        &tm_info.tm_hour, &tm_info.tm_min, &tm_info.tm_sec) != 3)
    {
        printf("Invalid time format\n");
        return;
    }
    tm_info.tm_mon -= 1;
    tm_info.tm_year -=1900;

    t1 = mktime(&tm_info);
    if(t1 == (time_t)-1)
    {
        printf("Invalid datetime\n");
        return;
    }
    t2 = time(NULL);
    diff = difftime(t2, t1);
    if(flag == NULL || flag[0] != '-' || flag[1] == '\0')
    {
        printf("Invalid flag\n");
        return;
    }
    if(flag[1] == 's')
        printf("%.0f seconds\n", diff);
    else if(flag[1] == 'm')
        printf("%.0f minutes\n", diff / 60.0);
    else if(flag[1] == 'h')
        printf("%.0f hours\n", diff / 3600.0);
    else if(flag[1] == 'y')
        printf("%.2f years\n", diff / (365.0 * 24.0 * 3600.0));
    else
        printf("Unknown flag\n");
}


void sanctions()
{
    int confirm;
    char username[7];
    printf("Enter username to block: ");
    scanf("%6s", username);
    user_struct* user = find_user(username);
    if(!user)
    {
        printf("User not found.\n");
        return;
    }
    printf("Enter password to confirm blocking: ");
    scanf("%d", &confirm);
    if(confirm == 52)
    {
        user->blocked = 1;
        printf("User %s is now blocked.\n", user->login);
    }
    else
        printf("Blocking cancelled.\n");
}

void user_session(user_struct* user)
{
    char command[100];
    char data_str[11], time_str[9], flag[3];
    int logout = 0;
    while(1)
    {
        printf("\nEnter command: ");
        scanf("%8s", command);

        switch (command[0])
        {
        case 'T':
            print_time();
        break;
        case 'D':
            print_date();
        break;
        case 'H':
            scanf("%8s %10s %8s %2s", command, data_str, time_str, flag);
            //printf("data: %s, time: %s, flag: %s", data_str, time_str, flag);
            howmuch(data_str, time_str, flag);
        break;
        case 'L':
            logout = 1;
        break;
        case 'S':
            sanctions();
        break;
        default:
            printf("Unknown command.\n");
        break;
        }

        if (logout)
            break;
    }
}

int main()
{
    int err = 0;
    while(1)
    {
        printf("\n1 - Register\n2 - Login\nChoose: ");
        int choice;
        scanf("%d", &choice);
        if(choice == 1)
        {
            err = register_user();
            if (err)
                return err;
        }
        else if(choice == 2)
        {
            user_struct* user = login_user();
            if(user)
                user_session(user);
        }
        else
            printf("Invalid choice (((\n");
    }
    free(users);
    return 0;
}
