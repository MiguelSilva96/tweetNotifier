# compiler
CXX = g++
CXXFALGS = -std=c++11 -Wpedantic -Wall
LFLAGS = -loauth -lcurl -lsfml-audio
INCLUDES = -I.
SRCS = tweetNotifier.cpp exampleApp.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = tweetNotifier

all: $(MAIN)
	@echo compiled

$(MAIN): $(OBJS)
	$(CXX) $(CXXFALGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFALGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
