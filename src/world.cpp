#include "world.hpp"

World::World()
{
}

World::~World()
{
}

void World::init(vec2 screen)
{
  collisionSystem.setScreenInfo(screen);
  vector<shared_ptr<Entity>> entities = entityManager.getEntities();
  projection = glm::ortho(0.0f, static_cast<GLfloat>(screen.x), static_cast<GLfloat>(screen.y), 0.0f, -1.0f, 1.0f);
  entityManager = EntityManager();

  ResourceFactory::spawnMany(entityManager); // TODO: maybe remove

  vec2 player_spawn = vec2(200, -100.0f);
  Entity p = PlayerFactory::build(player_spawn);
  entityManager.addEntity(p);
  //printVec2("(world)player spawn:", player_spawn);

  Entity mapDataEntity = MapEntityFactory::createMapEntityFromFile(map_path("map0.txt"));
  entityManager.addEntity(mapDataEntity);
  tileMapSystem.loadTileMap(entityManager);

  entityManager.addEntity(EnemySpawnFactory::build(2.0));

  enemySystem.getMap(entityManager);
  physicsSystem.setScreenInfo(screen);

  // create background entity
  Entity backgroundEntity = BackgroundEntityFactory::createBackgroundEntity();
  entityManager.addEntity(backgroundEntity);

  // Generate the build tower ui entity
  vector<Entity> towerUiEntities = TowerUiEntityFactory::createTowerUiButtons();
  for (Entity towerUiEntity : towerUiEntities)
    entityManager.addEntity(towerUiEntity);
	
	Entity w = WavesetManagerFactory::build(waveset_path("waveset0.txt"));
	entityManager.addEntity(w);
}

void World::update(float dt)
{
	wavesetSystem.handleBuildAndDefensePhase(entityManager, dt);

  enemySystem.move(dt, entityManager);

  vector<shared_ptr<Entity>> entities = entityManager.getEntities();
  playerSystem.interpInput(entityManager, dt, keys, keysProcessed);
  physicsSystem.moveEntities(entityManager, dt);
	collisionSystem.checkCollisions(entityManager, wavesetSystem);
	spriteSystem.updateElapsedTime(dt);

  // Background Update
  backgroundSystem.update(entityManager);

  // Build Tower UI
  towerUiSystem.interpInput(entityManager, keys);
  towerUiSystem.update(entityManager, dt);

  // Towers
  towerAttackSystem.checkRangeAndShootProjectiles(entityManager);
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
  // Background will render first
  backgroundSystem.render(entityManager, projection);
  spriteSystem.drawSprites(entityManager, projection);
  billboardSystem.drawBillboards(entityManager, projection);
  towerRangeDisplaySystem.drawRanges(entityManager, projection);
  towerUiSystem.render(entityManager, projection);
  HUD::getInstance().draw();
}

void World::destroy()
{

}
