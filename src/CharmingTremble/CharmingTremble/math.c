#include "math.h"
#include "util.h"

static unsigned long power2[32];

void MathInit()
{
	int i=0,j=1;
	for(;i<32;i++)
	{
		power2[i]=j;
		j=j<<1;
	}
	LoggerInfo("Math library initialized");//数学库初始化不会失败...
}

unsigned long MathNextPower2( unsigned long v )
{
	int i=0;
	for(;i<32;i++)
	{
		if(power2[i]>=v)
		{
			return power2[i];
		}
	}
	return power2[31];
	/*return v<=2?2:
		v<=4?4:
		v<=8?8:
		v<=16?16:
		v<=32?32:
		v<=64?64:
		v<=128?128:
		v<=256?256:
		v<=512?512:
		v<=1024?1024:
		v<=2048?2048:
		v<=4096?4096:
		v<=8192?8192:
		v<=16384?16384:
		v<=32768?32768:
		v<=*/
}
