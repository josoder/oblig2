#include <stdio.h>
#include <string.h>
#include "md5.h"
#include <stdbool.h>

const char *ADD = "-add";
const char *TEST = "-test";
const char *STRIP = "-string"; 
// Add marker to now where the md5 checksum start. (If the file already has one)
const char *MARK = "md5_checksum:"; 
const char *END_MARK = "md5_checksum end";

bool has_digest(FILE *fptr);


/**
 * Check if the checksum at the end of the file is valid
 */
int checksum_is_valid(char *filename){
    
} 


/**
 *Calculate the size
 */
size_t calc_file_size(FILE *f){
	fseek(f, 0L, SEEK_END);
	size_t sz = ftell(f); 
	rewind(f); 
	return sz;
}

/**
 * Print a mark to the file where the checksum starts
 */
int print_mark_to_file(char *filename) {
    FILE *fptr = fopen(filename, "a");

    if(fptr == NULL){
        puts("error opening file while printing checksum");
        return 1;
    }

    fputs(MARK, fptr);
    fclose(fptr);
    return 0; 
}


/**
 *Add checksum to file
 */ 
int add_checksum_to_file(unsigned char *digest, char *filename){
	FILE *fptr = fopen(filename, "ab");

	if(fptr == NULL) {
		return 1;
	}
    // add marker before checksum
	if (print_mark_to_file(filename)!=0){
        return 1; 
    }
    
    fwrite(digest, sizeof(digest[0]), sizeof(digest), fptr);
    fclose(fptr);
	return 0; 
}

/**
 * Print description on how to use this app.
 */
void print_usage(){
	puts("wrong nr of args"); 
	puts("usage:");
	puts("md5 -add <filename>");
	puts("md5 -test <filename>");
	puts("md5 -strip <filename>");
}

/**
 * Check if a file already contains a digest
 */ 
bool has_digest(FILE *fptr){
    unsigned char buff[strlen(MARK)]; 
    
    if(calc_file_size(fptr)<sizeof(buff) + MD5_BLOCK_SIZE){
        return false;
    }
    
    // Place cursor at where the mark would have been placed
    fseek(fptr, calc_file_size(fptr) - (sizeof(buff) + MD5_BLOCK_SIZE), SEEK_SET); 
    fread(buff, sizeof(buff), sizeof(buff[0]), fptr); 

    for(int i=0; i<strlen(MARK); i++){
        if(buff[i]!=MARK[i]) return false;
    }

    return true;
}

void create_digest(char *buffer, char *digest,  MD5_CTX ctx){
    md5_update(&ctx, buffer, sizeof(buffer)); 
	
	md5_final(&ctx, digest);	
}

/**
 * usage: 
 * md5 -add <filename>
 * md5 -test <filename>
 * md5 -strip <filename>
 */
int main(int argc, char *argv[]){
	if(argc<3 && (strcmp(argv[1], ADD)!=0 || strcmp(argv[1], TEST)!=0 || strcmp(argv[1], STRIP)!=0)){
		print_usage();
		return 1;
	}

	unsigned char digest[MD5_BLOCK_SIZE];    
	MD5_CTX ctx;
	
	FILE *fptr = fopen("file", "rb"); 
	if(fptr==NULL){
		puts("error opening file");
		return 1;
	}

    size_t file_size = calc_file_size(fptr);

	unsigned char buffer[file_size];

	// read file into buffer
	fread(buffer, file_size, 1, fptr);
    rewind(fptr);

    bool contains_digest = has_digest(fptr);
    fclose(fptr);

    
    if(strcmp(argv[1], ADD)==0){
        if(contains_digest){
                puts("file already contains checksum");
                    return 1;
                }
        create_digest(digest, buffer, ctx);
        add_checksum_to_file(digest, argv[2]);    
    }   
    else if(strcmp(argv[1], TEST)==0){
        if(!contains_digest){
            puts("file does not contain any checksum");
            return 1; 
        }
        
    }  

	return 0;
}


