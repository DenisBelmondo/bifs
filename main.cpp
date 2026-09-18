#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdio.h>

template <typename T, typename T_SIZE, T_SIZE N>
consteval auto arraySize(const T (&array)[N]) -> T_SIZE {
	return sizeof (array) / sizeof (array[0]);
}

static inline auto directionOf(float angle) -> Vector2 {
	return { cosf(angle), -sinf(angle) };
}

static auto DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color) -> void {
	float x = position.x;
	float y = position.y;
	float z = position.z;

	// Set desired texture to be enabled while drawing following vertex data
	rlSetTexture(texture.id);

	// Vertex data transformation can be defined with the commented lines,
	// but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
	//rlPushMatrix();
	// NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
	//rlTranslatef(2.0f, 0.0f, 0.0f);
	//rlRotatef(45, 0, 1, 0);
	//rlScalef(2.0f, 2.0f, 2.0f);

	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Front Face
	rlNormal3f(0.0f, 0.0f, 1.0f); // Normal Pointing Towards Viewer
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	// Bottom Left Of The Quad
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	// Bottom Right Of The Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	// Top Right Of The Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	// Top Left Of The Quad
	// Back Face
	rlNormal3f(0.0f, 0.0f, - 1.0f);
	// Normal Pointing Away From Viewer
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	// Bottom Right Of The Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	// Top Right Of The Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	// Top Left Of The Quad
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	// Bottom Left Of The Quad
	// Top Face
	rlNormal3f(0.0f, 1.0f, 0.0f);
	// Normal Pointing Up
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	// Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	// Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	// Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	// Top Right Of The Texture and Quad
	// Bottom Face
	rlNormal3f(0.0f, - 1.0f, 0.0f);
	// Normal Pointing Down
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	// Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	// Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	// Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	// Bottom Right Of The Texture and Quad
	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);
	// Normal Pointing Right
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	// Bottom Right Of The Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	// Top Right Of The Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	// Top Left Of The Quad
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	// Bottom Left Of The Quad
	// Left Face
	rlNormal3f( - 1.0f, 0.0f, 0.0f);
	// Normal Pointing Left
	rlTexCoord2f(0.0f, height);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	// Bottom Left Of The Quad
	rlTexCoord2f(1.0f, height);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	// Bottom Right Of The Quad
	rlTexCoord2f(1.0f, 0.0f);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	// Top Right Of The Quad
	rlTexCoord2f(0.0f, 0.0f);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	// Top Left Of The Quad
	rlEnd();
	//rlPopMatrix();

	rlSetTexture(0);
}

struct TileMap {
	struct GetTileResult {
		enum {
			FAILURE,
			SUCCESS	=	1	<<	0,
			OOB			=	1	<<	1,
		} code;

		int tileValue;

		inline auto isValid() const {
			return (code & SUCCESS) != 0;
		}
	};

	int *tiles;
	int width;
	int height;

	auto tryGetTile(float x, float y) const -> GetTileResult {
		GetTileResult result {};

		const auto xx = (int)floorf(x + 0.5f);
		const auto yy = (int)floorf(y + 0.5f);
		const auto i = yy * width + xx;

		// should probably return something here that signifies oob
		if (xx < 0 || xx >= width || yy < 0 || yy >= height) {
			result.code = GetTileResult::FAILURE;
			return result;
		}

		int tile = tiles[i];

		result.tileValue = tile;
		
		if (tile != 0) {
			result.code = GetTileResult::SUCCESS;
		}

		return result;
	}
};

struct Thing {
	int kind;
	float radius;
	int spriteIndex;
	Vector2 position;
	Vector2 velocity;
	float angle;
	bool isAlive;
};

struct Game {
	TileMap tileMap;

	Vector2 playerPosition;
	float playerAngle;
	float playerElevation;

	enum {
		GROUNDED,
		JUMPING,
	} playerJumpState;

	float playerJumpT;

	int thingCount;
	Thing *things;
};

namespace thingKind {
	enum {
		INVALID,
		TEST,
		COUNT,
	};
};

namespace spriteIndex {
	enum {
		NONE,
		TEST,
		COUNT,
	};
};

auto testMove(TileMap *tileMap, Vector2 from, Vector2 relative, Vector2 *outMove) -> bool {
	Vector2 move = relative;
	Vector2 testMove = from + relative;
	int tile = 0;
	bool couldMoveAllTheWay = true;

	TileMap::GetTileResult getTileResult = tileMap->tryGetTile(from.x, testMove.y);

	if (getTileResult.isValid() || tile != 0) {
		move.y = 0.f;
		couldMoveAllTheWay = false;
	}
	
	getTileResult = tileMap->tryGetTile(testMove.x, from.y);

	if (getTileResult.isValid() || tile != 0) {
		move.x = 0.f;
		couldMoveAllTheWay = false;
	}

	*outMove = move;

	return couldMoveAllTheWay;
}

struct TestRayResult {
	Vector2 endpoint;
	Thing *thing;

	enum {
		NOTHING,
		TILE,
		THING,
	} what;
};

auto testRay(const Game *game, Vector2 from, Vector2 to) -> TestRayResult {
	static const float STEP_SIZE = 0.1f;

	TestRayResult rayResult {};

	const Vector2 delta = to - from;
	const Vector2 direction = Vector2Normalize(delta);
	const float distance = Vector2Length(delta);

	for (float t = 0; t <= distance; t += STEP_SIZE) {
		const Vector2 p = from + direction * t;
		const TileMap::GetTileResult result = game->tileMap.tryGetTile(p.x, p.y);

		if (result.isValid()) {
			rayResult.what = TestRayResult::TILE;
			rayResult.endpoint = p;
			return rayResult;
		}

		for (int i = 0; i < game->thingCount; i++) {
			Thing &thing = game->things[i];

			if (!thing.isAlive) {
				continue;
			}

			if (Vector2Distance(p, thing.position) < thing.radius) {
				rayResult.what = TestRayResult::THING;
				rayResult.endpoint = p;
				rayResult.thing = &thing;
				return rayResult;
			}
		}
	}

	return rayResult;
}

auto gameUpdate(Game *game) -> void {
	// move player

	if (IsKeyDown(KEY_SPACE)) {
		if (game->playerJumpState == Game::GROUNDED) {
			game->playerJumpState = Game::JUMPING;
		}
	}

	if (game->playerJumpState == Game::JUMPING) {
		game->playerJumpT = fmin(game->playerJumpT + 0.05f, 1);

		if (game->playerJumpT >= 1) {
			game->playerJumpState = Game::GROUNDED;
			game->playerJumpT = 0;
		}
	}

	auto turnAxis = (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT);

	game->playerAngle -= turnAxis / 8.f;

	auto forwardAxis = (float)IsKeyDown(KEY_W) - (float)IsKeyDown(KEY_S);
	auto strafeAxis = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A);

	Vector2 forwardMove = directionOf(game->playerAngle);

	forwardMove *= forwardAxis;

	auto strafeAngle = game->playerAngle - PI / 2.f;
	Vector2 strafeMove = directionOf(strafeAngle);

	strafeMove *= strafeAxis;

	Vector2 move = { forwardMove.x + strafeMove.x, forwardMove.y + strafeMove.y };

	if (Vector2LengthSqr(move)) {
		move = Vector2Normalize(move);
	}

	move /= 4.f;

	testMove(&game->tileMap, game->playerPosition, move, &move);
	game->playerPosition += move;

	// kill shit

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		TestRayResult rayResult = testRay(
			game,
			game->playerPosition,
			game->playerPosition + directionOf(game->playerAngle) * 2.f);

		if (rayResult.what == TestRayResult::THING) {
			rayResult.thing->isAlive = false;
			puts("yeowch!");
		}
	}

	// things update

	for (int i = 0; i < game->thingCount; i++) {
		Thing &thing = game->things[i];
		// bool hitSomething = testMove(&game->tileMap, thing.position, thing.velocity, &move);
		TileMap::GetTileResult result = game->tileMap.tryGetTile(thing.position.x + thing.velocity.x, thing.position.y + thing.velocity.y);

		if (result.isValid()) {
			thing.velocity = thing.velocity * -1;
			thing.velocity = Vector2Rotate(thing.velocity, GetRandomValue(0, 1) * PI / 4.f);
		}

		thing.position += thing.velocity;
	}
}

auto gameSpawnThing(Game *game, int thingKind, float whereX, float whereY) -> Thing* {
	Thing newThing {};

	newThing.kind = thingKind;
	newThing.radius = 0.5f;
	newThing.position.x = whereX;
	newThing.position.y = whereY;
	newThing.isAlive = true;
	game->things[game->thingCount] = newThing;

	return &game->things[game->thingCount++];
}

const double TICK_RATE = 1.0 / 30.0;
const int MAX_FRAME_SKIP = 16;

auto main() -> int {
	Shader baseShader;
	Shader surfaceShader;

	Game game {};

	static const int tileMap[] = {
		1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,
		1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	game.tileMap.tiles = (int *)tileMap;
	game.tileMap.width = 20;
	game.tileMap.height = 20;
	game.playerPosition = Vector2 { 2, 2 };

	static Thing thingBuffer[1024];

	game.things = thingBuffer;
	
	Thing *newThing = gameSpawnThing(&game, thingKind::TEST, 2, 2);

	newThing->spriteIndex = spriteIndex::TEST;
	newThing->velocity = Vector2 { -0.1f, 0.1f };

	Camera3D camera = {
		.position = {},
		.target = {},
		.up = Vector3 { 0, 1, 0 },
		.fovy = 75,
		.projection = CAMERA_PERSPECTIVE,
	};

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(640, 400, "Buster Impulse");
	DisableCursor();

	Model planeModel = LoadModelFromMesh(GenMeshPlane(1000, 1000, 1, 1));

	baseShader = LoadShader("static/shaders/base.vert", "static/shaders/base.frag");
	surfaceShader = LoadShader("static/shaders/surface.vert", "static/shaders/surface.frag");

	int baseShaderTexture0Loc = GetShaderLocation(baseShader, "texture0");
	int surfaceShaderTexture0Loc = GetShaderLocation(surfaceShader, "texture0");

	static const char *flatTextureFileNames[] = {
		"static/textures/flats/flat0.png",
	};

	static Texture2D flatTextures[256];

	for (size_t i = 0; i < arraySize(flatTextureFileNames); i++) {
		flatTextures[i] = LoadTexture(flatTextureFileNames[i]);
	}

	static const char *spriteTextureFileNames[] = {
		"static/textures/sprites/test.png",
	};

	static Texture2D spriteTextures[256];

	for (size_t i = 0; i < arraySize(spriteTextureFileNames); i++) {
		spriteTextures[i] = LoadTexture(spriteTextureFileNames[i]);
	}

	double oldTime = GetTime();
	double lag = 0;

	while (!WindowShouldClose()) {
		game.playerAngle -= GetMouseDelta().x / 125.f;

		double newTime = GetTime();
		double delta = newTime - oldTime;

		oldTime = newTime;
		lag = fmin(lag + delta, TICK_RATE * MAX_FRAME_SKIP);

		while (lag >= TICK_RATE) {
			gameUpdate(&game);
			lag -= TICK_RATE;
		}

		camera.position.x = game.playerPosition.x;
		camera.position.y = 0.5f + sinf(game.playerJumpT * PI);
		camera.position.z = game.playerPosition.y;

		Vector2 playDir = directionOf(game.playerAngle);

		camera.target =
			camera.position +
			Vector3 { playDir.x, 0, playDir.y };

		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D(camera);

		DrawModelEx(planeModel, {}, Vector3UnitX, 0, Vector3One(), GRAY);
		DrawModelEx(planeModel, Vector3UnitY * 2, Vector3UnitX, 180, Vector3One(), DARKGRAY);

		// walls

		BeginShaderMode(surfaceShader);

		if (game.tileMap.tiles) {
			for (auto y = 0; y < game.tileMap.height; y++) {
				for (auto x = 0; x < game.tileMap.width; x++) {
					auto value = game.tileMap.tiles[y * game.tileMap.width + x];

					if (value != 0) {
						SetShaderValueTexture(surfaceShader, surfaceShaderTexture0Loc, flatTextures[0]);
						DrawCubeTexture(flatTextures[0], { float(x), 1, float(y) }, 1, 2, 1, WHITE);
					}
				}
			}
		}

		EndShaderMode();

		// things

		for (int i = 0; i < game.thingCount; i++) {
			const Thing &thing = game.things[i];
			const Texture2D &spriteTexture = spriteTextures[thing.spriteIndex - 1];

			if (!thing.isAlive) {
				continue;
			}

			DrawBillboardRec(
				camera,
				spriteTexture,
				Rectangle {
					.x = 0,
					.y = 0,
					.width = (float)spriteTexture.width,
					.height = (float)spriteTexture.height,
				},
				Vector3 {
					thing.position.x,
					spriteTexture.height / 64.f / 2.f,
					thing.position.y,
				},
				Vector2 {
					(float)spriteTexture.width,
					(float)spriteTexture.height,
				} / 64.f,
				WHITE);
		}

		EndMode3D();
		EndDrawing();
	}

	UnloadModel(planeModel);

	for (size_t i = 0; i < arraySize(flatTextureFileNames); i++) {
		UnloadTexture(flatTextures[i]);
	}

	for (size_t i = 0; i < arraySize(spriteTextureFileNames); i++) {
		UnloadTexture(spriteTextures[i]);
	}

	UnloadShader(baseShader);
	UnloadShader(surfaceShader);

	CloseWindow();
}
