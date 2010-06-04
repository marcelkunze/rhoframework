# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_PAFSchema.mk,v 1.2 2001-05-16 16:57:23 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_PAFSchema, $(LINK_PAFSchema)]

# Stop gap solution to link Rho (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_RHO                       += $(PACKAGE)GNUmakefile
override LINK_ROOT                      += $(PACKAGE)GNUmakefile

