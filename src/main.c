#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	InitializeLib(ImageHandle, SystemTable);
	Print(L"Welcome to KaOS v0.0.1, a world full of carot!\n");
	for(;;) __asm__("hlt");
	
}

