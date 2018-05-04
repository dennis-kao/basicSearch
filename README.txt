CIS*4430 A4
Dennis Kao
April 7th, 2018

What has been done
- Online and offline processing
- Indexing
- All 3 file generation
- Calculating keyword frequency using the next keyword position offset

What hasn't been done
- Viewing 10 documents at a time
- The program lists all documents because it is 11:41 PM as of writing this sentence

Limitations
The program is limited by the amount of ram the PC has to store the list of:
	documents,
	their offset positions,
	and term frequencies

The program also iterates through a vector to try to match a keyword to find its frequency.

Improvements
Using a hashmap would improve the speed of finding a keyword frequency 
instead of having to iterate through the vector.

User guide
1. Type make
2. Change the variable fullTestPath in avltree.cpp to the path of the raw document input file.
This path must be relative to the location of the program.
3. Type ./offline
4. This will generate the 3 index files
5. Type ./online
6. Type in a query and then a number to get the body of a document

Testing plan
1. define fullTestPath as "inputFiles/test.txt_1" in avltree.cpp
2. make
3. ./offline
4. ./online
5. enter this as query: 25-year-old garbage gold truck marijuana
6. you should see this as output:

Select a document from the numbered list:
1. legalizing illicit drugs
2. tribute war hero
3. county's plan garbage
trash disposal quickly los angeles city residents end making unjustified concessions
Your selection:

7. enter 2
8. this should be the output:

Body:

long ago americans believed marijuana harmful tobacco presidential adviser cocaine benign illicit drugs widespread back police officers fought dangerous lonely unappreciated war drugs los angeles police officer kelly key troopers
paralyzed decades ago bullet fired serve search warrant suspected cocaine dealer key died week spent rest life wheelchair taught youngsters dangers drugs temptations streets
quadriplegic volunteered days week gwen bolden foundation after-school counseling program rough neighborhood south downtown los angeles key tutor mentor boys man lives bought book-packs shoes treated youngsters lunch experience restaurant quietly things teach youngsters feel good role model war drugs
drug exploded exponentially bullet cut short key's once-bright police career destroys lives families communities wholesale terror nightmare grows officers key risk lives law enforcement leaders beginning recognize war waged equal fervor underlying social make illicit drugs alluring prevention important enforcement
kelly key turned day died buried tuesday full police department honors tombstone read pioneer hero war drugs

Citations

I used this code for the AVL tree:
	https://www.sanfoundry.com/cpp-program-implement-avl-trees/