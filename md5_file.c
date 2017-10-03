#include <stdio.h>
#include <string.h>
#include "md5.h"

const char *ADD = "-add";
const char *TEST = "-test";
const char *STRIP = "-string"; 

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
 *Add checksum to file
 */ 
int add_checksum_to_file(unsigned char *digest, char *filename){
	FILE *fptr = fopen(filename, "ab");

	if(fptr == NULL) {
		return 1;
	}

	fwrite(digest, sizeof(digest[0]), sizeof(digest), fptr);
	fclose(fptr);
	return 0; 
}

/**
 * Adds md5 checksum to the end of the file
 */
int compute_and_append_md5(char *filename, FILE *fptr,MD5_CTX ctx, unsigned char *digest){
	size_t file_size = calc_file_size(fptr);

	unsigned char buffer[file_size];

	// read file into buffer
	fread(buffer, file_size, 1, fptr);
	fclose(fptr);

	md5_update(&ctx, buffer, file_size); 
	
	md5_final(&ctx, digest);	
	
	if(add_checksum_to_file(digest, filename)!=0){
		puts("error while opening file");
		puts("could not print checksum to end of file...");
		return 1;
	}

	return 0;
}


void print_usage(){
	puts("wrong nr of args"); 
	puts("usage:");
	puts("md5 -add <filename>");
	puts("md5 -test <filename>");
	puts("md5 -strip <filename>");

}

/**
 * usage: 
 * md5 -add <filename>
 * md5 -test <filename>
 * md5 -strip <filename>
 */
int main(int argc, char *argv[]){
	if(argc<3 || strcmp(argv[1], ADD)!=0){
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

	if(strcmp(argv[1], ADD)==0){
		compute_and_append_md5(argv[2], fptr,ctx, digest);
	}
		
	return 0;
}


