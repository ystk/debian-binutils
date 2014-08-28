/* NDS32-specific support for 32-bit ELF.
   Copyright (C) 2012-2014 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.*/

#ifndef ELF32_NDS32_H
#define ELF32_NDS32_H

/* Relocation flags encoded in r_addend.  */

/* Relocation flags for R_NDS32_ERLAX_ENTRY.  */

/* Set if relax on this section is done or disabled.  */
#define R_NDS32_RELAX_ENTRY_DISABLE_RELAX_FLAG			(1 << 31)
/* Optimize for performance.  */
#define R_NDS32_RELAX_ENTRY_OPTIMIZE_FLAG			(1 << 30)
/* Optimize for size.  Branch destination 4-byte adjustment
   may be disabled.  */
#define R_NDS32_RELAX_ENTRY_OPTIMIZE_FOR_SPACE_FLAG		(1 << 29)
/* To distinguish the assembly code generated by compiler
   or written manually.  */
#define R_NDS32_RELAX_ENTRY_VERBATIM_FLAG			(1 << 28)
/* EX9 and link-time IFC must be explicitly enabled, so we
   won't mess up handcraft assembly code.  */
/* Enable EX9 optimization for this section.  */
#define R_NDS32_RELAX_ENTRY_EX9_FLAG				(1 << 2)
/* Enable IFC optimization for this section.  */
#define R_NDS32_RELAX_ENTRY_IFC_FLAG				(1 << 3)


/* Relocation flags for R_NDS32_INSN16.  */

#define R_NDS32_INSN16_CONVERT_FLAG				(1 << 0)
/* Convert a gp-relative access (e.g., lwi.gp)
   to fp-as-gp access (lwi37.fp).
   This value is used by linker internally only.
   It's fine to change the vlaue.  */
#define R_NDS32_INSN16_FP7U2_FLAG				(1 << 1)

/* Relocation flags for R_NDS32_RELAX_REGION_OMIT_FP_START/END.  */

/* OMIT_FP_FLAG marks the region for applying fp-as-gp
   optimization.  */
#define R_NDS32_RELAX_REGION_OMIT_FP_FLAG			(1 << 0)
/* NOT_OMIT_FP_FLAG is set if this region is not worth
   for fp-as-gp.  */
#define R_NDS32_RELAX_REGION_NOT_OMIT_FP_FLAG			(1 << 1)
/* Suppress EX9 optimization in the region.  */
#define R_NDS32_RELAX_REGION_NO_EX9_FLAG			(1 << 2)
/* A Innermost loop region.  Some optimizations is suppressed
   in this region due to performance drop.  */
#define R_NDS32_RELAX_REGION_INNERMOST_LOOP_FLAG		(1 << 4)

/* Relax tag for nds32_elf_relax_section, we have to specify which
   optimization do in this round.  */
enum
{
  NDS32_RELAX_NONE_ROUND = 0,
  NDS32_RELAX_JUMP_IFC_ROUND = 1,
  NDS32_RELAX_EX9_BUILD_ROUND,
  NDS32_RELAX_EX9_REPLACE_ROUND

};

/* Optimization status mask.  */
#define NDS32_RELAX_JUMP_IFC_DONE	(1 << 0)
#define NDS32_RELAX_EX9_DONE		(1 << 1)

/* Optimization turn on mask.  */
#define NDS32_RELAX_JUMP_IFC_ON		(1 << 0)
#define NDS32_RELAX_EX9_ON		(1 << 1)

/* The break 0xea defined for ex9 table to keep for trace32 to use 0xeaea.  */
#define INSN_BREAK_EA   0x64001d4a

extern void nds32_insertion_sort
  (void *, size_t, size_t, int (*) (const void *, const void *));

extern int         nds32_elf_ex9_init (void);
extern void        nds32_elf_ex9_reloc_jmp (struct bfd_link_info *);
extern void        nds32_elf_ex9_finish (struct bfd_link_info *);
extern bfd_boolean nds32_elf_ex9_itb_base (struct bfd_link_info *);
extern void        nds32_elf_ex9_import_table (struct bfd_link_info *);
extern bfd_boolean nds32_elf_ifc_reloc (void);
extern bfd_boolean nds32_elf_ifc_finish (struct bfd_link_info *);
extern int         nds32_convert_32_to_16 (bfd *, uint32_t, uint16_t *, int *);
extern int         nds32_convert_16_to_32 (bfd *, uint16_t, uint32_t *);
extern void        bfd_elf32_nds32_set_target_option (struct bfd_link_info *, int, int,
						      FILE *, int, int, int, int, FILE *, FILE *,
						      int, int, bfd_boolean, bfd_boolean);

#define nds32_elf_hash_table(info) \
  (elf_hash_table_id ((struct elf_link_hash_table *) ((info)->hash)) \
   == NDS32_ELF_DATA ? ((struct elf_nds32_link_hash_table *) ((info)->hash)) : NULL)

/* Hash table structure for target nds32.  There are some members to
   save target options passed from nds32elf.em to bfd.  */

struct elf_nds32_link_hash_table
{
  struct elf_link_hash_table root;

  /* Short-cuts to get to dynamic linker sections.  */
  asection *sgot;
  asection *sgotplt;
  asection *srelgot;
  asection *splt;
  asection *srelplt;
  asection *sdynbss;
  asection *srelbss;

  /* Small local sym to section mapping cache.  */
  struct sym_cache sym_cache;

  /* Target dependent options.  */
  int relax_fp_as_gp;		/* --mrelax-omit-fp  */
  int eliminate_gc_relocs;	/* --meliminate-gc-relocs  */
  FILE *sym_ld_script;		/* --mgen-symbol-ld-script=<file>  */
  /* Disable if linking a dynamically linked executable.  */
  int load_store_relax;
  int target_optimize;		/* Switch optimization.  */
  int relax_status;		/* Finished optimization.  */
  int relax_round;		/* Going optimization.  */
  FILE *ex9_export_file;	/* --mexport-ex9=<file>  */
  FILE *ex9_import_file;	/* --mimport-ex9=<file>  */
  int update_ex9_table;		/* --mupdate-ex9.  */
  int ex9_limit;
  bfd_boolean ex9_loop_aware;	/* Ignore ex9 if inside a loop.  */
  bfd_boolean ifc_loop_aware;	/* Ignore ifc if inside a loop.  */
};
#endif
