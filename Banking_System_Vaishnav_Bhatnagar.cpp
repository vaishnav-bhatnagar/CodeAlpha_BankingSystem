#include <stdio.h>
#include <stdlib.h>

struct Account
{
    int accountNumber;
    char holderName[50];
    float balance;
};

int accountExists(int accountNumber);
void createAccount();
void depositMoney();
void withdrawMoney();
void balanceEnquiry();
void displayAllAccounts();
void deleteAccount();

int main()
{
    int choice;

    do
    {
        printf("\n========================================");
        printf("\n      BANK ACCOUNT MANAGEMENT SYSTEM");
        printf("\n========================================");
        printf("\n1. Create New Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Balance Enquiry");
        printf("\n5. Display All Accounts");
        printf("\n6. Delete Account");
        printf("\n7. Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                createAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                balanceEnquiry();
                break;

            case 5:
                displayAllAccounts();
                break;

            case 6:
                deleteAccount();
                break;

            case 7:
                printf("\nThank You For Using Our Banking System!\n");
                break;

            default:
                printf("\nInvalid Choice!\n");
        }

    } while(choice != 7);

    return 0;
}

int accountExists(int accountNumber)
{
    FILE *fp;
    struct Account acc;

    fp = fopen("accounts.dat", "rb");

    if(fp == NULL)
        return 0;

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        if(acc.accountNumber == accountNumber)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void createAccount()
{
    FILE *fp;
    struct Account acc;

    fp = fopen("accounts.dat", "ab");

    if(fp == NULL)
    {
        printf("\nError Opening File!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accountNumber);

    if(accountExists(acc.accountNumber))
    {
        printf("\nAccount Number Already Exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", acc.holderName);

    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);

    fclose(fp);

    printf("\nAccount Created Successfully!\n");
}

void depositMoney()
{
    FILE *fp;
    struct Account acc;
    int accNo;
    float amount;
    int found = 0;

    fp = fopen("accounts.dat", "rb+");

    if(fp == NULL)
    {
        printf("\nNo Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        if(acc.accountNumber == accNo)
        {
            printf("Account Holder : %s\n", acc.holderName);
            printf("Current Balance: %.2f\n", acc.balance);

            printf("Enter Amount To Deposit: ");
            scanf("%f", &amount);

            acc.balance += amount;

            fseek(fp, -(long)sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);

            printf("\nDeposit Successful!");
            printf("\nUpdated Balance: %.2f\n", acc.balance);

            found = 1;
            break;
        }
    }

    if(!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

void withdrawMoney()
{
    FILE *fp;
    struct Account acc;
    int accNo;
    float amount;
    int found = 0;

    fp = fopen("accounts.dat", "rb+");

    if(fp == NULL)
    {
        printf("\nNo Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        if(acc.accountNumber == accNo)
        {
            printf("Account Holder : %s\n", acc.holderName);
            printf("Current Balance: %.2f\n", acc.balance);

            printf("Enter Amount To Withdraw: ");
            scanf("%f", &amount);

            if(amount > acc.balance)
            {
                printf("\nInsufficient Balance!\n");
            }
            else
            {
                acc.balance -= amount;

                fseek(fp, -(long)sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);

                printf("\nWithdrawal Successful!");
                printf("\nRemaining Balance: %.2f\n", acc.balance);
            }

            found = 1;
            break;
        }
    }

    if(!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

void balanceEnquiry()
{
    FILE *fp;
    struct Account acc;
    int accNo;
    int found = 0;

    fp = fopen("accounts.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        if(acc.accountNumber == accNo)
        {
            printf("\n================================");
            printf("\nAccount Number : %d", acc.accountNumber);
            printf("\nHolder Name    : %s", acc.holderName);
            printf("\nBalance        : %.2f", acc.balance);
            printf("\n================================\n");

            found = 1;
            break;
        }
    }

    if(!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

void displayAllAccounts()
{
    FILE *fp;
    struct Account acc;
    int count = 0;

    fp = fopen("accounts.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo Accounts Found!\n");
        return;
    }

    printf("\n==============================================================");
    printf("\nAccount No\tHolder Name\t\tBalance");
    printf("\n==============================================================\n");

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        printf("%d\t\t%-20s\t%.2f\n",
               acc.accountNumber,
               acc.holderName,
               acc.balance);
        count++;
    }

    if(count == 0)
    {
        printf("No Accounts Found!\n");
    }

    fclose(fp);
}

void deleteAccount()
{
    FILE *fp, *temp;
    struct Account acc;
    int accNo;
    int found = 0;

    fp = fopen("accounts.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo Accounts Found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    printf("\nEnter Account Number To Delete: ");
    scanf("%d", &accNo);

    while(fread(&acc, sizeof(acc), 1, fp) == 1)
    {
        if(acc.accountNumber == accNo)
        {
            found = 1;
        }
        else
        {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if(found)
        printf("\nAccount Deleted Successfully!\n");
    else
        printf("\nAccount Not Found!\n");
}
