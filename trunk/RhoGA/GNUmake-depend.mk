#PAF standalone makefile
PACKAGE = RhoGA

LIBA	= lib$(PACKAGE).a
LIBSO	= lib$(PACKAGE).so

# Lists of objects to include in library:
NAMELIST = GA1DArrayGenome GAAllele GADemeGA GAListGenome GASStateGA GATree gaerror \
GA1DBinStrGenome GAScaling GATreeBASE \
GA2DArrayGenome GABaseGA GAGenome GASelector GATreeGenome garandom \
GA2DBinStrGenome GABin2DecGenome GAIncGA GAParameter GASimpleGA \
GA3DArrayGenome GABinStr GAList GAPopulation GAStatistics gabincvt \
GA3DBinStrGenome GADCrowdingGA GAListBASE GARealGenome GAStringGenome

HDRS    = $(addsuffix .h, $(NAMELIST) )
OBJS    = $(addsuffix .o, $(NAMELIST) )
#ROOTCINTTARGETS := $(filter-out $(PACKAGE)_LinkDef.rdl, $(wildcard *.rdl)) $(wildcard $(PACKAGE)_LinkDef.rdl)
#ROOTCINTHDRS :=   $(addsuffix .hh, $(basename $(ROOTCINTTARGETS)) )
ROOTCINTHDRS := $(HDRS) $(PACKAGE)_LinkDef.h

# Default action 
all : $(ROOTCINTHDRS) $(LIBA) $(LIBSO)

hdr: $(ROOTCINTHDRS)

shlib: $(LIBSO)

# Static library:
$(LIBA) : $(OBJS)
	rm -f $@
	$(AR) $(ARFLAGS)  $(LIBDIR)/$@ $^

# Shared library:
$(LIBSO) : $(OBJS)
	$(LD) -g -L$(RHO)/lib $(SOFLAGS) $(GLIBS) -o $(LIBDIR)/$@ $^
	$(LD) -dynamiclib -single_module -undefined dynamic_lookup -install_name $(LIBDIR)/$@ -L$(RHO)/lib $(LDFLAGS) $(GLIBS) -o $(LIBDIR)/$@ $^

# Rules for Dictionary:
$(PACKAGE)Cint.o : $(PACKAGE)Cint.cxx 
	$(CXX) $(CXXFLAGS) -c $<
	@echo "-----$(PACKAGE)Cint.o---------"

$(PACKAGE)Cint.cxx : $(ROOTCINTHDRS)
	rootcint -f $@ -c  -I.. $^
	@echo "-----$(PACKAGE)Cint.cxx---------"

# Static pattern rule for object file dependency on sources:
$(OBJS) : %.o : %.cxx  %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

#$(ROOTCINTHDRS) : %.h : %.rdl
#	cp $< $@

# Some extra dependecies from #includes (needs to be completed,
# if in doubt do a "gmake clean" first):

#
# Complete cleanup:
#
clean :
	rm  -f *.o 
	rm  -f $(PACKAGE)Cint.*
	rm  -f $(LIBA) $(LIBSO)


