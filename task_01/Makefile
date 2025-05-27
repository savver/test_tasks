CC = gcc

all:
	$(CC) get_uniq_num/get_uniq_num.c          \
	      get_uniq_num_mantests/manual_tests.c \
	      main.c                               \
	      -O0 -o uniq_num_tester
	      
	$(CC) get_uniq_num/get_uniq_num.c          \
	      get_uniq_num_mantests/manual_tests.c \
	      unity/unity.c                        \
	      test_runner.c                        \
	      -O0 -o test_runner_uniq_num_tester

clean:
	rm -f uniq_num_tester *.o
	rm -f test_runner_uniq_num_tester *.o
