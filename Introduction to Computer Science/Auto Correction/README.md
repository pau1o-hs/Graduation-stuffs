# Mining twitter data API

Given a reference book and a list of tweets of the Twitter API, print the misspelled words in the tweets
(words that do not exist in the reference book)

## Input

* A reference book
```
Alice was beginning to get very tired of sitting by her sister on the
bank, and of having nothing to do: once or twice she had peeped into the
book her sister was reading, but it had no pictures or conversations in
it, ’and what is the use of a book,’ thought Alice ’without pictures or
conversations?’
```
* A list of tweets
```
{
      "created_at": "Thu May 10 15:24:15 +0000 2018",
      "id_str": "850006245121695744",
      "text": "Here is the Tweet message.",
      "user": {
      },
      "place": {
      },
      "entities": {
      },
      "extended_entities": {
      }
}
```

### Example
```
./spellcheck
dictionary.txt
tweets.json
```

## Output

* The incorrects words in the i-th tweet, separated by lines of the respective tweets

### Example
```
Wat ar yah doin
Theyre
Dis iss mi jam
```
