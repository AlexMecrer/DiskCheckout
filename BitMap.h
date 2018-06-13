#pragma once
#include<wdm.h>

typedef struct KBTable {//Kb表
	PCHAR KbTable;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb表
	PKBTABLE MbTable;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//这个是GB表
	PMBTABLE Table;
}DC_BITMAP,*PDC_BITMAP;

NTSTATUS DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
);
