#include "BitMap.h"

NTSTATUS 
DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
)
{
	ULONG GbSize = TotalSize.QuadPart >> 30;
	AimMap->Table = (PMBTABLE)ExAllocatePoolWithTag(NonPagedPool,sizeof(PMBTABLE)*GbSize,'GbS');
	if (AimMap->Table == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroBytes(AimMap->Table,sizeof(PMBTABLE)*GbSize);
	return STATUS_SUCCESS;
}
