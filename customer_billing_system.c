#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define GOTOXY(x,y) printf("\033[%d;%dH",y,x)

void delay(int time)
{
    clock_t start = clock();
    while ((clock() - start) < time * CLOCKS_PER_SEC / 1000);
}

void input();
void writefile();
void search();
void output();

struct date {
    int month;
    int day;
    int year;
};

struct account {
    int number;
    char name[100];
    int acct_no;
    char mobile_no[15];   // corrected type
    char street[100];
    char city[50];        // added city field
    char acct_type;
    float oldbalance;
    float payment;
    float newbalance;     // added newbalance field
    struct date lastpayment;
} customer;

int tl, sl, ts;

int main() {
    int i, n;
    char ch;
    system("clear");
    printf("CUSTOMER BILLING SYSTEM :\n\n");
    printf("==============================\n\n");
    printf("1: ADD account on list \n");
    printf("2: SEARCH customer account \n");
    printf("3: EXIT \n");
    printf("===============================\n");

    do {
        printf("\nSelect what you want to do ?? \n");
        ch = getchar();
    } while (ch < '1' || ch > '3');

    switch (ch) {
        case '1':
            system("clear");
            printf("\nHow many customers do you want to add : ");
            scanf("%d", &n);
            getchar();
            for (i = 0; i < n; i++) {
                input();
                if (customer.payment > 0.0)
                    customer.acct_type = (customer.payment < customer.oldbalance) ? 'O' : 'D';
                else
                    customer.acct_type = (customer.oldbalance > 0) ? 'D' : 'C';
                customer.newbalance = customer.oldbalance - customer.payment;
                writefile();
            }
            main();
            break;

        case '2':
            getchar();
            system("clear");
            printf("Search by what ?? \n\n");
            printf("1: search by customer number \n");
            printf("2: search by customer name \n");
            search();
            main();
            break;

        case '3':
            system("clear");
            delay(500);
            GOTOXY(10,25);
            printf("A PROJECT BY PRACHI \n");
            delay(500);
            exit(1);
    }
    return 0;
}

void input() {
    FILE *fp = fopen("file.txt","ab+");
    fseek(fp,0,SEEK_END);
    tl = ftell(fp);
    sl = sizeof(customer);
    ts = tl/sl;
    customer.number = ts + 1;

    printf("\nCustomer no: %d\n", customer.number);

    printf("Account number : ");
    scanf("%d",&customer.acct_no);
    getchar();

    printf("Customer name : ");
    fgets(customer.name,100,stdin);

    printf("Mobile number : ");
    fgets(customer.mobile_no,15,stdin);

    printf("Street : ");
    fgets(customer.street,100,stdin);

    printf("City : ");
    fgets(customer.city,50,stdin);

    printf("Previous balance : ");
    scanf("%f",&customer.oldbalance);

    printf("Current payment : ");
    scanf("%f",&customer.payment);

    printf("Last payment date (dd mm yyyy): ");
    scanf("%d %d %d",&customer.lastpayment.day,&customer.lastpayment.month,&customer.lastpayment.year);

    fclose(fp);
}

void writefile() {
    FILE *fp = fopen("file.txt","ab+");
    fwrite(&customer,sizeof(customer),1,fp);
    fclose(fp);
}

void search() {
    FILE *fp;
    int choice, num, found=0;
    char name[100];

    fp = fopen("file.txt","rb");
    if(fp == NULL) {
        printf("File not found!\n");
        return;
    }

    printf("Enter choice: ");
    scanf("%d",&choice);
    getchar();

    if(choice==1) {
        printf("Enter customer number: ");
        scanf("%d",&num);
        while(fread(&customer,sizeof(customer),1,fp)==1) {
            if(customer.number==num) {
                output();
                found=1;
                break;
            }
        }
    } else if(choice==2) {
        printf("Enter customer name: ");
        fgets(name,100,stdin);
        name[strcspn(name,"\n")] = 0;
        while(fread(&customer,sizeof(customer),1,fp)==1) {
            if(strcmp(customer.name,name)==0) {
                output();
                found=1;
                break;
            }
        }
    }

    if(!found) printf("Record not found!\n");
    fclose(fp);
}

void output() {
    printf("\nCustomer No: %d\n", customer.number);
    printf("Account No: %d\n", customer.acct_no);
    printf("Name: %s", customer.name);
    printf("Mobile: %s", customer.mobile_no);
    printf("Street: %s", customer.street);
    printf("City: %s", customer.city);
    printf("Old Balance: %.2f\n", customer.oldbalance);
    printf("Payment: %.2f\n", customer.payment);
    printf("New Balance: %.2f\n", customer.newbalance);
    printf("Last Payment Date: %d/%d/%d\n", customer.lastpayment.day, customer.lastpayment.month, customer.lastpayment.year);
}
