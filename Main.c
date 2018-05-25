#include"DiskCheckout.h"
NTSTATUS 
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hKey;
	OBJECT_ATTRIBUTES attri;
	UNICODE_STRING KeyName = RTL_CONSTANT_STRING(HKEY);
	UNICODE_STRING Upper = RTL_CONSTANT_STRING(PATHNAME);
	InitializeObjectAttributes(&attri,&KeyName,OBJ_OPENIF|OBJ_KERNEL_HANDLE,NULL,NULL);
	status = ZwCreateKey(&hKey,KEY_SET_VALUE,&attri,0,NULL,REG_OPTION_NON_VOLATILE,REG_OPENED_EXISTING_KEY);
	if (NT_SUCCESS(status))
	{

		status = ZwSetValueKey(hKey,&Upper,0,REG_SZ,);
		ZwClose(hKey);
	}
	return status;
}