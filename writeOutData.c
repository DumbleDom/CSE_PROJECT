#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "sha256.h"

// sha-256 stuff start
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))


static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};
// sha-256 stuff end

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
void displayProfile(struct profile p){
    printf("%s\n%s\n%06d\n%d\n",p.userName,p.password,p.id,p.status);
}
void displayBusiness(struct business b){
    printf("%s\nOwned by: %s\n%s\nrating: %.1f with %d reviews\nTags:",b.busName,b.ownerName,b.address,b.rating,b.numReviews);
    int tagNum = b.tagCode;
    for(int i = 0; i < NUM_TAGS; i++){
        if(tagNum%2)
            printf("%s ",tags[i]);
        tagNum /= 2;
    }
    printf("\n\n");
}
int main(){
    FILE* fin = fopen(BUSINESS_FILE,"rb");
    fread(businesses,sizeof(struct business), MAX_NUM_BUSINESS,fin);
    fclose(fin);
    for(int i = 0; i < 6; i++){
        displayBusiness(businesses[i]);
    }
    struct profile profiles[MAX_NUM_USERS];
    fin = fopen(PROFILE_FILE,"rb");
    fread(profiles,sizeof(struct profile), MAX_NUM_USERS,fin);
    fclose(fin);
    for(int i = 0; i < 5; i++){
        displayProfile(profiles[i]);
    }
    return 0;
}