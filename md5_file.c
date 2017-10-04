#include <stdio.h>
#include <string.h>
#include "md5.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * MD5 to file
 * Reads a file, compute a checksum and add it to the end of the file. 
 * See readme.md for more information.
 *
 * Compile with make, clean with make clean
 **/


const char *ADD = "-add";
const char *TEST = "-test";
const char *STRIP = "-strip"; 
// Add marker to now where the md5 checksum start. (If the file already has one)
const char *MARK = "md5_checksum:"; 
const char *END_MARK = "md5_checksum end";

void create_digest(char *buffer,size_t size_of_buffer, char *digest);
bool has_digest(FILE *fptr);
size_t calc_file_size(FILE *f);
bool is_equal(char *buf1, char *buf2);

/**
 * Remove checksum from file 
 */
int strip_checksum(char *filename){
    FILE *f = fopen(filename, "rb"); 
    if(f==NULL){
        puts("error opening file while trying to strip md5 checksum");
        return 1; 
    }

    size_t size = calc_file_size(f); 
    BYTE buffer[size];
    int offset = MD5_BLOCK_SIZE+strlen(MARK);
    int bytes_to_read = size - offset;
    fread(buffer, 1, bytes_to_read, f);
    
    fclose(f);
    
    FILE *f2 = fopen(filename, "w+"); 
    if (f2==NULL) {
        puts("error while trying to write to file during md5 strip");
        return 1; 
    }

    fwrite(buffer, 1, bytes_to_read, f); 
    fclose(f2);
    
    puts("checkum is now deleted");

    return 0; 
}


/**
 * Check if the checksum at the end of the file is valid
 */
int checksum_is_valid(char *filename){
   FILE *f = fopen(filename, "rb");
   
   BYTE md5_buffer[MD5_BLOCK_SIZE];
   fseek(f, -MD5_BLOCK_SIZE, SEEK_END);
   
   fread(md5_buffer, 1, MD5_BLOCK_SIZE, f); 
    
   puts("checksum attached to file is:");
   for(int i=0; i<MD5_BLOCK_SIZE; i++){
       printf("%02X", (BYTE) md5_buffer[i]);
   }
   
   int limit = (strlen(MARK)+MD5_BLOCK_SIZE);
   int b_size = calc_file_size(f) - limit; 
   BYTE f_buffer[b_size];
   
   BYTE md5_check[MD5_BLOCK_SIZE];
   fread(f_buffer, 1, b_size, f);
   create_digest(f_buffer, b_size, md5_check);
   puts(""); 
   puts("recalculated file checksum is:");
   for(int i=0; i<MD5_BLOCK_SIZE; i++){
       printf("%02X", (BYTE) md5_check[i]);
   }

   puts("");

   
   fclose(f);

   if(!is_equal) {
       puts("the checksum does not match up");
       return 1;
   }
   puts("checksum atached is correct");
   return 0; 
} 

bool is_equal(char *buf1, char *buf2){
    for(int i=0; i<MD5_BLOCK_SIZE; i++){
        if(buf1[i]!=buf2[i]) return false;
    }
    return true; 
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
    // add marker before checksum
    if (print_mark_to_file(filename)!=0){
        return 1; 
    }
    
    FILE *fptr = fopen(filename, "ab");

    if(fptr == NULL) {
        return 1;
    }


    fwrite(digest, 1, MD5_BLOCK_SIZE, fptr);
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
    fseek(fptr, -(sizeof(buff)+MD5_BLOCK_SIZE), SEEK_END); 
    fread(buff, 1, sizeof(buff), fptr); 
    
    // check if the read bytes contains the mark
    if(strcmp(buff, MARK)!=0) return false;

    return true;
}   

void create_digest(char *buffer,size_t size_of_buffer, char *digest){
    MD5_CTX ctx;
    md5_init(&ctx);
    
    md5_update(&ctx, buffer, size_of_buffer); 
   
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

    BYTE digest[MD5_BLOCK_SIZE];    
    FILE *fptr = fopen(argv[2], "rb"); 
    if(fptr==NULL){
        puts("error opening file");
        return 1;
    }

    size_t file_size = calc_file_size(fptr);

    unsigned char buffer[file_size];

    // read file into buffer
    fread(buffer, file_size, 1, fptr);
   
    bool contains_digest = has_digest(fptr);
    fclose(fptr);

    
    if(strcmp(argv[1], ADD)==0){
        if(contains_digest){
            puts("file already contains checksum");
            return 1;
        }
        
    create_digest(buffer, file_size,digest);
        int res = add_checksum_to_file(digest, argv[2]);
        if(res!=0) return 1; 
        
        puts("added checksum to file"); 
        return 0;
    }  
    else if(strcmp(argv[1], TEST)==0){
            
            if(!contains_digest){
             puts("file does not contain any checksum");
                return 1; 
            }
         
            return checksum_is_valid(argv[2]);
    } 
    else if(strcmp(argv[1], STRIP)==0){
        if(!contains_digest) {
            puts("nothing to remove..");
            return 1;
        }
        
        return strip_checksum(argv[2]); 
    }

    return 0;
}


