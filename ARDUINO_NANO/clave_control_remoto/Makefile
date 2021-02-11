ARDUINO_CLI = arduino-cli
BOARD_TYPE = arduino:avr:nano
SERIAL_PORT = /dev/ttyUSB0

.PHONY: all compile upload clean

all: compile

compile:
	$(ARDUINO_CLI) compile --export-binaries --fqbn $(BOARD_TYPE) .

upload:
	$(ARDUINO_CLI) upload -p $(SERIAL_PORT) --fqbn $(BOARD_TYPE) .

clean:
	@rm -rf build
