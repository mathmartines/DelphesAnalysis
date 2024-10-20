CXX = g++
CPPFLAGS = -I/opt/homebrew/Cellar/root/6.32.06/include/root -I/Users/martines/Desktop/Physics/delphes-master -I/Users/martines/Desktop/PhD/Programs/DelphesAnalysis/include
CXXFLAGS = -stdlib=libc++ -pthread -std=c++17 -m64 
LDFLAGS = -L/opt/homebrew/Cellar/root/6.32.06/lib/root -L/Users/martines/Desktop/Physics/delphes-master -Wl,-rpath,/opt/homebrew/Cellar/root/6.32.06/lib/root -Wl,-rpath,/Users/martines/Desktop/Physics/delphes-master
LIBS = -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lDelphes -stdlib=libc++ -lpthread -lm -ldl

IDIR = include/DelphesAnalysis
ODIR = lib

_DEPS = EventData EventLoop
DEPS = $(patsubst %, $(IDIR)/%.h, $(_DEPS)) 
OBJ = $(patsubst %, $(ODIR)/%.o, $(_DEPS))

# Ensure that the output directory exists
$(ODIR):
	mkdir -p $(ODIR)

# Compile object files from the source files
$(ODIR)/%.o: src/%.cpp $(IDIR)/%.h | $(ODIR)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for ATLAS analysis
ATLAS13DITAUPATH = examples/DY/di-tau-ATLAS13TEV

ditauAtlas13tev: ditauAtlas13tev.o $(OBJ) ditauAtlas13tev_selections.o ditauAtlas13tev_cuts.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

ditauAtlas13tev.o: $(ATLAS13DITAUPATH)/ditauAtlas13tev.cpp $(DEPS) $(ATLAS13DITAUPATH)/ditauAtlas13tev_selections.h $(ATLAS13DITAUPATH)/ditauAtlas13tev_cuts.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

ditauAtlas13tev_selections.o: $(ATLAS13DITAUPATH)/ditauAtlas13tev_selections.cpp $(ATLAS13DITAUPATH)/ditauAtlas13tev_selections.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

ditauAtlas13tev_cuts.o: $(ATLAS13DITAUPATH)/ditauAtlas13tev_cuts.cpp $(ATLAS13DITAUPATH)/ditauAtlas13tev_cuts.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -rf $(ODIR)/*.o 
	rm -rf *.o 
	rm -rf ditauAtlas13tev

# Phony targets
.PHONY: clean