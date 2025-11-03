#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLEAR (Color){0}
#define X (512)
#define Y (512)

typedef struct {
	Texture2D pixTruTex, pixFlsTex;
	Rectangle boardInfo;
	int pixelSize;
	bool pixelBorders;
	bool board[X][Y];
} GameData;

static GameData initGameData (void) {
	constexpr int pixelSize = 4;
	GameData ret = {
		.pixelSize = pixelSize,
		.boardInfo = {
			.x = X * pixelSize / 2, .y = Y * pixelSize / 2,
			.width = X * pixelSize, .height = Y * pixelSize,
		},
	};
	Image pixelTrue = GenImageColor(ret.pixelSize, ret.pixelSize, WHITE);
	Image pixelFals = GenImageColor(ret.pixelSize, ret.pixelSize, CLEAR);
	if (ret.pixelSize > 6) {
		Rectangle border = {0, 0, ret.pixelSize, ret.pixelSize};
		ImageDrawRectangleLines(&pixelFals, border, ret.pixelSize / 12, WHITE);
		ImageDrawRectangleLines(&pixelTrue, border, ret.pixelSize / 12, BLACK);
	}
	ret.pixTruTex = LoadTextureFromImage(pixelTrue);
	ret.pixFlsTex = LoadTextureFromImage(pixelFals);
	UnloadImage(pixelTrue);
	UnloadImage(pixelFals);
	return ret;
}

static void randomizeBoard (bool board[X][Y]) {
	srand(time(NULL));
	unsigned t = rand(), shift = 0;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			board[i][j] = t >> shift & 1;
			shift++;
			if (shift >= sizeof(int) * 8) {
				shift = 0;
				t = rand();
			}
		}
	}
}

static void drawBoard (GameData dat) {
	Vector2 where = {
		.x = GetScreenWidth() / 2 - dat.boardInfo.x,
		.y = GetScreenHeight() / 2 - dat.boardInfo.y,
	};
	for (size_t i = 0; i < Y; i++) {
		where.x = GetScreenWidth() / 2 - dat.boardInfo.x;
		for (size_t j = 0; j < X; j++) {
			if (dat.board[j][i]) DrawTextureV(dat.pixTruTex, where, WHITE);
			else DrawTextureV(dat.pixFlsTex, where, WHITE);
			where.x += dat.pixelSize;
		}
		where.y += dat.pixelSize;
	}
}

static GameData updateBoard (GameData dat) {
	GameData ret = dat;
	int sumArea (int i, int j) {
		int ret = 0;
		for (int k = 0; k < 3; k++) {
			for (int l = 0; l < 3; l++) {
				int offsetX = (i + k - 1) % X;
				int offsetY = (j + l - 1) % Y;
				if (offsetX < 0) offsetX += X;
				if (offsetY < 0) offsetY += Y;
				bool center = k == 1 && l == 1;
				if (dat.board[offsetX][offsetY] && !center) ret++;
			}
		}
		return ret;
	}
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			int sum = sumArea(i, j);
			if (sum < 2) ret.board[i][j] = false;
			else if (sum > 3) ret.board[i][j] = false;
			else if (sum == 3) ret.board[i][j] = true;
		}
	}
	return ret;
}

int main (void) {
	InitWindow(1024, 1024, "gol");
	GameData dat = initGameData();
	randomizeBoard(dat.board);
	while (!WindowShouldClose()) {
		// if (IsKeyPressed(KEY_N))
		dat = updateBoard(dat);
		if (IsKeyPressed(KEY_R)) randomizeBoard(dat.board);
		BeginDrawing();
		ClearBackground(BLACK);
		drawBoard(dat);
		EndDrawing();
	}
	CloseWindow();
}
