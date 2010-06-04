# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_RhoExamples.mk,v 1.1.1.1 2001-05-04 16:12:35 marcel Exp $
#++ NON-STANDARD
	LINKLISTDEPENDS += [LINK_RhoExamples, $(LINK_RhoExamples)]

# Stop gap solution to link Rho (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk
#
override LINK_Rho	+= $(PACKAGE)GNUmakefile
override LINK_ROOT	+= $(PACKAGE)GNUmakefile
