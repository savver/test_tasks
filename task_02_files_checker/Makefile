CC = gcc

all:
	$(CC) MD5-hash-Calculator/md5c.c \
	      files_checker.c \
	      main.c -lm -O0 -o files_checker
	      
	$(CC) MD5-hash-Calculator/md5c.c \
	     files_checker.c \
	     unity/unity.c \
	     test_runner.c  -lm -O0 -o test_runner_files_checker

clean:
	rm -f files_checker *.o
	rm -f test_runner_files_checker *.o
