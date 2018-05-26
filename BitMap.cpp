#include "BitMap.h"

NTSTATUS 
DCBitMapInit(
	PDC_BITMAP AimMap,
	ULONG RegionNumber,
	ULONG RegionSize
)
{
	NTSTATUS status = STATUS_SUCCESS;
	RegionNumber = (RegionNumber)+(sizeof(ULONG) - RegionNumber % sizeof(ULONG));
	PULONG Buffer = (PULONG)ExAllocatePoolWithTag(NonPagedPool,RegionNumber,'BTP');
	RtlInitializeBitMap(&AimMap->RegionMap,Buffer,RegionNumber);
	return status;
}
