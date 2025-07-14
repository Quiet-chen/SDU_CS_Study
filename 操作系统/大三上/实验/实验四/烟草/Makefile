hdrs = ipc.h
opts = -g -c 

p_src = producer.c ipc.c
p_obj = producer.o ipc.o
c_T_src = consumer_tobacco.c ipc.c
c_T_obj = consumer_tobacco.o ipc.o
c_P_src = consumer_paper.c ipc.c
c_P_obj = consumer_paper.o ipc.o
c_G_src = consumer_glue.c ipc.c
c_G_obj = consumer_glue.o ipc.o

all: producer consumer_tobacco consumer_paper consumer_glue

producer: $(p_obj)
	gcc $(p_obj) -o producer
producer.o: $(p_src) $(hdrs)
	gcc $(opts) $(p_src)

consumer_tobacco: $(c_T_obj)
	gcc $(c_T_obj) -o consumer_tobacco
consumer_tobacco.o: $(c_T_src) $(hdrs)
	gcc $(opts) $(c_T_src)

consumer_paper: $(c_P_obj)
	gcc $(c_P_obj) -o consumer_paper
consumer_paper.o: $(c_P_src) $(hdrs)
	gcc $(opts) $(c_P_src)

consumer_glue: $(c_G_obj)
	gcc $(c_G_obj) -o consumer_glue
consumer_glue.o: $(c_G_src) $(hdrs)
	gcc $(opts) $(c_G_src)

clean: 
	rm producer consumer_tobacco consumer_paper consumer_glue *.o
