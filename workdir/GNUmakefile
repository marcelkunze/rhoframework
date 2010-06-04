# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile,v 1.1 2001-05-27 07:36:18 marcel Exp $
#################################################################
#++ library products				[build it with 'lib']

LIBREMOVEFILES := 
LIBTMPLFILES := 
LIBDDLORDERED := 

#################################################################
#++ extra binary products	[not in production, build it with extrabin]

EXTRABINS := 

#################################################################
#++ binary products				[build it with 'bin']

BINS := 
BINCCFILES := 

#++ Binary rules		 [in production, build it with 'bin']

#++ shell script products.. 			[build it with 'bin']
BINSCRIPTS := 

#################################################################
#++ regression test scripts			[build it with 'test']
#################################################################
#++ NON-STANDARD
SHELL := /bin/sh
MAKEFLAGS = --no-print-directory -s

help:
	echo "gmake options:"
	echo "  gmake setup RELDIR=dirname       Initial setup. RELDIR optional."
	echo "  gmake release RELDIR=dirname     Set release directory to dirname"
	echo "     Used to switch easily between release directories."
	echo "  gmake migrate TODIR=dirname       Copy workdir to new location"
	echo "     Used to move workdir out of a release to a private location,"
	echo "     or replicate it"

## Initial setup
setup: release
	if [ ! -d dat ] ; then \
		if [ -d RELEASE/gnbase ]; then \
			ln -f -s RELEASE/gnbase/dat dat;\
		else \
			ln -f -s PARENT/gnbase/dat dat;\
		fi; \
	fi
	if test ! -d bin ; then ln -f -s RELEASE/bin bin ; fi

## RELEASE is the link to the release directory.
## By default, it assumes that the workdir directory is
## sitting directly under the release directory.
#RELEASE:
#	if test ! -d RELEASE ; then echo "No (or bad) release directory specified. \
#    Release directory assumed to be ../" ; ln -f -s ../ RELEASE ; fi

## Set release directory. 'gmake release RELDIR=dirname'
release: RELEASE PARENT ONLINEPARENT GraDisplay bfield.dat 

RELEASE:
	rm -f RELEASE ; if [ "$(RELDIR)" = "" ] ; then \
      echo RELDIR not specified. Defaults to ../ ; RELDIR=../ ; \
      else RELDIR=$(RELDIR) ; fi ; \
    ln -f -s $$RELDIR RELEASE ; echo Release directory set to $$RELDIR

PARENT: RELEASE
	rm -f PARENT
	if [ -r RELEASE/.current ] ; then \
		ln -s ${BFDIST}/releases/`cat RELEASE/.current` PARENT ; \
	else  ln -s RELEASE PARENT ; fi ;

ONLINEPARENT:
	rm -f ONLINEPARENT
	if [ -r ../.online ] ; then \
	    	if [ ! -d ${BFDIST}/online_releases/`cat ../.online` ]; then \
		    echo "warning: `cat ../.online` does not exist for ONLINEPARENT"; \
	 	fi; \
		ln -s ${BFDIST}/online_releases/`cat ../.online` ONLINEPARENT ; \
	fi ;

GraDisplay: RELEASE PARENT
	rm -f GraDisplay
	if [ -r RELEASE/GraDisplay ] ; then \
		ln -s RELEASE/GraDisplay GraDisplay ; \
	else  ln -s PARENT/GraDisplay GraDisplay ; fi ;

bfield.dat: RELEASE PARENT
	rm -f bfield.dat
	if [ -r RELEASE/BFieldImpl/bfield.dat ] ; then \
		ln -s RELEASE/BFieldImpl/bfield.dat bfield.dat ; \
	else  ln -s PARENT/BFieldImpl/bfield.dat bfield.dat ; fi ;

migrate:
	if [ "$(TODIR)" = "" ] ; then \
      echo 'Usage: gmake migrate TODIR=destination' ; \
    else \
       echo Copying workdir to $(TODIR) ; \
       if test -d $(TODIR) ; then echo "Directory $(TODIR) already exists! Abort." ; \
       exit ; fi ; mkdir $(TODIR) ; \
       $(MAKE) clean ;  tar cvf $(TODIR)/copy.tar . ; \
       cd $(TODIR) ; tar xvf copy.tar ; rm copy.tar ; \
       echo "workdir copied. You must 'gmake setup ...' in the new directory."; \
       echo "You also have to redo 'gmake setup ...' in this directory."; \
    fi	

clean:
	rm -f RELEASE dat

schema:

include:

lib:

shlib:

slib:

bin:

rbin:

binscripts:

.PHONY: release include lib shlib bin binscripts ONLINEPARENT

#################################################################
#++ include standard makefile from SoftRelTools.
# CANNOT USE THIS FOR WORKDIR include SoftRelTools/standard.mk
