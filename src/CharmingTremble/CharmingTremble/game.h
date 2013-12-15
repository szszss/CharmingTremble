#pragma once
#define WINDOW_TITLE "Charming Tremble"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FRAME 1000/60

#ifdef OS_WINDOWS //let's fuck WinDef
	typedef int BOOL;
	#define TRUE 1
	#define FALSE 0
#else
	#ifndef BOOL
	#define BOOL int
	#define TRUE 1
	#define FALSE 0
	#endif
#endif

typedef struct implWorld                World;              //游戏世界
typedef struct implLinkedList           LinkedList;         //链表
typedef struct implLinkedListIterator   LinkedListIterator; //链表遍历器
typedef struct implLinkedListNode       _LinkedListNode;    //链表节点
typedef struct implHashTree             HashTree;           //哈希二叉树
typedef struct implHashTreeNode         _HashTreeNode;      //哈希二叉树节点
typedef struct implEntityPrototype      EntityPrototype;    //实体原型
typedef struct implEntity               Entity;             //实体
typedef struct implEntityBlock          EntityBlock;        //砖块实体
typedef struct implEntityPlayer         EntityPlayer;       //玩家实体
typedef struct implGuiButton            GuiButton;          //按钮
typedef struct implMTRandomGen          MTRandomGen;        //梅森旋转算法随机数发生器
typedef struct implLogger               Logger;             //日志记录器
typedef struct implStringBuilder        StringBuilder;      //字符串构造器

enum ScreenState {
	SS_MAINMENU,
	SS_GAME};

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
　　　　　　　　　　          _,○､ 
　　　　　　　○､　　　　 ,　'´　〈ﾍ.） 
　　　　　　／,ﾊ_）　　／　　　　｀メ. 
　　　　 ／　　└-‐'´　　　　　 [__,ﾊ 
　　 　./＼＿i＞-‐‐ｧ'´￣｀Yヽ､_　 | 
　　　/_ｒ'"´Y_＞-‐'"´￣￣｀ヽ./｀ヽ! 
　　ｒ'´7-‐'"´ 　 i　　　ﾊ_＿i　 ＼__,ﾊ 
　 〈Y´　　 i　__ ,ﾊ 　 / _」__,ﾊ　　| ｀ヽ〉　　　 
　　`!　/　ﾊ´ _」_ |／ '´i,ﾉ ｀ﾘ　 /　　 !／! 
　　 |　! 　 |ｨ´i,ﾉ !　 　　'､__ﾝ|／ 　　/　/ 　ﾄ 、 
　　 ',　＼,ﾊ.` '､.ﾉ　 .　　　 ⊂⊃ .／､_,.!　 /　,ﾊ　　 ,| 
　　　＼ | ヽ!⊃　 　 -‐ '　 ,.ｲ　 〈 　 　|／レへ,! 　/ ! 
　　　 　｀ヽ,|｀＞ｧ‐---rｧ' /`ﾄ､へ） .／ 　 　 　 !／　| 
　　　　　　 ﾚへ!ノレアiレ'::::/　ﾒ、 /　　 　　 　 |､/ヽ| 
　　　　 ＿_ ｀＼ 　_/ ,|.!:::::ｲ|＿　∨　　 　 　　,ﾊ　　 | 
　　　　 ＼/￣ /´￣`ｧt‐ｧ'´　 ｀　i 　　　 　　/　　　/ 
　　　　　　｀ヽ〈　　　 | |　|　　 　 人/ 　 　／　　 ／ 
　　　　 ＜⌒７`ーｧ'i |￣｀i-r‐＜ヽ.__,,..イ‐--‐＜ 
　　　　　　＼! .／　//::::::::| |　　　,ハ　 　　　 　く＼ 
　　　　　　　rく/　く〈:ﾛ::ﾛ::|/　 　　　 ト､　　 　＜　 ｀＞ 
　　　　　　 /＼ﾝ､_ `ー-‐'　　/ 　　 ﾉ'〉＞､,_＿,.＞'´ 
　　　　　∠..＿,ﾍ./こ＞-＜こン‐ｧ´!ノ　 　 
　　　　　　　　　 ﾄ-‐/´￣｀'トｰ‐ｧ'´ 
　　　　　　　 　　!__/　　 　 ﾄ-‐/ 
　　　　　　　　　　　　　　　 !__/

				   Grace and Mercy
*/