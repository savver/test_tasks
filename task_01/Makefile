CC = gcc

all:
	$(CC) get_uniq_num/get_uniq_num.c          \
	      get_uniq_num_mantests/manual_tests.c \
	      main.c                               \
	      -O0 -o uniq_num_tester

clean:
	rm uniq_num_tester *.o
