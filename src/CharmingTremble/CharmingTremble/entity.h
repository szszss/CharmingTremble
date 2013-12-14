typedef struct  
{
	EntityPrototype* prototype;
	int posX;
	int posY;
} Entity;

typedef struct  
{
	struct Entity;
	int width;
} EntityBlock;

typedef struct  
{
	struct Entity;
	int life;
} EntityPlayer;

typedef struct  
{
	void (*update)(void*);
	void (*render)(void*);
	void (*destroy)(void*);
} EntityPrototype;

int CallbackDestroyEntity(void* entity);