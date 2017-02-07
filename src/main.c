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

	// Initialize framebuffer (GOP)
    static EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
	static EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

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

	// Initialize the quick'n'dirty memory manager
	init_memory_manager(totalMemory, freeMemory, lastAddress);
	
	// Make a string with OS name and version
	wchar_t *osName = kmalloc(200 * sizeof(wchar_t));
	wchar_t *osMajor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osMinor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osRevision = kmalloc(3 * sizeof(wchar_t));
	strf(osName, 7, L"KaOS v", itoa(KAOS_VERSION_MAJOR, osMajor, 10), L".", 
		itoa(KAOS_VERSION_MINOR, osMinor, 10), L".", 
			itoa(KAOS_VERSION_REVISION, osRevision, 10), L", the Karrot OS !");

	// Make a string with memory informations
	wchar_t *strTotalMem = kmalloc(50 * sizeof(wchar_t));
	wchar_t *strFreeMem = kmalloc(50 * sizeof(wchar_t));
	wchar_t *memsize = kmalloc(20 * sizeof(wchar_t));


	uint32_t gbuff1[Gop->Mode->FrameBufferSize*sizeof(uint32_t)];
	int i=0;

	while(1) {

		// Draw background
		drawBoxToBuffer(gbuff1, 0, 0, KAOS_SCREEN_WIDTH, KAOS_SCREEN_HEIGHT, 0x00555753);

		// Draw memory information
		for(int l=0; l<64; l++) {
			for(int k=0; k<118; k++) {
				zputPixel(gbuff1, KAOS_SCREEN_WIDTH-124+k, 10+l, 0x004e9a06);
			}
		}
		strf(strTotalMem, 3, L"TOTAL : ", itoa(get_total_memory()/1024/1024, memsize, 10), L" Mo");
		strf(strFreeMem, 3, L"FREE : ", itoa(get_free_memory()/1024/1024, memsize, 10), L" Mo");
		zputString(gbuff1, KAOS_SCREEN_WIDTH-120, 26, 0x00ffffff, strTotalMem);
		zputString(gbuff1, KAOS_SCREEN_WIDTH-120, 42, 0x00ffffff, strFreeMem);

		// Draw a window with scrolling content
		zdrawWindow(gbuff1, 50, 150, 800, 480, 0x00000000, osName);		
		zputString(gbuff1, 60, 180+(i*16), 0x00ffffff, L"Memory stress test in progress, please wait for the world's end...\n");
		i++;
		if(i>26) i=0;
		
		// Display the entire frame
		blitBufferToScreen(Gop->Mode->FrameBufferBase, gbuff1);

		// Memory overflow test... due to the stupid memory allocator, this must kill the kernel's code in memory
		char *stress=kmalloc(200000);
		for(uint64_t m=0; m<200000; m+=1000) stress[m]='A';

	}
	
/*


	// Read keyboard raw input
	int linePos=180;
	int cursorPos=55;
	int maxcharInLine=(800/8)*8;
	char key;
	wchar_t c=0;
	wchar_t *strKeycode = kmalloc(3 * sizeof(wchar_t));

	while(1) {

		key = getScancode();
		c=scancodeToChar(key);

		// Display charcode
		for(l=0; l<16; l++) {
			for(k=0; k<34; k++) {
				putPixel(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-40+k, 82+l, 0x0075507b);
			}
		}
		putString(Gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-36, 84, 0x00ffffff, itoa(key, strKeycode, 16));
		
		// Draw char
		switch(c) {

			case '\n':
				cursorPos=55;
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
				if(cursorPos>=maxcharInLine+55-8) {
					cursorPos=55;
					linePos+=16;
				}

		}
	
	}

*/

}

