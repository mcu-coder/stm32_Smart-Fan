#ifndef __BEEP_H
#define	__BEEP_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  1
#define OFF 0

#define LED PBout(0) 
#define BEEP PBout(1)

void BEEP_GPIO_Config(void);

#endif /* __LED_H */
