# MemoryLeakDetectionTool
This project implements C library for detection of memory leak in C applications.

# To run the application 
## first compile the mld library using command 
 # gcc -g -c mld.c -o mld.o
## compile your application 
  # gcc -g -c "application_name.c" -o " application name.o "
## create executable of  application using 
  # gcc -g -o "application" mld.o "application.o"
  ## run application using
  # ./ "application"
 
