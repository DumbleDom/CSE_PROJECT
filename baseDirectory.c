#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUSINESS_FILE "businesses.bin"
#define MAX_BUSINESS_NAME 50
#define MAX_OWNER_NAME 20
#define ADDRESS_LENGTH 100
#define MAX_NUM_BUSINESS 100
#define NUM_TAGS 2
char tags[NUM_TAGS][20] = {"Restaurant","Market"};
enum tags{
    Restaurant = 1,
    Market
};
struct business{
    char busName[MAX_BUSINESS_NAME];
    char owner[MAX_OWNER_NAME];
    char address[ADDRESS_LENGTH];
    int tagCode;
    float rating;
};
void displayBusiness(struct business b);
void loadBusinesses(struct business b[]);
void writeBusinesses();
int main(){
    struct business Businesses[MAX_NUM_BUSINESS];
    loadBusinesses(Businesses);
    displayBusiness(Businesses[0]);
    displayBusiness(Businesses[1]);

}
void loadBusinesses(struct business b[]){
    FILE* busFile = fopen(BUSINESS_FILE,"r");
    fseek(busFile,0,SEEK_SET);
    fread(b,sizeof(struct business),MAX_NUM_BUSINESS,busFile);
    fclose(busFile);
}
void displayBusiness(struct business b){
    printf("%s\nOwned by: %s\n%s\n%.1f\nTags:",b.busName,b.owner,b.address,b.rating);
    int tagNum = b.tagCode;
    for(int i = 0; i < NUM_TAGS; i++){
        if(tagNum%2)
            printf("%s ",tags[i]);
        tagNum /= 2;
    }
    printf("\n\n");
}
