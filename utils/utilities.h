#ifndef __UTILITIES_H__
#define __UTILITIES_H__



/******************************************************************************/
								/* SIZES */

#define BYTE 						8
#define INT_SIZE 					sizeof(int)
#define WORD_SIZE					sizeof(double)
#define SIZE_PTR 					sizeof(void *)
#define SIZE_OF_DOUBLE				sizeof(double)
#define SIZE_OF_INT					sizeof(int)

#define SIZEOF_VAR(var)				((size_t)((&var) + 1) - (size_t)(&var))
#define SIZEOF_TYPE(type)			(size_t)((type *)0 + 1)
#define SIZEOF_ARRAY(arr)			sizeof(arr)/sizeof(arr[0])


#define OFFSETOF(TYPE, MEMBER) 			(void *)(&((TYPE *)NULL)->MEMBER)
#define OFFSETOF_SIZE_T(TYPE, MEMBER)	((size_t)&(((TYPE *)0)->MEMBER))

#define NULL_TERMIN 1				/* byte terminator '\0' */
#define MAX_INT_DIG 10				/* Biggest int number will have 10 digits */

/******************************************************************************/
				/* Error Handling */

/*#define SUCCESS 				0*/
/*#define ALLOC_ERR 			1*/
/*#define FUNC_FAILED 			2*/

#define INVALID_PTR 			(void *)0xBADC0FFEE0DDF00D
#define INVALID_ADDRESS 		0xBADC0FFEE0DDF00D

/* 64 bit 0xBADC0FFEE0DDF00D || 32 bit 0xDEADBEEF */
#define DEAD_MEM(TYPE)			(TYPE)0xBADC0FFEE0DDF00D


#ifndef NDEBUG
	#define DEBUG_MODE(code) 	code
	#define TEST_MODE(code) 	code
#else
	#define DEBUG_MODE(code)
	#define TEST_MODE(code)
#endif /* D_DEBUG */


#define RETURN_IF_BAD(is_bad, err_msg, ret_val) 				\
						if(!is_bad)								\
						{ fputs(err_msg, stderr); return ret_val; }


#define RETURN_IF_BAD_NESTED(is_bad, err_msg, ret_val, to_free) \
						if(!is_bad)								\
						{ fputs(err_msg, stderr);				\
						free(to_free); return ret_val; }

#define ASSERT_NOT_NULL(ptr, str)	assert((NULL != (ptr)) && (str))



/******************************************************************************/
							/* General UTIL */

#define UNUSED(x) 					(void)(x)
#define COMMA						,

/******************************************************************************/
							/* Tests Print Data Types */

#define PRINT_MSG(str) 					puts(#str"\t")
#define PRINT_INT(int_type) 			printf("%d\n", int_type)
#define PRINT_SIZE_T(size_t_type) 		printf("=>\t%lu\n", size_t_type)
#define PRINT_HEX_ADDR(pointer) 		printf("=>\t%x\n", pointer)
#define PRINT_PTR_ADDRESS(ptr) 			printf("Address \t%p\n", (void *)ptr)
#define NEW_LINE				 		putchar('\n');

#define PRINT_STATUS_MSG(str)			puts("\t"#str)

/* Print Data Types and Messages */
#define PRINTM_INT(str, int_type) 		printf(#str"\t%d\n", int_type)
#define PRINTM_SIZE_T(str, size_t_type) printf(#str"\t%lu\n", size_t_type)
#define PRINTM_PTR_ADDRESS(str, ptr) 	printf(#str"\t%p\n", (void *)ptr)

/* Printing Colors Amram INC */
#define GREEN printf("\033[0;32m")
#define RED printf("\033[0;31m")
#define YELLOW printf("\033[0;33m")
#define PURPLE printf("\033[0;35m")
#define BLUE printf("\033[0;34m")
#define DEFAULT printf("\033[0m")

#define PRINT_IS_SUCCESS(flag, function_name)									\
						if (1 == flag) 											\
						{ GREEN; PRINT_STATUS_MSG(Is function_name); DEFAULT; } \
						else 													\
						{ RED; PRINT_STATUS_MSG(Not function_name); DEFAULT;}


/******************************************************************************/
							/* Typedef */

typedef enum bool {FALSE = 0, TRUE = 1} bool_ty;


#endif /* __UTILITIES_H__ */