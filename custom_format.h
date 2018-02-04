#ifndef _CUSTOM_FORMAT_
#define _CUSTOM_FORMAT_


//               +------------------------+
//               |      FILE HEADER       |
//               |                        |
//               +------------------------|
//               |      COFF HEADER       |
//               |                        |
//               +------------------------|
//               |      CUSTOM HEADER     |
//               |                        |
//               +------------------------|
//               |     SECTIONS HEADER    |
//               |           ...          |
//               +------------------------|
//               |    Certificate Table   |
//               |           ...          |
//               +------------------------|
//               |         Address        |
//               |           ...          |
//               |                        |
//               +------------------------|


//               +------------------------+
//               |         STACK          |
//               |           |            |
//               |           |            |
//               |           V            |
//               |                        |
//               +------------------------|
//               |                        |
//               |           ^            |
//               |           |            |
//               |           |            |
//               |          HEAP          |
//               +------------------------|
//               |          BSS           |
//               +------------------------|
//               |          DATA          |
//               +------------------------|
//               |          CODE          |
//               +------------------------|

/*
 *   Bits for f_flags:
 *
 *	F_RELFLG	relocation info stripped from file
 *	F_EXEC		file is executable  (i.e. no unresolved external
 *			references)
 *	F_LNNO		line numbers stripped from file
 *	F_LSYMS		local symbols stripped from file
 *	F_MINMAL	this is a minimal object file (".m") output of fextract
 *	F_UPDATE	this is a fully bound update file, output of ogen
 */

#define  COFF_F_RELFLG		0000001
#define  COFF_F_EXEC		0000002
#define  COFF_F_LNNO		0000004
#define  COFF_F_LSYMS		0000010
#define  COFF_F_MINMAL		0000020
#define  COFF_F_UPDATE		0000040

#define	COFF_MAGIC	        0x15C   /* custom_format    */


struct file_hdr{
	unsigned short 	f_magic;	/* Magic number (0x15C) */	
	unsigned short 	f_nscns;	/* Number of Sections */
	long    		f_timdat;	/* Time & date stamp */
	long 			f_symptr;	/* File pointer to Symbol Table  (DEPRECATED) */
	long 			f_nsyms;	/* Number of Symbols    (DEPRECATED)*/
	unsigned short 	f_coffhdr;	/* Sizeof(coff header) */
	unsigned short 	f_flags;	/* Flags */
};

struct coff_hdr{
	unsigned short 	magic;          /* Magic Number                    */
	unsigned short 	vstamp;         /* Version stamp                   */
	unsigned long 	tsize;          /* Code size in bytes (sum all of sections) */
	unsigned long 	dsize;          /* Initialised data size     (.DATA)      */
	unsigned long 	bsize;          /* Uninitialised data size   (.BSS)      */
	unsigned long 	addrptr;        /* File pointer to address                */
};

struct custom_hdr{
	unsigned short 	cversion;			/* Custom Version stamp                   */
	unsigned long 	certificate_table;  /* Digital certificate table, sign by ED25519  */
	unsigned long 	certificate_size;   /* certificate size  */
};

struct certificate_table{
	unsigned char   sign[32];	/* Signature 256bit , for signing   => sign(Data (HMAC-SHA256) + copyright, privateKey) ==> DataSigned , 
													  for verifying => verify(DataSigned, publicKey) */
	unsigned char   copyright[50];
};

#define ARCH_X86_32 0x01
#define ARCH_X86_64 0x02
#define ARCH_ARM_32 0x03
#define ARCH_ARM_64 0x04

struct address{
	unsigned short	architecture;	/* Set CPU architectures*/
	unsigned long 	entryptr;       /* Entry point  (RVA)                   */
	unsigned long 	code_start;     /* Base of Code used for this file (RVA)*/
	unsigned long 	data_start;     /* Base of Data used for this file (RVA)*/
};

#define STYP_CODE		0x0020	/* The section contains executable code.*/
#define STYP_DATA		0x0040	/* The section contains initialised data.*/
#define STYP_BSS		0x0080	/* The COFF file contains no data for this section, but needs space to be allocated for it.*/

struct section{
	char			s_name[8];	/* Section Name */
	long			s_paddr;	/* Physical Address */
	long			s_vaddr;	/* Virtual Address */
	long			s_size;		/* Section Size in Bytes */
	long			s_scnptr;	/* File offset to the Section data */
	long			s_relptr;	/* File offset to the Relocation table for this Section */
	long			s_lnnoptr;	/* File offset to the Line Number table for this Section */
	unsigned short	s_nreloc;	/* Number of Relocation table entries */
	unsigned short	s_nlnno;	/* Number of Line Number table entries */
	unsigned short	s_architecture;	/* Set CPU architectures, 0 is public */
	long			s_flags;	/* Flags for this section */
};

struct relocation{
	long			r_vaddr;	/* Reference Address */
	long			r_symndx;	/* Symbol index */
	unsigned short	r_type;		/* Type of relocation */
};

struct line_number {
	union
	{
		long		l_symndx;	/* Symbol Index */
		long		l_paddr;	/* Physical Address */
	} l_addr;
	unsigned short	l_lnno;		/* Line Number */
};

struct symbol_table {
	char		n_name[8];	/* Symbol Name */
	long		n_value;	/* Value of Symbol */
	short		n_scnum;	/* Section Number */
	unsigned short	n_type;	/* Symbol Type */
	char		n_sclass;	/* Storage Class */
	char		n_numaux;	/* Auxiliary Count */
};

#endif // _CUSTOM_FORMAT_