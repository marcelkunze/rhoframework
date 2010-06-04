# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_RhoSelector.mk,v 1.2 2001-05-16 16:19:30 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_RhoSelector, $(LINK_RhoSelector)]

# Stop gap solution to link RHO (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_RHO	+= $(PACKAGE)GNUmakefile
override LINK_ROOT	+= $(PACKAGE)GNUmakefile

