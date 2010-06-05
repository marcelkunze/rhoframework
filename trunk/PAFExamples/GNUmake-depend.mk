# platform independent part

BINDIR = ../bin

PROGRAM = Skim incBDstarPi MuMuAnalysis hadronic dimuon gammagamma twoprong y4sfit
OBJS    = $(addsuffix .o, $(PROGRAM) )

all : $(PROGRAM)

Skim : Skim.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

incBDstarPi : incBDstarPi.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

dimuon : dimuon.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

gammagamma: gammagamma.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

hadronic : hadronic.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

twoprong : twoprong.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

y4sfit: y4sfit.o y4s.o
	$(CXX) -g $(CXXFLAGS) $< y4s.o $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

MuMuAnalysis: MuMuAnalysis.o
	$(CXX) -g $(CXXFLAGS) $< $(RHOLIBS) $(GLIBS) -o $(BINDIR)/$@

# Static pattern rule for object file dependency on sources:
.SUFFIXES: .cc

.cc.o: %.o : %.cc  %.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Some extra dependecies from #includes (needs to be completed,
# if in doubt do a "gmake clean" first):

#
# Complete cleanup:
#
clean :
	rm  -f *.o
	cd $(BINDIR); rm $(PROGRAM)

