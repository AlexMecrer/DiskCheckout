#pragma once
#include<wdm.h>

typedef struct KBTable {//Kb��
	PCHAR KbTable;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb��
	PKBTABLE MbTable;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//�����GB��
	PMBTABLE Table;
}DC_BITMAP,*PDC_BITMAP;

NTSTATUS DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
);
