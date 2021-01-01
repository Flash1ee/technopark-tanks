#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <string>

#include "messages.hpp"
#include "menu.h"
#include "game.h"
#include "game_session.hpp"
#include "statistics.h"
#include "end_event.h"
#include "text_event.h"

#define SINGLE false
#define MULTY true

GameSession::GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer,
                         std::string server_ip, int server_port)
    :
      m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), window_title, sf::Style::Fullscreen),
      m_is_multiplayer(is_multiplayer) {
    m_level.LoadFromFile(map_path);
    MapObject player = m_level.GetFirstObject("player1");
    

    sf::FloatRect p_pos = player.rect;
    m_player_pos = {p_pos.left + p_pos.width / 2, p_pos.top - p_pos.width / 2};
    finish.openFromFile(MORTAL_PATH);
}

GameSession::~GameSession() {};

void GameSession::WaitForOtherPlayers() {
    sf::Packet packet;

    while (true) {
        if (m_game_client.RecieveFromServer(packet)) {
            GameActionType game_status;
            packet >> game_status;

            if (game_status == GameActionType::GameBegin) {
                std::cout << "ALL USERS ARE CONNECTED. GAME WILL START NOW!"
                          << std::endl;
                return;
            }
        }
    }
}
int GameSession::single_play(GameSession &game, sf::IntRect pl_rect) {
    bool was_count = false;
    bool first = true;
    Statistic stats(m_window);
    Sound sounds;
    sounds.play(GAME_START);

    std::vector<MapObject> walls_objs = m_level.GetAllObjects(WALL);
    for (auto &i : m_level.GetAllObjects(WALL_PLAYER_BASE)) {
        walls_objs.push_back(i);
    }
    for (auto &i : m_level.GetAllObjects(WALL_ENEMY_BASE)) {
        walls_objs.push_back(i);
    }

    bool wictory = false;

    std::vector<MapObject> brick_objs = m_level.GetAllObjects(BRICKS);
    std::vector<MapObject> player_obj = m_level.GetAllObjects(MAIN_PLAYER_BASE);
    std::vector<MapObject> enemy_obj = m_level.GetAllObjects(MAIN_ENEMY_BASE);
    std::vector<MapObject> grass_obj = m_level.GetAllObjects(GRASS);


    sf::IntRect grass_sprite = {};
    DestructibleWalls walls;


    for (auto i: grass_obj) {
        sf::Vector2f grass_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Object> grass = std::make_shared<Object>(OBJECT_IMAGE,
        sf::IntRect(272, 32, 16, 16), grass_pos, 0, Direction::UP);
        walls.grass.push_back(grass);
    }

    for (auto i: walls_objs) {
        sf::Vector2f wall_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Wall> wall = std::make_shared<Wall>(
        m_level, OBJECT_IMAGE, sf::IntRect(256, 16, 16, 16), wall_pos, 0,
        HP, Direction::UP, i.name);
        walls.walls.push_back(wall);
    }

    for (auto i: brick_objs) {
        sf::Vector2f brick_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Brick> brick = std::make_shared<Brick>(
        m_level, OBJECT_IMAGE, sf::IntRect(256, 0, 16, 16), brick_pos, 0,
        HP, Direction::UP);
        walls.bricks.push_back(brick);
    }

    for (auto i: player_obj) {
        sf::Vector2f base_player_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<BasePlayer> basePlayer = std::make_shared<BasePlayer>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_player_pos, 0,
                HP, Direction::UP);
        walls.base_player.push_back(basePlayer);
    }
    for (auto i: enemy_obj) {
        sf::Vector2f base_enemy_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<BaseEnemy> baseEnemy = std::make_shared<BaseEnemy>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_enemy_pos, 0,
                HP, Direction::UP);
        walls.base_enemy.push_back(baseEnemy);
    }

    std::shared_ptr<Player> this_player = std::make_shared<Player>(
        m_level, OBJECT_IMAGE, pl_rect, m_player_pos, 0.05,
        HP, Direction::UP);
    std::cout << "PLAYER POS IS" << m_player_pos.x << " " << m_player_pos.y << std::endl;


    std::vector<Bots*> all_bots;
    std::vector<MapObject> spawn;
    spawn.push_back(m_level.GetFirstObject(SPAWN_ONE));
    spawn.push_back(m_level.GetFirstObject(SPAWN_TWO));

    MapObject boss = m_level.GetFirstObject(BOSS_NAME);
    sf::FloatRect boss_pos = boss.rect;
    sf::Vector2f boss_position = {boss_pos.left + boss_pos.width / 2, boss_pos.top - boss_pos.width / 2};

    std::vector<BotBoss*> botBoss;


    sf::Vector2f old_pos = this_player->getPos();
    sf::Vector2f new_pos = old_pos;

    std::map<int, std::shared_ptr<Player>> players;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    std::vector<std::shared_ptr<Bullet>> bots_bullets;

    sf::Clock clock, boss_timer_shoots;
    sf::Clock main_timer, timer_bots;
    sf::Clock time_player_visability;

    bool is_new_user = true;

    size_t count_bots = 0;


    int stop = 0;
    int time_before = clock.getElapsedTime().asSeconds();
    timer_bots.restart();

    EndEvent wasted(G_LOSE);
    EndEvent win(G_VICTORY);

    TextEvent bots_count(BOT_KILL, this_player->getCount());
    TextEvent destroy(DESTROY, this_player->getCount());
    TextEvent boss_text(BOSS_APPEAR, this_player->getCount());

    sf::Time last_pl_bull;
    sf::Time pause_time;
    sf::Time start_pause_time;
    pause_time.Zero;
    last_pl_bull.Zero;

    while (m_window.isOpen()) {
        sf::Time times = timer_bots.getElapsedTime();

        sf::Time boss_timer = boss_timer_shoots.getElapsedTime();

        sf::Time timer_visible = time_player_visability.getElapsedTime();


        float time = clock.getElapsedTime().asMicroseconds();

        clock.restart();
        time /= TIME_AS_FPS;


        if (times.asSeconds() > SPAWN_BOT_TIME && sounds.MainSoundStopped() && count_bots < 1 && !wictory) {
            for (int i = 0; i < 2; i++) {
                size_t ind = 0;
                if (i % 2) {
                    ind = 1;
                }
                sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
                                          spawn[ind].rect.top - spawn[ind].rect.width / 2};

                all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.05,
                                            HP, Direction::UP));
                stop += 1;
            }
            timer_bots.restart();

        }

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (finish.getStatus() == sf::SoundSource::Playing) {
                        finish.pause();
                    }
                    start_pause_time = main_timer.getElapsedTime();

                    Menu gameMenu(1, m_window);
                    m_window.setView(m_window.getDefaultView());  //"оживляем" камеру в окне sfml
                    if (gameMenu.show(m_window) == STOP_RUN) {
                        return STOP_RUN;
                    }
                    pause_time += main_timer.getElapsedTime() - start_pause_time;
                    clock.restart();
                    if (finish.getStatus() == sf::SoundSource::Paused) {
                        finish.play();
                    }
                }
                if (event.key.code == sf::Keyboard::Space
                    && (main_timer.getElapsedTime().asSeconds() - last_pl_bull.asSeconds() > 0.2)) {
                    this_player->setShot(true);
                    last_pl_bull = main_timer.getElapsedTime();
                }
            }

            if (this_player->getShot() && this_player->getHp() > 0) {
                this_player->setShot(false);
                auto bullet_dir = this_player->getDir();
                sf::Vector2f bullet_pos;
                if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                    bullet_pos.x = this_player->getPos().x + 4.5;
                    bullet_pos.y = this_player->getPos().y + 4.5;
                } else {
                    bullet_pos.x = this_player->getPos().x + 3.5;
                    bullet_pos.y = this_player->getPos().y + 3.5;
                }
                bool is_bot = false;
                auto new_b = std::make_shared<Bullet>(m_level,
                                                      OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                      bullet_pos, 0.3,
                                                      bullet_dir, 1, is_bot, -1);

               
                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                sounds.play(FIRE);
            }


        }
        if ((this_player->getCount() < LEFT_BOTS_TO_SPAWN_BOT) && (botBoss.size() == 0) && !wictory) {
            botBoss.push_back(new BotBoss(m_level, OBJECT_IMAGE, sf::IntRect(178, 129, 13, 13), boss_position, 0.03,
                                          HP * 2, Direction::UP));
            sounds.stop_all();
            this_player->stop_visability();
            sounds.play(BOSS);
            boss_text.SetTimer(main_timer.getElapsedTime());
        }

        for (int i = 0; i < all_bots.size(); i ++) {
            if (all_bots[i]->getShot() && (main_timer.getElapsedTime().asSeconds() - all_bots[i]->GetShootTime() > 1)) {
                all_bots[i]->setShot(false);
                all_bots[i]->SetShootTime(main_timer.getElapsedTime().asSeconds());

                auto bullet_dir = all_bots[i]->getDir();
                sf::Vector2f bullet_pos;

                if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                    bullet_pos.x = all_bots[i]->getPos().x + 4.5;
                    bullet_pos.y = all_bots[i]->getPos().y + 4.5;
                } else {
                    bullet_pos.x = all_bots[i]->getPos().x + 3.5;
                    bullet_pos.y = all_bots[i]->getPos().y + 3.5;
                }

                auto is_bot = true;
                auto new_b = std::make_shared<Bullet>(m_level,
                                                      OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                      bullet_pos, 0.3,
                                                      bullet_dir, 1, is_bot, i);
                bots_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                sounds.play(FIRE);
            }
        }
        for (auto &i : botBoss) {
            if (boss_timer.asSeconds() > 0.5) {

                if (i->getShot()) {
                    i->setShot(false);
                    auto bullet_dir = i->getDir();
                    sf::Vector2f bullet_pos;
                    if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                        bullet_pos.x = i->getPos().x + 4.5;
                        bullet_pos.y = i->getPos().y + 4.5;
                    } else {
                        bullet_pos.x = i->getPos().x + 3.5;
                        bullet_pos.y = i->getPos().y + 3.5;
                    }
                    auto is_bot = true;
                    for (int it = 0; it < 4; it++) {
                        auto new_b = std::make_shared<Bullet>(m_level,
                                                              OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                              bullet_pos, 0.3,
                                                              static_cast<Direction>(it), 1, is_bot, 5);
                        bots_bullets.push_back(new_b);  // Copying is too expensive
                        new_bullets.push_back(new_b);
                    }

                    sounds.play(FIRE);
                }
                boss_timer_shoots.restart();
            }
        }


        for (auto &i : all_bots) {
            i->move(time, *this_player, all_bots, &walls, all_bullets, botBoss);
        }
        for (auto &i : botBoss) {
            i->move(time, *this_player, all_bots, &walls, all_bullets, botBoss);
        }

        if (this_player->getHp() > 0) {
            if (!this_player->makeAction(time, &walls)) {
                sounds.play(BACKGROUND);
            }
        }
        this_player->checkCollisionsBots(all_bots, botBoss);
        old_pos = new_pos;
        new_pos = this_player->getPos();

        {  // Drawing is here
           
            for (auto& curr_bullet : all_bullets) {
                curr_bullet->move(time, *this_player, all_bots, &walls, botBoss);
            }

            if (timer_visible.asSeconds() > 5 && !(this_player->get_visability())) {
                m_cam.view.zoom(2);
                this_player->set_visability(true);
            }
            for (auto& curr_bullet : bots_bullets) {
                size_t pre_hp = this_player->getHp();
                curr_bullet->moveBots(time, *this_player, &walls, botBoss, all_bots);

                  if (this_player->getHp() != pre_hp) {
                    auto probability = rand() % 100;
                    if (probability < PROBABILITY_VIEW && this_player->get_visability()) {
                        m_cam.view.zoom(0.5);
                        this_player->set_visability(false);
                        this_player->play_visability();
                        time_player_visability.restart();
                    }
                }   
            }

            m_cam.changeViewCoords(new_pos, m_level.GetTilemapWidth(), m_level.GetTilemapHeight());

            m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
            m_window.clear();
            m_level.Draw(m_window);

            for (int i = 0; i < all_bullets.size(); i++) {
                if (all_bullets[i]->getLife()) {
                    m_window.draw(all_bullets[i]->getSprite());
                } else {
                    all_bullets.erase(all_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }
            for (int i = 0; i < bots_bullets.size(); i++) {
                
                if (bots_bullets[i]->getLife()) {
                    m_window.draw(bots_bullets[i]->getSprite());
                } else {
                    bots_bullets.erase(bots_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }
            if (all_bots.size() != count_bots) {
                sounds.play(SPAWN);
                count_bots = all_bots.size();
            }
            if (this_player->getHp() > 0) {
                m_window.draw(this_player->getSprite());
            }
            for (int i = 0; i < botBoss.size(); i ++) {
                if (botBoss[i]->getHp() > 0) {
                    m_window.draw(botBoss[i]->getSprite());
                }
                if (botBoss[i]->getHp() <= 0) {
                    botBoss.erase(botBoss.begin() + i);
                }
            }
            for (int i = 0; i < walls.walls.size(); i++) {
                if (walls.walls[i]->getHp() > 0) {
                        m_window.draw(walls.walls[i]->getSprite());
                }
                if (walls.walls[i]->getHp() <= 0) {
                    walls.walls.erase(walls.walls.begin() + i);
                }
            }
            for (int i = 0; i < walls.bricks.size(); i++) {
                if (walls.bricks[i]->getHp() > 0) {
                        m_window.draw(walls.bricks[i]->getSprite());
                }
                if (walls.bricks[i]->getHp() <= 0) {
                    walls.bricks.erase(walls.bricks.begin() + i);
                }
            }
            for (int i = 0; i < walls.base_player.size(); i++) {
                if (walls.base_player[i]->getHp() >= 0) {
                    m_window.draw(walls.base_player[i]->getSprite());
                }
                if (walls.base_player[i]->getHp() <= 0) {
                    this_player->setHp(0);
                }
            }
            for (int i = 0; i < walls.base_enemy.size(); i++) {
                if (walls.base_enemy[i]->getHp() >= 0) {
                    m_window.draw(walls.base_enemy[i]->getSprite());
                }
            }
            for (int i = 0; i < all_bots.size(); i++) {
                if (all_bots[i]->getHp() > 0) {
                    m_window.draw(all_bots[i]->getSprite());
                }
                if (all_bots[i]->getHp() == 0) {
                    if (first) {
                        first = false;
                        sounds.play(BLOOD);
                    }
                    this_player->setCount(this_player->getCount() - 1);
                    all_bots.erase(all_bots.begin() + i);
                    if (!first) {
                        sounds.play(KILL);
                    }
                    count_bots--;
                }
            }
            for (int i = 0; i < walls.grass.size(); i++) {
                        m_window.draw(walls.grass[i]->getSprite());
            }

            if (this_player->getHp() <= 0) {
                sounds.stop_all();
                finish.stop();
                if (wasted.GetTimer() == sf::Time::Zero) {
                    sounds.MainSoundStopped();
                    wasted.SetTimer(main_timer.getElapsedTime());
                    stats.update(m_window, this_player->getHp(), 
                        walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
                }
                if (wasted.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
                    return STOP_RUN;
                }
            } else if (walls.base_enemy[0]->getHp() > 0) {
                stats.update(m_window, this_player->getHp(), 
                        walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
            } else {
                stats.update(m_window, this_player->getHp(), 
                        walls, 0);
            }

            stats.draw(m_window);
            bots_count.update(m_window, main_timer.getElapsedTime());
            boss_text.update(m_window, main_timer.getElapsedTime());

            if (this_player->getCount() <= 0) {
                if (!was_count) {
                    sounds.stop_all();
                    finish.play();
                    sounds.play(FINISH);
                    destroy.SetTimer(main_timer.getElapsedTime());
                }
                destroy.update(m_window, main_timer.getElapsedTime());
                was_count = true;
            }


            if (walls.base_enemy[0]->getHp() <= 0) {
                if (win.GetTimer() == sf::Time::Zero) {
                    finish.pause();
                    sounds.stop_all();
                    win.SetTimer(main_timer.getElapsedTime());
                }
                if (win.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
                    return STOP_RUN;
                }
                for (int i = 0; i < all_bots.size(); i++) {
                    all_bots[i]->setHp(0);
                }
                for (int i = 0; i < botBoss.size(); i++) {
                    botBoss[i]->setHp(0);
                }
                wictory = true;
            }
            m_window.display();
        }
    }
    return 0;
}
int GameSession::multi_play(GameSession &game, sf::IntRect pl_rect) {
    bool was_count = false;
    bool first = true;
    auto player_pos_raw = m_level.GetFirstObject("player1");
    sf::Vector2f this_player_pos = {player_pos_raw.rect.left, player_pos_raw.rect.top - player_pos_raw.rect.width};

    if(game.m_game_client.connectToServer())
    {
        puts("HERE GOING");
        sf::Packet packet;
        PlayerAction new_player_msg;
        
        while (m_game_client.m_socket->receive(packet) != sf::Socket::Done) { }

        if(packet.getDataSize() > 0)
        {
            packet >> new_player_msg;
            game.m_user_id = new_player_msg.player_id;
            std::cout << "MY ID" << game.m_user_id << std::endl;
            if (m_user_id == 2) {
                player_pos_raw = m_level.GetFirstObject("player2");
                m_player_pos = {player_pos_raw.rect.left, player_pos_raw.rect.top - player_pos_raw.rect.width};
            }

            std::cout << "Client id on server = " << m_user_id << std::endl;
        }
        else
        {
            std::cout << "cannot get client id from server" << std::endl;
            exit(-1);
        }

        game.WaitForOtherPlayers();  
    }

    std::cout << "here" << std::endl;

    Statistic stats(m_window);
    Sound sounds;
    sounds.play(GAME_START);

    std::vector<MapObject> walls_objs = m_level.GetAllObjects(WALL);
    for (auto &i : m_level.GetAllObjects(WALL_PLAYER_BASE)) {
        walls_objs.push_back(i);
    }
    for (auto &i : m_level.GetAllObjects(WALL_ENEMY_BASE)) {
        walls_objs.push_back(i);
    }

    bool wictory = false;

    std::vector<MapObject> brick_objs = m_level.GetAllObjects(BRICKS);
    std::vector<MapObject> player_obj = m_level.GetAllObjects(MAIN_PLAYER_BASE);
    std::vector<MapObject> enemy_obj = m_level.GetAllObjects(MAIN_ENEMY_BASE);
    std::vector<MapObject> grass_obj = m_level.GetAllObjects(GRASS);


    DestructibleWalls walls;


    for (auto i: grass_obj) {
        sf::Vector2f grass_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Object> grass = std::make_shared<Object>(OBJECT_IMAGE,
        sf::IntRect(272, 32, 16, 16), grass_pos, 0, Direction::UP);
        walls.grass.push_back(grass);
    }

    for (auto i: walls_objs) {
        sf::Vector2f wall_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Wall> wall = std::make_shared<Wall>(
        m_level, OBJECT_IMAGE, sf::IntRect(256, 16, 16, 16), wall_pos, 0,
        HP, Direction::UP, i.name);
        walls.walls.push_back(wall);
    }
    for (auto i: brick_objs) {
        sf::Vector2f brick_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<Brick> brick = std::make_shared<Brick>(
        m_level, OBJECT_IMAGE, sf::IntRect(256, 0, 16, 16), brick_pos, 0,
        HP, Direction::UP);
        walls.bricks.push_back(brick);
    }
    for (auto i: player_obj) {
        sf::Vector2f base_player_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<BasePlayer> basePlayer = std::make_shared<BasePlayer>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_player_pos, 0,
                HP, Direction::UP);
        walls.base_player.push_back(basePlayer);
    }
    for (auto i: enemy_obj) {
        sf::Vector2f base_enemy_pos = {i.rect.left, i.rect.top - i.rect.width};
        std::shared_ptr<BaseEnemy> baseEnemy = std::make_shared<BaseEnemy>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_enemy_pos, 0,
                HP, Direction::UP);
        walls.base_enemy.push_back(baseEnemy);
    }

    std::shared_ptr<Player> this_player = std::make_shared<Player>(
        m_level, OBJECT_IMAGE, sf::IntRect(1, 2, 13, 13), m_player_pos, 0.05,
        HP, Direction::UP);

    std::vector<Bots*> all_bots;
    std::vector<MapObject> spawn;
    spawn.push_back(m_level.GetFirstObject(SPAWN_ONE));
    spawn.push_back(m_level.GetFirstObject(SPAWN_TWO));

    std::map<int, std::shared_ptr<Player>> players;
    std::map<std::vector<Bots*>, bool> shot;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    std::vector<std::shared_ptr<Bullet>> bots_bullets;
    sf::Clock clock, timer_bots, timer_shoots, main_timer;
    bool is_new_user = true;

    size_t count_bots = 0;


    int stop = 0;
    int time_before = clock.getElapsedTime().asSeconds();
    timer_bots.restart();

    EndEvent wasted(G_LOSE);
    EndEvent win(G_VICTORY);

    TextEvent bots_count(BOT_KILL, this_player->getCount());
    TextEvent destroy(DESTROY, this_player->getCount());
    TextEvent boss_text(BOSS_APPEAR, this_player->getCount());


    std::vector<BotBoss*> botBoss;


    sf::Time last_pl_bull;
    sf::Time pause_time;
    sf::Time start_pause_time;
    pause_time.Zero;
    last_pl_bull.Zero;

    while (m_window.isOpen()) {
        sf::Time times = timer_bots.getElapsedTime();
        sf::Time timer = timer_shoots.getElapsedTime();

        float time = clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах

        clock.restart();  //перезагружает время
        time /= TIME_AS_FPS;      //скорость игры


        if (times.asSeconds() > SPAWN_BOT_TIME && sounds.MainSoundStopped() && count_bots < 0 && !wictory) {
            for (int i = 0; i < 2; i++) {
                size_t ind = 0;
                if (i % 2) {
                    ind = 1;
                }
                sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
                                          spawn[ind].rect.top - spawn[ind].rect.width / 2};

                // sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
                all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.05,
                                            HP, Direction::UP));
                stop += 1;
            }
            timer_bots.restart();

        }

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    start_pause_time = main_timer.getElapsedTime(); 
                    if (finish.getStatus() == sf::SoundSource::Playing) {
                        finish.pause();
                    }

                    Menu gameMenu(1, m_window);
                    m_window.setView(m_window.getDefaultView());  //"оживляем" камеру в окне sfml
                    if (gameMenu.show(m_window) == STOP_RUN) {
                        return STOP_RUN;
                    }
                    pause_time += main_timer.getElapsedTime() - start_pause_time;
                    clock.restart();
                    if (finish.getStatus() == sf::SoundSource::Paused) {
                        finish.play();
                    }
                }
                if (event.key.code == sf::Keyboard::Space
                    && (main_timer.getElapsedTime().asSeconds() - last_pl_bull.asSeconds() > 0.2) )
                    {
                    this_player->setShot(true);
                    last_pl_bull = main_timer.getElapsedTime();
                }
            }

            if (this_player->getShot() && this_player->getHp() > 0) {
                this_player->setShot(false);
                auto bullet_dir = this_player->getDir();
                sf::Vector2f bullet_pos;
                if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                    bullet_pos.x = this_player->getPos().x + 4.5;
                    bullet_pos.y = this_player->getPos().y + 4.5;
                } else {
                    bullet_pos.x = this_player->getPos().x + 3.5;
                    bullet_pos.y = this_player->getPos().y + 3.5;
                }
                bool is_bot = false;
                auto new_b = std::make_shared<Bullet>(m_level,
                                                      OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                      bullet_pos, 0.3,
                                                      bullet_dir, 1, is_bot, m_user_id);


                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                sounds.play(FIRE);
            }



        if ((timer.asSeconds() > 1) && (timer.asSeconds() < 1.07)) {

            for (auto &i : all_bots) {
                if (i->getShot()) {
                    i->setShot(false);
                    auto bullet_dir = i->getDir();
                    sf::Vector2f bullet_pos;
                    if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                        bullet_pos.x = i->getPos().x + 4.5;
                        bullet_pos.y = i->getPos().y + 4.5;
                    } else {
                        bullet_pos.x = i->getPos().x + 3.5;
                        bullet_pos.y = i->getPos().y + 3.5;
                    }
                    auto is_bot = true;
                    auto new_b = std::make_shared<Bullet>(m_level,
                                                          OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                          bullet_pos, 0.3,
                                                          bullet_dir, 1, is_bot, m_user_id);

                    // sf::Packet packet;
                    // PlayerAction new_bullet_action = {-1, bullet_pos, bullet_dir,
                    // PlayerActionType::NewBullet};
                    // action_vector.actions.push_back(new_bullet_action);

                    bots_bullets.push_back(new_b);  // Copying is too expensive
                    new_bullets.push_back(new_b);
                    sounds.play(FIRE);
                    // sounds[static_cast<int>(SoundType::BULLET)].play();
                }
            }
            timer_shoots.restart();
        }

        for (auto &i : all_bots) {
            i->move(time, *this_player, all_bots, &walls, all_bullets, botBoss);
        }

        if (this_player->getHp() > 0) {
            if (!this_player->makeAction(time, &walls)) {
                sounds.play(BACKGROUND);
            }
        }
        this_player->checkCollisionsBots(all_bots, botBoss);
        this_player->checkCollisionsPlayers(players);

        PlayerActionVector action_vector;

        auto curr_player_pos = this_player->getPos();
        auto curr_player_dir = this_player->getDir();

        if (is_new_user) {
            if (m_user_id == 0) {
                
            }
            PlayerAction create_new_player = { m_user_id, curr_player_pos, curr_player_dir, PlayerActionType::NewPlayer};
            action_vector.push(create_new_player);
            is_new_user = false;
            std::cout << "I notified server about me" << std::endl;
        }
        else
        {
            PlayerAction action = { m_user_id, curr_player_pos, curr_player_dir, PlayerActionType::UpdatePlayer};
            action_vector.push(action);
            std::cout << "DIR SEND direction "<<(int)curr_player_dir << std::endl;

        }

        {  // Gathering info for sending to server
            if (new_bullets.size() > 0) {
                PlayerAction bullet_action;
                for(auto& curr_bullet : new_bullets)
                {
                    auto bullet_pos = curr_bullet->getPos();
                    auto bullet_dir = curr_bullet->getDir();

                    bullet_action = {m_user_id, bullet_pos, bullet_dir, PlayerActionType::NewBullet }; // bullets have no id
                    action_vector.push(bullet_action);
                    std::cout << "DIR SEND "<<(int)bullet_action.direction << std::endl;
                }

                new_bullets.clear();
            }
        }

        if (action_vector.get_size() > 0) {
            sf::Packet packet;
            packet << action_vector;
            m_game_client.SendToServer(packet);
            std::cout << "I send all my action to server" << std::endl;
        } 

         // getting info from server and applying it to current session
        PlayerActionVector others_actions;

        sf::Packet packet;

        m_game_client.RecieveFromServer(packet);

        packet >> others_actions;

        if(others_actions.get_size() > 0)
        {
            std::cout << "I recieved " << others_actions.get_size() << " actions" << std::endl;
            while(others_actions.get_size() > 0)
            {
                PlayerAction action = others_actions.pop();
                
                switch (action.msg_type)
                {
                    case PlayerActionType::NewPlayer:
                    {
                        std::cout << "New player connected to game" << std::endl;
                        int id = action.player_id;
                        Direction dir = action.direction;
                        sf::Vector2f pos = action.position;

                        auto new_palyer = std::make_shared<Player>(m_level,
                                                                    OBJECT_IMAGE,
                                                                    sf::IntRect(1, 130, 16, 16),
                                                                    pos, 0.07, 100, dir);

                        std::cout << "Other player added. Position is " << pos.x << " " << pos.y << std::endl;
                        
                        players.insert(std::make_pair(id, new_palyer));
                    }
                    break;


                    case PlayerActionType::UpdatePlayer:
                    {
                        //std::cout << "Other player should be updated" << std::endl;
                        int id = action.player_id;
                        if(id == m_user_id)
                        {
                            std::cout << "PNX" << std::endl;
                        }
                        auto new_dir = action.direction;
                        std::cout << "RECIEVED DIR IS " << static_cast<int>(new_dir) << std::endl;
                        auto new_pos = action.position;
                        std::cout << "NEW DIR RECEIVED is "<< (int)new_dir << std::endl;


                        auto player_iter = players.find(id);

                        if(player_iter == players.end())
                        {
                            std::cout << "No player with id " << id << std::endl;
                            exit(-1);
                        }
                        else
                        {
                            players[id]->setDir(new_dir);
                            players[id]->setPos(new_pos);
                        }

                        std::cout << "Other player new pos is " << new_pos.x << " " << new_pos.y << std::endl;

                    } break;

                    case PlayerActionType::NewBullet: {
                        std::cout << "Other player shooted" << std::endl;
                        sf::Vector2f pos = action.position;
                        Direction dir = action.direction;
                        std::shared_ptr<Bullet> new_b(new Bullet(m_level,
                                                    OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                    pos, 0.3, dir, 1, false, action.player_id));

                        all_bullets.push_back(new_b);

                    } break;

                    case PlayerActionType::UpdateBullet: {
                    } break;

                    case PlayerActionType::DeleteBullet: {
                    } break;

                    default:
                        break;
                }
            }
        }

            for (auto& curr_bullet : all_bullets) {
                curr_bullet->move(time, *this_player, all_bots, &walls, players);
            }
            
            for (auto& curr_bullet : bots_bullets) {
                curr_bullet->moveBots(time, *this_player, &walls, players);
            }

            m_cam.changeViewCoords(this_player->getPos(), m_level.GetTilemapWidth(), m_level.GetTilemapHeight());
            // m_cam.changeView();

            m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
            m_window.clear();
            m_level.Draw(m_window);

            for (int i = 0; i < all_bullets.size(); i++) {
                if (all_bullets[i]->getLife()) {
                    m_window.draw(all_bullets[i]->getSprite());
                } else {
                    all_bullets.erase(all_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }

            for(auto& other_player : players) {
                if (other_player.second->getHp() > 0 ) {
                    m_window.draw(other_player.second->getSprite());
                } else {
                    if (win.GetTimer() == sf::Time::Zero) {
                        finish.pause();
                        sounds.stop_all();
                        // sounds.play(WIN);
                        win.SetTimer(main_timer.getElapsedTime());
                    }
                    if (win.update(m_window, m_cam.view, main_timer.getElapsedTime()))
                        return STOP_RUN;
                    }

                }

            for (int i = 0; i < bots_bullets.size(); i++) {
                
                if (bots_bullets[i]->getLife()) {
                    m_window.draw(bots_bullets[i]->getSprite());
                } else {
                    bots_bullets.erase(bots_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }
            if (all_bots.size() != count_bots) {
                sounds.play(SPAWN);
                count_bots = all_bots.size();
            }
            if (this_player->getHp() > 0) {
                m_window.draw(this_player->getSprite());
            }
            for (int i = 0; i < walls.walls.size(); i++) {
                if (walls.walls[i]->getHp() > 0) {
                        m_window.draw(walls.walls[i]->getSprite());
                }
                if (walls.walls[i]->getHp() <= 0) {
                    walls.walls.erase(walls.walls.begin() + i);
                }
            }
            for (int i = 0; i < walls.bricks.size(); i++) {
                if (walls.bricks[i]->getHp() > 0) {
                        m_window.draw(walls.bricks[i]->getSprite());
                }
                if (walls.bricks[i]->getHp() <= 0) {
                    walls.bricks.erase(walls.bricks.begin() + i);
                }
            }
            for (int i = 0; i < walls.base_player.size(); i++) {
                if (walls.base_player[i]->getHp() >= 0) {
                    m_window.draw(walls.base_player[i]->getSprite());
                }
                if (walls.base_player[i]->getHp() <= 0) {
                    walls.base_player.erase(walls.base_player.begin() + i);
                    this_player->setHp(0);
                }
            }
            for (int i = 0; i < walls.base_enemy.size(); i++) {
                if (walls.base_enemy[i]->getHp() >= 0) {
                    m_window.draw(walls.base_enemy[i]->getSprite());
                }
            }
            for (int i = 0; i < all_bots.size(); i++) {
                if (all_bots[i]->getHp() > 0) {
                    m_window.draw(all_bots[i]->getSprite());
                }
                if (all_bots[i]->getHp() == 0) {
                    if (first) {
                        first = false;
                        sounds.play(BLOOD);
                    }
                    this_player->setCount(this_player->getCount() - 1);
                    all_bots.erase(all_bots.begin() + i);
                    if (!first) {
                        sounds.play(KILL);
                    }
                    count_bots--;
                }
            }
            for (int i = 0; i < walls.grass.size(); i++) {
                        m_window.draw(walls.grass[i]->getSprite());
            }
            if (this_player->getHp() <= 0) {
                sounds.stop_all();
                finish.stop();
                if (wasted.GetTimer() == sf::Time::Zero) {
                    sounds.MainSoundStopped();
                    wasted.SetTimer(main_timer.getElapsedTime());
                    stats.update(m_window, this_player->getHp(), 
                        walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
                }
                if (wasted.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
                    return STOP_RUN;
                }
            } else if (walls.base_enemy[0]->getHp() > 0) {
                stats.update(m_window, this_player->getHp(), 
                        walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
            } else {
                stats.update(m_window, this_player->getHp(), 
                        walls, 0);
            }
            for (auto &i : players) {
                if ((i.second->getHp() <= 0) && m_user_id == i.first) {
                    std::cout << "END" << std::endl;
                    if (win.GetTimer() == sf::Time::Zero) {
                        finish.pause();
                        sounds.stop_all();
                        win.SetTimer(main_timer.getElapsedTime());
                }
                if (win.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
                    return STOP_RUN;
                }
                    wictory = true;
                } else if (walls.base_enemy[0]->getHp() > 0) {
                    stats.update(m_window, this_player->getHp(), walls,
                                 main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
                }
            }
            stats.draw(m_window);
            if (this_player->getCount() <= 0) {
                if (!was_count) {
                    sounds.stop_all();
                    finish.play();
                    sounds.play(FINISH);
                    destroy.SetTimer(main_timer.getElapsedTime());
                }
                destroy.update(m_window, main_timer.getElapsedTime());
                was_count = true;
            }
                if (walls.base_enemy[0]->getHp() <= 0) {
                    if (win.GetTimer() == sf::Time::Zero) {
                        finish.pause();
                        sounds.stop_all();
                        win.SetTimer(main_timer.getElapsedTime());
                    }
                    if (win.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
                        return STOP_RUN;
                    }
                    for (int i = 0; i < all_bots.size(); i++) {
                        all_bots[i]->setHp(0);
                    }
                    wictory = true;
                }
            m_window.display();
        }
    }
    return 0;

}
int GameSession::Run(sf::IntRect pl_rect) {

    int rc = 0;

    switch(m_is_multiplayer) {
        case(SINGLE):
            rc = single_play(*this, pl_rect);
            break;
        case(MULTY):
            rc = multi_play(*this, pl_rect);
            break;
    }
    // if (m_is_multiplayer) {
    //     WaitForOtherPlayers();
    //     // RunOnlineGame()
    // }
    
    // bool was_count = false;
    // bool first = true;
    // Statistic stats(m_window);
    // Sound sounds;
    // sounds.play(GAME_START);

    // std::vector<MapObject> walls_objs = m_level.GetAllObjects(WALL);
    // for (auto &i : m_level.GetAllObjects(WALL_PLAYER_BASE)) {
    //     walls_objs.push_back(i);
    // }
    // for (auto &i : m_level.GetAllObjects(WALL_ENEMY_BASE)) {
    //     walls_objs.push_back(i);
    // }

    // bool wictory = false;

    // std::vector<MapObject> brick_objs = m_level.GetAllObjects(BRICKS);
    // std::vector<MapObject> player_obj = m_level.GetAllObjects(MAIN_PLAYER_BASE);
    // std::vector<MapObject> enemy_obj = m_level.GetAllObjects(MAIN_ENEMY_BASE);
    // std::vector<MapObject> grass_obj = m_level.GetAllObjects(GRASS);


    // sf::IntRect grass_sprite = {};
    // DestructibleWalls walls;


    // for (auto i: grass_obj) {
    //     sf::Vector2f grass_pos = {i.rect.left, i.rect.top - i.rect.width};
    //     std::shared_ptr<Object> grass = std::make_shared<Object>(OBJECT_IMAGE,
    //     sf::IntRect(272, 32, 16, 16), grass_pos, 0, Direction::UP);
    //     walls.grass.push_back(grass);
    // }

    // for (auto i: walls_objs) {
    //     sf::Vector2f wall_pos = {i.rect.left, i.rect.top - i.rect.width};
    //     std::shared_ptr<Wall> wall = std::make_shared<Wall>(
    //     m_level, OBJECT_IMAGE, sf::IntRect(256, 16, 16, 16), wall_pos, 0,
    //     HP, Direction::UP, i.name);
    //     walls.walls.push_back(wall);
    // }

    // for (auto i: brick_objs) {
    //     sf::Vector2f brick_pos = {i.rect.left, i.rect.top - i.rect.width};
    //     std::shared_ptr<Brick> brick = std::make_shared<Brick>(
    //     m_level, OBJECT_IMAGE, sf::IntRect(256, 0, 16, 16), brick_pos, 0,
    //     HP, Direction::UP);
    //     walls.bricks.push_back(brick);
    // }

    // for (auto i: player_obj) {
    //     sf::Vector2f base_player_pos = {i.rect.left, i.rect.top - i.rect.width};
    //     std::shared_ptr<BasePlayer> basePlayer = std::make_shared<BasePlayer>(
    //             m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_player_pos, 0,
    //             HP, Direction::UP);
    //     walls.base_player.push_back(basePlayer);
    // }
    // for (auto i: enemy_obj) {
    //     sf::Vector2f base_enemy_pos = {i.rect.left, i.rect.top - i.rect.width};
    //     std::shared_ptr<BaseEnemy> baseEnemy = std::make_shared<BaseEnemy>(
    //             m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_enemy_pos, 0,
    //             HP, Direction::UP);
    //     walls.base_enemy.push_back(baseEnemy);
    // }

    // std::shared_ptr<Player> this_player = std::make_shared<Player>(
    //     m_level, OBJECT_IMAGE, pl_rect, m_player_pos, 0.05,
    //     HP, Direction::UP);


    // std::vector<Bots*> all_bots;
    // std::vector<MapObject> spawn;
    // spawn.push_back(m_level.GetFirstObject(SPAWN_ONE));
    // spawn.push_back(m_level.GetFirstObject(SPAWN_TWO));

    // MapObject boss = m_level.GetFirstObject(BOSS_NAME);
    // sf::FloatRect boss_pos = boss.rect;
    // sf::Vector2f boss_position = {boss_pos.left + boss_pos.width / 2, boss_pos.top - boss_pos.width / 2};

    // std::vector<BotBoss*> botBoss;


    // sf::Vector2f old_pos = this_player->getPos();
    // sf::Vector2f new_pos = old_pos;

    // std::map<int, std::shared_ptr<Player>> players;

    // std::vector<std::shared_ptr<Bullet>> new_bullets;
    // std::vector<std::shared_ptr<Bullet>> all_bullets;
    // std::vector<std::shared_ptr<Bullet>> bots_bullets;

    // sf::Clock clock, boss_timer_shoots;
    // sf::Clock main_timer, timer_bots;
    // sf::Clock time_player_visability;

    // bool is_new_user = true;

    // size_t count_bots = 0;


    // int stop = 0;
    // int time_before = clock.getElapsedTime().asSeconds();
    // timer_bots.restart();

    // EndEvent wasted(G_LOSE);
    // EndEvent win(G_VICTORY);

    // TextEvent bots_count(BOT_KILL, this_player->getCount());
    // TextEvent destroy(DESTROY, this_player->getCount());
    // TextEvent boss_text(BOSS_APPEAR, this_player->getCount());

    // sf::Time last_pl_bull;
    // sf::Time pause_time;
    // sf::Time start_pause_time;
    // pause_time.Zero;
    // last_pl_bull.Zero;

    // while (m_window.isOpen()) {
    //     sf::Time times = timer_bots.getElapsedTime();

    //     sf::Time boss_timer = boss_timer_shoots.getElapsedTime();

    //     sf::Time timer_visible = time_player_visability.getElapsedTime();


    //     float time = clock.getElapsedTime().asMicroseconds();

    //     clock.restart();
    //     time /= TIME_AS_FPS;


    //     if (times.asSeconds() > SPAWN_BOT_TIME && sounds.MainSoundStopped() && count_bots < COUNT_BOTS_TO_KILL && !wictory) {
    //         for (int i = 0; i < 2; i++) {
    //             size_t ind = 0;
    //             if (i % 2) {
    //                 ind = 1;
    //             }
    //             sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
    //                                       spawn[ind].rect.top - spawn[ind].rect.width / 2};

    //             all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.05,
    //                                         HP, Direction::UP));
    //             stop += 1;
    //         }
    //         timer_bots.restart();

    //     }

    //     sf::Event event;
    //     while (m_window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             m_window.close();
    //             exit(0);
    //         }

    //         if (event.type == sf::Event::KeyReleased) {
    //             if (event.key.code == sf::Keyboard::Escape) {
    //                 if (finish.getStatus() == sf::SoundSource::Playing) {
    //                     finish.pause();
    //                 }
    //                 start_pause_time = main_timer.getElapsedTime();

    //                 Menu gameMenu(1, m_window);
    //                 m_window.setView(m_window.getDefaultView());  //"оживляем" камеру в окне sfml
    //                 if (gameMenu.show(m_window) == STOP_RUN) {
    //                     return STOP_RUN;
    //                 }
    //                 pause_time += main_timer.getElapsedTime() - start_pause_time;
    //                 clock.restart();
    //                 if (finish.getStatus() == sf::SoundSource::Paused) {
    //                     finish.play();
    //                 }
    //             }
    //             if (event.key.code == sf::Keyboard::Space
    //                 && (main_timer.getElapsedTime().asSeconds() - last_pl_bull.asSeconds() > 0.2)) {
    //                 this_player->setShot(true);
    //                 last_pl_bull = main_timer.getElapsedTime();
    //             }
    //         }

    //         if (this_player->getShot() && this_player->getHp() > 0) {
    //             this_player->setShot(false);
    //             auto bullet_dir = this_player->getDir();
    //             sf::Vector2f bullet_pos;
    //             if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
    //                 bullet_pos.x = this_player->getPos().x + 4.5;
    //                 bullet_pos.y = this_player->getPos().y + 4.5;
    //             } else {
    //                 bullet_pos.x = this_player->getPos().x + 3.5;
    //                 bullet_pos.y = this_player->getPos().y + 3.5;
    //             }
    //             bool is_bot = false;
    //             auto new_b = std::make_shared<Bullet>(m_level,
    //                                                   OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
    //                                                   bullet_pos, 0.3,
    //                                                   bullet_dir, 1, is_bot, -1);

               
    //             all_bullets.push_back(new_b);  // Copying is too expensive
    //             new_bullets.push_back(new_b);
    //             sounds.play(FIRE);
    //         }


    //     }
    //     if ((this_player->getCount() < LEFT_BOTS_TO_SPAWN_BOT) && (botBoss.size() == 0) && !wictory) {
    //         botBoss.push_back(new BotBoss(m_level, OBJECT_IMAGE, sf::IntRect(178, 129, 13, 13), boss_position, 0.03,
    //                                       HP * 2, Direction::UP));
    //         sounds.stop_all();
    //         this_player->stop_visability();
    //         sounds.play(BOSS);
    //         boss_text.SetTimer(main_timer.getElapsedTime());
    //     }

    //     for (int i = 0; i < all_bots.size(); i ++) {
    //         if (all_bots[i]->getShot() && (main_timer.getElapsedTime().asSeconds() - all_bots[i]->GetShootTime() > 1)) {
    //             all_bots[i]->setShot(false);
    //             all_bots[i]->SetShootTime(main_timer.getElapsedTime().asSeconds());

    //             auto bullet_dir = all_bots[i]->getDir();
    //             sf::Vector2f bullet_pos;

    //             if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
    //                 bullet_pos.x = all_bots[i]->getPos().x + 4.5;
    //                 bullet_pos.y = all_bots[i]->getPos().y + 4.5;
    //             } else {
    //                 bullet_pos.x = all_bots[i]->getPos().x + 3.5;
    //                 bullet_pos.y = all_bots[i]->getPos().y + 3.5;
    //             }

    //             auto is_bot = true;
    //             auto new_b = std::make_shared<Bullet>(m_level,
    //                                                   OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
    //                                                   bullet_pos, 0.3,
    //                                                   bullet_dir, 1, is_bot, i);
    //             bots_bullets.push_back(new_b);  // Copying is too expensive
    //             new_bullets.push_back(new_b);
    //             sounds.play(FIRE);
    //         }
    //     }
    //     for (auto &i : botBoss) {
    //         if (boss_timer.asSeconds() > 0.5) {

    //             if (i->getShot()) {
    //                 i->setShot(false);
    //                 auto bullet_dir = i->getDir();
    //                 sf::Vector2f bullet_pos;
    //                 if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
    //                     bullet_pos.x = i->getPos().x + 4.5;
    //                     bullet_pos.y = i->getPos().y + 4.5;
    //                 } else {
    //                     bullet_pos.x = i->getPos().x + 3.5;
    //                     bullet_pos.y = i->getPos().y + 3.5;
    //                 }
    //                 auto is_bot = true;
    //                 for (int it = 0; it < 4; it++) {
    //                     auto new_b = std::make_shared<Bullet>(m_level,
    //                                                           OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
    //                                                           bullet_pos, 0.3,
    //                                                           static_cast<Direction>(it), 1, is_bot, 5);
    //                     bots_bullets.push_back(new_b);  // Copying is too expensive
    //                     new_bullets.push_back(new_b);
    //                 }

    //                 sounds.play(FIRE);
    //             }
    //             boss_timer_shoots.restart();
    //         }
    //     }


    //     for (auto &i : all_bots) {
    //         i->move(time, *this_player, all_bots, &walls, all_bullets, botBoss);
    //     }
    //     for (auto &i : botBoss) {
    //         i->move(time, *this_player, all_bots, &walls, all_bullets, botBoss);
    //     }

    //     if (this_player->getHp() > 0) {
    //         if (!this_player->makeAction(time, &walls)) {
    //             sounds.play(BACKGROUND);
    //         }
    //     }
    //     this_player->checkCollisionsBots(all_bots, botBoss);
    //     old_pos = new_pos;
    //     new_pos = this_player->getPos();

    //     if (m_is_multiplayer) {
    //         PlayerActionVector action_vector;

    //         if (is_new_user) {
    //             sf::Packet new_player_packet;
    //             PlayerAction create_new_player = {m_game_client.m_id, new_pos,
    //                                               this_player->getDir(),
    //                                               PlayerActionType::NewPlayer};
    //             new_player_packet << create_new_player;
    //             m_game_client.SendToServer(new_player_packet);
    //             is_new_user = false;
    //             std::cout << "I notified server about me" << std::endl;
    //         }

    //         {  // Gathering info for sending to server

    //             Direction dir = this_player->getDir();

    //             if (new_pos != old_pos) {
    //                 sf::Packet packet;
    //                 PlayerAction action = {m_game_client.m_id, new_pos, dir,
    //                                        PlayerActionType::UpdatePlayer};

    //                 action_vector.actions.push_back(action);
    //                 old_pos = new_pos;
    //             }

    //             if (new_bullets.size() > 0) {
    //                 PlayerAction action;
    //                 for (auto& curr_bullet : new_bullets) {
    //                     action = {
    //                         -1, new_pos, dir,
    //                         PlayerActionType::NewBullet};  // bullets have no id
    //                     action_vector.actions.push_back(action);
    //                 }

    //                 new_bullets.clear();
    //             }
    //         }

    //         if (action_vector.actions.size() > 0) {
    //             sf::Packet packet;
    //             packet << action_vector;
    //             m_game_client.SendToServer(packet);
    //             std::cout << "I send all my action to server" << std::endl;
    //         } else
    //             std::cout << "Nothing to send right now" << std::endl;

    //         {  // getting info from server and applying it to current session
    //             PlayerActionVector others_actions;

    //             sf::Packet packet;

    //             m_game_client.RecieveFromServer(packet);

    //             if (packet.getDataSize() > 0) {
    //                 packet >> others_actions;

    //                 for (auto& action : others_actions.actions) {
    //                     switch (action.msg_type) {
    //                         case PlayerActionType::NewPlayer: {
    //                             std::cout << "New player connected to game"
    //                                       << std::endl;
    //                             int id = action.player_id;
    //                             Direction dir = action.direction;
    //                             sf::Vector2f pos = action.position;
    //                             players.insert(std::make_pair(
    //                                 id, std::make_shared<Player>(
    //                                         m_level, OBJECT_IMAGE,
    //                                         sf::IntRect(1, 2, 13, 13), pos, 0.1,
    //                                         HP, dir)));
    //                         } break;

    //                         case PlayerActionType::UpdatePlayer: {
    //                             std::cout << "Other player should be updated"
    //                                       << std::endl;
    //                             int id = action.player_id;
    //                             sf::Vector2f new_pos = action.position;
    //                             players[id]->setPos(new_pos);
    //                         } break;

    //                         case PlayerActionType::NewBullet: {
    //                             std::cout << "Other player shooted"
    //                                       << std::endl;
    //                             sf::Vector2f pos = action.position;
    //                             Direction dir = action.direction;
    //                             std::shared_ptr<Bullet> new_b(new Bullet(m_level,
    //                                 OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(321, 100, 8, 8),
    //                                 pos, 0.5, dir, 1, false, -1));
    //                             all_bullets.push_back(new_b);
    //                         } break;

    //                         case PlayerActionType::UpdateBullet: {
    //                         } break;

    //                         case PlayerActionType::DeleteBullet: {
    //                         } break;

    //                         default:
    //                             break;
    //                     }
    //                 }

    //             } else {
    //                 std::cout << "Nothing to apply right now" << std::endl;
    //             }
    //         }
    //     }

    //     {  // Drawing is here
           
    //         for (auto& curr_bullet : all_bullets) {
    //             curr_bullet->move(time, *this_player, all_bots, &walls, botBoss);
    //         }

    //         if (timer_visible.asSeconds() > 5 && !(this_player->get_visability())) {
    //             m_cam.view.zoom(2);
    //             this_player->set_visability(true);
    //         }
    //         for (auto& curr_bullet : bots_bullets) {
    //             size_t pre_hp = this_player->getHp();
    //             curr_bullet->moveBots(time, *this_player, &walls, botBoss, all_bots);

    //               if (this_player->getHp() != pre_hp) {
    //                 auto probability = rand() % 100;
    //                 if (probability < PROBABILITY_VIEW && this_player->get_visability()) {
    //                     m_cam.view.zoom(0.5);
    //                     this_player->set_visability(false);
    //                     this_player->play_visability();
    //                     time_player_visability.restart();
    //                 }
    //             }   
    //         }

    //         m_cam.changeViewCoords(new_pos, m_level.GetTilemapWidth(), m_level.GetTilemapHeight());

    //         m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
    //         m_window.clear();
    //         m_level.Draw(m_window);

    //         for (int i = 0; i < all_bullets.size(); i++) {
    //             if (all_bullets[i]->getLife()) {
    //                 m_window.draw(all_bullets[i]->getSprite());
    //             } else {
    //                 all_bullets.erase(all_bullets.begin() + i);
    //                 sounds.play(BRICK);
    //             }
    //         }
    //         for (int i = 0; i < bots_bullets.size(); i++) {
                
    //             if (bots_bullets[i]->getLife()) {
    //                 m_window.draw(bots_bullets[i]->getSprite());
    //             } else {
    //                 bots_bullets.erase(bots_bullets.begin() + i);
    //                 sounds.play(BRICK);
    //             }
    //         }
    //         if (all_bots.size() != count_bots) {
    //             sounds.play(SPAWN);
    //             count_bots = all_bots.size();
    //         }
    //         if (this_player->getHp() > 0) {
    //             m_window.draw(this_player->getSprite());
    //         }
    //         for (int i = 0; i < botBoss.size(); i ++) {
    //             if (botBoss[i]->getHp() > 0) {
    //                 m_window.draw(botBoss[i]->getSprite());
    //             }
    //             if (botBoss[i]->getHp() <= 0) {
    //                 botBoss.erase(botBoss.begin() + i);
    //             }
    //         }
    //         for (int i = 0; i < walls.walls.size(); i++) {
    //             if (walls.walls[i]->getHp() > 0) {
    //                     m_window.draw(walls.walls[i]->getSprite());
    //             }
    //             if (walls.walls[i]->getHp() <= 0) {
    //                 walls.walls.erase(walls.walls.begin() + i);
    //             }
    //         }
    //         for (int i = 0; i < walls.bricks.size(); i++) {
    //             if (walls.bricks[i]->getHp() > 0) {
    //                     m_window.draw(walls.bricks[i]->getSprite());
    //             }
    //             if (walls.bricks[i]->getHp() <= 0) {
    //                 walls.bricks.erase(walls.bricks.begin() + i);
    //             }
    //         }
    //         for (int i = 0; i < walls.base_player.size(); i++) {
    //             if (walls.base_player[i]->getHp() >= 0) {
    //                 m_window.draw(walls.base_player[i]->getSprite());
    //             }
    //             if (walls.base_player[i]->getHp() <= 0) {
    //                 this_player->setHp(0);
    //             }
    //         }
    //         for (int i = 0; i < walls.base_enemy.size(); i++) {
    //             if (walls.base_enemy[i]->getHp() >= 0) {
    //                 m_window.draw(walls.base_enemy[i]->getSprite());
    //             }
    //         }
    //         for (int i = 0; i < all_bots.size(); i++) {
    //             if (all_bots[i]->getHp() > 0) {
    //                 m_window.draw(all_bots[i]->getSprite());
    //             }
    //             if (all_bots[i]->getHp() == 0) {
    //                 if (first) {
    //                     first = false;
    //                     sounds.play(BLOOD);
    //                 }
    //                 this_player->setCount(this_player->getCount() - 1);
    //                 all_bots.erase(all_bots.begin() + i);
    //                 if (!first) {
    //                     sounds.play(KILL);
    //                 }
    //                 count_bots--;
    //             }
    //         }
    //         for (int i = 0; i < walls.grass.size(); i++) {
    //                     m_window.draw(walls.grass[i]->getSprite());
    //         }

    //         if (this_player->getHp() <= 0) {
    //             sounds.stop_all();
    //             finish.stop();
    //             if (wasted.GetTimer() == sf::Time::Zero) {
    //                 sounds.MainSoundStopped();
    //                 wasted.SetTimer(main_timer.getElapsedTime());
    //                 stats.update(m_window, this_player->getHp(), 
    //                     walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
    //             }
    //             if (wasted.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
    //                 return STOP_RUN;
    //             }
    //         } else if (walls.base_enemy[0]->getHp() > 0) {
    //             stats.update(m_window, this_player->getHp(), 
    //                     walls, main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
    //         } else {
    //             stats.update(m_window, this_player->getHp(), 
    //                     walls, 0);
    //         }

    //         stats.draw(m_window);
    //         bots_count.update(m_window, main_timer.getElapsedTime());
    //         boss_text.update(m_window, main_timer.getElapsedTime());

    //         if (this_player->getCount() <= 0) {
    //             if (!was_count) {
    //                 sounds.stop_all();
    //                 finish.play();
    //                 sounds.play(FINISH);
    //                 destroy.SetTimer(main_timer.getElapsedTime());
    //             }
    //             destroy.update(m_window, main_timer.getElapsedTime());
    //             was_count = true;
    //         }


    //         if (walls.base_enemy[0]->getHp() <= 0) {
    //             if (win.GetTimer() == sf::Time::Zero) {
    //                 finish.pause();
    //                 sounds.stop_all();
    //                 win.SetTimer(main_timer.getElapsedTime());
    //             }
    //             if (win.update(m_window, m_cam.view, main_timer.getElapsedTime())) {
    //                 return STOP_RUN;
    //             }
    //             for (int i = 0; i < all_bots.size(); i++) {
    //                 all_bots[i]->setHp(0);
    //             }
    //             for (int i = 0; i < botBoss.size(); i++) {
    //                 botBoss[i]->setHp(0);
    //             }
    //             wictory = true;
    //         }
    //         m_window.display();
    //     }
    // }
    return 0;
}