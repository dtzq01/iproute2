# SPDX-License-Identifier: GPL-2.0
IPCOMMON=ipaddress.o ipaddrlabel.o iproute.o iprule.o ipnetns.o \
    rtm_map.o iptunnel.o ip6tunnel.o tunnel.o ipneigh.o ipntable.o iplink.o \
    ipmaddr.o ipmonitor.o ipmroute.o ipprefix.o iptuntap.o iptoken.o \
    ipxfrm.o xfrm_state.o xfrm_policy.o xfrm_monitor.o iplink_dummy.o \
    iplink_ifb.o iplink_nlmon.o iplink_team.o iplink_vcan.o iplink_vxcan.o \
    iplink_vlan.o link_veth.o link_gre.o iplink_can.o iplink_xdp.o \
    iplink_macvlan.o ipl2tp.o link_vti.o link_vti6.o link_xfrm.o \
    iplink_vxlan.o tcp_metrics.o iplink_ipoib.o ipnetconf.o link_ip6tnl.o \
    link_iptnl.o link_gre6.o iplink_bond.o iplink_bond_slave.o iplink_hsr.o \
    iplink_bridge.o iplink_bridge_slave.o iplink_dsa.o ipfou.o iplink_ipvlan.o \
    iplink_geneve.o iplink_vrf.o iproute_lwtunnel.o ipmacsec.o ipila.o \
    ipvrf.o iplink_xstats.o ipseg6.o iplink_netdevsim.o iplink_rmnet.o \
    ipnexthop.o ipmptcp.o iplink_bareudp.o iplink_wwan.o ipioam6.o \
    iplink_amt.o iplink_batadv.o iplink_gtp.o iplink_virt_wifi.o \
    ipstats.o
IPOBJ=ip.o 
LIB_IP=iprt2_ip.o
IPOBJ += $(IPCOMMON)
LIB_IP += $(IPCOMMON)
RTMONOBJ=rtmon.o


include ../config.mk

CFLAGS += -fpic

ALLOBJ=$(IPOBJ) $(RTMONOBJ)
SCRIPTS=routel
TARGETS=ip rtmon iprt2_ip_test 

all: $(TARGETS) $(SCRIPTS)

ip: $(IPOBJ) $(LIBNETLINK)
	$(QUIET_LINK)$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

rtmon: $(RTMONOBJ)
	$(QUIET_LINK)$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

iprt2_ip_test: iprt2_ip_test.o $(LIBNETLINK) libiprt2_ip.so 
	$(QUIET_LINK)$(CC) $(CFLAGS) $^  $(LDFLAGS) $(LDLIBS) -liprt2_ip -L$(PWD)/ip -o $@

libiprt2_ip.so: $(LIB_IP) $(LIBNETLINK)
	$(QUIET_CC)$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@

install: all
	install -m 0755 $(TARGETS) $(DESTDIR)$(SBINDIR)
	install -m 0755 $(SCRIPTS) $(DESTDIR)$(SBINDIR)
	install -m 0644  libiprt2_ip.so $(LIBDIR)
clean:
	rm -f $(ALLOBJ) $(TARGETS)

SHARED_LIBS ?= y
ifeq ($(SHARED_LIBS),y)

LDLIBS += -ldl
LDFLAGS += -Wl,-export-dynamic,--rpath=../lib

else

ip: static-syms.o
static-syms.o: static-syms.h
static-syms.h: $(wildcard *.c)
	files="$^" ; \
	for s in `grep -B 3 '\<dlsym' $$files | sed -n '/snprintf/{s:.*"\([^"]*\)".*:\1:;s:%s::;p}'` ; do \
		sed -n '/'$$s'[^ ]* =/{s:.* \([^ ]*'$$s'[^ ]*\) .*:extern char \1[] __attribute__((weak)); if (!strcmp(sym, "\1")) return \1;:;p}' $$files ; \
	done > $@

endif
