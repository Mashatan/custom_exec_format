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


//Features custom format is following :
//  1- Digital Signature
//  2- Support 64-bit 
//  2- Support Fat Binaray
//  3- Support Meta data


//Refrence : https://wiki.osdev.org/COFF
#define	COFF_F_RELFLG	0000001 /* relocation info stripped from file */
#define	COFF_F_EXEC		0000002 /* file is executable  (i.e. no unresolved external references) */
#define	COFF_F_LNNO		0000004 /* line numbers stripped from file */
#define	COFF_F_LSYMS	0000010 /* local symbols stripped from file */
#define	COFF_F_MINMAL	0000020 /* this is a minimal object file (".m") output of fextract */
#define	COFF_F_UPDATE	0000040 /* this is a fully bound update file, output of ogen */

#define	COFF_MAGIC		0x15C   /* custom_format    */


struct file_hdr{
	unsigned short 	f_magic;	/* Magic number (0x15C) */	
	unsigned short 	f_nscns;	/* Number of Sections */
	long			f_timdat;	/* Time & date stamp */
	long			f_symptr;	/* File pointer to Symbol Table  (DEPRECATED) */
	long			f_nsyms;	/* Number of Symbols    (DEPRECATED)*/
	unsigned short	f_coffhdr;	/* Sizeof(coff header) */
	unsigned short	f_flags;	/* Flags */
};

struct coff_hdr{
	unsigned short 	magic;          /* Magic Number	(0x169)						*/
	unsigned short 	vstamp;         /* Version stamp							*/
	unsigned long 	tsize;          /* Code size in bytes (sum all of sections) */
	unsigned long 	dsize;          /* Initialised data size     (.DATA)		*/
	unsigned long 	bsize;          /* Uninitialised data size   (.BSS)			*/
	unsigned long 	addrptr;        /* File pointer to address					*/
	unsigned long 	addrsize;       /* size of address							*/
};

struct custom_hdr{
	unsigned short 	cversion;			/* Custom Version stamp                   */
	unsigned long 	certificate_table;  /* Digital certificate table  */
	unsigned long 	certificate_size;   /* certificate size  */
	unsigned char   checksum[32];		/* SHA-1 256bit (sum all of sections)*/
};

struct certificate_table{
	unsigned char   sign[32];	/* Signature 256bit ,by ED25519	for signing   => sign(Data (HMAC-SHA256) + copyright, privateKey) ==> DataSigned , 
																for verifying => verify(DataSigned, publicKey) */
	unsigned char   copyright[50];
};

/*CPU architectures flags*/
#define ARCH_X86_32 0x01
#define ARCH_X86_64 0x02
#define ARCH_ARM_32 0x03
#define ARCH_ARM_64 0x04
struct address{
	unsigned short		architecture;	/* Set CPU architectures*/
	unsigned long		entryptr;		/* Entry point  (RVA)                   */
	unsigned long long	image_base;		/* Image Base, support 64-bit , default is 0x40000               */
	unsigned long		code_start;		/* Base of Code used for this architecture (RVA)*/
	unsigned long		data_start;		/* Base of Data used for this architecture (RVA)*/

	/*
		VA = ImageBase + RVA
	*/
};

#define STYP_CODE		0x0010		/* The section contains executable code.  Permission: Read/Execute */
#define STYP_DATA		0x0020		/* The section contains initialised data. Permission: Read/Write */
#define STYP_BSS		0x0040		/* The COFF file contains no data for this section, but needs space to be allocated for it. Permission: Read/Write */

struct section{
	char			s_name[8];		/* Section Name */
	long			s_paddr;		/* Physical Address */
	long			s_vaddr;		/* Virtual Address */
	long			s_size;			/* Section Size in Bytes */
	long			s_scnptr;		/* File offset to the Section data */
	long			s_relptr;		/* File offset to the Relocation table for this Section */
	unsigned short	s_nlnno;		/* Number of Line Number table entries */
	long			s_lnnoptr;		/* File offset to the Line Number table for this Section */
	unsigned short	s_nreloc;		/* Number of Relocation table entries */
	unsigned short	s_architecture;	/* Set CPU architectures, zero is common */
	long			s_flags;		/* Flags for this section */
};

struct relocation{
	long			r_vaddr;	/* Reference Address	*/
	long			r_symndx;	/* Symbol index			*/
	unsigned short	r_type;		/* Type of relocation	*/
};

struct line_number {
	union
	{
		long		l_symndx;	/* Symbol Index		*/
		long		l_paddr;	/* Physical Address */
	} l_addr;
	unsigned short	l_lnno;		/* Line Number		*/
};

struct symbol_table {
	char			n_name[8];	/* Symbol Name		*/
	long			n_value;	/* Value of Symbol	*/
	short			n_scnum;	/* Section Number	*/
	unsigned short	n_type;		/* Symbol Type		*/
	char			n_sclass;	/* Storage Class	*/
	char			n_numaux;	/* Auxiliary Count	*/
};

#endif // _CUSTOM_FORMAT_