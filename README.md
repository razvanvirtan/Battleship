# Battleship #
This is a Linux command line version of the famous game `Battleship`.  
The project is realised using C and `ncurses`.

## Instructions ##
Before starting the game, you will need to define your map. A valid map consists
of a 10x10 grid with 2 kind of squares: free squares (marked with `space`) and squares
occupied by a battleship (marked with `X`).  
Here is a valid map:
```
| | | | | |x| | | | |
|x|x|x|x| | | | | | |
| | | | | | | |x|x|x|
| |x| | | | | | | | |
| | | | |x| | | | |x|
| | | | |x| | | | | |
|x|x| | |x| | |x|x| |
| | | | | | | | | | |
| | |x| | | | |x| | |
| | |x| | | | | | | |
```

You can find 2 default maps in `map1` and `map2`.  
In order to run the game, use `make build` and than:
```
./battleship <map1_filename> <map2_filename> ...
```
Please note that you need to provide at least one map as command line argument.  
You can choose wheter to run a clasic `New game` or a hard one, with the computer
playing in a "smart" way.
