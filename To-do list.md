# To-do List

## Short term

- Clean up code a bit
- Make sure values for errors (NO_HEALTH_INFO, TIME_UNAVAILABLE, etc.) make
  sense
- Test more alpha values
- Actually double check memory leaks and shit
- Add error logging
- Add config through Polybar env variables
  - Icons for:
    - Charging
      - Charging
      - Not charging
    - Battery health
      - Low threshold battery
      - High threshold battery
      - Any number of middle icons
    - Time left
  - Print formatting (possibly)
  - Sample frequency
  - Number of initial samples
  - Moving average alpha value
  - Min and max battery icon thresholds

## Long term

- Use multithreading to use different refresh rates for the battery percent
  checking and battery life remaining.
- Use udev events to send signals to the program rather than just looping to
  check BAT files periodically

## Done :)

- Create way of initializing moving_average data rather than just starting at
  0.0 and letting it figure it out after many iterations
  - I ended up declaring a macro for number of initial samples and just doing
    a normal average of the first N samples