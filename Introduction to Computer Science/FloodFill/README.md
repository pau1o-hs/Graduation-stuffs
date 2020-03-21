# Resources Balancing

Implemented verification of the distribution of resources in terms of distance to the bases of two different players.

## Input

* An 8x8 matrix, representing the map of the game
* Player 1 position
* Player 2 position
* Amount of resources
* Resources positions (one per line)

### Example
```
0 1 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0 0
0 1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1 0
0 0 0 0 0 0 0 1 0
0 0 0 0 0 0 0 1 0
0 0 0 0 0 0 0 1 0
0 0
8 8
1
4 4
```

## Output

