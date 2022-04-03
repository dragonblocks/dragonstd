CC=cc -g -Wall -Wextra -Werror -fmax-errors=1 -o
TEST=valgrind --leak-check=full --show-leak-kinds=all --quiet

run: test_array test_list test_tree test_queue test_flag test_refcount_map
	$(TEST) ./test_array && \
	$(TEST) ./test_list  && \
	$(TEST) ./test_tree  && \
	$(TEST) ./test_queue && \
	$(TEST) ./test_flag  && \
	$(TEST) ./test_refcount_map

test_array: test_array.c ../array.c ../array.h ../bits/callback.h
	$(CC) test_array test_array.c ../array.c

test_list: test_list.c ../list.c ../list.h ../bits/callback.h ../bits/wrappers.h
	$(CC) test_list test_list.c ../list.c

test_tree: test_tree.c ../tree.c ../tree.h ../bits/callback.h ../bits/wrappers.h
	$(CC) test_tree test_tree.c ../tree.c

test_queue: test_queue.c ../queue.c ../queue.h ../list.c ../list.h ../bits/callback.h ../bits/wrappers.h
	$(CC) test_queue test_queue.c ../queue.c ../list.c -lpthread

test_flag: test_flag.c ../flag.c ../flag.h
	$(CC) test_flag test_flag.c ../flag.c -lpthread

test_refcount_map: test_refcount_map.c ../refcount.c ../refcount.h ../map.c ../map.h ../tree.c ../tree.h ../bits/callback.h ../bits/wrappers.h
	$(CC) test_refcount_map test_refcount_map.c ../refcount.c ../map.c ../tree.c -lpthread

clean:
	rm -rf test_array test_list test_tree test_queue test_flag test_refcount_map
