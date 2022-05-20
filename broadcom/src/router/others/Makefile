ifneq ($(wildcard $(SRCBASE)/cy_conf.mak),)
  include $(SRCBASE)/cy_conf.mak
endif

all :

install: all
	install -d $(INSTALLDIR)/etc
	install -d $(INSTALLDIR)/sbin
	install -d $(INSTALLDIR)/usr/sbin
	install -d $(INSTALLDIR)/lib
	install -d $(INSTALLDIR)/www
	install -d $(INSTALLDIR)/lib/modules/2.4.20/parport

	install services $(INSTALLDIR)/etc
	echo "order hosts,bind" > $(INSTALLDIR)/etc/host.conf
	echo "hosts: files dns" > $(INSTALLDIR)/etc/nsswitch.conf
	install netcam_mfc_activeX.cab $(INSTALLDIR)/www

ifeq ($(CONFIG_CHIP5350),y)
	install 5350/lp.o $(INSTALLDIR)/lib/modules/2.4.20/parport
	install 5350/parport.o $(INSTALLDIR)/lib/modules/2.4.20/parport
	install 5350/parport_splink.o $(INSTALLDIR)/lib/modules/2.4.20/parport
else
	install 4702/lp.o $(INSTALLDIR)/lib/modules/2.4.20/parport
	install 4702/parport.o $(INSTALLDIR)/lib/modules/2.4.20/parport
	install 4702/parport_splink.o $(INSTALLDIR)/lib/modules/2.4.20/parport
endif

	#install waveserver $(INSTALLDIR)/usr/sbin/waveserver
	
	ln -sf /tmp/resolv.conf $(INSTALLDIR)/etc/resolv.conf
	ln -sf /tmp/hosts $(INSTALLDIR)/etc/hosts
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500g.Deluxe.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500g.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500gv2.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL520g.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500g-X.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500b.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500bv2.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL300g.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/WL500g.Premium.CFG
	ln -sf /tmp/settings $(INSTALLDIR)/www/ASUS\ Wireless\ Router.CFG	
	ln -sf /tmp/settings $(INSTALLDIR)/www/Settings.CFG

	ln -sf /var/tmp/display.jpg $(INSTALLDIR)/www/display.jpg

ifeq ($(SYSLOG_SUPPORT),1)
	install -m 755 rotatelog.sh $(INSTALLDIR)/usr/sbin
endif

clean:

