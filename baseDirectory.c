#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUSINESS_FILE "businesses.bin"
#define MAX_BUSINESS_NAME 50
#define MAX_NAME_LENGTH 20
#define ADDRESS_LENGTH 100
#define MAX_NUM_BUSINESS 100
#define MAX_REVIEW_LENGTH 250
#define MAX_NUM_REVIEWS 20
#define NUM_TAGS 2


char tags[NUM_TAGS][20] = {"Restaurant","Market"};
enum tags{
    Verified = 1,
    Restaurant,
    Market
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
    char name[MAX_NAME_LENGTH];
    char userName[MAX_NAME_LENGTH];
    //sha-256 encryption from internet maybe?
    char password[256];
    int id;
    int status;
};
struct business{
    char busName[MAX_BUSINESS_NAME];
    char address[ADDRESS_LENGTH];
    struct review reviews[MAX_NUM_REVIEWS];
    char ownerName[MAX_NAME_LENGTH];
    int ownerId;
    int tagCode;
    float rating;
};


void displayBusiness(struct business b);
int loadBusinesses(struct business b[]);
void writeBusinesses();
void viewOwnerPage(struct profile user, struct business ownedBusiness);
void adminPage(struct profile user);
struct business searchByName(struct business businesses[], char search[]);
struct business searchByTag(struct business businesses[]);
void guestView(struct business b);
void adminView(struct business b);

int main(){
    printf("---------------------------------\n");
    printf("-        Welcome to the         -\n");
    printf("- Better Better Business Bureau -\n");
    printf("---------------------------------\n\n");

    //printf("Would you like to log in(y/n)?"); To be implemented
    struct profile userPro;
    int option;
    struct business businesses[MAX_NUM_BUSINESS];
    if(!loadBusinesses(businesses)){
        return 0;
    }

    //Main menu loop
    do{
        //1. Search by name
            //Ask for name, preview results 10 at a time
            //use 1 - 10 to select a business
            //redirect to business page
        //2. search by tags
            //use ints related to enums to search
            //view results 10 at a time
            //use 1 - 10 to select a business
            //redirect to business page
        //3. Log out -> login prompt 
            //If not logged in, Log in option
        //4. Owner page option
void adminPage(struct profile user);
            //Check is userStatus is businessOwner
            //Search to find the business belonging to user
            //if no user, or no business attached to user, give an error and continue loop
            //If business found, enter the ownerPage
        //5. Admin page option
            //check to see if user has admin status
            //if so, open adminPage
        //6. exit

    }while(option != 6);

}


int loadBusinesses(struct business b[]){
    FILE* busFile = fopen(BUSINESS_FILE,"r");
    if(busFile == NULL){
        printf("file failed to open");
        return 0;
    }
    fread(b,sizeof(struct business),MAX_NUM_BUSINESS,busFile);
    fclose(busFile);
    return 1;
}

void displayBusiness(struct business b){
    printf("%s\nOwned by: %s\n%s\n%.1f\nTags:",b.busName,b.ownerName,b.address,b.rating);
    int tagNum = b.tagCode;
    for(int i = 0; i < NUM_TAGS; i++){
        if(tagNum%2)
            printf("%s ",tags[i]);
        tagNum /= 2;
    }
    printf("\n\n");
}

void viewOwnerPage(struct profile user, struct business ownedBusiness){
    //print statement elling user what page they're editing
    //enter loop to 
    //1. Edit Business description and tags(other than verified)
    //2. View all reviews
        //give option to respond to any review
    //3. exit
}

void adminPage(struct profile user){
    //Options to
    //1. search shops same way as guest
    //2. Search by tag
        //both lead to admin veiw of shop rather than guest view
        //can edit shop, remove reviews, and verification status
    //3. view unverified shops
    //4. quit
}