# k Nearest Neighbors

**Algorithm: kNN**

A very famous algorithm is kNN, which uses similarity measures (Euclidean distance) to classify a new unknown example. 
The kNN algorithm was implemented in order to, given the characteristics of the Iris flower and petal, 
discover which species it belongs.

## Input

* Train file
* Test file
* Number of neighbors

The **training set** is the reference data used to classify correctly the new species in the **test set**

### Example
```
./knn
train.csv test.csv 3
```

## Output

* The class guessed by the kNN followed by the real class (classified by the specialist), for each example in test set
* The kNN hit rate

### Example
```
setosa setosa
setosa versicolor
virginica virginica
0.6667
```
