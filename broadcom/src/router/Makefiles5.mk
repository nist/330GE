
################################
# ROBO switch Diags, and Libs
################################
robo:  swlib

swdiag: ncurses swlib
	$(MAKE) -C ../switch/swdiag

swlib:
ifneq ($(wildcard $(TOP)/../switch/swmod/swmod.c),)
	$(MAKE) -C ../switch/swlib
endif

swlib-install:
	#install -m 755 ../switch/swlib/libsw.so $(INSTALLDIR)/robo/usr/lib/
	install -d $(INSTALLDIR)/robo/usr/lib
	install -d $(INSTALLDIR)/robo/usr/sbin
ifneq ($(wildcard $(TOP)/../switch/swmod/swmod.c),)
	install -m 755 ../switch/swlib/libsw.so $(INSTALLDIR)/robo/usr/lib/
	# copy it to platform specific directories for the FSF tree compilation.
	install -d ../switch/swlib/$(PLATFORM)
	cp -rf  ../switch/swlib/libsw.* ../switch/swlib/$(PLATFORM)/
else
	install -m 755 ../switch/swlib/$(PLATFORM)/libsw.so $(INSTALLDIR)/robo/usr/lib/
endif

robo-clean:
ifneq ($(wildcard $(TOP)/../switch/swmod/swmod.c),)
	$(MAKE) -C ../switch clean
endif

robo-install: swlib-install
	#install -m 755 ../switch/swdiag/swdiag $(INSTALLDIR)/robo/usr/sbin/

## ROBO switch end

################################
# PPTP Client
################################
pptpclient pptpclient-%:
        $(MAKE) -C pptpclient $* INSTALLDIR=$(INSTALLDIR)/pptpclient

##PPTP Client end
