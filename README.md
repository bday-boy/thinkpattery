# thinkpattery

A program for tracking battery usage, remaining time, and health.

## Important info

This program is intended specifically for use with
[Polybar](https://github.com/polybar/polybar) because it just prints data to
stdout. Additionally, while I created it to be used on Thinkpads (hence the
name), I *think* it should work on just about any Linux laptop. I'm still
pretty new to Linux and C, so don't quote me on that. The only thing
that makes it designed for Thinkpads is that it finds multiple battery
directories in /sys/class/power_supply and sums all their relevant energy info
rather than only using a single battery like
the default Polybar module does. However, it has additional functionality
that the default module doesn't
have, so it may still be worth using even with only 1 battery.

## Installation

To create and install the binary, just run the following from the top-level
directory:

```bash
make
install -m 755 thinkpattery /path/to/installation
```

I like to install mine at ~/.local/bin but it's up to you.

## Configuration

Because I'm pretty new to C, configuration for this program is pretty limited
and annoying to do. Most configuration-related variables are either a global
const string or a macro. All icon consts can be found in battery_tracker.c,
the macro for the exponential moving average alpha value can be found in
moving_average.h (read about what changing the alpha value does
[here](https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average)),
and the update frequency macro (SAMPLE_FREQUENCY) can be found in common.h.
All icons that I use are from [Fontawesome](https://fontawesome.com/), so
you can either install that and use them as well or use your own icons.
