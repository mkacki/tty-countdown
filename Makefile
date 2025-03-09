
default: compile

clean:
	rm tty-countdown
compile:
	gcc tty-countdown.c -o tty-countdown
install:
	cp ./tty-countdown /usr/local/bin/
all: clean compile install

uninstall:
	rm /usr/local/bin/tty-countdown

