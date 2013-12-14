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
	unsigned long long score; //尽管玩到40亿分有些不太可能,但还是多多益善吧!别在乎那4byte的内存了.
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