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

void encrypt (char input[],char stuck[], int sizeOfStuck){
    char die[256];
    memset(stuck, 0, sizeOfStuck);
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


int main(){
    FILE* fout = fopen(PROFILE_FILE,"w");
    char* password = malloc(sizeof(char)*266);
    password = "password123";
    char stuck[256] = "";
    encrypt(password,stuck,sizeof(stuck));
    struct profile ADMIN;
    strcpy(ADMIN.userName,"Dominic");
    strcpy(ADMIN.password,stuck);
    ADMIN.id=101;
    ADMIN.status = admin;
    fwrite(&ADMIN,sizeof(struct profile),1,fout);
	password = "";
    password = "livelong";
    encrypt(password,stuck,sizeof(stuck));
	printf("%s",password);
    struct profile placeholder;
    strcpy(placeholder.userName,"John Doe");
    strcpy(placeholder.password,stuck);
    placeholder.id=102;
    placeholder.status = businessOwner;
    fwrite(&placeholder,sizeof(struct profile),1,fout);
    password = "flyhigh";
    encrypt(password,stuck,sizeof(stuck));
    struct profile placeholder2;
    strcpy(placeholder2.userName,"Gabriel");
    strcpy(placeholder2.password,stuck);
    placeholder2.id=107;
    placeholder2.status = guest;
    fwrite(&placeholder2,sizeof(struct profile),1,fout);
    password = "stayclean";
    encrypt(password,stuck,sizeof(stuck));
    struct profile placeholder3;
    strcpy(placeholder3.userName,"Jane Smith");
    strcpy(placeholder3.password,stuck);
    placeholder3.id=104;
    placeholder3.status = businessOwner;
    fwrite(&placeholder3,sizeof(struct profile),1,fout);
    fclose(fout);
    fout = fopen(BUSINESS_FILE,"w");
    struct business b = {"","","",0,-1};
    b.tagCode = 5;
    b.rating = 5;
    b.numReviews = 0;
    struct business businesses[5] = { 0 };

    // Sample data (5 businesses)
    businesses[0] = (struct business){
        .busName = "Pecan Lodge",
        .address = "2702 Main St, Dallas, TX 75226",
        .ownerName = "John Doe",
        .ownerId = 102,
        .tagCode = 3,
        .rating = 4.7,
		.numReviews = 0
    };

    businesses[1] = (struct business){
        .busName = "Central Market",
        .address = "5750 E Mockingbird Ln, Dallas, TX 75206",
        .ownerName = "Jane Smith",
        .ownerId = 103,
        .tagCode = 5,
        .rating = 4.5
    };

    businesses[2] = (struct business){
        .busName = "Babe's Chicken Dinner House",
        .address = "104 N Carroll Ave, Carrollton, TX 75006",
        .ownerName = "Mike Johnson",
        .ownerId = 104,
        .tagCode = 3,
        .rating = 4.6
    };

    businesses[3] = (struct business){
        .busName = "Wild About Music",
        .address = "2626 Howell St, Dallas, TX 75204",
        .ownerName = "Sarah Williams",
        .ownerId = 105,
        .tagCode = 0, // No tag
        .rating = 4.2
    };

    businesses[4] = (struct business){
        .busName = "DFW Elite Detail",
        .address = "1601 W Northwest Hwy, Grapevine, TX 76051",
        .ownerName = "Chris Taylor",
        .ownerId = 106,
        .tagCode = 1,
        .rating = 4.8
    };

    // Write to binary file
    FILE* file = fopen(BUSINESS_FILE, "wb");
    if (!file) {
        perror("Error creating business file");
    }

    fwrite(businesses, sizeof(struct business), 5, file);
    fclose(file);

    printf("Business file created successfully with %d slots (%d populated)\n", 5);
}