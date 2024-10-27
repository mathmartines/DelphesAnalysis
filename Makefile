CXX = g++
CPPFLAGS = -I/opt/homebrew/Cellar/root/6.32.06/include/root -I/Users/martines/Desktop/Physics/delphes-master -I/Users/martines/Desktop/PhD/Programs/DelphesAnalysis/include
CXXFLAGS = -stdlib=libc++ -pthread -std=c++17 -m64 
LDFLAGS = -L/opt/homebrew/Cellar/root/6.32.06/lib/root -L/Users/martines/Desktop/Physics/delphes-master -Wl,-rpath,/opt/homebrew/Cellar/root/6.32.06/lib/root -Wl,-rpath,/Users/martines/Desktop/Physics/delphes-master
LIBS = -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lDelphes -stdlib=libc++ -lpthread -lm -ldl

IDIR = include/DelphesAnalysis
ODIR = lib

_DEPS = EventData EventLoop Analysis Observable
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

ditauAtlas13tev: ditauAtlas13tev.o $(OBJ) SelectionsATLAS-ditau.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

ditauAtlas13tev.o: $(ATLAS13DITAUPATH)/ditauAtlas13tev.cpp $(DEPS) $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(ATLAS13DITAUPATH)/EventDataATLAS.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsATLAS-ditau.o: $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.cpp $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for the CMS dielectron analysis
CMS13TEVDILEPTON = examples/DY/cms-dilepton-13TEV

cms_dielectron_13TEV: cms_dielectron_13TEV.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dielectron_13TEV.o: $(CMS13TEVDILEPTON)/cms_dielectron_13TEV.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsCMS_dilepton.o: $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.cpp $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_dimuon_13TEV: cms_dimuon_13TEV.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dimuon_13TEV.o: $(CMS13TEVDILEPTON)/cms_dimuon_13TEV.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -rf $(ODIR)/*.o 
	rm -rf *.o 
	rm -rf ditauAtlas13tev
	rm -rf cms_dielectron_13TEV
	rm -rf cms_dimuon_13TEV

# Phony targets
.PHONY: clean