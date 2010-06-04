# 
# autolink GNUmakefile fragment for RHO package
#
#  M. Kunze, Mar 2k
#
# $ID:$
#

ifeq (,$(ROOTVER))
ROOTVER := 3.02-07
endif

ifeq (,$(ROOTSYS))
ROOTPATH := $(BFROOT)/package/root
ROOTSYS = $(ROOTPATH)/$(ROOTVER)/$(BFARCH)
endif

override CPPFLAGS       += -I$(RHO) -I$(ROOTSYS)/include -I$(TOPDIR)/shtmp/$(BFARCH)/$(PACKAGE)

ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lEG -lTreePlayer -lProof 
 
ifneq (,$(findstring Shared,$(ROPT)))

override LDFLAGS        += -L$(RHO)/$(BFARCH)/lib \
			   -lRhoMath_$(BFCURRENT) \
			   -lRhoHistogram_$(BFCURRENT) \
			   -lRhoBase_$(BFCURRENT) \
			   -lRhoConditions_$(BFCURRENT) \
                           -lRhoTools_$(BFCURRENT) \
			   -lRhoNNO_$(BFCURRENT) \
			   -lRhoGA_$(BFCURRENT) \
			   -lRhoSelector_$(BFCURRENT) \
			   -lKangaSchema_$(BFCURRENT) \
			   -lPAFSchema_$(BFCURRENT) \
			   -lRhoManager_$(BFCURRENT) $(ROOTGLIBS)
else

override LDFLAGS        += -L$(RHO)/lib -L$(RHO)/$(BFARCH)/lib \
			-lRhoMath \
			-lRhoBase \
			-lRhoConditions \
			-lRhoTools \
			-lRhoNNO \
			-lRhoGA \
			$(ROOTGLIBS)

endif

override LINK_ROOT       += RHO

