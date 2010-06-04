# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_RhoHistogram.mk,v 1.1.1.1 2001-05-04 11:54:04 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_RhoHistogram, $(LINK_RhoHistogram)]

# Stop gap solution to link Rho (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_RHO	+= $(PACKAGE)GNUmakefile
override LINK_ROOT	+= $(PACKAGE)GNUmakefile

