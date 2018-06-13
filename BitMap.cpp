#include "BitMap.h"

NTSTATUS 
DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
)
{
	ULONG GbSize = TotalSize.QuadPart >> 30;
	AimMap->Table = (PMBTABLE*)ExAllocatePoolWithTag(NonPagedPool,sizeof(PMBTABLE)*GbSize,'GbS');
	AimMap->Size = GbSize;
	if (AimMap->Table == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroBytes(AimMap->Table,sizeof(PMBTABLE)*GbSize);
	return STATUS_SUCCESS;
}

NTSTATUS 
DCBitMapDelect(
	PDC_BITMAP AimMap
)
{

	for (int i = 0; i < AimMap->Size; i++)
	{
		if (AimMap->Table[i]!=NULL)
		{
			for (int j = 0; j < AimMap->Table[i]->Size; j++)
			{
				if (AimMap->Table[i]->MbTable[j] != NULL)
				{
					for (int z = 0; AimMap->Table[i]->MbTable[j]->Size; z++)
					{
						if (AimMap->Table[i]->MbTable[j]->KbTable!= NULL)
						{
							ExFreePoolWithTag(AimMap->Table[i]->MbTable[j]->KbTable,'Gbs');//Bug
						}
					}
				}
			}
		}
	}
	return STATUS_SUCCESS;
}