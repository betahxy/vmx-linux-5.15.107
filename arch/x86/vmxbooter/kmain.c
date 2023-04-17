#include <asm/bootparam.h>

struct boot_params boot_params;
char *cmd_line_str = "auto";
struct setup_data init_setup_data;

void kmain(void);
extern char input_data, input_data_end;
/*
	0x0000'7FFFFFFFFFFF	user upper
	0xffff'800000000000	machine kernel base
	0xffff'ffff80000000	linux kernel base
	0xffff'ffff81000000	linux kernel base -> 16M base address


	ffff80000	000
	    |
	    v
1111 1111 1' 111 1111 10' 00 0000 000' 0 0000 0000'
0x1ff       ' 0x1fe
*/

#define IMAGE_VADDR_START (0xffffffff80000000UL)
#define _va(x) ((unsigned long)(x) + IMAGE_VADDR_START)
#define _pa(x) ((unsigned long)(x) - IMAGE_VADDR_START)

static __attribute__((unused))
void *memset(void *dst, int b,  unsigned long len)
{
	char *p = dst;

	while (len--)
		*(p++) = b;
	return dst;
}

static __attribute__((unused))
int memcmp(const void *s1, const void *s2, unsigned long n)
{
	size_t ofs = 0;
	int c1 = 0;

	while (ofs < n && !(c1 = ((unsigned char *)s1)[ofs] - ((unsigned char *)s2)[ofs])) {
		ofs++;
	}
	return c1;
}

void kmain(void)
{

	char *vga_base = (char *)_va(0xb8000);
	char *vmlinux_base = (char *)_va(0x1000000);
	char *guest_pos = &input_data;
	char *guest_pos_end = &input_data_end;

	memset(&boot_params, 0, sizeof(boot_params));
	if ((unsigned long)guest_pos_end > (unsigned long)vmlinux_base) 
	{
		while (1)
		{
			/* code */
		}
		
	}

	unsigned long image_sz = (unsigned long)guest_pos_end - (unsigned long)guest_pos;
	/* copy image */
	for (unsigned long i = 0; i < image_sz; i++) {
		vmlinux_base[i] = guest_pos[i];
	}

	vga_base[0] = '!';
	boot_params.sentinel = 0xff;
	boot_params.hdr.version = 0x0215;
	boot_params.hdr.vid_mode = 0xFFFF;
	boot_params.hdr.boot_flag = 0xAA55;
	((char *)(&boot_params.hdr.header))[0] = 'H';
	((char *)(&boot_params.hdr.header))[1] = 'd';
	((char *)(&boot_params.hdr.header))[2] = 'r';
	((char *)(&boot_params.hdr.header))[3] = 's';
	boot_params.hdr.type_of_loader = 0xFF;
	boot_params.hdr.loadflags = 1; // the protected-mode code is loaded at 0x100000
	boot_params.hdr.ramdisk_image = 0; //The 32-bit linear address of the initial ramdisk or ramfs. Leave at zero if there is no initial ramdisk/ramfs.
	boot_params.hdr.ramdisk_size = 0; // Size of the initial ramdisk or ramfs. Leave at zero if there is no initial ramdisk/ramfs.
	boot_params.hdr.heap_end_ptr = 0xe000 - 0x200;
	boot_params.hdr.loadflags |= 0x80; /* CAN_USE_HEAP */;
	boot_params.hdr.cmdline_size = _pa(cmd_line_str);
	boot_params.hdr.cmdline_size = 255;
	// init_setup_data.next = 0;
	// boot_params.hdr.setup_data = _pa(&init_setup_data);
	boot_params.hdr.setup_data = 0; //no setup data
	asm volatile("mov %0, %%rsi;movabs $0x1000000, %%rax; jmpq   *%%rax;"::"r"((unsigned long)_pa(&boot_params)));
	while (1)
	{
		/* code */
	}
}
