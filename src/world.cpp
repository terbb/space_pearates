#include "world.hpp"

World::World()
{
}

World::~World()
{
}

void World::init(vec2 screen)
{
  hud = HUD(screen.x, screen.y);
  vector<shared_ptr<Entity>> entities = entityManager.getEntities();
	projection = glm::ortho(0.0f, static_cast<GLfloat>(screen.x), static_cast<GLfloat>(screen.y), 0.0f, -1.0f, 1.0f);
	entityManager = EntityManager();

	Entity r = ResourceFactory::build(vec2(screen.x / 2, 50), vec2(16, 16));
	entityManager.addEntity(r);

	Entity p = PlayerFactory::build(vec2(screen.x / 2, 10.0f), vec2(50.0f, 65.0f));
	entityManager.addEntity(p);

	Entity mapDataEntity = MapEntityFactory::createMapEntityFromFile(map_path("map0.txt"));
	entityManager.addEntity(mapDataEntity);
	tileMapSystem.loadTileMap(entityManager);

  EnemySpawnFactory esf;
  Entity es = esf.build(2.0);
  entityManager.addEntity(es);

	physicsSystem.setScreenInfo(screen);
}

void World::update(float dt)
{
  enemySpawnSystem.spawnEnemy(entityManager);
  enemySpawnSystem.reduceElapsedTime(entityManager, dt);

	enemySystem.getMap(entityManager);
	enemySystem.move(dt, entityManager);

  vector<shared_ptr<Entity>> entities = entityManager.getEntities();
  playerSystem.interpInput(entityManager, dt, keys, keysProcessed);
  physicsSystem.moveEntities(entityManager, dt);
	spriteSystem.updateElapsedTime(dt);

  // Towers
  towerAttackSystem.checkRangeAndShootAimProjectiles(entityManager);
  towerAttackSystem.reduceElapsedTimeToNextFire(entityManager, dt);

  // OffScreen garbage check
  projectileGarbageSystem.destroyOffScreenEntities(entityManager);
	resourceSystem.handleResourceSpawnAndDespawn(entityManager, dt);
}

void World::processInput(float dt)
{

}

void World::draw()
{
  spriteSystem.drawSprites(entityManager, projection);
  billboardSystem.drawBillboards(entityManager, projection);
  towerRangeDisplaySystem.drawRanges(entityManager, projection);
  hud.draw();
}

void World::destroy()
{

}
