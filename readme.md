### MD5 checksums with c. School assignment
One of does "not very useful type of application", just a IO in C practice. 

## usage
Takes 2 arguments, descriped below.
The checksum is written to file as binary. 

# compile with make
clean with make clean 

-add , add md5 checksum to end of file
-test , recompute the checksum and validate it with one attached to the end of the file
-strip , remove the checksum from the file

md5 -add "filename"
md5 -test "filename"
md5 -strip "filename"
