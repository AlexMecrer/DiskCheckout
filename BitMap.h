#pragma once
#include<wdm.h>

typedef struct KBTable {//Kb��
	PCHAR KbTable;
	ULONG Size;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb��
	PKBTABLE* MbTable;
	ULONG Size;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//�����GB��
	PMBTABLE* Table;
	ULONG Size;
}DC_BITMAP,*PDC_BITMAP;

NTSTATUS DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
);

NTSTATUS
DCBitMapDelect(
	PDC_BITMAP AimMap
);

NTSTATUS
DCBitMapQuery(
	PDC_BITMAP AimMap,
	LARGE_INTEGER offset
);