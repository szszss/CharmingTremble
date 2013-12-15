#pragma once
#define WINDOW_TITLE "Charming Tremble"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FRAME 1000/60

typedef struct implWorld World; //游戏世界
typedef struct implLinkedList LinkedList; //链表
typedef struct implLinkedListIterator LinkedListIterator; //链表遍历器
typedef struct impl_LinkedListNode _LinkedListNode; //链表节点
typedef struct implEntityPrototype EntityPrototype; //实体原型
typedef struct implEntity Entity; //实体
typedef struct implEntityBlock EntityBlock; //砖块实体
typedef struct implEntityPlayer EntityPlayer; //玩家实体

enum WorldType {
	TYPE_UNKNOWN=-1,
	TYPE_NORMAL=1,
	TYPE_REPLAY=10,
	TYPE_DEMO=100};

enum WorldDifficulty {
	DIFF_UNKNOWN=-1,
	DIFF_PEACEFUL=0,
	DIFF_EASY=1,
	DIFF_NORMAL=2,
	DIFF_HARD=3,
	DIFF_LUNATIC=4,
	DIFF_EXTRA=5,
	DIFF_PHANTOM=6};

void GameCrash(char* cause);
void GameExit();

/*
　　　　　　　　　　_,○? 
　　　　　　　○?　　　　 ,　'?　〈?.） 
　　　　　　／,?_）　　／　　　　｀メ. 
　　　　 ／　　└-‐'?　　　　　 [__,? 
　　 　./＼＿i＞-‐‐?'?￣｀Yヽ?_　 | 
　　　/_ｒ'"?Y_＞-‐'"?￣￣｀ヽ./｀ヽ! 
　　ｒ'?7-‐'"? 　 i　　　?_＿i　 ＼__,? 
　 〈Y?　　 i　__ ,? 　 / _」__,?　　| ｀ヽ〉　　　 
　　`!　/　?? _」_ |／ '?i,? ｀?　 /　　 !／! 
　　 |　! 　 |??i,? !　 　　'?__?|／ 　　/　/ 　? 、 
　　 ',　＼,?.` '?.?　 .　　　 ?? .／?_,.!　 /　,?　　 ,| 
　　　＼ | ヽ!?　 　 -‐ '　 ,.?　 〈 　 　|／レへ,! 　/ ! 
　　　 　｀ヽ,|｀＞?‐---r?' /`??へ） .／ 　 　 　 !／　| 
　　　　　　 ?へ!ノレアiレ'::::/　?、 /　　 　　 　 |?/ヽ| 
　　　　 ＿_ ｀＼ 　_/ ,|.!:::::?|＿　∨　　 　 　　,?　　 | 
　　　　 ＼/￣ /?￣`?t‐?'?　 ｀　i 　　　 　　/　　　/ 
　　　　　　｀ヽ〈　　　 | |　|　　 　 人/ 　 　／　　 ／ 
　　　　 ＜⌒７`ー?'i |￣｀i-r‐＜ヽ.__,,..イ‐--‐＜ 
　　　　　　＼! .／　//::::::::| |　　　,ハ　 　　　 　く＼ 
　　　　　　　rく/　く〈:?::?::|/　 　　　 ト?　　 　＜　 ｀＞ 
　　　　　　 /＼??_ `ー-‐'　　/ 　　 ?'〉＞?,_＿,.＞'? 
　　　　　∠..＿,?./こ＞-＜こン‐??!ノ　 　 
　　　　　　　　　 ?-‐/?￣｀'ト?‐?'? 
　　　　　　　 　　!__/　　 　 ?-‐/ 
　　　　　　　　　　　　　　　 !__/

				   Grace and Mercy
*/