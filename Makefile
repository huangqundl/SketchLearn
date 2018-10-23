EXEC += dp_simulate
EXEC += dp_ovs
EXEC += controller
EXEC += trace_preprocess
EXEC += true_flows

all: $(EXEC)

CFLAGS = -Wall -std=c++11 -O3 -mavx2 # -mavx512f -mavx512cd -mavx512pf -mavx512er #-fno-omit-frame-pointer -O3 -mavx2
CFLAGS += -D TRACE_DIR_RAM

HEADER += util.h packet.h config.h hash.h tuple.h adapter_record_ram.h adapter_record_ram.c
SRC += util.c packet.c config.c hash.c cpu.c global.c
LIBS = -lpthread -lrt -lpcap -lm -liniparser

dp_simulate: dp_simulate.c alg_sketchlearn.h alg_sketchlearn.c ringbuffer.h ringbuffer.c $(SRC) $(HEADER)
	g++ $(CFLAGS) -o $@ $< $(SRC) ringbuffer.c alg_keytbl.c alg_sketchlearn.c adapter_record_ram.c $(LIBS)

dp_ovs: dp_ovs.c alg_sketchlearn.h alg_sketchlearn.c ringbuffer.h ringbuffer.c $(SRC) $(HEADER)
	g++ $(CFLAGS) -o $@ $< $(SRC) ringbuffer.c alg_keytbl.c alg_sketchlearn.c adapter_record_ram.c $(LIBS) -g

controller: controller.c alg_sketchlearn.h alg_sketchlearn.c ringbuffer.h ringbuffer.c $(SRC) $(HEADER)
	g++ $(CFLAGS) -o $@ $< $(SRC) ringbuffer.c alg_keytbl.c alg_sketchlearn.c $(LIBS)

trace_preprocess: trace_preprocess.c adapter_dir.h adapter_dir.c packet.h packet.c
	g++ $(CFLAGS) -o $@ $< config.c global.c packet.c adapter_dir.c $(LIBS) 

true_flows: true_flows.c alg_keytbl.h alg_keytbl.c $(SRC) $(HEADER)
	g++ $(CFLAGS) -o $@ alg_keytbl.c ringbuffer.c adapter_record_ram.c $< $(SRC) $(LIBS)


clean:
	rm -rf $(EXEC)
