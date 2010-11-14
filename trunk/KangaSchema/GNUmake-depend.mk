#Standalone makefile
PACKAGE = KangaSchema

LIBA	= lib$(PACKAGE).a
LIBSO	= lib$(PACKAGE).so

# Lists of objects to include in library:
NAMELIST = RooEvtObj \
           RooPersObj \
           RooRef \
           RooClonesVectorI \
           RooClonesVectorR \
           RooClonesClassObj \
           BtaCandidateR \
	   BtaFitParamsR \
	   BtaFitParamsR_002 \
           BtaMicroCandR \
           BtaMicroCandR_001 \
           BtaMicroCandR_002 \
           BtaMicroCandR_003 \
           BtaMicroCandR_004 \
	   BtaMicroCandR_005 \
	   BtaCompositeCandR_000 \
           BtaCandListR \
	   BtaCandListR_001 \
	   BtaClonedCandR_000 \
	   BtaClonedCompositeCandR_000 \
           BtaCandPtrR \
           EidInfoR \
           StdHepR \
           StdHepTrkR \
           kanga \
           TBtaMicroCandR \
           TBtaFitParamsR \
           TBtaFitParamsR_001 \
           TBtaTrkQualR \
           TBtaCalQualR \
           TBtaPidQualR \
           TBtaIfrQualR \
           TBtaPidInfoR \
	   BtaAbsVertexR \
	   BtaCandAssocMapR \
	   ComPackBase \
	   ComPackExpFloat \
	   ComPackFlatFloat \
	   ComPackFloat \
	   ComPackNonFlatFloat \
	   ComPackUFloat \
	   RooValArray \
	   VtxVertexR \
	   BtaMicroCandidate \
	   KangaReader

HDRS    = $(addsuffix .h, $(NAMELIST) )
OBJS    = $(addsuffix .o, $(NAMELIST) )
#ROOTCINTTARGETS := $(filter-out $(PACKAGE)_LinkDef.rdl, $(wildcard *.rdl)) $(wildcard $(PACKAGE)_LinkDef.rdl)
#ROOTCINTHDRS :=   $(addsuffix .h, $(basename $(ROOTCINTTARGETS)) )
ROOTCINTHDRS := $(HDRS) $(PACKAGE)_LinkDef.h

# Default action 
all : $(ROOTCINTHDRS) $(LIBA) $(LIBSO)

hdr: $(ROOTCINTHDRS)

shlib: $(LIBSO)

# Static library:
$(LIBA) : $(OBJS)  $(PACKAGE)Cint.o
	-rm -f $@
	$(AR) $(ARFLAGS)  $(LIBDIR)/$@ $^

# Shared library:
$(LIBSO) : $(OBJS)  $(PACKAGE)Cint.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(GLIBS) -o $(LIBDIR)/$(LIBSO) $(EXPLLINKLIBS)

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
	-rm  -f *.o 
	-rm  -f $(PACKAGE)Cint.*
	-rm  -f $(LIBA) $(LIBSO)


