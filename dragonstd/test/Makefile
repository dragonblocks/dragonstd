CC=cc -g -Wall -Wextra -Werror -fmax-errors=1 -fsanitize=address
TEST=valgrind --leak-check=full --show-leak-kinds=all --quiet

run: test_array test_list test_tree test_queue test_flag test_refcount_map
	$(TEST) ./test_array && \
	$(TEST) ./test_list  && \
	$(TEST) ./test_tree  && \
	$(TEST) ./test_queue && \
	$(TEST) ./test_flag  && \
	$(TEST) ./test_refcount_map

test_%: test_%.c ../*.h ../*.c ../bits/*.h ../bits/*.c
	$(CC) $< ../*.c ../bits/*.c -o $@ -lpthread

clean:
	rm -rf test_array test_list test_tree test_queue test_flag test_refcount_map
