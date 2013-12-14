#include "collection.h"
#include "entity.h"

enum WorldType {TYPE_UNKNOWN=-1,
				TYPE_NORMAL=1,
				TYPE_REPLAY=10,
				TYPE_DEMO=100};
enum WorldDifficulty {DIFF_UNKNOWN=-1,
					  DIFF_PEACEFUL=0,
					  DIFF_EASY=1,
					  DIFF_NORMAL=2,
					  DIFF_HARD=3,
					  DIFF_LUNATIC=4,
					  DIFF_EXTRA=5,
					  DIFF_PHANTOM=6};

typedef struct
{
	char *playerName;
	EntityPlayer *player;
	long seed;
	unsigned long tick;
	unsigned long long score; //�����浽40�ڷ���Щ��̫����,�����Ƕ�����ư�!���ں���4byte���ڴ���.
	LinkedList *blockList;
	LinkedList *powerupList;
	LinkedList *operateStack;
	enum WorldType type;
	enum WorldDifficulty difficulty;
}World;

World* WorldNewGame(char* playerName,long seed,enum WorldType type,enum WorldDifficulty difficulty);
void WorldStart(World* world);
void WorldEnd(World* world);
void WorldUpdate(World* world);
void WorldDestory(World* world);