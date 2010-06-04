# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_RhoMath.mk,v 1.1.1.1 2001-05-04 16:16:37 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_RhoMath, $(LINK_RhoMath)]

# Stop gap solution to link Rho (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_RHO	+= $(PACKAGE)GNUmakefile
override LINK_ROOT	+= $(PACKAGE)GNUmakefile

