SRCDIR = .
SRCINC = ./inc
OBJDIR = .
BINDIR = ./bin


CC=g++
#CXX=g++
TARGET:=httpAgent


CFLAGS:= -g -Wall -Wno-deprecated -I. -fPIC

LDFLAGS:= -L/usr/local/lib -lzlog -lcrypto  -lrdkafka -levpp

SRC := $(wildcard $(SRCDIR)/*.cpp) \
       $(wildcard $(ADD_DIR)/*.cpp)

OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

DEPS = $(SRC:.cpp=.d)
.PHONY : all deps clean veryclean install objs

all : $(TARGET)

deps : $(DEPS)

objs : $(OBJS) $(OBJS1)

$(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) -I $(SRCINC) $< -o $@

install:
	mv $(TARGET) $(BINDIR)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.d
	rm -f $(BINDIR)/$(TARGET)
	rm -f ./$(TARGET)

$(TARGET) : $(OBJS)  
	$(CXX) $(CFLAGS) $(OBJS)  -o $(TARGET) $(LDFLAGS)

