# simple-chinese-search-engine
This is a course exercise of Data Structure, which is mostly writen in C and partly in Golang.

## How to build
*The project use [Gumbo](https://github.com/google/gumbo-parser) to parse the HTML files and [Neko](https://github.com/rocwong/neko) to route HTTP requests, make sure they are both installed.*

```sh
git clone https://github.com/cubarco/simple-chinese-search-engine
cd simple-chinese-search-engine
make
```

## Usage
`genindex` is used for generating the inverted index file.

`cli` is the command-line version of this engine.

`httpd` is the back-end of the Web version of this search engine.

```sh
./genindex # generate index.dat, this must be executed at first
export LD_LIBRARY_PATH=. # in order to add libsearch.so to the libpath
./cli # search something
./httpd # open the url http://localhost:55555, and try it
```
