#include <stdio.h>
#include <string.h>
#include "stdio.h"

int authorithation()
{
    char login[7];
    int pin = 0;
    int mode = 0;
    printf("Press Enter to log in or 1 to make registration: ");
    scanf("%d", &mode);

    if (mode == 1)
    {
        
    }
    else
    {
        printf("Login: ");
        scanf("%s", login);
        printf("PIN: ");
        scanf("%d", &pin);
    }
}

int main()
{
    while (1)
    {
        if (!authorithation())
        {
            printf("Above all you should to sign up");
            continue;
        }
    }
    return 0;
}