#pragma once
#include<wdm.h>


#define GETGB(a) a >>30
#define GETMB(a) a>>20-(a>>30)<<30
#define GETKB(a) a>>10-(a>>20)<<20
#define GETBY(a) a-(a>>10)<<10
#define SET_LENGTH_VALUE(a,b) a.GbSize=GETGB(b);\
	a.MbSize=GETMB(b);\
	a.KbSize = GETKB(b);\
	a.ByteSize=GETBY(b);\
	a.TotalSize=b

#define GET_GB_USETABLE(a) a->UserMap
#define GET_MB_USETABLE(a,b) a->Table[b.GbSize]->UserMap 
#define GET_KB_USETABLE(a,b) a->Table[b.GbSize]->MbTable[b.MbSize]->UserMap
#define GET_BYTE_USETABLE(a,b) a->Table[b.GbSize]->MbTable[b.MbSize]->KbTable[b.KbSize]->UserMap


typedef struct LengthInfor{
	ULONG GbSize;
	ULONG MbSize;
	ULONG KbSize;
	ULONG ByteSize;
	ULONGLONG TotalSize;
}LENGTHINFO,*PLENGTHINFO;


typedef struct ByteTable {
	RTL_BITMAP UserMap;
}BYTETABLE,*PBYTETABLE;


typedef struct KBTable {//Kb表
	PBYTETABLE* KbTable;
	ULONG Size;
	RTL_BITMAP UserMap;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb表
	PKBTABLE* MbTable;
	ULONG Size;
	RTL_BITMAP UserMap;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//这个是GB表
	PMBTABLE* Table;
	ULONG Size;
	RTL_BITMAP UserMap;
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
	PLENGTHINFO Length
);
NTSTATUS
DCBitMapMask(
	PDC_BITMAP AimMap,
	LARGE_INTEGER offset,
	LENGTHINFO Length
);