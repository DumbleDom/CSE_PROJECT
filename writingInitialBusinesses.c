#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "sha256.h"

#define BUSINESS_FILE "businesses.bin"
#define NEXT_ID "id.txt"
#define PROFILE_FILE "profiles.bin"
#define MAX_BUSINESS_NAME 50
#define MAX_NAME_LENGTH 20
#define ADDRESS_LENGTH 100
#define MAX_NUM_BUSINESS 100
#define MAX_REVIEW_LENGTH 250
#define MAX_NUM_USERS 500
#define MAX_NUM_REVIEWS 20
#define NUM_TAGS 3



char tags[NUM_TAGS][20] = {"verified","restaurant","market"};
enum tags{
    Verified = 1,
    Restaurant,
    Market,
    END
};
enum userStatus{
    guest,
    businessOwner,
    admin
};

struct review{
    char author[MAX_NAME_LENGTH];
    char body[MAX_REVIEW_LENGTH];
    int starRating;
    char response[MAX_REVIEW_LENGTH];
};
struct profile{
    char userName[MAX_NAME_LENGTH];
    char password[266];
    int id;
    int status;
}userProf, placeHold;
struct business{
    char busName[MAX_BUSINESS_NAME];
    char address[ADDRESS_LENGTH];
    struct review reviews[MAX_NUM_REVIEWS];
    char ownerName[MAX_NAME_LENGTH];
    int ownerId;
    int tagCode;
    float rating;
    int numReviews;
}businesses[MAX_NUM_BUSINESS];
int main(){
    FILE* fout = fopen(PROFILE_FILE,"w");
    struct profile ADMIN = {"Dominic","password123",001,admin};
    fwrite(&ADMIN,sizeof(struct profile),1,fout);
    struct profile placeholder = {"Prosper","live long",002,businessOwner};
    fwrite(&placeholder,sizeof(struct profile),1,fout);
    struct profile placeholder2 = {"Gabriel","fly high",003,guest};
    fwrite(&placeholder2,sizeof(struct profile),1,fout);
    struct profile placeholder3 = {"Sam Soapsman","stay clean",004,businessOwner};
    fwrite(&placeholder3,sizeof(struct profile),1,fout);
    fclose(fout);
    fout = fopen(BUSINESS_FILE,"w");
    struct business b = {"","","",0,-1};
    b.tagCode = 5;
    b.rating = 5;
    b.numReviews = 0;
    strcpy(b.busName,"Sam's soap shop");
    strcpy(b.ownerName,"Sam Soapsman");
    strcpy(b.address,"123 Memory Lane");\
    b.ownerId = 4;
    fwrite(&b,sizeof(struct business), 1, fout);
    b.tagCode = 3;
    strcpy(b.busName,"Proper Pizzaria");
    strcpy(b.ownerName,"Prosper");
    strcpy(b.address,"125 Memory Lane");
    b.ownerId = 2;
    fwrite(&b,sizeof(struct business), 1, fout);
    fclose(fout);
}