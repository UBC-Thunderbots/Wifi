# Make File

rm=/bin/rm -f
CC= cc 
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -lpthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL

WPS_PIN_STATION_APP = rsi_wps_pin_station.o 

all: $(WPS_PIN_STATION_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(WPS_PIN_STATION_APP) core *.o *~ rsi_wc_app
