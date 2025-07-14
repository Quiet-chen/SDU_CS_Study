head = train.h 
srcs = train.cc 
objs = train.o 
opts = -w -g -c 
all: train 
train: $(objs)
	g++ $(objs) -o train
train.o: $(srcs) $(head)
	g++ $(opts) $(srcs)
clean:
	rm train *.o