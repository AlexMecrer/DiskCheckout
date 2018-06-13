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

typedef struct LengthInfor{
	ULONG GbSize;
	ULONG MbSize;
	ULONG KbSize;
	ULONG ByteSize;
	ULONGLONG TotalSize;
}LENGTHINFO,*PLENGTHINFO;


typedef struct ByteTable {
	PCHAR ByteTable;
}BYTETABLE,*PBYTETABLE;


typedef struct KBTable {//Kb表
	PBYTETABLE* KbTable;
	ULONG Size;
	PCHAR BeUse;
}KBTABLE,*PKBTABLE;


typedef struct MBTable {//Mb表
	PKBTABLE* MbTable;
	ULONG Size;
	PCHAR BeUse;
}MBTABLE,*PMBTABLE;

typedef struct DCBitMap {//这个是GB表
	PMBTABLE* Table;
	ULONG Size;
	PCHAR BeUse;
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
VOID
DCBitMaskBit(
	PVOID AimMap,
	ULONG Length
);