# Recommendation System

Implemented a simple film recommendation that use as notes from users with more similar tastes as a basis for viewing as notes
of a determined user in relation to a movie, and using these notes to create a list of movie recommendations ordered by these
predicted notes.

## Input

### First line
* n, the number of database users
* i, the number of items in the database
* t, the threshold for selecting the most similar users

### Matrix
* the user × item score matrix 

Unrated movies receive zero in the matrix

### Example
```
5 4 0.7
5 3 2 0
1 3 4 3
0 3 0 2
0 0 0 3
1 3 2 4
```

## Output

Estimated grades of the unrated movies of respective user line. DI for Insufficient data

### Example
```
DI
0.89 2.79
1.50 3.50 2.50
```
