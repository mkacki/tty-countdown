
default: compile

clean:
	rm -f tty-countdown
compile:
	gcc tty-countdown.c -o tty-countdown
install:
	cp ./tty-countdown /usr/local/bin/
all: clean compile install

uninstall:
	rm /usr/local/bin/tty-countdown
test: clean compile
	./tty-countdown 00:01:00 Test

