#include <efi.h>
#include <efilib.h>
#include "main.h"
#include "memory.h"
#include "lib/types.h"
#include "lib/string.h"
#include "drivers/io.h"
#include "drivers/display.h"
#include "drivers/keyboard.h"

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	/*
	 * UEFI boot loader
	 */

	InitializeLib(ImageHandle, SystemTable);
	EFI_STATUS status=EFI_SUCCESS;

	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);

	// Getting memory informations
	// http://stackoverflow.com/questions/17591351/converting-efi-memory-map-to-e820-map
	
	uint64_t totalMemory=0;
	uint64_t freeMemory=0;
	uint64_t firstAddress=0;
	uint64_t lastAddress=0;

    uint64_t MemMapSize=sizeof(EFI_MEMORY_DESCRIPTOR)*16;
    uint64_t MemMapSizeOut=MemMapSize;
    uint64_t MemMapKey=0;
    uint64_t MemMapDescriptorSize=0;
    uint32_t MemMapDescriptorVersion=0;
    uint64_t DescriptorCount=0;
    uint8_t *buffer=NULL;
    EFI_MEMORY_DESCRIPTOR *MemoryDescriptorPtr=NULL;

	// Discover memory map size
    do {

        buffer = AllocatePool(MemMapSize);
        if (buffer == NULL) {
			break;
        }

        status = SystemTable->BootServices->GetMemoryMap(&MemMapSizeOut, (EFI_MEMORY_DESCRIPTOR*)buffer,
			&MemMapKey, &MemMapDescriptorSize, &MemMapDescriptorVersion);

        if (status != EFI_SUCCESS) {
            FreePool(buffer);
            MemMapSize += sizeof(EFI_MEMORY_DESCRIPTOR)*16;
        }

    } while (status != EFI_SUCCESS);

	// Discover memory map and calcul total/free memory
    if (buffer != NULL) {

        DescriptorCount = MemMapSizeOut / MemMapDescriptorSize;
        MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*)buffer;

        for (int i=0; i<DescriptorCount; i++) {

            MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*)(buffer + (i*MemMapDescriptorSize));

            if (MemoryDescriptorPtr->Type == EfiLoaderData
		        || MemoryDescriptorPtr->Type == EfiBootServicesData
		        || MemoryDescriptorPtr->Type == EfiRuntimeServicesData
		        || MemoryDescriptorPtr->Type == EfiConventionalMemory) {

				freeMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;
				if(firstAddress==0) firstAddress=MemoryDescriptorPtr->PhysicalStart;
				lastAddress = MemoryDescriptorPtr->PhysicalStart + MemoryDescriptorPtr->NumberOfPages * EFI_PAGE_SIZE;

			}

			totalMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;

        }

        FreePool(buffer);

    }
    freeMemory=freeMemory/1024/1024;
    totalMemory=totalMemory/1024/1024;


	// Initialize framebuffer (GOP)

    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &GopGuid, NULL, (void **)&Gop);

	// Set desired resolution
    for (int i = 0; i < Gop->Mode->MaxMode; i++) {

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
        uint64_t infoSize;
        Gop->QueryMode(Gop, i, &infoSize, &info);

		if(info->HorizontalResolution>=KAOS_SCREEN_WIDTH && info->VerticalResolution>=KAOS_SCREEN_HEIGHT) {
			Gop->SetMode(Gop, i);
			break;
		}

		FreePool(info);

    }


	// Exiting UEFI land
	uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, MemMapKey);


	/*
	 * Kernel main. In the futur, must be loaded from external file...
	 */
	 
	int k,l;
	// Blank screen
	for(k=0; k<KAOS_SCREEN_WIDTH*KAOS_SCREEN_HEIGHT; k++) { putPixel(Gop->Mode->FrameBufferBase, k, 0, 0x00000000); }
	
	// Draw a 50x20 green box...
	for(l=0; l<20; l++) {
		for(k=0; k<50; k++) {
			putPixel(Gop->Mode->FrameBufferBase, 10+k, 10+l, 0x00336600);
		}
	}
	
	// Draw a 50x80 orange box...
	for(l=0; l<80; l++) {
		for(k=0; k<50; k++) {
			putPixel(Gop->Mode->FrameBufferBase, 10+k, 30+l, 0x00ff8000);
		}
	}

	// ...the OS name and version... (we need a correct printf-like function...)
	wchar_t *osName = malloc_stub(200 * sizeof(wchar_t), lastAddress-8000);
	wchar_t *nbr = malloc_stub(3 * sizeof(wchar_t), lastAddress-7500);
	strcpy(osName, L"KaOS v");
	strcat(osName, itoa(KAOS_VERSION_MAJOR, nbr, 10));
	strcat(osName, L".");
	strcat(osName, itoa(KAOS_VERSION_MINOR, nbr, 10));
	strcat(osName, L".");
	strcat(osName, itoa(KAOS_VERSION_REVISION, nbr, 10));
	strcat(osName, L", the Karrot OS !");
	putString(Gop->Mode->FrameBufferBase, 70, 55, 0x00ff8000, osName);

	// ...a line...
	for(k=0; k<KAOS_SCREEN_WIDTH-20; k++) {
		putPixel(Gop->Mode->FrameBufferBase, 10+k, 120, 0x00ff8000);
	}

	// At this point, we need a malloc() function. So we need a real memory manager !
	wchar_t *strbuffer = malloc_stub(20 * sizeof(wchar_t), lastAddress-7000); // Temporary workaround

	for(l=0; l<64; l++) {
		for(k=0; k<72; k++) {
			putPixel(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-104+k, 10+l, 0x004e9a06);
		}
	}
	putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 26, 0x00ffffff, itoa(totalMemory, strbuffer, 10));
	putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 42, 0x00ffffff, itoa(freeMemory, strbuffer, 10));
	putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 58, 0x00ffffff, itoa(firstAddress, strbuffer, 16));
	putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 74, 0x00ffffff, itoa(lastAddress, strbuffer, 16));

	// Read keyboard raw input
	int linePos=136;
	int cursorPos=8;
	int maxcharInLine=(KAOS_SCREEN_WIDTH/16)*16;
	char key;
	wchar_t c=0;

	while(1) {

		key = getScancode();
		c=scancodeToChar(key);

		// Display charcode in the lower right corner
		for(l=0; l<16; l++) {
			for(k=0; k<36; k++) {
				putPixel(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-54+k, KAOS_SCREEN_HEIGHT-150+l, 0x0075507b);
			}
		}
		putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-50, KAOS_SCREEN_HEIGHT-150, 0x00ffffff, itoa(key, strbuffer, 16));
		
		// We need a solid double-buffering solution here
		// and simply redraw the entire screen buffer @fixme)
		
		// Draw char
		switch(c) {

			case '\n':
				cursorPos=8;
				linePos+=16;
				break;

			case '\b':
				cursorPos-=8;
				if(cursorPos<8) {
					cursorPos=maxcharInLine-16;
					linePos-=16;
				}
				for(l=0; l<16; l++) {
					for(k=0; k<8; k++) {
						putPixel(Gop->Mode->FrameBufferBase, cursorPos+k, linePos+l, 0x00000000);
					}
				}
				break;

			default:

				putChar(Gop->Mode->FrameBufferBase, cursorPos, linePos, 0x00ffffff, c);
				cursorPos+=8;
				if(cursorPos>=maxcharInLine-8) {
					cursorPos=8;
					linePos+=16;
				}

		}
	
	}

}

