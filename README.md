![alt text](https://github.com/zivkovicmilos/BPlusTree/tree/master/img/BannerImg.jpg)

# Project Description
The goal of this homework project was to implement an efficient B+ Tree to work with a given Movie Database (obtained from The Movie Database (TMDb), in .csv format). The following conditions are set specified:
1. The DB is static
2. Deletion from the DB is done through marking (the physical entry is still in the database, however, it is logically marked as "deleted")
3. The B+ Tree must use a sparse index
4. The keys of the B+ Tree are a concatenation of the YYYY and MM fields
5. The user defines the degree of the B+ Tree (maximum 10)
6. The B+ Tree contains the first movie with the key YYYYMM

# Features
The following menu is used for interacting with the Movie DB through the B+ Tree.

![alt text](./img/menu.svg)

# Program Breakdown
## Create DB
The program can read through an already prepared database (with a .csv file), or through manual input. Once the database is inputted, it is sorted using Insertion sort, and later on indexed. 
## Delete Entry (Mark)
The selected entry in the database is logically marked, but not physically deleted. This of course affects the B+ Tree in the following way:
- When an entry is marked, the B+ Tree attempts to find the next movie in the database that matches the YYYYMM key, and if successful points to the newly found movie
- If not successful (meaning there exists no other movie with the YYYYMM key, and the previously marked movie was the last one in the DB), the B+ Tree removes the YYYYMM entry completely (from the tree)
## Delete DB
Deletes the database sequentially.
## Print DB 
Prints all the movies in the DB in a sorted order.
## Create B+ Tree
Creates a B+ Tree object with a given degree (max 10), and sets the tree to use a database that was imported previously. If no database was found, the tree cannot be created.
## Add Entry to B+ Tree
Adds specific entries to the B+ Tree from the imported database. The user can choose whether or not to add just a couple of entries, or the entire database.
## Delete Entry from B+ Tree
Completely removes a specified movie from the B+ Tree.
## Find Entry in Tree
Locates the specified movie within the tree.
## Print B+ Tree
Prints out the B+ Tree using Level Order.
## Find Entries in Range
Finds and prints out all the movies within a given range (year). The algorithms firstly traverses the tree to find the starting position, then sequentially goes through the database to find all the matching movies.
## Statistics
Prints out the number of movies for each given year.
## Print Leaves
Prints out the leaf nodes of the B+ Tree.
## Delete B+ Tree
Deletes the B+ Tree using Level Order traversal.
## Find Intersection
When given 2 year ranges, the algorithm finds and prints out all the movies that were in the intersection.
## Stop
Terminates the program.