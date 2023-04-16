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

void kmain(void)
{

	char *vga_base = (char *)(0xb8000 + 0xffffffff80000000);
	char *vmlinux_base = (char *)0xffffffff81000000;
	char *guest_pos = &input_data;
	char *guest_pos_end = &input_data_end;

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

	asm volatile("movabs $0x1000000, %%rax; jmpq   *%%rax;"::);
	while (1)
	{
		/* code */
	}
}
