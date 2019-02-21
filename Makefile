CC=g++ -fopenmp
CFLAGS= -mavx2 -std=c++14 -O3 -Wall -Wextra -pedantic
LDFLAGS=
EXEC=evaluate evaluate_single search_GC_n3 search_GC_n4 search_depth_n3 search_depth_n4 filter_n3 filter_n4 gen_n3 gen_n4

all: $(EXEC)

evaluate: evaluate.o
	$(CC) -o $@ $^ $(LDFLAGS) -DEVA

evaluate.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DEVA

evaluate_single: evaluate_single.o
	$(CC) -o $@ $^ $(LDFLAGS) -DEVA_Single

evaluate_single.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DEVA_Single

search_GC_n3: search_GC_n3.o
	$(CC) -o $@ $^ $(LDFLAGS) -DGC -DSIZEINBIT=3

search_GC_n3.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DGC -DSIZEINBIT=3

search_GC_n4: search_GC_n4.o
	$(CC) -o $@ $^ $(LDFLAGS) -DGC -DSIZEINBIT=4

search_GC_n4.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DGC -DSIZEINBIT=4


search_depth_n3: search_depth_n3.o
	$(CC) -o $@ $^ $(LDFLAGS) -DDEPTH -DSIZEINBIT=3

search_depth_n3.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DDEPTH -DSIZEINBIT=3

search_depth_n4: search_depth_n4.o
	$(CC) -o $@ $^ $(LDFLAGS) -DDEPTH -DSIZEINBIT=4

search_depth_n4.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DDEPTH -DSIZEINBIT=4


filter_n3: filter_n3.o
	$(CC) -o $@ $^ $(LDFLAGS) -DFILTER -DSIZEINBIT=3

filter_n3.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DFILTER -DSIZEINBIT=3

filter_n4: filter_n4.o
	$(CC) -o $@ $^ $(LDFLAGS) -DFILTER -DSIZEINBIT=4

filter_n4.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DFILTER -DSIZEINBIT=4


gen_n3: gen_n3.o
	$(CC) -o $@ $^ $(LDFLAGS) -DGEN -DSIZEINBIT=3

gen_n3.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DGEN -DSIZEINBIT=3

gen_n4: gen_n4.o
	$(CC) -o $@ $^ $(LDFLAGS) -DGEN -DSIZEINBIT=4

gen_n4.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -DGEN -DSIZEINBIT=4


clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	rm test_faster
	rm test_lighter

gencheck:
	find ./R -maxdepth 1 -name "*.c" -print > results.c
	$(CC) $(CFLAGS) genTest.cpp -o genTest && ./genTest

checkfaster:
	$(CC) $(CFLAGS) faster_test.cpp -o test_faster && ./test_faster

checklighter:
	$(CC) $(CFLAGS) faster_test.cpp -o test_lighter && ./test_lighter