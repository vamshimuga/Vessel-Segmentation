#ifndef LEVELSETROUTINES_H
#define LEVELSETROUTINES_H

#define max(x,y)    ((x>y) ? x : y )
#define min(x,y)    ((x<y) ? x : y )


#define NEIGHBORSIZE 1
#define LSMASK_FARAWAY       0x00
#define LSMASK_ACTIVE       0x10
#define LSMASK_MOVEUP       0x20
#define LSMASK_MOVEDOWN     0x30
#define LSMASK_SLEEP		 0x40
#define LSMASK_WAKEDUP		 0x50
#define LSMASK_OUT1NEIGHBOR	 0x60
#define LSMASK_IN1NEIGHBOR	 0x70
#define LSMASK_CHECKINGIN1NEIGHBOR	 0x80
#define LSMASK_CHECKINGOUT1NEIGHBOR	 0x90
#define LSMASK_OUT2NEIGHBOR	 0xa0
#define LSMASK_IN2NEIGHBOR	 0xb0

#define LSMASK_READOUTMASK     0xf0
#define MAXSTEP 0.8
#endif // LEVELSETROUTINES_H
