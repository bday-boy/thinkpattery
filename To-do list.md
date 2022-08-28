# To-do List

## To-do:

- Clean up code a bit
- Make sure values for errors (NO_HEALTH_INFO, TIME_UNAVAILABLE, etc.) make
  sense
- Test more alpha values
- Create way of initializing moving_average data rather than just starting at
  0.0 and letting it figure it out after many iterations
  - Quick 'n dirty: Collect n (maybe 5) data points, compute each adjacent
    pair's slopes, and average the slopes
- Actually double check memory leaks and shit