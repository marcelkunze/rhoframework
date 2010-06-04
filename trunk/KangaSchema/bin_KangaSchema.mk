# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_KangaSchema.mk,v 1.1.1.1 2001-05-15 07:01:39 marcel Exp $
#++ NON-STANDARD
        LINKLISTDEPENDS += [LINK_KangaSchema, $(LINK_KangaSchema)]

# Stop gap solution to link Rho (it is not yet in PackageList)
include $(TOPDIR)/RhoMakefiles/link_Rho.mk

override LINK_RHO                       += $(PACKAGE)GNUmakefile
override LINK_ROOT                      += $(PACKAGE)GNUmakefile
override LINK_RW                        += $(PACKAGE)GNUmakefile

