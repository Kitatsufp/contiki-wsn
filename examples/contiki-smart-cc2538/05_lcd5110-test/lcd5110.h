/**************************************************************************************************
**************************************************************************************************/



/**************************************************************************************************
 *                                          INCLUDES
 **************************************************************************************************/

/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/* These are used to specify which line the text will be printed */
#define HAL_LCD_LINE_1      0x00
#define HAL_LCD_LINE_2      0x01
/*
   This to support LCD with extended number of lines (more than 2).
   Don't use these if LCD doesn't support more than 2 lines
*/
#define HAL_LCD_LINE_3      0x02
#define HAL_LCD_LINE_4      0x03
#define HAL_LCD_LINE_5      0x04
#define HAL_LCD_LINE_6      0x05
#define HAL_LCD_LINE_7      0x06
#define HAL_LCD_LINE_8      0x07

/* Max number of chars on a single LCD line */
#define HAL_LCD_MAX_CHARS   16
#define HAL_LCD_MAX_BUFF    25

extern  void  initLcd(void);
extern  void  LCD_set_XY(unsigned char X, unsigned char Y);
extern  void  LCD_write_char( char c);
extern  void  LCD_write_byte(char data,unsigned  int command);
extern  void  LCD_clear(void);
//extern	void  LCD_write_char(unsigned  char c);
extern  void  LCD_write_english_string(unsigned  char X,unsigned char Y, char *s);

extern  void  gpio_set(int port, int bit);
extern  void  gpio_reset(int port, int bit);
