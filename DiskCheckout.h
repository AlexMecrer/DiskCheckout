#include<wdm.h>

#define HKEY L"\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{71A27CDD-812A-11D0-BEC7-08002BE2092F}"
#define PATHNAME L"UpperFilters"
#define TmpFileName L"AlexMecrer"

typedef struct DISKEX {
	PDEVICE_OBJECT FilteObject;
	PDEVICE_OBJECT NextObject;
	LIST_ENTRY RequitList;
	struct {
		ULONG RequitNumber;
		KSPIN_LOCK mutex;
	}R;
	BOOLEAN PsOffFalg;
	LARGE_INTEGER DiskSize;
	PRTL_BITMAP DiskMap;
}DISKEXTEND,*PDISKEXTEND;

typedef struct Param {
	PDISKEXTEND Me;
}PARAM,PPARAM;






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