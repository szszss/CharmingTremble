#pragma once
#define WINDOW_TITLE "Charming Tremble"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FRAME 1000/60

typedef struct implWorld World; //��Ϸ����
typedef struct implLinkedList LinkedList; //����
typedef struct implLinkedListIterator LinkedListIterator; //���������
typedef struct impl_LinkedListNode _LinkedListNode; //����ڵ�
typedef struct implEntityPrototype EntityPrototype; //ʵ��ԭ��
typedef struct implEntity Entity; //ʵ��
typedef struct implEntityBlock EntityBlock; //ש��ʵ��
typedef struct implEntityPlayer EntityPlayer; //���ʵ��

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
��������������������_,��? 
����������������?�������� ,��'?����?.�� 
��������������,?_�������������������. 
�������� ��������-�\'?���������� [__,? 
���� ��./�ܣ�i��-�\�\?'?����Y�c?_�� | 
������/_��'"?Y_��-�\'"?������c./��c! 
������'?7-�\'"? �� i������?_��i�� ��__,? 
�� ��Y?���� i��__ ,? �� / _��__,?����| ��c�������� 
����`!��/��?? _��_ |�� '?i,? ��?�� /���� !��! 
���� |��! �� |??i,? !�� ����'?__?|�� ����/��/ ��? �� 
���� ',����,?.` '?.?�� .������ ?? .��?_,.!�� /��,?���� ,| 
�������� | �c!?�� �� -�\ '�� ,.?�� �� �� ��|�����,! ��/ ! 
������ ����c,|�ࣾ?�\---r?' /`??�أ� .�� �� �� �� !����| 
������������ ?��!�Υ쥢i��'::::/��?�� /���� ���� �� |?/�c| 
�������� ��_ ��� ��_/ ,|.!:::::?|�ߡ��š��� �� ����,?���� | 
�������� ��/�� /?��`?t�\?'?�� �ࡡi ������ ����/������/ 
��������������c�������� | |��|���� �� ��/ �� �������� �� 
�������� ���У�`�`?'i |����i-r�\���c.__,,..���\--�\�� 
��������������! .����//::::::::| |������,�ϡ� ������ ������ 
��������������r��/������:?::?::|/�� ������ ��?���� ������ �ࣾ 
������������ /��??_ `�`-�\'����/ ���� ?'����?,_��,.��'? 
������������..��,?./����-������\??!�Ρ� �� 
������������������ ?-�\/?����'��?�\?'? 
�������������� ����!__/���� �� ?-�\/ 
������������������������������ !__/

				   Grace and Mercy
*/