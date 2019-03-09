Perceptron Implementation
===============================
Homework01

Target
===============================



Requirement
===============================

* OS : Linux
* Compiler : gcc with c99 , g++ with c++11
* Project Builder : cmake


Usage 
===============================

## Project Build
Go to the source folder , 
````
$ mkdir build
$ cd build
$ cmake ..
````
a project will build in SourceCode/build folder

## Configure
In the project folder (SourceCode/build for instance), you will see some files as follow :

```
$ ls
run.sh config.ini etc.
```

In config.ini I had setted the default dataset folder "../dataset", 
if you cmake the project in the other folder , you may should change the value .


## Run
if you run ./run.sh or sh run.sh , the project will run automatically,include : 
* compile and clean the project
* assign dataset into 5 pieces randomly (the will stored into output/ )
* train models
* classify and calculate the precision , recall and f1score

this may cost 30 or more seconds , base on the performance of PC


[![asciicast](https://asciinema.org/a/nn8k3NP96UGtPbEaqXJiDCtRE.svg)](https://asciinema.org/a/nn8k3NP96UGtPbEaqXJiDCtRE)