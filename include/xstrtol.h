#ifndef __XSTRTOL_H__

enum strtol_error
  {         
    LONGINT_OK = 0,

    /* These two values can be ORed together, to indicate that both
       errors occurred.  */
    LONGINT_OVERFLOW = 1,
    LONGINT_INVALID_SUFFIX_CHAR = 2,
        
    LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW = (LONGINT_INVALID_SUFFIX_CHAR
                                                 | LONGINT_OVERFLOW),
    LONGINT_INVALID = 4
  };                    
typedef enum strtol_error strtol_error;


#endif
