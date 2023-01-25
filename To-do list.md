# To-do List

## Short term

### Additional functionality

- Add config through Polybar 3.6.0 env variables
  - Icons for:
    - Charging
    - Battery energy
      - Low threshold battery
      - High threshold battery
      - 3 middle icons
    - Time left
    - Battery health
  - Print formatting (possibly)
  - Sample frequency
  - Number of initial samples
  - Moving average alpha value
  - Min and max battery icon thresholds
- Add error logging

### Code cleanup

- Find a better way to use global values because I hate macros (maybe extern
  some consts)
- Find less ugly way of checking if any state change occured
- Implement dependency injection or even avoid using struct pointers altogether
  in favor of using basic type pointers (i.e. keeping pointers for battery
  health and percent in the BatteryTracker struct rather than a pointer to
  the BatteryInfo struct itself)

### Miscellaneous checks

- Make sure values for errors (NO_HEALTH_INFO, TIME_UNAVAILABLE, etc.) make
  sense
- Test more alpha values
- Actually double check memory leaks and shit

## Long term

- Use udev events to send signals to the program rather than just looping to
  check BAT files periodically
  - Turns out this might not work?

## Done :)

- Create way of initializing moving_average data rather than just starting at
  0.0 and letting it figure it out after many iterations
  - I ended up declaring a macro for number of initial samples and just doing
    a normal average of the first N samples
