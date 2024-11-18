test: NatAllocator.o test.cpp
	g++ test.cpp NatAllocator.o -o app
NatAllocator.o: NatAllocator.h NatAllocator.cpp
	g++ -c NatAllocator.cpp -o NatAllocator.o