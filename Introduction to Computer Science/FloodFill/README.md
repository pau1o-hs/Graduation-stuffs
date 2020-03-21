# Resources Balancing

**Algorithm: FloodFill**

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

* Map coverage when a four-direction flood fill is applied, from Player 1 and stopping once resource j has been found
* Map coverage when a four-direction flood fill is applied, from Player 2 and stopping once resource j has been found
* Player p has advantage or the game is balanced

### Example
```
0.152542
0.152542
O mapa eh balanceado
```
