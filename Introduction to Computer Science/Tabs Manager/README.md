# Manipulating a list

**Algorithms: List and rapid sort**

Implemented a program that manages tabs in a browser using linked lists.

## Input

* [1] Insert a tab (Title, url, date and time)
```
1
USP
http://www.usp.br
20 11
08 30
```
* [2] Switch tab (Tab title, position to be replaced)
```
2
USP
3
```
* [3] Order by date and time
* [4] Display all tabs configuration

### Example
```
1
USP
http://www.usp.br
20 11
08 30
1
ICMC USP
http://www.icmc.usp.br
20 11
09 15
1
FOG
http://www.fog.icmc.usp.br
21 11
21 30
2
FOG
1
2
USP
3
4
3
4
5
```

## Output

* Display the tabs configuration

### Example
```
FOG http://www.fog.icmc.usp.br 21/11 21:30
ICMC USP http://www.icmc.usp.br 20/11 09:15
USP http://www.usp.br 20/11/2018 08:30

USP http://www.usp.br 20/11 08:30
ICMC USP http://www.icmc.usp.br 20/11 09:15
FOG http://www.fog.icmc.usp.br 21/11 21:30
```
