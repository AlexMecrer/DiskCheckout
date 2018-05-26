#pragma once
#include<wdm.h>

typedef struct RegNode{
	LIST_ENTRY LisrHead;
	ULONG Index;
	RTL_BITMAP RegData;
}REGNODE,*PREGNODE;

typedef struct DCBitMap {
	RTL_BITMAP RegionMap;//先查询这个表，如果存在，则查询RegData
	REGNODE RegData;
}DC_BITMAP,*PDC_BITMAP;

NTSTATUS DCBitMapInit(
	PDC_BITMAP AimMap,
	ULONG RegionNumber,
	ULONG RegionSize
);
