#include<wdm.h>
#include"BitMap.h"
#define HKEY L"\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{71A27CDD-812A-11D0-BEC7-08002BE2092F}"
#define PATHNAME L"UpperFilters"
#define TmpFileName L"AlexMecrer"
#define DBMSIZE 512

typedef struct AddressNode {

};


typedef struct DISKEX {
	PDEVICE_OBJECT FilteObject;
	PDEVICE_OBJECT NextObject;
	LIST_ENTRY RequitList;
	struct {
		volatile ULONG RequitNumber;
		KSPIN_LOCK mutex;
	}R;
	BOOLEAN PsOffFalg;
	LARGE_INTEGER DiskSize;
	DC_BITMAP DiskMap;
}DISKEXTEND,*PDISKEXTEND;

typedef struct Param {
	PDISKEXTEND Me;
}PARAM,PPARAM;



#pragma pack(1)
typedef struct _DP_FAT16_BOOT_SECTOR
{
	UCHAR		JMPInstruction[3];
	UCHAR		OEM[8];
	USHORT		BytesPerSector;
	UCHAR		SectorsPerCluster;
	USHORT		ReservedSectors;
	UCHAR		NumberOfFATs;
	USHORT		RootEntries;
	USHORT		Sectors;
	UCHAR		MediaDescriptor;
	USHORT		SectorsPerFAT;
	USHORT		SectorsPerTrack;
	USHORT		Heads;
	DWORD32		HiddenSectors;
	DWORD32		LargeSectors;
	UCHAR		PhysicalDriveNumber;
	UCHAR		CurrentHead;
} DP_FAT16_BOOT_SECTOR, *PDP_FAT16_BOOT_SECTOR;

typedef struct _DP_FAT32_BOOT_SECTOR
{
	UCHAR		JMPInstruction[3];
	UCHAR		OEM[8];
	USHORT		BytesPerSector;
	UCHAR		SectorsPerCluster;
	USHORT		ReservedSectors;
	UCHAR		NumberOfFATs;
	USHORT		RootEntries;
	USHORT		Sectors;
	UCHAR		MediaDescriptor;
	USHORT		SectorsPerFAT;
	USHORT		SectorsPerTrack;
	USHORT		Heads;
	DWORD32		HiddenSectors;
	DWORD32		LargeSectors;
	DWORD32		LargeSectorsPerFAT;
	UCHAR		Data[24];
	UCHAR		PhysicalDriveNumber;
	UCHAR		CurrentHead;
} DP_FAT32_BOOT_SECTOR, *PDP_FAT32_BOOT_SECTOR;

typedef struct _DP_NTFS_BOOT_SECTOR
{
	UCHAR		Jump[3];					//0
	UCHAR		FSID[8];					//3
	USHORT		BytesPerSector;				//11
	UCHAR		SectorsPerCluster;			//13
	USHORT		ReservedSectors;			//14
	UCHAR		Mbz1;						//16		
	USHORT		Mbz2;						//17
	USHORT		Reserved1;					//19
	UCHAR		MediaDesc;					//21
	USHORT		Mbz3;						//22
	USHORT		SectorsPerTrack;			//24
	USHORT		Heads;						//26
	ULONG		HiddenSectors;				//28
	ULONG		Reserved2[2];				//32
	ULONGLONG	TotalSectors;				//40
	ULONGLONG	MftStartLcn;				//48
	ULONGLONG	Mft2StartLcn;				//56
}DP_NTFS_BOOT_SECTOR, *PDP_NTFS_BOOT_SECTOR;
#pragma pack()





NTSTATUS
IoQDMCompletion(
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp,
	IN PVOID  Context
);









NTSTATUS QueryDiskInformation(
	PDEVICE_OBJECT AimObject,
	LARGE_INTEGER TotalSize,
	DWORD64 Cluster,
	DWORD64 SectorSize
);





VOID KDIskstartRoutine(
	PVOID StartContext
);





NTSTATUS DiskCheckAddDevice(
	_In_ struct _DRIVER_OBJECT *DriverObject,
	_In_ struct _DEVICE_OBJECT *PhysicalDeviceObject
);



NTSTATUS
DiskCheckDispath(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
);