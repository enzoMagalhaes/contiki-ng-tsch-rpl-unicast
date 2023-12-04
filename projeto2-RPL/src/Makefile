CONTIKI_PROJECT = unicast-client unicast-server

all: $(CONTIKI_PROJECT)

CONTIKI=../..

PLATFORMS_EXCLUDE = sky native z1

MAKE_MAC = MAKE_MAC_TSCH

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/shell

include $(CONTIKI)/Makefile.include
