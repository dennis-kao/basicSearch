basicSearch
========

basicSearch implements two components of modern day information retrieval systems - an inverted list file structure and query matching using the vector space model.

The "offline" program stores document keywords in an AVL tree and then writes this information to disk in an inverted index list for efficient retrieval.
Queries or searches can then be made using the "online" program, which filters and then ranks documents according to the [vector space model](https://en.wikipedia.org/wiki/Vector_space_model).

This program was written for an information retrieval course, CIS*4430.

### Inverted list file format

online generates 2 files: dictionary.txt and postings.txt.

<b>dictionary.txt</b>
```
743
25-year-old 1
abuse 1
acceptable 1
```
Format: keyword appears_in_n_documents

<b>postings.txt</b>
```
826
1 1
3 1
1 1
1 1
1 1
```
Format: document_id keyword_count

The keyword frequency in the input file can be determined as follows:

1. Find the keyword in dictionary.txt by iterating through each line and adding appears_in_n_documents to a global count
2. The global count indicates the first line in postings.txt that pertains to the keyword. The appears_in_n_documents count beside the keyword tells you how many lines to read in starting from the global count
3. Read in those lines which tell you which documents contain the keyword.
4. Refer to docids.txt for the byte offsets of each document in the input text file. The format of docids.txt looks like this:
```
LA010190-0001 26 43
LA010190-0002 392 505
```
where the line number corresponds to the document ID, the first text block is the document name and the following 2 numbers are the byte positions for the beginning and end of each document.

### Limitations
The program is limited by the amount of ram the PC has to store the list of:
	documents,
	their byte offset positions in the input file,
	and keyword frequencies.

The program also iterates through a vector to try to match a keyword to find its frequency. Using a hashmap would improve the speed of finding a keyword frequency.

### How to compile and run the program
1. Type make
2. Change the variable fullTestPath in avltree.cpp to the path of the raw document input file.
This path must be relative to the location of the program.
3. Type ./offline
4. This will generate the 3 index files
5. Type ./online
6. Type in a query and then a number to get the body of a document

### Sample output
1. define fullTestPath as "inputFiles/test.txt_1" in avltree.cpp
2. make
3. ./offline
4. ./online
5. enter this as query: 25-year-old garbage gold truck marijuana
6. you should see this as output:

```
Select a document from the numbered list:
1. legalizing illicit drugs
2. tribute war hero
3. county's plan garbage
trash disposal quickly los angeles city residents end making unjustified concessions
Your selection: 2

Body:

long ago americans believed marijuana harmful tobacco presidential adviser cocaine benign illicit drugs widespread back police officers fought dangerous lonely unappreciated war drugs los angeles police officer kelly key troopers
paralyzed decades ago bullet fired serve search warrant suspected cocaine dealer key died week spent rest life wheelchair taught youngsters dangers drugs temptations streets
quadriplegic volunteered days week gwen bolden foundation after-school counseling program rough neighborhood south downtown los angeles key tutor mentor boys man lives bought book-packs shoes treated youngsters lunch experience restaurant quietly things teach youngsters feel good role model war drugs
drug exploded exponentially bullet cut short key's once-bright police career destroys lives families communities wholesale terror nightmare grows officers key risk lives law enforcement leaders beginning recognize war waged equal fervor underlying social make illicit drugs alluring prevention important enforcement
kelly key turned day died buried tuesday full police department honors tombstone read pioneer hero war drugs
```

### Citations

I used and modified this code for the AVL tree:
	https://www.sanfoundry.com/cpp-program-implement-avl-trees/
