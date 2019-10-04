#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* xor(char *a, char *b) {
    int i, length=strlen(a);
    char *result;
    result = (char *)malloc(sizeof(char)*length);
    for (i=0; i<length; i++) {
        if(a[i]==b[i])
        {
            result[i]='0';
        }
        else
        {
            result[i]='1';
        }
    }
    return(result);
}

char* mod2div(char *dividend,char *divisor) {
    int dividend_length = strlen(dividend);
    int divisor_length = strlen(divisor);
    int i;
    char *tmp;
    tmp = (char *)malloc(sizeof(char)*(divisor_length-1));
    for(i=0; i<divisor_length; i++) {
        tmp[i] = dividend[i];
    }
    int index=divisor_length;
    char* zero;
    while (index < dividend_length)
    {
        if (tmp[0] == '1')
        {
            tmp = xor(divisor, tmp);
            for(i=0; i<divisor_length-1; i++)
                tmp[i]=tmp[i+1];
            tmp[divisor_length-1]=dividend[index];
        }
        else
        {
            zero = (char *)malloc(sizeof(char)*(divisor_length));
            for(i=0; i<divisor_length; i++)
            {
                zero[i]='0';
            }
            tmp = xor(zero, tmp);
            for(i=0; i<divisor_length-1; i++)
                tmp[i]=tmp[i+1];
            tmp[divisor_length-1]=dividend[index];
        }
        index += 1;
    }

    if (tmp[0] == '1')
    {
        tmp = xor(divisor, tmp);
    }
    else
    {
        zero = (char *)malloc(sizeof(char)*(divisor_length));
        for(i=0; i<divisor_length; i++)
        {
            zero[i]='0';
        }
        tmp = xor(zero, tmp);
    }
    char *crc;
    crc = (char *)malloc(sizeof(char)*(divisor_length-1));
    for(i=0; i<=divisor_length-1; i++)
        crc[i]=tmp[i+1];
    return(crc);
}
char* append_bits(char *message,char *append_with,int zeros) {
    int i;
    int message_length = strlen(message);
    int append_with_length = strlen(append_with);
    char *result;
    result = (char *)malloc(sizeof(char)*(message_length+append_with_length-1));
    for(i=0; i<strlen(message); i++)
        result[i]=message[i];
    if(zeros==1) {
        for(i=0; i<strlen(append_with)-1; i++)
            result[message_length+i]='0';
    }
    else {
        for(i=0; i<strlen(append_with); i++)
            result[message_length+i]=append_with[i];
    }
    return(result);
}
int main() {
    int i;
    printf("==========CYCLIC REDUNDANCY CHECK==========");
    printf("\n\t\t\t\t\tS. DHANYA ABHIRAMI\n\n");
    int message_length,divisor_length;
    printf("Enter Message Length: ");
    scanf("%d",&message_length);
    printf("Enter Divisor Length: ");
    scanf("%d",&divisor_length);
    char message[message_length],divisor[divisor_length];
    printf("\nSENDER SIDE\n");
    printf("Enter Message: ");
    scanf("%s",message);
    printf("Enter Divisor: ");
    scanf("%s",divisor);
    char *appended_message=append_bits(message,divisor,1);
    printf("Appended Message: %s\n", appended_message);
    char *crc = mod2div(appended_message,divisor);
    printf("CRC Bits: %s\n", crc);
    char *encoded_message = append_bits(message,crc,0);
    printf("Encoded Message: %s\n", encoded_message);
    printf("\nRECEIVER SIDE\n");
    char received_message[message_length+divisor_length-1];
    printf("Enter received message: ");
    scanf("%s",received_message);
    char *received_crc = mod2div(received_message,divisor);
    for(i=0; i<strlen(divisor)-1; i++) {
        if(received_crc[i]!='0') {
            printf("Message is corrupted !!\n");
            exit(0);
        }
    }
    printf("Message is correct \n");
    return(0);
}