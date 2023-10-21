#ifndef _BLINK_MODL_H_
#define _BLINK_MODL_H

extern void blink_setup();
extern void blink_handler();
extern void blink_change( const int *seq);
extern const int blinkSeqNormal[];
extern const int blinkSeqSOS[];
extern unsigned long currentMillis;

#endif /* _BLINK_MODL_H_ */