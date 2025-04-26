#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "sha256.h"

#define BUSINESS_FILE "businesses.bin"
#define USERNAMES_FILE "usernames.txt"
#define PASSWORDS_FILE "passwords.txt"
#define MAX_BUSINESS_NAME 50
#define MAX_NAME_LENGTH 20
#define ADDRESS_LENGTH 100
#define MAX_NUM_BUSINESS 100
#define MAX_REVIEW_LENGTH 250
#define MAX_NUM_REVIEWS 20
#define NUM_TAGS 2


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

int loadBusinesses(struct business b[]);
void displayBusiness(struct business b);
//unimplemented
void adminPage(struct profile user);
struct business searchByName(struct business businesses[]);
struct business searchByTag(struct business businesses[]);
void logIn(struct profile *user);
void guestView(struct business b);
void adminView(struct business b);
void writeBusinesses();
void viewOwnerPage(struct business ownedBusiness);

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
        
            //Check is userStatus is businessOwner
            //Search to find the business belonging to user
            //if no user, or no business attached to user, give an error and continue loop
            //If business found, enter the ownerPage
        //5. Admin page option
            //check to see if user has admin status
            //if so, open adminPage
        //6. exit
        printf("Enter a nummberr to proceed\n");
        printf("1. Search for a shop by name\n");
        printf("2. Seach for shops using tags\n");
        if(&userPro == NULL){
            printf("3. Log In\n");
            printf("4. QUIT");
            scanf("%d",&option);
            switch(option){
                case 1:
                    searchByName(businesses);
                    break;
                case 2:
                    searchByTag(businesses);
                    break;
                case 3:
                    logIn(&userPro);
                    break;
                case 4:
                    break;
                default:
                    printf("Please enter a valid number");
                    break;
            }
        } else {
            printf("3. Log out\n");
            if(userPro.status == businessOwner){
                printf("4. View you business page\n");
                printf("5. QUIT\n");
                scanf("%d",&option);
                switch(option){
                    case 1:
                        searchByName(businesses);
                        break;
                    case 2:
                        searchByTag(businesses);
                        break;
                    case 3:
                        &userPro == NULL;
                        break;
                    case 4:
                        //search for business owned by user
                        //return error if business is not found
                        //if business is found, open the business page
                        int found = 0;
                        int index = -1;
                        for(int i = 0; i < sizeof(businesses)/sizeof(businesses[0]);i++){
                            if(userPro.id == businesses[i].ownerId){
                                found = 1;
                                index = i;
                            }
                        }
                        if(found){
                            viewOwnerPage(businesses[index]);
                        } else {
                            printf("Couldn't find you business");
                        }
                        break;
                    case 5:
                        break;
                    default:
                        printf("Please enter a valid number");
                        break;
                }
            } else if (userPro.status == admin){
                printf("4. Enter Admin view\n");
                printf("5. QUIT\n");
                scanf("%d",&option);
                switch(option){
                    case 1:
                        searchByName(businesses);
                        break;
                    case 2:
                        searchByTag(businesses);
                        break;
                    case 3:
                        &userPro == NULL;
                        break;
                    case 4:
                        adminPage(userPro);
                        break;
                    case 5:
                        break;
                    default:
                        printf("Please enter a valid number");
                        break;
                }
            } else {
                printf("4. QUIT\n");
                scanf("%d",&option);
                switch(option){
                    case 1:
                        searchByName(businesses);
                        break;
                    case 2:
                        searchByTag(businesses);
                        break;
                    case 3:
                        &userPro == NULL;
                        break;
                    case 4:
                        break;
                    default:
                        printf("Please enter a valid number");
                        break;
                }
            }
        }
        printf("\n");

    }while(!(option == 4 && (userPro.status == businessOwner || userPro.status == admin))&& (option != 5));

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

void viewOwnerPage(struct business ownedBusiness){
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


bool compare(struct profile *user)
{
    char username[MAX_NAME_LENGTH];
    char password[256];
    int tempid;
    int signup;
    
    
    FILE *list = fopen(USERNAMES_FILE,"a"); //creates it if it doesn't exist
    fclose(list);
    list = fopen(PASSWORDS_FILE,"a"); //creates it if it doesn't exist
    fclose(list);
    list = fopen(USERNAMES_FILE,"r");
    //printf("%d\n",numlines(list)); debuging the numlines
    
    int linenumber=0;

    bool checkfile (FILE *list, char username[MAX_NAME_LENGTH]) {
        char storage[50]; //hold usernames from the file while checking them
        linenumber=0;
        if(list != NULL) { //check if list have content
            while(fgets(storage,sizeof(storage),list) != NULL) { //if list have content, get it line by line and compare it to the username.
                if (*storage == '\n') //can't remember what this bit does
                    continue;
                sscanf(storage, "%[^\n]", storage);
                //printf("storage:%s\n",storage); // for debug, checks the current value of storage
                //printf("username:%s\n",username); //for debug, checks the value of username
        
                if(!strcmp(storage, username)) {
                    return true; //the string was found in the file, congrats
                } else {linenumber++;}
            }
            return false; //no luck
        }
    }
    
    int numlines(FILE *list){
        // make sure they are in mode r before use
        // Extract characters from file and store in character c
        int count = 0;
        char c;
        for (c = getc(list); c != EOF; c = getc(list))
            if (c == '\n') // Increment count if this character is newline
                count = count + 1;
        return count;
    }
    
    char stuck[266];
    
    void encrypt (char input[]){
        char die[256];
        memset(stuck, 0, sizeof stuck);
        int length = strlen(input);
        SHA256_CTX context;
        unsigned char md[32];
        sha256_init(&context);
        sha256_update(&context, (unsigned char *)input, length);
        sha256_final(&context, md);
        
        int i;
        for(i = 0; i < sizeof(md); i++) {
            //printf("%0x", md[i]);
            sprintf(die,"%0x", md[i]);
            strcat(stuck,die);
        }
        //printf("\n");
        
    }
    encrypt("abc");
    //printf("%s",stuck);
    
    
    //okay here's the code, everything above is basically just setup


    printf("If you already have an account, enter 1.\nIf you want to create an account, enter 2.\n");
    scanf("%d",&signup);
    printf("Enter username: ");
    scanf("%s",username);
    printf("Enter password: ");
    scanf("%s",password);
    signup--;

    if(signup){
        if(checkfile(list,username)){
            printf("That username is taken!\n");
            return true;
        } else {
            fclose(list);
            list = fopen(USERNAMES_FILE,"a");
            fprintf(list,"%s\n",username); //add username to list
            fclose(list);
            list = fopen(USERNAMES_FILE,"r");
            tempid = numlines(list); //use number of lines to get id (this means they are logged in)
            
            //encrypt password; still needs to happen
            encrypt(password);
            //printf("%s",stuck);
            
            fclose(list);
            list = fopen(PASSWORDS_FILE,"a");
            fprintf(list,"%s\n",stuck); //add password to different list
            fclose(list);
            list = fopen(USERNAMES_FILE,"r");
            memcpy((user->userName),username,sizeof(username)); //add username and id values to the user structure
            user->id = tempid; //add username and id values to the user structure
            printf("Congratulations! ");
            return false;
        }
    } else {
        checkfile(list,username); //find their username in the list
        tempid=linenumber; //get id
        
        int count = 0;
        char line[256];
        char temppass[256];
        fclose(list);
        list = fopen(PASSWORDS_FILE,"r");
        while (fgets(line, sizeof line, list) != NULL) {//retrieve password from list with corresponding line number
            if (count == tempid) {
                sscanf(line, "%[^\n]", temppass); //i'm so cool
                count++;
            } else {count++;}
        }
        
        //decrypt password
        encrypt(password);
        
        if (!strcmp(stuck,temppass)){ //compare entered password with decrypted password
            printf("Congratulations! ");
            memcpy((user->userName),username,sizeof(username));
            user->id = tempid; //add username and id values to the user structure//log them in, update id
            return false;
        } else {
            printf("The username or password was incorrect.\n");
            return true;
        }
    }
}

void logIn(struct profile *user){
    while(compare(user)){
        
    }
}


//sha functions
void sha256_transform(SHA256_CTX *ctx, const BYTE data[]){
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx){
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len){
	WORD i;
	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[]){
	WORD i;
	i = ctx->datalen;
	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	} else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

