// $Id$
/********************************************************************
 * DCI <-> VMI/VMS Tool v1.0 (Dec/2005)
 * coded by El Bucanero
 *
 * Copyright (C) 2005 Damian Parrino <bucanero@fibertel.com.ar>
 * http://www.bucanero.com.ar/
 *
 * Greetz to:
 * Marcus Comstedt for his great info & resources					<http://mc.pp.se/dc/>
 * Dan Potter for KallistiOS and other great libs like Tsunami		<http://gamedev.allusion.net/>
 * AndrewK / Napalm 2001 for the lovely DC load-ip/tool				<http://adk.napalm-x.com/>
 * Lawrence Sebald for the MinGW/msys cross compiler tutorial		<http://ljsdcdev.sunsite.dk/>
 *
 * and last, but not least, thanks to SEGA for my ALWAYS LOVED Dreamcast! =)
 *
 ********************************************************************/

#include <string.h>
#include <stdio.h>

#include "dcvmu.hpp"

//--------------------
//	Main functions
//--------------------

char* decodeMonth(uint8 m) {
	switch(m) {
		case 0x01: return("Jan");
		case 0x02: return("Feb");
		case 0x03: return("Mar");
		case 0x04: return("Apr");
		case 0x05: return("May");
		case 0x06: return("Jun");
		case 0x07: return("Jul");
		case 0x08: return("Aug");
		case 0x09: return("Sep");
		case 0x10: return("Oct");
		case 0x11: return("Nov");
		default: return("Dec");
	}
}

void printFileInfo(DreamcastFile *df) {
	vmu_timestamp_t ts;
	
	ts=df->getTime();
	printf(" [i] Name: %s\n", df->getName());
	printf(" [i] Size: %d bytes\n", df->getSize());
	printf(" [i] Type: %s File\n", ((df->isGameFile()) ? "Game" : "Data"));
	printf(" [i] Date: %02x %s %02x%02x\n", ts.day, decodeMonth(ts.month), ts.cent, ts.year);
	printf(" [i] Time: %02x:%02x:%02x\n", ts.hour, ts.min, ts.sec);
	printf(" [i] Copy Protected: %s\n", ((df->isCopyProtected()) ? "Yes" : "No"));
}

int convertFile(VirtualFile *src, VirtualFile *dst) {
	printf(" [+] Reading %s ... ", src->getFileName());
	if (src->readData() != 0) {
		printf(" [-] Source file %s could not be readed.\n", src->getFileName());
		return(-1);
	}
	printf("OK!\n");
	printFileInfo(src->getDCFile());
	dst->setDCFile(src->getDCFile());
	printf(" [+] Writing %s ... ", dst->getFileName());
	if (dst->writeData() != 0) {
		printf(" [-] Destination file %s could not be written.\n", dst->getFileName());
		return(-1);
	}
	printf("OK!\n");
	return(0);
}

int main(int argc, char **argv) {
	VirtualFile *src, *dst;
	char tmp[13];
	
	printf("\n DCI <-> VMI/VMS Tool v1.0\n El Bucanero - www.bucanero.com.ar\n\n");
	argc--;
	if (argc != 2) {
		printf("usage: dci4vmi <source file> <destination file>\n\n");
		printf("about:\n This tool converts files between .DCI and .VMI/VMS formats.\n\n");
		printf("NOTE:\n DCI -> VMI/VMS: Don't specify .VMI/VMS extension for destination file.\n\n");
		printf("examples:\n dci4vmi SONIC.DCI SONIC\n");
		printf(" dci4vmi SHENMUE.VMI SHENMUE.DCI\n");
		return(0);
	}
	if (strstr(argv[1], ".DCI") || strstr(argv[1], ".dci")) {
		sprintf(tmp, "%.8s.VMI", argv[2]);
		src=new DCIFile(argv[1]);
		dst=new VMIFile(tmp, argv[1], "El Bucanero www.bucanero.com.ar", argv[2]);
	} else if (strstr(argv[1], ".VMI") || strstr(argv[1], ".vmi")) {
		src=new VMIFile(argv[1], "", "", "");
		dst=new DCIFile(argv[2]);
	} else {
		printf(" [-] ERROR: Invalid source file: <%s>.\n", argv[1]);
		printf(" [-] File extension not recognized.\n");
		printf(" [-] Valid extensions are: .VMI .vmi .DCI .dci.\n");
		return(-1);
	}
	if (convertFile(src, dst) != 0) {
		printf(" [-] Failed!\n");
	} else {
		printf(" [+] All done!\n");
	}
	delete dst;
	src->setDCFile(NULL);
	delete src;
	return(0);
}
