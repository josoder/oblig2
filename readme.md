# MD5 checksums with c. School assignment
A not so very useful type of application, just a IO in C practice. 
The md5 algorithm: http://bradconte.com/md5_c

## usage
Takes 2 arguments, descriped below.
The checksum is written to file as binary. 

### run it
-make
<br>
-make clean 
```
-add , add md5 checksum to end of file
-test , recompute the checksum and validate it with one attached to the end of the file
-strip , remove the checksum from the file

md5 -add "filename"
md5 -test "filename"
md5 -strip "filename"
```
