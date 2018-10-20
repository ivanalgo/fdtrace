
SOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))
IS = $(patsubst %.cpp, %.i, $(SOURCES))
ICS = $(patsubst %.cpp, %.ic, $(SOURCES))

CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -fPIC -shared
LDFLAGS = -ldl -fPIC -shared

all: fdcheck.so 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.i: %.cpp
	$(CXX) $(CXXFLAGS) -E -o $@ $<

%.ic: %.i
	indent $< -o $@

fdcheck.so: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(IS) $(ICS) fdcheck.so
