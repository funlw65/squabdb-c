/*---------- headerfile for squabdb.ddl ----------*/
/* alignment is 8 */

/*---------- structures ----------*/
struct breeds {  /* size 351 */
    char    name[60];
    char    category[11];
    char    origin[60];
    char    imageid[60];
    char    description[160];
};

/*---------- record names ----------*/
#define BREEDS 1000L

/*---------- field names ----------*/
#define NAME 1001L
#define CATEGORY 1002L
#define ORIGIN 1003L
#define IMAGEID 1004L
#define DESCRIPTION 1005L

/*---------- key names ----------*/

/*---------- sequence names ----------*/

/*---------- integer constants ----------*/
