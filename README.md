
# tty-countdown

This is simple terminal utility for counting down time 

[_(video)_](./tty-countdown.mp4)

default usage :

tty-countdown hh:mm:ss message

## Example usage

```
    tty-countdown 01:01:00 "Time to complete task" 

    tty-countdown 00:00:03 "Countdown to zero" && notify-send -u LOW "Time passed"

```
## Install 

```
   make
   sudo make install
```

## Unistall

```
    sudo make uninstall
```
