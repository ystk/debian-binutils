#as: -64 -Av9
#objdump: -dr
#name: sparc64 wrhpr

.*: +file format .*sparc.*

Disassembly of section .text:

0+ <.text>:
   0:	81 98 40 00 	wrhpr  %g1, %hpstate
   4:	83 98 80 00 	wrhpr  %g2, %htstate
   8:	87 98 c0 00 	wrhpr  %g3, %hintp
   c:	8b 99 00 00 	wrhpr  %g4, %htba
  10:	b9 99 40 00 	wrhpr  %g5, %hstick_offset
  14:	bb 99 80 00 	wrhpr  %g6, %hstick_enable
  18:	bf 99 c0 00 	wrhpr  %g7, %hstick_cmpr
