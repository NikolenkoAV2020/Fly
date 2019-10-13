//---------------------------------------------------------------------------
/*****************************************************************************
******************************************************************************
****               КЛАСС ЗАЗИПОВКИ и РАЗЗИПОВКИ ФАЙЛОВ                    ****
****                                                                      ****
****    Алгоритм взят из пособия "Техника для сжатия данных высокого      ****
****     исполнения", Тэрри A. Уэльский, IEEE Computer Vol 17, No 6       ****
****                       (Июнь 1984), cтр. 8-19                         ****
****                                                                      ****
****    Автор     : Широкий С.М.                                          ****
****    Версия    : 01.01 / 01.04.2006 /                                  ****
******************************************************************************
*****************************************************************************/
//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma   hdrstop

#include <FlyZip.h>

#include <errno.h>
#include <fcntl.h>
#include <string>
#include <io.h>

static char buff[BITS];

//****************************************************************************
//*** Функция задания начальных значений переменным КЛАССА
//****************************************************************************
Zip::Zip()
{
  maxbits = BITS;
  maxmaxcode = 1 << BITS;
  hsize = HSIZE;
  free_ent = exit_stat = nomagic = zcat_flg = 0;
  quiet = 1;
  block_compress = BLOCK_MASK;
  clear_flg = 0;
  ratio = force = valid = 0;
  checkpoint = CHECK_GAP;
  overwrite =  recursive = 0;
  
  offset = 0, size = 0;
  
  in = nullptr;

  memset(Slech,'\0', sizeof(Slech));  
  strcpy(Slech,
              #ifdef _UNDER_LINUX_
                 "/"); 
              #else
                 "\\"); 
              #endif

  in_count  = 1;	// размер при входе
  bytes_out = 0;	// размер сжатого выхода
  out_count = 0;	// #of codes output (for debugging)

#ifndef vax
  lmask[0] = 0xff;
  lmask[1] = 0xfe;
  lmask[2] = 0xfc;
  lmask[3] = 0xf8;
  lmask[4] = 0xf0;
  lmask[5] = 0xe0;
  lmask[6] = 0xc0;
  lmask[7] = 0x80;
  lmask[8] = 0x00;
  
  rmask[0] = 0x00;
  rmask[1] = 0x01;
  rmask[2] = 0x03;
  rmask[3] = 0x07;
  rmask[4] = 0x0f;
  rmask[5] = 0x1f;
  rmask[6] = 0x3f;
  rmask[7] = 0x7f;
  rmask[8] = 0xff;
#endif // vax
}


//****************************************************************************
//*** Функция выхода из КЛАССА
//****************************************************************************
Zip::~Zip()
{
  fclose(stdin);
  fclose(stderr);
  fclose(stdout);

  if(in) fclose(in);
}

//****************************************************************************
//*** Функции зазиповки и раззиповки файлов (видимая)
//****************************************************************************
int Zip::zip_unzip(char *NameFile, char *DirOut, bool flg_zip, bool over, char *cod_return)
{// flg_zip: true - зазиповать, false - раззиповать
  overwrite = over;
  
  // Подлинник не имел волшебный номер
  #ifdef COMPATIBLE
    nomagic = 1;	
  #endif
  
  if(maxbits < INIT_BITS) maxbits = INIT_BITS;
  if(maxbits > BITS) maxbits = BITS;
  maxmaxcode = 1 << maxbits;
  
  int hRes = comprexx(NameFile, DirOut, flg_zip);
  if(hRes) strcpy(cod_return, COD_ERROR);
  
  if(stdin ) fclose(stdin);
  if(stderr) fclose(stderr);
  if(stdout) fclose(stdout);
  if(in) fclose(in);

  return hRes;
}


//****************************************************************************
//*** Функции зазиповки и раззиповки файлов
//****************************************************************************
int Zip::comprexx(char *fileptr, char *DirOut, bool flg_zip)
{
  char tempname[MAXPATHLEN], *cp;
  strcpy(tempname, fileptr);
  errno = 0;

  stat(tempname, &insbuf);

  exit_stat = 0;
  // Настройка исходных данных перед compression/decompression
  if(!flg_zip)
  {// DECOMPRESSION
      if(!zcat_flg && _stricmp(tempname + strlen(tempname) - 2, ".Z") != 0)
      {
         if(!quiet) error_exit(4, tempname);
         return 4;
      }

      // Open input file
      if((in =fopen(tempname, "rb")) == nullptr)
      {
         error_exit(11, tempname);
         return 11;
      }

      // Проверка файла на требуемый формат
      if(nomagic == 0)
      {
         if((getc(in) != ('\037' & 0xFF)) || (getc(in) != ('\235' & 0xFF)))
         {
           error_exit(5, tempname);
           return 5;
         }

         maxbits = getc(in);
         block_compress = maxbits & BLOCK_MASK;
         maxbits &= BIT_MASK;
         maxmaxcode = 1 << maxbits;
         if(maxbits > BITS)
         {
           sprintf(COD_ERROR,	"Файл %s compressed with %d bits, can only handle %d bits", tempname, maxbits, BITS);
           return 1;
         }
      }

      // Формирование выходного файла
      strcpy(ofname, DirOut);
      cp = rindex(tempname, Slech[0]); cp++;
      strcat(ofname, cp);

      // Проверка на наличие расширения .Z 
      if(_stricmp(ofname + strlen(ofname) - 2, ".Z") == 0)
       ofname[strlen(ofname) - 2] = '\0';  // Удаляем .Z 
  }
  else
  {// COMPRESSION
      if(_stricmp(tempname + strlen(tempname) - 2, ".Z") == 0)
      {
         error_exit(7, tempname);
         return 7;
      }

      if(insbuf.st_nlink > 1 && !force)
      {
         sprintf(COD_ERROR, "Файл %s используется %d пользователями. Пропущен", tempname, insbuf.st_nlink - 1);
         return 1;
      }

      // Открываем исходный файл
      if((in =fopen(tempname, "rb")) == nullptr)
      {
         error_exit(11, tempname);
         return 11;
      }

      fsize = (long) insbuf.st_size;
		  
      // tune hash table size for small files -- ad hoc,
      // but the sizes match earlier #defines, which
      // serve as upper bounds on the number of output codes.
      hsize = HSIZE;
      if ( fsize < (1 << 12) )
          hsize = min_d ( 5003, HSIZE );
      else if ( fsize < (1 << 13) )
          hsize = min_d ( 9001, HSIZE );
      else if ( fsize < (1 << 14) )
          hsize = min_d ( 18013, HSIZE );
      else if ( fsize < (1 << 15) )
          hsize = min_d ( 35023, HSIZE );
      else if ( fsize < 47000 )
          hsize = min_d ( 50021, HSIZE );

      // Формирование выходного файла
      strcpy(ofname, DirOut);
      cp = rindex(tempname, Slech[0], true); cp++;
	  strcat(ofname, &Slech[0]);
      strcat(ofname, cp);
      strcat(ofname, ".Z");
  } // COMPRESSION

  // Проверка на размер имени файла
  if(strlen(cp) > 128)
  {error_exit(9, ofname); return 9;}

  // Открытие выходного файла с проверкой на повторяемость файла 
  // (overwrite: true - перезапись, false - пропустить если есть)
  if(!zcat_flg)
  {
     valid = 1;

     if(!overwrite && stat(ofname, &statbuf) == 0)
     {
        error_exit(-2,ofname);
        return -2;
     }

     if(!freopen(ofname, "wb", stdout))
     {
        error_exit(12, ofname);
        return 12;
     }
  }
  else
  {
     #ifdef _UNDER_LINUX_
     
     #else
       _setmode(_fileno(stdout), O_BINARY);
     #endif
  }

  setvbuf(stdout, IOoutbuf, _IOFBF, sizeof(IOoutbuf));

  // Выполнение compression/decompression
  if(!flg_zip) 
    decompress();
    else
    compress();    

  return 0;
}


//****************************************************************************
//*** Функция зазиповки файлов
//****************************************************************************
// Algorithm:  use open addressing double hashing (no chaining) on the
// prefix code / next character combination.  We do a variant of Knuth's
// algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
// secondary probe.  Here, the modular division first probe is gives way
// to a faster exclusive-or manipulation.  Also do block compression with
// an adaptive reset, whereby the code table is cleared when the compression
// ratio decreases, but after the table fills.  The variable-length output
// codes are re-sized at this point, and a special CLEAR code is generated
// for the decompressor.  Late addition:  construct the table according to
// file size for noticeable speed improvement on small files.  Please direct
// questions about this implementation to ames!jaw.
int Zip::compress()
{
  register long fcode;
  register code_int i = 0;
  register int c;
  register code_int ent;
#ifdef XENIX_16
  register code_int disp;
#else	// Стандартная машина  
  register int disp;
#endif
  register code_int hsize_reg;
  register int hshift;

#ifndef COMPATIBLE
  if(nomagic == 0)
  {
     putchar('\037');    putchar('\235');
	   putchar((char)(maxbits | block_compress));
	   if(ferror(stdout) && valid)
     {
	     remove(ofname);
       return 1;
     }
  }
#endif // COMPATIBLE

  offset = 0;
  bytes_out = 3; // includes 3-byte header mojo
  out_count = 0;
  clear_flg = 0;
  ratio = 0;
  in_count = 1;
  checkpoint = CHECK_GAP;
  maxcode = MAXCODE(n_bits = INIT_BITS);
  free_ent = ((block_compress) ? FIRST : 256 );

  ent = getc(in);

  hshift = 0;
  for(fcode = (long) hsize;  fcode < 65536L; fcode *= 2L)
   hshift++;
  hshift = 8 - hshift;		// set hash code range bound

  hsize_reg = hsize;
  cl_hash( (count_int) hsize_reg);		// clear hash table

#ifdef SIGNED_COMPARE_SLOW
  while ( (c = getc(in)) != (unsigned) EOF ) {
#else
  while ( (c = getc(in)) != EOF ) {
#endif
	  in_count++;
	  fcode = (long) (((long) c << maxbits) + ent);
 	  i = ((c << hshift) ^ ent);	// xor hashing

	  if ( htabof (i) == fcode )
    {
	    ent = codetabof (i);
	    continue;
	  }
    else if ( (long)htabof (i) < 0 )	// empty slot
	         goto nomatch;

 	  disp = hsize_reg - i;		// secondary hash (after G. Knott)
	  if ( i == 0 )
	     disp = 1;

probe:
	  if((i -= disp) < 0)
	   i += hsize_reg;

	  if( htabof (i) == fcode )
    {
	    ent = codetabof (i);
	    continue;
    }

	  if( (long)htabof (i) > 0 ) 
	    goto probe;
      
nomatch:
	  output ( (code_int) ent );
	  out_count++;
 	  ent = c;
#ifdef SIGNED_COMPARE_SLOW
	  if ( (unsigned) free_ent < (unsigned) maxmaxcode) {
#else
	  if ( free_ent < maxmaxcode ) {
#endif
 	    codetabof(i) = (unsigned short)(free_ent++);	// code -> hashtable
	    htabof(i) = fcode;
    }
	  else if ( (count_int)in_count >= checkpoint && block_compress )
	        cl_block ();
  }

  // Put out the final code
  output( (code_int)ent );
  out_count++;
  output( (code_int)-1 );

	// exit(2) if no savings
  if(bytes_out > in_count)
	 exit_stat = 2;
  
  return 0;
}


//*****************************************************************
// Output the given code.
// Inputs:
// 	code:	A n_bits-bit integer.  If == -1, then EOF.  This assumes
//		that n_bits =< (long)wordsize - 1.
// Outputs:
// 	Outputs code to the file.
// Assumptions:
//	Chars are 8 bits long.
// Algorithm:
// 	Maintain a BITS character long buffer (so that 8 codes will
// fit in it exactly).  Use the VAX insv instruction to insert each
// code in turn.  When the buffer fills up empty it and start over.
//****************************************************************************
int Zip::output(code_int code)
{
  // On the VAX, it is important to have the register declarations
  // in exactly the order given, or the asm will break.
  register int r_off = offset, bits= n_bits;
  register char * bp = buff;

  if(code >= 0) 
  {
#ifdef vax
	  // VAX DEPENDENT!! Implementation on other machines is below.
	  // Translation: Insert BITS bits from the argument starting at
	  // offset bits from the beginning of buf.
   
	  0;	// Work around for pcc -O bug with asm and if stmt
	  asm( "insv	4(ap),r11,r10,(r9)" );
#else // not a vax
    // byte/bit numbering on the VAX is simulated by the following code
	  // Get to the first byte.
	  bp += (r_off >> 3);
	  r_off &= 7;

	  //Since code is always >= 8 bits, only need to mask the first hunk on the left
	 *bp = (*bp & rmask[r_off]) | (code << r_off) & lmask[r_off];
	  bp++;
	  bits -= (8 - r_off);
	  code >>= 8 - r_off;

	  // Get any 8 bit parts in the middle (<=1 for up to 16 bits).
	if( bits >= 8 ) {
	   *bp++ = (char)code;
	    code >>= 8;
	    bits -= 8;
    }
	// Last bits.
	if(bits) *bp = (char)code;
#endif // vax
	  
    offset += n_bits;
	if ( offset == (n_bits << 3) ) {
	    bp = buff;
	    bits = n_bits;
	    bytes_out += bits;
	    do putchar(*bp++);
	    while(--bits);
	    offset = 0;
    }

	  // If the next entry is going to be too big for the code size, then increase it, if possible
	  if ( free_ent > maxcode || (clear_flg > 0))
    {
      // Write the whole buffer, because the input side won't
      // discover the size increase until after it has read it.
	    if(offset > 0)
      {
      	if(fwrite( buff, 1, n_bits, stdout ) != n_bits  &&  valid)
        {
	         remove(ofname);
           return 1;
        }
        bytes_out += n_bits;
	    }
	    offset = 0;

	    if(clear_flg)
      {
    	  maxcode = MAXCODE (n_bits = INIT_BITS);
	      clear_flg = 0;
	    }
	    else
      {
	    	n_bits++;
	    	if( n_bits == maxbits )
		    maxcode = maxmaxcode;
	    	else
		    maxcode = MAXCODE(n_bits);
	    }
    }
  }
  else
  {
  	// At EOF, write the rest of the buffer.
	  if(offset > 0)
	   fwrite( buff, 1, (offset + 7) / 8, stdout );
	  
    bytes_out += (offset + 7) / 8;
	  offset = 0;
	  fflush( stdout );
	  if(ferror(stdout) && valid)
    {
	     remove(ofname);
       return 1;
    }
  }
 
  return 0;
}


//****************************************************************************
//*** Функция раззиповки файлов
//****************************************************************************
// This routine adapts to the codes in the
// file building the "string" table on-the-fly; requiring no table to
// be stored in the compressed file.  The tables used herein are shared
// with those of the compress() routine.  See the definitions above.
int Zip::decompress()
{
  register char_type *stackp;
  register int finchar;
  register code_int code, oldcode, incode;
 
  // As above, initialize the first 256 entries in the table.
  maxcode = MAXCODE(n_bits = INIT_BITS);
  for ( code = 255; code >= 0; code-- )
  {
	  tab_prefixof(code) = 0;
	  tab_suffixof(code) = (char_type)code;
  }
  
  free_ent = ((block_compress) ? FIRST : 256 );
  finchar = oldcode = getcode();

  // Если уже конец файла, то идем на выход
  if(oldcode == -1)	        
   return 0;			            

  // Первый код должен быть 8 bits = char
  putchar( (char)finchar );	

  // Crash if can't write
  if(ferror(stdout) && valid)
  {
	  remove(ofname);
    return 1;
  }

  stackp = de_stack;

  while((code = getcode()) > -1)
  {
     if((code == CLEAR) && block_compress)
     {
	      for( code = 255; code >= 0; code--)
		     tab_prefixof(code) = 0;
	      clear_flg = 1;
	      free_ent = FIRST - 1;
	      // O, untimely death!
        if((code = getcode ()) == -1 )	
	    	 break;
     }
	 
     incode = code;

     // Special case for KwKwK string.
	   if( code >= free_ent )
     {
       *stackp++ = finchar;
	      code = oldcode;
     }
	  
     // Generate output characters in reverse order
#ifdef SIGNED_COMPARE_SLOW
     while( ((unsigned long)code) >= ((unsigned long)256) )
#else
     while( code >= 256 )
#endif
     {
   	   *stackp++ = tab_suffixof(code);
	      code = tab_prefixof(code);
     }
	 
    *stackp++ = finchar = tab_suffixof(code);
 	 
     // And put them out in forward order
     do
	     putchar ( *--stackp );
     while( stackp > de_stack );

	   // Генерация нового входа.
	   if( (code=free_ent) < maxmaxcode )
     {
	      tab_prefixof(code) = (unsigned short)oldcode;
	      tab_suffixof(code) = finchar;
	      free_ent = code+1;
     }
	 
     // Поминайте предшествующий код.
	   oldcode = incode;
  }
  
  fflush(stdout);
  if(ferror(stdout) && valid)
  {
	  remove(ofname);
    return 1;
  }
   
  return 0;
}


//****************************************************************************
//** Чтение одной строки кода из стандартной вкладки. Если EOF, return -1. 
//****************************************************************************
long int Zip::getcode()
{//On the VAX, it is important to have the register declarations in exactly the order given, or the asm will break.
  register code_int code;
  register int r_off, bits;
  register char_type *bp = buf;

  if( clear_flg > 0 || offset >= size || free_ent > maxcode )
  {
    // Если следующий вход будет слишком большим для текущего кода, необходимо его увелтчить
    // Это подразумевает чтение a new buffer full, too.
	  if( free_ent > maxcode )
    {
	    n_bits++;
	    if ( n_bits == maxbits )
		    maxcode = maxmaxcode;	// won't get any bigger now
	      else
		    maxcode = MAXCODE(n_bits);
	  }
	  if ( clear_flg > 0)
    {
    	maxcode = MAXCODE (n_bits = INIT_BITS);
	    clear_flg = 0;
	  }

  	size = fread( buf, 1, n_bits, in);
	  if ( size <= 0 )
	     return -1;			// конец файла

		offset = 0;
	  // Круглый размер вплоть до целого числа кодировать
	  size = (size << 3) - (n_bits - 1);
  }

  r_off = offset;
  bits = n_bits;

#ifdef vax
    asm( "extzv   r10,r9,(r8),r11" );
#else // not a vax
	// Get to the first byte.
	bp += (r_off >> 3);
	r_off &= 7;
	// Get first part (low order bits)
#ifdef NO_UCHAR
	code = ((*bp++ >> r_off) & rmask[8 - r_off]) & 0xff;
#else
	code = (*bp++ >> r_off);
#endif // NO_UCHAR 
	bits -= (8 - r_off);
	r_off = 8 - r_off;		// now, offset into code word
	// Get any 8 bit parts in the middle (<=1 for up to 16 bits).
	if ( bits >= 8 ) {
#ifdef NO_UCHAR
	    code |= (*bp++ & 0xff) << r_off;
#else
	    code |= *bp++ << r_off;
#endif // NO_UCHAR
	    r_off += 8;
	    bits -= 8;
	}

  // high order bits.
  code |= (*bp & rmask[bits]) << r_off;

#endif // vax
  offset += n_bits;

  return code;
}


//****************************************************************************
//** Табличная очистка для блочного компресса 
//****************************************************************************
int Zip::cl_block ()
{ 
  register long int rat;
  checkpoint = in_count + CHECK_GAP;

  // shift will overflow
  if(in_count > 0x007fffff)
  {	
	  rat = bytes_out >> 8;
	  if(rat == 0) 	// Don't divide by zero
	    rat = 0x7fffffff;
	    else
	    rat = in_count / rat;
  } 
  else rat = (in_count << 8) / bytes_out;	// 8 fractional bits

  if(rat > ratio)
  {
	  ratio = rat;
  }
  else 
  {
	  ratio = 0;
 	  cl_hash ( (count_int) hsize );
	  free_ent = FIRST;
	  clear_flg = 1;
	  output ( (code_int) CLEAR );

  }
  
  return 0;
}


//****************************************************************************
//*** Функция получения имени файла
//****************************************************************************
//****************************************************************************
char * Zip::rindex(register char *s, register char c, bool excludeExt)
{
	char* p;
	for (p = nullptr; *s; s++) {
		if (*s == c || *s == '/' || *s == '\\')	p = s;
	}
	// Исключение из имени файла расширения
	if (excludeExt && p) {
		int n = strlen(p);
		while (--n>=0) {
			if (p[n] == '.') { p[n] = '\0'; break; }
		}
	}
	return p;
}


//****************************************************************************
int Zip::cl_hash(register count_int hsize)
{// восстановление кодового стола

#ifndef XENIX_16	// Normal machine
	register count_int *htab_p = htab+hsize;
#else
	register j;
	register long k = hsize;
	register count_int *htab_p;
#endif

	register long i;
	register long m1 = -1;

#ifdef XENIX_16
    for(j=0; j<=8 && k>=0; j++,k-=8192) {
	i = 8192;
	if(k < 8192) {
		i = k;
	}
	htab_p = &(htab[j][i]);
	i -= 16;
	if(i > 0) {
#else
	i = hsize - 16;
#endif
 	do {// might use Sys V memset(3) here
		*(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);
#ifdef XENIX_16
	}
    }
#endif
  
  for( i += 16; i > 0; i-- )
   *--htab_p = m1;
  
  return 0;
}


//****************************************************************************
//*** Функция получения описания ошибки по номеру
//****************************************************************************
void Zip::error_exit(int i, char *p)
{
  switch(i)
  {
    case -2:  sprintf(COD_ERROR,"Файл %s существует. Пропощен.", p); break;
    case  1:  break;
    case  2:  sprintf(COD_ERROR,"Ошибка: Файла %s.Z в каталоге не обнаружено!", p); break;
    case  3:  sprintf(COD_ERROR,"Ошибка: %s is a directory - ignored", p); break;
    case  4:  sprintf(COD_ERROR,"Ошибка: Расширение файла %s - не .Z", p); break;
    case  5:  sprintf(COD_ERROR,"Ошибка: Файл %s либо не того формата, либо не сжат", p); break;
    case  7:  sprintf(COD_ERROR,"Ошибка: %s: уже запакован (имеет расширение .Z). Пропущен", p); break;
    case  9:  sprintf(COD_ERROR,"Ошибка: %s: название файла слишком длинное (больше 128 символов)", p); break;
    case 10:  sprintf(COD_ERROR,"Ошибка: %s is not a directory or a regular file - ignored", p); break;
    case 11:  sprintf(COD_ERROR,"Ошибка чтения файла %s", p); break;
    case 12:  sprintf(COD_ERROR,"Ошибка создания файла %s", p); break;
  }

  return;
}



