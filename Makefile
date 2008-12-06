CXX = g++
#CXXFLAGS = -march=opteron

SOURCES = main.cpp Lattice.cpp Site.cpp Species.cpp 
SOURCES += TwoBodyInteractions.cpp 
SOURCES += TwoBodyInteractionEnergyContainer.cpp SelfEnergy.cpp
SOURCES += OrderParameter.cpp Bridge.cpp
SOURCES += Metropolis.cpp
SOURCES += Frm.cpp Reaction.cpp Diffusion.cpp
TARGET = dmcut2

LDFLAGS =
CXXFLAGS += -I. -DHAVE_SSE2

OBJECTS = Random.o
OBJECTS += $(SOURCES:.cpp=.o)

INC = .
RM = rm -f

DOC = doxygen
DOCCFG = $(DOC).cfg

# BEGIN *NIX
#CXXFLAGS = -msse2 -pipe
#CXXFLAGS += -fno-rtti
#CXXFLAGS += -fno-threadsafe-statics
#LDFLAGS += -Wl,--as-needed
#LDFLAGS += -Wl,--hash-style=gnu
# IF C2D only, gcc-4.2
#CXXFLAGS += -march=nocona
# END IF
# IF OSX
#CXXFLAGS += -fast
# ELSE IF LINUX
CXXFLAGS += -O2 -Wall
# END IF
#CXXFLAGS += -fvisibility-inlines-hidden -ftree-vectorize
# END *NIX

# BEGIN 64-bit
CXXFLAGS += -m64 -DMTPREC64
LDFLAGS += -Wl,-arch,x86_64
# END 64-bits

# BEGIN Release
CXXFLAGS += -DNDEBUG
#CXXFLAGS += -DBOOST_NO_EXCEPTIONS
# END Release

CXXFLAGS += -I$(INC)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $? $(LDFLAGS) -o $@ $(LIBS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -g -pg $? -o $(TARGET) $(LIBS)

Random.o: RandomLib/Random.cpp
	$(CXX) $(CXXFLAGS) -funroll-loops -finline-functions $^ -c -o $@

#.cpp: .hpp
#.o: .cpp
#	$(CXX) $(CXXFLAGS) -I$(INC) -c -o $@

.o: .cpp

clean:
	$(RM) *.o dmcut2

