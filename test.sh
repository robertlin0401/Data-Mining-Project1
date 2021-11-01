make
./apriori.exe -i data/test.txt -o output/output-test.txt -s 0.2 -c 0.5
./apriori.exe -i data/ibm-2020.txt -o output/output-2020.txt -s 0.2 -c 0.9
./apriori.exe -i data/ibm-2021.txt -o output/output-2021.txt -s 0.02 -c 0.9
make clean
