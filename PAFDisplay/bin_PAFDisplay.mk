# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_PAFDisplay.mk,v 1.1.1.1 2001-05-25 12:00:43 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_PAFDisplay, $(LINK_PAFDisplay)]

# Stop gap solution to link PAF (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_PAF                       += $(PACKAGE)GNUmakefile
override LINK_ROOT                      += $(PACKAGE)GNUmakefile

