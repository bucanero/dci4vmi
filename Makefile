# dci4vmi util
# (c)2005 Damian Parrino
# $Id: Makefile,v 1.1.1.1 2001/09/26 07:05:00 bardtx Exp $

all:
	g++ -c dcvmu.cpp
	g++ -c dci4vmi.cpp
	g++ -o dci4vmi dcvmu.o dci4vmi.o
