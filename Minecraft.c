#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

#define COLOR_BLACK 0
#define COLOR_DARKBLUE FOREGROUND_BLUE
#define COLOR_DARKGREEN FOREGROUND_GREEN
#define COLOR_DARKCYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_DARKRED FOREGROUND_RED
#define COLOR_DARKMAGENTA (FOREGROUND_RED | FOREGROUND_BLUE)
#define COLOR_DARKYELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define COLOR_DARKGRAY (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_GRAY FOREGROUND_INTENSITY
#define COLOR_BLUE (FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define COLOR_GREEN (FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define COLOR_CYAN (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_RED (FOREGROUND_INTENSITY | FOREGROUND_RED)
#define COLOR_MAGENTA (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)
#define COLOR_YELLOW (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
#define COLOR_WHITE (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BG_COLOR_DARKBLUE BACKGROUND_BLUE
#define BG_COLOR_DARKGREEN BACKGROUND_GREEN
#define BG_COLOR_DARKCYAN (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_COLOR_DARKRED BACKGROUND_RED
#define BG_COLOR_DARKMAGENTA (BACKGROUND_RED | BACKGROUND_BLUE)
#define BG_COLOR_DARKYELLOW (BACKGROUND_RED | BACKGROUND_GREEN)
#define BG_COLOR_DARKGRAY (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_COLOR_GRAY BACKGROUND_INTENSITY
#define BG_COLOR_BLUE (BACKGROUND_INTENSITY | BACKGROUND_BLUE)
#define BG_COLOR_GREEN (BACKGROUND_INTENSITY | BACKGROUND_GREEN)
#define BG_COLOR_CYAN (BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_COLOR_RED (BACKGROUND_INTENSITY | BACKGROUND_RED)
#define BG_COLOR_MAGENTA (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE)
#define BG_COLOR_YELLOW (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN)
#define BG_COLOR_WHITE (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

#define CHAR_BLOCK 0x2588				// U+2588: Full Block		██
#define CHAR_SMALLBLOCK 0x25A0			// U+25A0: Black Square		■
#define CHAR_HORIZ_LINE 0x2500			// U+2500: Box Drawings Light Horizontal
#define CHAR_VERT_LINE 0x2502			// U+2502: Box Drawings Light Vertical
#define CHAR_CORNER_TL 0x250C			// U+250C: Box Drawings Down And Right
#define CHAR_CORNER_TR 0x2510			// U+2510: Box Drawings Down And Left
#define CHAR_CORNER_BL 0x2514			// U+2514: Box Drawings Up And Right
#define CHAR_CORNER_BR 0x2518			// U+2518: Box Drawings Up And Left

#define BLOCK unsigned char				// All of the base definitions of blocks in the game.
#define BLOCK_AIR 0
#define BLOCK_DIRT 1
#define BLOCK_GRASS 2
#define BLOCK_STONE 3
#define BLOCK_LOG 4
#define BLOCK_LEAVES 5
#define BLOCK_PLANKS 6
#define BLOCK_COAL 7
#define BLOCK_IRON 8
#define BLOCK_GOLD 9
#define BLOCK_DIAMOND 10

static int BLOCK_COLORS[255] = { // ID Lookup for a WORD based on the color of a block.
	COLOR_CYAN,							// Air
	COLOR_DARKRED,						// Dirt
	COLOR_GREEN,						// Grass
	COLOR_GRAY,							// Stone
	COLOR_DARKMAGENTA,					// Log
	COLOR_DARKGREEN,					// Leaves
	COLOR_DARKYELLOW,					// Planks
	COLOR_BLACK,						// Coal
	COLOR_DARKYELLOW,					// Iron
	COLOR_YELLOW,						// Gold
	COLOR_CYAN							// Diamond
};

static int BLOCK_COLORS_BG[255] = { // Clone of BLOCK_COLORS but for backgrounds.
	BG_COLOR_CYAN,						// Air
	BG_COLOR_DARKRED,					// Dirt
	BG_COLOR_GREEN,						// Grass
	BG_COLOR_GRAY,						// Stone
	BG_COLOR_DARKMAGENTA,				// Log
	BG_COLOR_DARKGREEN,					// Leaves
	BG_COLOR_DARKYELLOW,				// Planks
	COLOR_BLACK,						// Coal
	BG_COLOR_DARKYELLOW,				// Iron
	BG_COLOR_YELLOW,					// Gold
	BG_COLOR_CYAN						// Diamond
};

static wchar_t *BLOCK_NAMES[255] = { // ID Lookup for a pointer to the names of blocks.
	L"AIR",
	L"DIRT",
	L"GRASS",
	L"STONE",
	L"LOG",
	L"LEAVES",
	L"PLANKS",
	L"COAL",
	L"IRON",
	L"GOLD",
	L"DIAMOND"
};

#define WORLD_WIDTH 500					// The total world width.
#define WORLD_HEIGHT 200				// The total world height
#define RENDER_DIST 20					// The distance in all directions blocks are rendered.
#define RENDER_BARS_WIDTH 41			// The character width of the bars.
#define RENDER_DIST_SCR (RENDER_DIST * 2 + 1) // The total screen width and height.
#define RENDER_TCHARS ((RENDER_DIST_SCR * RENDER_DIST_SCR) * 2) // The total characters rendered to the screen.
#define RENDER_INV_WIDTH 66				// The characters used to render the inventory.

#define GEN_BASE 100					// The minimum height hills can spawn at.
#define GEN_BUMP_DIST 6					// The gap between each generator height change.
#define GEN_BUMP_HEIGHT 8				// The height of the hills.
#define GEN_TREE_CHANCE 13				// The chance, 1/x, to spawn a tree per column.
#define GEN_STONE_DEPTH 8				// The distance under the dirt that stone spawns.
#define GEN_STONE_RANDOM 3				// The max random offset of GEN_STONE_DEPTH

#define PLAYER_MAX_HP 20				// The maximum health the player can have.
#define PLAYER_MAX_FOOD 20				// The maximum food the player can have.
#define PLAYER_HUNGER_TICKS 90			// The ticks between hunger going down.
#define PLAYER_HUNGER_TICKS_REGEN 20	// The ticks between hunger going down when regenerating health.
#define PLAYER_HP_TICKS 10				// The ticks between health increases by 1.
#define INVENTORY_SIZE 10				// The size of the inventory (hotbar)
#define SCREEN_NONE 0					// The base game screen.
#define SCREEN_PAUSE 1					// The pause menu.
#define SCREEN_INVENTORY 2				// The inventory menu.
#define SCREEN_DEATH 3					// The death screen.

#define DIRECTION_LEFT 0				// The direction left.
#define DIRECTION_UP 1					// The direction up.
#define DIRECTION_DOWN 2				// The direction down.
#define DIRECTION_RIGHT 3				// The direction right.
#define INPUT_MOVE_LEFT 'a'				// Input: Move Left
#define INPUT_MOVE_RIGHT 'd'			// Input: Move Right
#define INPUT_LOOK_LEFT 'j'				// Input: Look Left
#define INPUT_LOOK_UP 'i'				// Input: Look Up
#define INPUT_LOOK_DOWN 'k'				// Input: Look Down
#define INPUT_LOOK_RIGHT 'l'			// Input: Look Right
#define INPUT_JUMP 'w'					// Input: Jump
#define INPUT_BREAK 'q'					// Input: Break Block
#define INPUT_PLACE 'e'					// Input: Place Block

int playerX, playerY;					// The player's X and Y coordinates.
int health = PLAYER_MAX_HP;				// The health of the player.
int food = PLAYER_MAX_FOOD;				// The food level of the player.
int ticksInAir = 0;						// For fall damage.
int inventoryCursor = 1;				// The selected index of the cursor.
char direction = 0;						// The direction the player's facing.
char damaged = 0;						// Draws the screen with a red border for 1 tick.
char screen = SCREEN_NONE;				// The screen, prefixed by "SCREEN_"
long long tick = 1;						// Increments each step. The game time in actions.

time_t startTime;						// The time the game was started. Used as a seed.
COORD zeroZero;							// Should contain (0, 0).
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;	// The original buffer info.
WORD originalWAttribs;					// The original console attributes.
HANDLE hConsole;						// The console handle
BLOCK* map;								// The world block ID map.
BLOCK inventory[INVENTORY_SIZE];		// The inventory.
int inventoryCounts[INVENTORY_SIZE];	// The inventory counts of items.
int heightMap[WORLD_WIDTH];				// The height map of the generated world.
char intToStringBuf[2];					// The buffer to output int_to_string characters into.
CHAR_INFO renderScn[RENDER_TCHARS];		// The characters that are drawn as the main world.
CHAR_INFO renderBrs[RENDER_BARS_WIDTH]; // The characters that are drawn as the health and food bars.
CHAR_INFO renderInv[RENDER_INV_WIDTH];	// The characters that are drawn as the inventory.

// World methods
BLOCK get_block(int x, int y) {
	if (x < 0 || x > WORLD_WIDTH)
		return BLOCK_AIR;
	if (y < 0 || y > WORLD_HEIGHT)
		return BLOCK_AIR;

	int offset = (WORLD_WIDTH * y) + x;
	return map[offset];
}
int set_block(int x, int y, BLOCK b) {
	if (x < 0 || x > WORLD_WIDTH)
		return 0;
	if (y < 0 || y > WORLD_HEIGHT)
		return 0;

	int offset = (WORLD_WIDTH * y) + x;
	map[offset] = b;
	return 0;
}
int fill_vertical(int x, int y, BLOCK b, int stopY) {
	int offset = (WORLD_WIDTH * y) + x;
	for (int _ = y; _ < stopY; _++) {
		map[offset] = b;
		offset += WORLD_WIDTH;
	}
	return 0;
}

// Inventory/Player methods
int check_dead() {
	// TODO: Die
	return 0;
}
int damage_player(int amount) {
	if (amount < 1)
		return 0;

	damaged = 1;
	health -= amount;
	return 0;
}
int set_screen(char s) {
	screen = s;
}
BLOCK get_item(int index) {
	if (index < 0 || index >= INVENTORY_SIZE)
		return 0;
	return inventory[index];
}
BLOCK get_item_current() {
	int i = inventoryCursor - 1;
	if (i < 0) i = 9;
	return get_item(i);
}
int get_item_count(int index) {
	if (index < 0 || index >= INVENTORY_SIZE)
		return 0;
	return inventoryCounts[index];
}
int get_item_count_current() {
	int i = inventoryCursor - 1;
	if (i < 0) i = 9;
	return get_item_count(i);
}
int set_item_count(int index, int count) {
	if (count > 99)
		return 1;

	if (count < 1) {
		inventory[index] = BLOCK_AIR;
		inventoryCounts[index] = 0;
		return 0;
	}
	else {
		inventoryCounts[index] = count;
		return 0;
	}
}
int set_item_count_current(int count) {
	int i = inventoryCursor - 1;
	if (i < 0) i = 9;
	set_item_count(i, count);
}

int add_item(BLOCK b, int count) {
	if (b == BLOCK_AIR) return 0;

	// Try to find an existing entry.
	int index = -1;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		BLOCK test = inventory[i];
		if (test == b) {
			index = i;
			break;
		}
	}

	char newEntry = 0;
	if (index == -1) {
		for (int i = 0; i < INVENTORY_SIZE; i++) {
			BLOCK test = inventory[i];
			if (test == BLOCK_AIR) {
				index = i;
				newEntry = 1;
				break;
			}
		}
	}

	if (index == -1)
		return 0; // inv full

	if (newEntry) {
		inventory[index] = b;
		if (count < 100)
			inventoryCounts[index] = count;
		else inventoryCounts[index] = 99;
	}
	else {
		int oldCount = inventoryCounts[index];
		int newCount = oldCount + count;
		if (newCount > 99)
			newCount = 99;
		inventoryCounts[index] = newCount;
	}
	return 0;
}
int remove_item(BLOCK b) {

	int index = -1;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		BLOCK test = inventory[i];
		if (test == b) {
			index = i;
			break;
		}
	}

	if (index == -1)
		return 1;

	inventory[index] = BLOCK_AIR;
	inventoryCounts[index] = 0;
	return 0;
}

float lerp(float a, float b, float t) {
	return (1 - t) * a + t * b;
}
int get_bg_color(int fore) {
	switch (fore) {
	case 0: return 0;
	case COLOR_DARKBLUE:
		return BG_COLOR_DARKBLUE;
	case COLOR_DARKGREEN:
		return BG_COLOR_DARKGREEN;
	case COLOR_DARKCYAN:
		return BG_COLOR_DARKCYAN;
	case COLOR_DARKRED:
		return BG_COLOR_DARKRED;
	case COLOR_DARKMAGENTA:
		return BG_COLOR_DARKMAGENTA;
	case COLOR_DARKYELLOW:
		return BG_COLOR_DARKYELLOW;
	case COLOR_DARKGRAY:
		return BG_COLOR_DARKGRAY;
	case COLOR_GRAY:
		return BG_COLOR_GRAY;
	case COLOR_BLUE:
		return BG_COLOR_BLUE;
	case COLOR_GREEN:
		return BG_COLOR_GREEN;
	case COLOR_CYAN:
		return BG_COLOR_CYAN;
	case COLOR_RED:
		return BG_COLOR_RED;
	case COLOR_MAGENTA:
		return BG_COLOR_MAGENTA;
	case COLOR_YELLOW:
		return BG_COLOR_YELLOW;
	case COLOR_WHITE:
		return BG_COLOR_WHITE;

	default:
		return BG_COLOR_MAGENTA;
	}
}
int set_console_color(int fore) {
	SetConsoleTextAttribute(hConsole, (WORD)fore);
	return 0;
}
int reset_console_color() {
	SetConsoleTextAttribute(hConsole, originalWAttribs);
	return 0;
}
char int_to_char(int in) {
	switch (in) {
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default: return '?';
	}
}
int int_to_string(int in) {
	if (in < 0)
		return 1;

	int first, last;
	if (in < 10) {
		char c = int_to_char(in);
		intToStringBuf[0] = '0';
		intToStringBuf[1] = c;
		return 0;
	}
	else {
		int digit1 = in;
		int digit2 = in % 10;
		while (digit1 >= 10)
			digit1 /= 10;
		intToStringBuf[0] = int_to_char(digit1);
		intToStringBuf[1] = int_to_char(digit2);
		return 0;
	}
	return 1;
}
int finalize() {
	free(map);
	reset_console_color();
	return 0;
}
int generate_map() {

	printf("Generating terrain...\n");

	// Generate the base terrain.
	float baseHeight = 0.0f;
	float nextHeight = GEN_BASE + (float)(rand() % GEN_BUMP_HEIGHT);
	float step = 1.0f / GEN_BUMP_DIST;

	for (int x = 0; x < WORLD_WIDTH; x++) {
		int rem = x % GEN_BUMP_DIST;

		if (rem == 0) {
			baseHeight = nextHeight;
			nextHeight = GEN_BASE + (float)(rand() % GEN_BUMP_HEIGHT);
		}

		float t = rem * step;
		int height = (int)lerp(baseHeight, nextHeight, t);
		int y = WORLD_HEIGHT - height;
		int stoneDepth = GEN_STONE_DEPTH - (rand() % GEN_STONE_RANDOM);
		heightMap[x] = y;
		fill_vertical(x, y, BLOCK_DIRT, y + stoneDepth);
		fill_vertical(x, y + stoneDepth, BLOCK_STONE, WORLD_HEIGHT);
		set_block(x, y, BLOCK_GRASS);
	}

	printf("Placing trees...\n");

	// Generate trees.
	for (int x = 0; x < WORLD_WIDTH; x++) {
		int treeRand = rand() % GEN_TREE_CHANCE;
		if (treeRand == 0) {
			int y = heightMap[x] - 1;
			int th = (rand() % 3) + 2;
			for(int i = 0; i < th; i++)
				set_block(x, y--, BLOCK_LOG);
			set_block(x, y, BLOCK_LEAVES);
			set_block(x + 1, y, BLOCK_LEAVES);
			set_block(x - 1, y--, BLOCK_LEAVES);
			set_block(x, y, BLOCK_LEAVES);
			set_block(x + 1, y, BLOCK_LEAVES);
			set_block(x - 1, y--, BLOCK_LEAVES);
			set_block(x, y, BLOCK_LEAVES);
		}
	}

	printf("Finalizing...\n");

	return 0;
}
int initialize_player() {
	playerX = WORLD_WIDTH / 2;
	playerY = heightMap[playerX] - 1;
	return 0;
}
int fill_inventory_bottom() {
	CHAR_INFO inf;
	inf.Char.UnicodeChar = CHAR_HORIZ_LINE;
	inf.Attributes = COLOR_WHITE;
	for (int i = 45; i < RENDER_INV_WIDTH - 1; i++) {
		int off = i - 45;
		int cursorOffset = inventoryCursor - 1;
		if (cursorOffset < 0) cursorOffset = 9;
		int cursor = ((cursorOffset) * 2);
		if (off == cursor || off == cursor + 1) {
			inf.Char.UnicodeChar = '^';
		}
		else {
			inf.Char.UnicodeChar = CHAR_HORIZ_LINE;
		}
		renderInv[i] = inf;
	}
	return 0;
}
int initialize_rendering() {
	memset(renderInv, 0, sizeof(CHAR_INFO) * RENDER_INV_WIDTH);
	for (int i = 0; i < 22; i++) {
		CHAR_INFO inf;
		inf.Char.UnicodeChar = CHAR_HORIZ_LINE;
		inf.Attributes = COLOR_WHITE;
		renderInv[i] = inf;
	}
	fill_inventory_bottom();
	CHAR_INFO tl, tr, bl, br, side;
	tl.Char.UnicodeChar = CHAR_CORNER_TL;
	tr.Char.UnicodeChar = CHAR_CORNER_TR;
	bl.Char.UnicodeChar = CHAR_CORNER_BL;
	br.Char.UnicodeChar = CHAR_CORNER_BR;
	side.Char.UnicodeChar = CHAR_VERT_LINE;
	tl.Attributes = COLOR_WHITE;
	tr.Attributes = COLOR_WHITE;
	bl.Attributes = COLOR_WHITE;
	br.Attributes = COLOR_WHITE;
	side.Attributes = COLOR_WHITE;
	renderInv[0]  = tl;
	renderInv[21] = tr;
	renderInv[44] = bl;
	renderInv[65] = br;
	renderInv[22] = side;
	renderInv[43] = side;
	return 0;
}
int render() {
	SetConsoleCursorPosition(hConsole, zeroZero);

	int writeAt = 0;

	// Render world.
	int blockOffset;
	for (int row = 0; row < RENDER_DIST_SCR; row++) {
		blockOffset = (((playerY - RENDER_DIST) + row) * WORLD_WIDTH) + (playerX - RENDER_DIST);
		int y = (playerY - RENDER_DIST) + row;

		for (int column = 0; column < RENDER_DIST_SCR; column++) {
			int x = (playerX - RENDER_DIST) + column;
			
			if ((x == playerX) & (y == playerY)) {
				blockOffset++;
				CHAR_INFO a, b;
				switch (direction) {
				case DIRECTION_LEFT:
					a.Char.UnicodeChar = '<';
					b.Char.UnicodeChar = '<';
					break;
				case DIRECTION_UP:
					a.Char.UnicodeChar = '/';
					b.Char.UnicodeChar = '\\';
					break;
				case DIRECTION_DOWN:
					a.Char.UnicodeChar = '\\';
					b.Char.UnicodeChar = '/';
					break;
				case DIRECTION_RIGHT:
					a.Char.UnicodeChar = '>';
					b.Char.UnicodeChar = '>';
					break;
				}
				a.Attributes = COLOR_MAGENTA;
				b.Attributes = COLOR_MAGENTA;
				renderScn[writeAt++] = a;
				renderScn[writeAt++] = b;
			}
			else {
				// Draw block.
				BLOCK draw = map[blockOffset++];
				WORD color = BLOCK_COLORS[draw];
				CHAR_INFO character;
				character.Char.UnicodeChar = CHAR_BLOCK;
				character.Attributes = color;
				renderScn[writeAt++] = character;
				renderScn[writeAt++] = character;
			}
		}
	}

	// Draw red border if hurt.
	if (damaged == 1) {
		damaged = 0;
		CHAR_INFO red;
		red.Char.UnicodeChar = CHAR_BLOCK;
		red.Attributes = COLOR_DARKRED;

		for (int border = 0; border < (RENDER_DIST_SCR * 2); border++)
			renderScn[border] = red;
		for (int row = 1; row < RENDER_DIST_SCR; row++) {
			int posA = RENDER_DIST_SCR * row * 2;
			int posB = (RENDER_DIST_SCR * (row + 1) * 2) - 1;
			renderScn[posA] = red;
			renderScn[posA + 1] = red;
			renderScn[posB] = red;
			renderScn[posB - 1] = red;
		}
		for (int border = RENDER_TCHARS - (RENDER_DIST_SCR * 2); border < RENDER_TCHARS; border++) {
			renderScn[border] = red;
		}
	}

	// Actually draw to the screen.
	COORD inputRegionSize = {
		RENDER_DIST_SCR * 2,
		RENDER_DIST_SCR
	};
	COORD inputRegionCoord = { 0, 0 };
	SMALL_RECT region = {
		0, 0,
		RENDER_DIST_SCR * 2,
		RENDER_DIST_SCR
	};
	WriteConsoleOutput(hConsole, renderScn, inputRegionSize, inputRegionCoord, &region);

	// Render health/hunger bars.

	/*COORD cursor = {0, RENDER_DIST_SCR};
	SetConsoleCursorPosition(hConsole, cursor);*/

	for (int h = 0; h < PLAYER_MAX_HP; h++) {
		WORD color = (h < health) ? COLOR_RED : COLOR_DARKGRAY;
		CHAR_INFO character;
		character.Char.UnicodeChar = CHAR_SMALLBLOCK;
		character.Attributes = color;
		renderBrs[h] = character;
	}

	CHAR_INFO middle;
	middle.Char.UnicodeChar = CHAR_BLOCK;
	middle.Attributes = COLOR_BLACK;
	renderBrs[20] = middle;

	for (int f = 0; f < PLAYER_MAX_FOOD; f++) {
		WORD color = (f < food) ? COLOR_GREEN : COLOR_DARKGRAY;
		CHAR_INFO character;
		character.Char.UnicodeChar = CHAR_SMALLBLOCK;
		character.Attributes = color;
		renderBrs[21 + f] = character;
	}

	int barsRoom = (RENDER_DIST_SCR * 2) - RENDER_BARS_WIDTH;

	inputRegionSize.X = RENDER_BARS_WIDTH;
	inputRegionSize.Y = 1;
	region.Left = barsRoom >> 1;
	region.Top = RENDER_DIST_SCR;
	region.Right = region.Left + RENDER_BARS_WIDTH;
	region.Bottom = region.Top + 1;
	WriteConsoleOutput(hConsole, renderBrs, inputRegionSize, inputRegionCoord, &region);

	// Render inventory.

	int invStart = 23;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		BLOCK item = inventory[i];
		int count = inventoryCounts[i];

		if (item == BLOCK_AIR) {
			CHAR_INFO none;
			none.Attributes = COLOR_BLACK;
			none.Char.UnicodeChar = CHAR_BLOCK;
			renderInv[invStart++] = none;
			renderInv[invStart++] = none;
			continue;
		}

		if (int_to_string(count))
			continue;

		CHAR_INFO inf;
		WORD colorOfBlock = BLOCK_COLORS[item];
		WORD bgColor = get_bg_color(colorOfBlock);
		inf.Attributes = 0 | bgColor;
		inf.Char.UnicodeChar = intToStringBuf[0];
		renderInv[invStart++] = inf;
		inf.Char.UnicodeChar = intToStringBuf[1];
		renderInv[invStart++] = inf;
	}

	fill_inventory_bottom();
	barsRoom = (RENDER_DIST_SCR * 2) - (RENDER_INV_WIDTH / 3);
	inputRegionSize.X = 22;
	inputRegionSize.Y = 3;
	region.Top = region.Top + 1;
	region.Bottom = region.Top + 3;
	region.Left = barsRoom >> 1;
	region.Right = region.Left + 22;
	WriteConsoleOutput(hConsole, renderInv, inputRegionSize, inputRegionCoord, &region);

	return 0;
}
int moveh(int deltaX) {
	int newX = playerX + deltaX;
	int newY = playerY;

	BLOCK atPos = get_block(newX, newY);
	BLOCK atPosB;
	if (atPos != BLOCK_AIR) {
		// Check above for stepping.
		atPos = get_block(newX, --newY);
		atPosB = get_block(playerX, newY);
		if (atPos != BLOCK_AIR || atPosB != BLOCK_AIR)
			return 0;
	}

	playerX = newX;
	playerY = newY;
	return 0;
}
int jump() {
	int newY = playerY - 1;

	// Check not obstructed.
	BLOCK atPos = get_block(playerX, newY);
	if (atPos != BLOCK_AIR)
		return 0;
	
	// Check standing on ground.
	atPos = get_block(playerX, playerY + 1);
	if (atPos == BLOCK_AIR)
		return 0;

	playerY = newY - 1; // compensate for gravity
	return 0;
}
int break_block() {
	int newX = playerX;
	int newY = playerY;

	switch (direction) {
	case DIRECTION_LEFT:
		newX--;
		break;
	case DIRECTION_RIGHT:
		newX++;
		break;
	case DIRECTION_UP:
		newY--;
		break;
	case DIRECTION_DOWN:
		newY++;
		break;
	}

	BLOCK block = get_block(newX, newY);
	if (block == BLOCK_AIR)
		return 0;

	// Break the block and add to inventory. TODO
	add_item(block, 1);
	set_block(newX, newY, BLOCK_AIR);

	return 0;
}
int place_block() {
	int newX = playerX;
	int newY = playerY;

	switch (direction) {
	case DIRECTION_LEFT:
		newX--;
		break;
	case DIRECTION_RIGHT:
		newX++;
		break;
	case DIRECTION_UP:
		newY--;
		break;
	case DIRECTION_DOWN:
		newY++;
		break;
	}

	BLOCK block = get_block(newX, newY);
	if (block != BLOCK_AIR)
		return 0;

	// Place the selected block. TODO
	BLOCK place = get_item_current();
	if (place == BLOCK_AIR)
		return 0;
	int count = get_item_count_current();
	set_block(newX, newY, place);
	set_item_count_current(--count);

	return 0;
}
int check_number_keys(unsigned char key) {
	switch (key) {
	case '0':
		inventoryCursor = 0;
		return 0;
	case '1':
		inventoryCursor = 1;
		return 0;
	case '2':
		inventoryCursor = 2;
		return 0;
	case '3':
		inventoryCursor = 3;
		return 0;
	case '4':
		inventoryCursor = 4;
		return 0;
	case '5':
		inventoryCursor = 5;
		return 0;
	case '6':
		inventoryCursor = 6;
		return 0;
	case '7':
		inventoryCursor = 7;
		return 0;
	case '8':
		inventoryCursor = 8;
		return 0;
	case '9':
		inventoryCursor = 9;
		return 0;
	default: return 0;
	}
}
int input() {
	unsigned char type = _getch();

	if (type == INPUT_MOVE_LEFT)
		moveh(-1);
	else if (type == INPUT_MOVE_RIGHT)
		moveh(1);
	else if (type == INPUT_JUMP)
		jump();
	
	if (type == INPUT_LOOK_LEFT)
		direction = DIRECTION_LEFT;
	if (type == INPUT_LOOK_UP)
		direction = DIRECTION_UP;
	if (type == INPUT_LOOK_DOWN)
		direction = DIRECTION_DOWN;
	if (type == INPUT_LOOK_RIGHT)
		direction = DIRECTION_RIGHT;

	if (type == INPUT_BREAK)
		break_block();
	if (type == INPUT_PLACE)
		place_block();

	check_number_keys(type);
	return 0;
}
int gravity() {

	int newY = playerY + 1;

	BLOCK block = get_block(playerX, newY);
	if (block != BLOCK_AIR)
		return 0;

	playerY = newY;
	return 0;
}
int falling() {

	BLOCK below = get_block(playerX, playerY + 1);
	if (below == BLOCK_AIR)
		ticksInAir++;
	else {
		if (ticksInAir > 4)
			damage_player(ticksInAir - 4);
		ticksInAir = 0;
	}
	return 0;
}
int status() {

	char regen = health < PLAYER_MAX_HP;
	
	if (tick % PLAYER_HP_TICKS == 0 && regen && food > 0)
		health++;

	int period = regen ? PLAYER_HUNGER_TICKS_REGEN : PLAYER_HUNGER_TICKS;

	if (tick % period == 0 && food > 0)
		food--;

	return 0;
}

int main() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
	originalWAttribs = bufferInfo.wAttributes;

	time(&startTime);
	srand((unsigned int)startTime);

	int _bSize = sizeof(BLOCK);
	int _wSize = WORLD_WIDTH * WORLD_HEIGHT;
	map = malloc(_wSize * _bSize);
	if(map != NULL)
		memset(map, BLOCK_AIR, _wSize);
	memset(inventory, BLOCK_AIR, INVENTORY_SIZE * _bSize);
	memset(inventoryCounts, 0, INVENTORY_SIZE * sizeof(int));

	zeroZero.X = 0;
	zeroZero.Y = 0;

	generate_map();
	initialize_player();
	initialize_rendering();

	// Game loop
	while (1) {
		render();
		input();
		gravity();
		falling();
		status();
		tick++;
	}

	// End of program.
	finalize();
}
