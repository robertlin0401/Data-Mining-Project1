CC	 := gcc
obj  := hash.o itemset.o transaction.o apriori.o frequent.o
path := src/
flag := -Wall

all: apriori

apriori: $(obj)
	$(CC) $(flag) -o $@ $(obj)

%.o: $(path)%.c
	$(CC) -c $^

.PHONY:clean
clean:
	rm -rf $(obj)
