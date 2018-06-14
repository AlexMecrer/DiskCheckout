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
	//AimMap->BeUse = (PCHAR)ExAllocatePoolWithTag(NonPagedPool,1024,'Use');
	RtlInitializeBitMap(&AimMap->UserMap,(PULONG)ExAllocatePoolWithTag(NonPagedPool,1024,'Tag'),1024);
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

USHORT 
DCBitMapQuery(
	PDC_BITMAP AimMap,
	LARGE_INTEGER Startoffset,
	PLENGTHINFO Length
)
/*
如果是部分使用那么被使用的长度将会被Length返回
*/
{
	LENGTHINFO BeforeHalf,EndHalf;
	BeforeHalf = EndHalf = {0};
	PCHAR Tmp;
	PULONG64 Tmp1;
	if (BeforeHalf.GbSize == EndHalf.GbSize)
	{
		SET_LENGTH_VALUE(BeforeHalf,Startoffset.QuadPart);
		SET_LENGTH_VALUE(EndHalf,Startoffset.QuadPart+Length->TotalSize);
		if (RtlAreBitsSet(&AimMap->UserMap, BeforeHalf.GbSize, 1))
		{
			if (BeforeHalf.MbSize == EndHalf.MbSize)
			{
				if (RtlAreBitsClear(&GET_MB_USETABLE(AimMap,BeforeHalf),BeforeHalf.MbSize,1))
				{
					return NOUSE;
				}
				if (BeforeHalf.KbSize == EndHalf.KbSize)
				{
					if (RtlAreBitsClear(&GET_KB_USETABLE(AimMap,BeforeHalf),BeforeHalf.KbSize,1))
					{
						return NOUSE;
					}
					if (RtlAreBitsSet(&GET_BYTE_USETABLE(AimMap,BeforeHalf),BeforeHalf.ByteSize, Length->ByteSize))
					{
						return FULLUSE;
					}
					return PARTUSE;
				}
				else
				{
					CHECK_KB:
					if (RtlAreBitsSet(&GET_KB_USETABLE(AimMap,BeforeHalf), BeforeHalf.KbSize, EndHalf.KbSize - BeforeHalf.KbSize))
					{
						if (RtlAreBitsSet(&GET_BYTE_USETABLE(AimMap, BeforeHalf), BeforeHalf.ByteSize, 1024 - BeforeHalf.ByteSize) &&
							RtlAreBitsSet(&GET_BYTE_USETABLE(AimMap, EndHalf), 0, EndHalf.ByteSize))
						{
							return FULLUSE;
						}
					}
					return PARTUSE;
				}
			}
			CHECK_MB:
			if (RtlAreBitsSet(&GET_MB_USETABLE(AimMap, BeforeHalf), BeforeHalf.MbSize, EndHalf.MbSize - BeforeHalf.MbSize))
			{
				if (RtlAreBitsSet(&GET_KB_USETABLE(AimMap, BeforeHalf), BeforeHalf.KbSize, 1024 - BeforeHalf.KbSize) &&
					RtlAreBitsSet(&GET_KB_USETABLE(AimMap, EndHalf), 0, EndHalf.KbSize))
				{
					goto CHECK_KB;
				}
			}
		}
		if (RtlAreBitsSet(&GET_GB_USETABLE(AimMap), BeforeHalf.GbSize, EndHalf.GbSize - BeforeHalf.GbSize))
		{
			if (RtlAreBitsSet(&GET_MB_USETABLE(AimMap, BeforeHalf), BeforeHalf.MbSize, 1024 - BeforeHalf.MbSize) &&
				RtlAreBitsSet(&GET_MB_USETABLE(AimMap, EndHalf), 0, EndHalf.MbSize))
			{
				goto CHECK_MB;
			}
		}
	}
	return NOUSE;
}

VOID 
DCBitMaskBit(
	PVOID AimMap,
	ULONG Length
)
{
	memset(AimMap,-1,Length);//Bug
	return;
}
