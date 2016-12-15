#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include "main.h"
#include "logo.h"

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	
    EFI_INPUT_KEY Key;
    
    InitializeLib(ImageHandle, SystemTable);
    
    uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut); //How to call ClearScreen() without generate warning ?
	
	Print(LOGO);
	Print(L"\nWelcome to KaOS v0.0.1, the Karrot OS !\n\n");
	Print(L"Press [H] to halt the system, [R] to reboot...\n");
	
	while ((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
	
	if(Key.UnicodeChar == 'h') {
	
		Print(L"System halted.");
		for(;;) __asm__("hlt");
		return EFI_SUCCESS;
	
	} else if(Key.UnicodeChar == 'r') {
	
		Print(L"Rebooting the system...\n");
		return EFI_SUCCESS;
		
	} else {
	
		Print(L"Error : Key pressed is '%c'\n", Key.UnicodeChar);
		return EFI_UNSUPPORTED;
	
	}
	
}

