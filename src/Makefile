CC	 := gcc
obj  := hash.o itemset.o transaction.o apriori.o frequent.o
flag := -Wall

all: apriori

apriori: $(obj)
	$(CC) $(flag) -o $@ $(obj)

%.o: %.c
	$(CC) -c $^

.PHONY:clean
clean:
	rm -rf $(obj)
