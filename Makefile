CXX = g++
CPPFLAGS = -I/opt/homebrew/Cellar/root/6.32.06/include/root -I/Users/martines/Desktop/Physics/delphes-master -I/Users/martines/Desktop/PhD/Programs/DelphesAnalysis/include -I/Users/martines/Desktop/PhD/Programs/DelphesAnalysis/external
CXXFLAGS = -stdlib=libc++ -pthread -std=c++17 -m64 
LDFLAGS = -L/opt/homebrew/Cellar/root/6.32.06/lib/root -L/Users/martines/Desktop/Physics/delphes-master -Wl,-rpath,/opt/homebrew/Cellar/root/6.32.06/lib/root -Wl,-rpath,/Users/martines/Desktop/Physics/delphes-master
LIBS = -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lDelphes -stdlib=libc++ -lpthread -lm -ldl

IDIR = include/DelphesAnalysis
ODIR = lib

_DEPS = EventData EventLoop EventAnalysis Observable ObjectSelection Distribution Utilities
DEPS = $(patsubst %, $(IDIR)/%.h, $(_DEPS)) 
OBJ = $(patsubst %, $(ODIR)/%.o, $(_DEPS))

# Ensure that the output directory exists
$(ODIR):
	mkdir -p $(ODIR)

# Compile object files from the source files
$(ODIR)/%.o: src/%.cpp $(IDIR)/%.h | $(ODIR)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for ATLAS analysis
ATLAS13DITAUPATH = examples/DY/atlas-ditau-13TEV

atlas_ditauhad_13TEV: atlas_ditauhad_13TEV.o $(OBJ) SelectionsATLAS-ditau.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_ditauhad_13TEV.o: $(ATLAS13DITAUPATH)/atlas_ditauhad_13TEV.cpp $(DEPS) $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(ATLAS13DITAUPATH)/EventDataATLAS.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_ditauhad_bveto_13TEV: atlas_ditauhad_bveto_13TEV.o $(OBJ) SelectionsATLAS-ditau.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_ditauhad_bveto_13TEV.o: $(ATLAS13DITAUPATH)/atlas_ditauhad_bveto_13TEV.cpp $(DEPS) $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(ATLAS13DITAUPATH)/EventDataATLAS.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_ditauhad_bveto_13TEV_eft_terms: atlas_ditauhad_bveto_13TEV_eft_terms.o $(OBJ) SelectionsATLAS-ditau.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_ditauhad_bveto_13TEV_eft_terms.o: $(ATLAS13DITAUPATH)/atlas_ditauhad_bveto_13TEV_eft_terms.cpp $(DEPS) $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(ATLAS13DITAUPATH)/EventDataATLAS.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_ditauhad_bveto_13TEV_leptoquark: atlas_ditauhad_bveto_13TEV_leptoquark.o $(OBJ) SelectionsATLAS-ditau.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_ditauhad_bveto_13TEV_leptoquark.o: $(ATLAS13DITAUPATH)/atlas_ditauhad_bveto_13TEV_leptoquark.cpp $(DEPS) $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(ATLAS13DITAUPATH)/EventDataATLAS.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsATLAS-ditau.o: $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.cpp $(ATLAS13DITAUPATH)/SelectionsATLAS-ditau.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for the CMS dilepton analysis
CMS13TEVDILEPTON = examples/DY/cms-dilepton-13TEV

cms_dielectron_13TEV: cms_dielectron_13TEV.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dielectron_13TEV.o: $(CMS13TEVDILEPTON)/cms_dielectron_13TEV.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_dielectron_13TEV_eft_terms: cms_dielectron_13TEV_eft_terms.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dielectron_13TEV_eft_terms.o: $(CMS13TEVDILEPTON)/cms_dielectron_13TEV_eft_terms.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_dimuon_13TEV: cms_dimuon_13TEV.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dimuon_13TEV.o: $(CMS13TEVDILEPTON)/cms_dimuon_13TEV.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_dimuon_13TEV_eft_terms: cms_dimuon_13TEV_eft_terms.o $(OBJ) SelectionsCMS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_dimuon_13TEV_eft_terms.o: $(CMS13TEVDILEPTON)/cms_dimuon_13TEV_eft_terms.cpp $(DEPS) $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(CMS13TEVDILEPTON)/EventDataCMS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsCMS_dilepton.o: $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.cpp $(CMS13TEVDILEPTON)/SelectionsCMS_dilepton.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for the CMS monolepton analysis
CMS13TEVMONOLEPTON = examples/DY/cms-monolepton-13TEV

cms_monoelectron_13TEV: cms_monoelectron_13TEV.o $(OBJ) SelectionsCMS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_monoelectron_13TEV.o: $(CMS13TEVMONOLEPTON)/cms_monoelectron_13TEV.cpp $(DEPS) $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.h $(CMS13TEVMONOLEPTON)/EventDataCMS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_monoelectron_13TEV_eft_terms: cms_monoelectron_13TEV_eft_terms.o $(OBJ) SelectionsCMS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_monoelectron_13TEV_eft_terms.o: $(CMS13TEVMONOLEPTON)/cms_monoelectron_13TEV_eft_terms.cpp $(DEPS) $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.h $(CMS13TEVMONOLEPTON)/EventDataCMS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_monomuon_13TEV: cms_monomuon_13TEV.o $(OBJ) SelectionsCMS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_monomuon_13TEV.o: $(CMS13TEVMONOLEPTON)/cms_monomuon_13TEV.cpp $(DEPS) $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.h $(CMS13TEVMONOLEPTON)/EventDataCMS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

cms_monomuon_13TEV_eft_terms: cms_monomuon_13TEV_eft_terms.o $(OBJ) SelectionsCMS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

cms_monomuon_13TEV_eft_terms.o: $(CMS13TEVMONOLEPTON)/cms_monomuon_13TEV_eft_terms.cpp $(DEPS) $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.h $(CMS13TEVMONOLEPTON)/EventDataCMS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsCMS_monolep.o: $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.cpp $(CMS13TEVMONOLEPTON)/SelectionsCMS_monolep.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for the ATLAS monolepton analysis
ATLAS13TEVMONOLEP = examples/DY/atlas-monolepton-13TEV

atlas_monoelectron_13TEV: atlas_monoelectron_13TEV.o $(OBJ) SelectionsATLAS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_monoelectron_13TEV.o: $(ATLAS13TEVMONOLEP)/atlas_monoelectron_13TEV.cpp $(DEPS) $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.h $(ATLAS13TEVMONOLEP)/EventDataATLAS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_monoelectron_13TEV_eft_terms: atlas_monoelectron_13TEV_eft_terms.o $(OBJ) SelectionsATLAS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_monoelectron_13TEV_eft_terms.o: $(ATLAS13TEVMONOLEP)/atlas_monoelectron_13TEV_eft_terms.cpp $(DEPS) $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.h $(ATLAS13TEVMONOLEP)/EventDataATLAS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_monomuon_13TEV: atlas_monomuon_13TEV.o $(OBJ) SelectionsATLAS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_monomuon_13TEV.o: $(ATLAS13TEVMONOLEP)/atlas_monomuon_13TEV.cpp $(DEPS) $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.h $(ATLAS13TEVMONOLEP)/EventDataATLAS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

atlas_monomuon_13TEV_eft_terms: atlas_monomuon_13TEV_eft_terms.o $(OBJ) SelectionsATLAS_monolep.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_monomuon_13TEV_eft_terms.o: $(ATLAS13TEVMONOLEP)/atlas_monomuon_13TEV_eft_terms.cpp $(DEPS) $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.h $(ATLAS13TEVMONOLEP)/EventDataATLAS_monolep.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsATLAS_monolep.o: $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.cpp $(ATLAS13TEVMONOLEP)/SelectionsATLAS_monolep.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Specific targets for the ATLAS dilepton analysis
ATLAS13TEVDILEP = examples/DY/atlas-dilepton-13TEV

atlas_dielectron_13TEV_eft_terms: atlas_dielectron_13TEV_eft_terms.o $(OBJ) SelectionsATLAS_dilepton.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

atlas_dielectron_13TEV_eft_terms.o: $(ATLAS13TEVDILEP)/atlas_dielectron_13TEV_eft_terms.cpp $(DEPS) $(ATLAS13TEVDILEP)/SelectionsATLAS_dilepton.h $(ATLAS13TEVDILEP)/EventDataATLAS_dilepton.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

SelectionsATLAS_dilepton.o: $(ATLAS13TEVDILEP)/SelectionsATLAS_dilepton.cpp $(ATLAS13TEVDILEP)/SelectionsATLAS_dilepton.h $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -rf $(ODIR)/*.o 
	rm -rf *.o 
	rm -rf ditauAtlas13tev
	rm -rf atlas_ditauhad_bveto_13TEV
	rm -rf cms_dielectron_13TEV
	rm -rf cms_dielectron_13TEV_eft_terms
	rm -rf cms_dimuon_13TEV_eft_terms
	rm -rf cms_dimuon_13TEV
	rm -rf cms_monoelectron_13TEV
	rm -rf cms_monoelectron_13TEV_eft_terms
	rm -rf cms_monomuon_13TEV
	rm -rf cms_monomuon_13TEV_eft_terms
	rm -rf atlas_monoelectron_13TEV
	rm -rf atlas_monoelectron_13TEV_eft_terms
	rm -rf atlas_monomuon_13TEV
	rm -rf atlas_monomuon_13TEV_eft_terms
	rm -rf atlas_dielectron_13TEV_eft_terms
	rm -rf atlas_ditauhad_bveto_13TEV_eft_terms
	
# Phony targets
.PHONY: clean