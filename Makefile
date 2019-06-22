#Lab6

all : testavl lab6_4A

lab6A: lab6_4A.cpp arbreavl.h
	g++ -o lab6_4A lab6_4A.cpp

testavl: testavl.cpp arbreavl.h
	g++ -o testavl testavl.cpp

clean :
	rm -f testavl
	rm -f lab6_4A

