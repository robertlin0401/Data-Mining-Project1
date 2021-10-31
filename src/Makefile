CC	:= gcc
obj := hash.o itemset.o transaction.o apriori.o

all: apriori

apriori: $(obj)
	$(CC) -o $@ $(obj)

%.o: %.c
	$(CC) -c $^

.PHONY:clean
clean:
	rm -rf $(obj)
