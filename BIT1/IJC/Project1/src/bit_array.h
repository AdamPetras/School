/*
 * bit_array.h
 *
 *  Created on: 7. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_
#include "error.h"
#include <syslog.h>
#include <stdio.h>
#include <limits.h>
//(size) --> ošetření při vstupu (3 + 4 * 2) != ((3+4)*2)
typedef unsigned long *bit_array_t;

/*(index >=32 ? ( BA_GET_BIT_(array,(((index)/(sizeof(unsigned long)*CHAR_BIT))+1))) :\
				BA_GET_BIT_(array,index))*/
//bez kontroly mezí
//#define BA_GET_BIT_(array,index) ((array[((index/32)+1)] & (1 << ((index%32)+1)))>0 ? 1 : 0)
#define BA_GET_BIT_(array,index) ((array[((index/(sizeof(*array)*CHAR_BIT))+1)] & (1UL << ((index%(sizeof(*array)*CHAR_BIT)))))>0 ? 1 : 0)
//bez kontroly mezí
//#define BA_SET_BIT_(array,index,value) (value !=0 ? ( array[((index/32)+1)] |= (1 << ((index%32)+1))) : (array[((index/32)+1)] &= ~(1 << ((index%32)+1))))
#define BA_SET_BIT_(array,index,value) (value !=0 ? ( array[((index/(sizeof(*array)*CHAR_BIT))+1)] |= (1UL << ((index%(sizeof(*array)*CHAR_BIT))))) : (array[((index/(sizeof(*array)*CHAR_BIT))+1)] &= ~(1UL << ((index%(sizeof(*array)*CHAR_BIT))))))

#define ba_ppm_set_bit(array,index,value) (value !=0 ? ( array[((index/(sizeof(*array)*CHAR_BIT)))] |= (1UL << ((index%(sizeof(*array)*CHAR_BIT))))) :\
					(array[((index/(sizeof(*array)*CHAR_BIT)))] &= ~(1UL << ((index%(sizeof(*array)*CHAR_BIT))))))

#define ba_ppm_get_bit(array,index) ((array[((index/(sizeof(*array)*CHAR_BIT)))] & (1UL << ((index%(sizeof(*array)*CHAR_BIT)))))>0 ? 1 : 0)

#define ba_create(array,size) unsigned long array[((size/(sizeof(unsigned long)*CHAR_BIT)) + (size % (sizeof(unsigned long)*CHAR_BIT) > 0 ? 1 : 0) + 1)] = {size}

#ifndef USE_INLINE
//get size in bits
#define ba_size(array) array[0]
/*1
 * 	if(index<0 || index>= arraysize)
 *  	errormsg;
 *  else
 *  {
 * 		int i = index/32;            				// i = index pole (array[i])
 *  	int posit= index%32;						// posit = bitová pozice v array[i]
 *  	value = value << posit;      				// flag = 0000...010...000   (odshiftovaná pozice)
 *		if(value == 0)
 *			~value;
 *		array[i] = array[i] | vyraz;      			// naství bit nté pozice na pozici array[i]
 *	}
 */
#define ba_set_bit(array, index, value) ((index) < 0 || ((index) >= ba_size(array)))?\
		(error_msg("Index %lu mimo rozsah 0..%lu", (index), (ba_size(array)-1))), 0 :\
		BA_SET_BIT_(array,index,value)
/*	if(index<0 || index>= arraysize)
 *  	errormsg;
 *  else
 *  {
 * 	int i = index/32;            				// i = index pole (array[i])
 *  int posit= index%32;
 *  value = 1 << posit;							// posit = bitová pozice v array[i]
 *	return (array[i] & value>0) ? 1 : 0;      	// naství bit nté pozice na pozici array[i]
 *	}
 */
#define ba_get_bit(array,index) ((index) < 0 || (index) >= ba_size(array))?\
		(error_msg("Index %lu mimo rozsah 0..%lu", (index), (ba_size(array)-1))), 0:\
		BA_GET_BIT_(array,index)
#else

inline unsigned long ba_size(bit_array_t array)
{
	return array[0];
}

inline void ba_set_bit(bit_array_t array,unsigned long index, int value)
{
	if(index>=ba_size(array))
		error_msg("Index %lu mimo rozsah 0..%lu", index, (ba_size(array)-1));
	else
		BA_SET_BIT_(array,index,value);
	//(index < 0 || (index >= ba_size(array)))?error_msg("Index %lu mimo rozsah 0..%lu", index, (ba_size(array)-1)):(value != 0 ?  (array[(((index)/(sizeof(unsigned long)*CHAR_BIT))+1)] |= (1 << (((index)%(sizeof(unsigned long)*CHAR_BIT))+1))) :BA_SET_BIT_(array,index,value));
}

inline int ba_get_bit(bit_array_t array, unsigned long index)
{
	if(index>=ba_size(array))
	{
		error_msg("Index %lu mimo rozsah 0..%lu", index, (ba_size(array)-1));
		return 0;
	}
	else
		return (BA_GET_BIT_(array,index));
	/*(index < 0 || index >= ba_size(array))?
			error_msg("Index %lu mimo rozsah 0..%lu", index, (ba_size(array)-1)):
	BA_GET_BIT_(array,index);*/
}

#endif
#endif /* BIT_ARRAY_H_ */
