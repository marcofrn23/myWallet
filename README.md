# myWallet
A simple software for investment portfolios management. It can take data from input files (.txt, .csv, .xml) containing raw financial market information with a pre-defined structure and then giving the opportunity to set them as a Mat and Vec objects, to display or to modify them. Mat and Vec are objects defined in the IT++ library ([Documentation](http://itpp.sourceforge.net/4.3.1/)). Starting from the retrieved data, it is then possible to create a new efficient user portfolio according to the market data, to manage it and to output it into a file.

## Dependencies
IT++

## Installing and linking IT++
MacOS installation (it will be put into /usr/local/Cellar): 
```
$ brew install itpp
```
Compiling and linking (itpp must stay in the working directory):
```
$ g++ mywallet.cpp -o mywallet -L. -litpp
```
---
***Project for "Object Oriented Programming" held by Prof. P.Zingaretti at UNIVPM, Italy.***
**Emanuele Palumbo, Marco Fringuelli**.


