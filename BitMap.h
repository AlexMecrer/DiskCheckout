#pragma once
#include<wdm.h>

typedef struct KBTable {//Kb表
	PCHAR KbTable;
	ULONG Size;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb表
	PKBTABLE* MbTable;
	ULONG Size;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//这个是GB表
	PMBTABLE* Table;
	ULONG Size;
}DC_BITMAP,*PDC_BITMAP;

#define NOUSE -1
#define PARTUSE 0
#define FULLUSE 1

NTSTATUS DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
);

NTSTATUS
DCBitMapDelect(
	PDC_BITMAP AimMap
);

USHORT
DCBitMapQuery(
	PDC_BITMAP AimMap,
	LARGE_INTEGER offset,
	PULONG Length
);