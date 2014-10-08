05_Hashing_Lab
==============

Implement a hash table in C++

Requirements
------------

1. `keyExists`, `find`, `remove`, and `size` should all be O(1)
2. `add` should be reasonably fast. Use linear probing to find an open slot in the hash table. This will be O(1), on average, except when `grow` is called.
3. `grow` should be O(n)
4. Do not leak memory


Reading
=======
"Open Data Structures," Chapter 5, except for 5.2.3. http://opendatastructures.org/ods-cpp/5_Hash_Tables.html

Questions
=========

#### 1. Which of the above requirements work, and which do not? For each requirement, write a brief response.

1. All these methods are O(1).
2. 'add' is able to probe quickly and insert the new element into the hash table. O(1) unless 'grow' is called.
3. `grow` iterates through all elements, so naturally it is O(n).
4. The destructor and grow take care of any memory leakage by deleting any unused memory.

#### 2. I decided to use two function (`keyExists` and `find`) to enable lookup of keys. Another option would have been to have `find` return a `T*`, which would be `NULL` if an item with matching key is not found. Which design do you think would be better? Explain your reasoning. You may notice that the designers of C++ made the same decision I did when they designed http://www.cplusplus.com/reference/unordered_map/unordered_map/
This method we used is better. It is better to check if a key exists and return a known yet impossible value if it does not exist than to try and return an element that may not exist.

#### 3. What is one question that confused you about this exercise, or one piece of advice you would share with students next semester?
I was confused as to what literal code would go into creating a hash table.