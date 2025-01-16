#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <string_view>

#include "Random.h"

class Potion
{
public:
    enum Type
    {
        health,
        strength,
        poison,
        max_type
    };

    enum Size
    {
        small,
        medium,
        large,
        max_size
    };

private:
    Type m_type{};
    Size m_size{};

public:
    Potion(Type type, Size size)
        : m_type{type}
        , m_size{size}
    {}

    Type getType() const {return m_type;}
    Size getSize() const {return m_size;}

    static std::string_view getPotionTypeName(Type type)
    {
        static constexpr std::string_view names[]{
            "Health",
            "Strength",
            "Poison"
        };
        return names[type];
    }

    static std::string_view getPotionSizeName(Size size)
    {
        static constexpr std::string_view names[]{
            "Small",
            "Medium",
            "Large"
        };
        return names[size];
    }

    std::string getName() const
    {
        std::stringstream result{};

        result << getPotionSizeName(getSize()) << " potion of " << getPotionTypeName(getType());

        return result.str();
    }

    static Potion getRandomPotion()
    {
        return Potion{
            static_cast<Type>(Random::get(0, max_type - 1)),
            static_cast<Size>(Random::get(0, max_size - 1))
        };
    }
};

class Creature
{
protected:
    std::string m_name{};
    char m_symbol{};
    int m_health{};
    int m_damage{};
    int m_gold{};

public:
    Creature(std::string_view name, char symbol, int health, int damage, int gold)
        : m_name{name}, m_symbol{symbol}, m_health{health}, m_damage{damage}, m_gold{gold}
    {}

    const std::string& getName() const {return m_name;}
    char getSymbol() const {return m_symbol;}
    int getHealth() const {return m_health;}
    int getDamage() const {return m_damage;}
    int getGold() const {return m_gold;}

    void addGold(int gold) {m_gold += gold;}
    void reduceHealth(int health) {m_health -= health;}
    bool isDead() const {return m_health <= 0;}
};

class Player: public Creature
{
private:
    int m_level{};

public:
    Player(std::string_view name, int level=1)
        : Creature{name, '@', 10, 1, 0}
        , m_level{level}
    {}

    int getLevel() const {return m_level;}

    void levelUp()
    {
        ++m_level;
        ++m_damage;
    }

    bool hasWon() {return m_level == 20;}

    void increaseHealth(int health) {m_health += health;}
    void inscreaseDamage(int damage) {m_damage += damage;}
};

class Monster: public Creature
{
private:
    static inline Creature monsterData[] {
        Creature{"dragon", 'D', 20, 4, 100},
        Creature{"orc", 'o', 4, 2, 25},
        Creature{"slime", 's', 1, 1, 10}
    };

public:
    enum class Type
    {
        dragon,
        orc,
        slime,
        max_types
    };

    Monster(Type type)
        : Creature{monsterData[static_cast<int>(type)]}
    {}

    static Monster getRandomMonster()
    {
        int choice{Random::get(0, static_cast<int>(Type::max_types) - 1)};
        return Monster{static_cast<Type>(choice)};
    }
};

std::string getName()
{
    std::string name{};
    std::cout << "Enter your name: ";
    std::getline(std::cin >> std::ws, name);
    return name;
}

Player setupPlayer()
{
    std::string name{getName()};
    return Player{name};
}

void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValid(char command)
{
    return command == 'r' || command == 'f';
}

char getCommand()
{
    char command{};

    while (!isValid(command))
    {
        std::cout << "(R)un or (F)ight: ";
        std::cin >> command;
        ignoreLine();
    }

    return command;
}

void attackPlayer(Player& player, const Monster& monster)
{
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
    player.reduceHealth(monster.getDamage());
}

void attackMonster(Monster& monster, const Player& player)
{
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";
    monster.reduceHealth(player.getDamage());
}

void upgradePlayer(Player& player, const Monster& monster)
{
    std::cout << "You killed the " << monster.getName() << ".\n";
    player.levelUp();
    player.addGold(monster.getGold());
    std::cout << "You are now level " << player.getLevel() << ".\n";
    std::cout << "You found " << monster.getGold() << " gold.\n";
}

std::ostream& operator<<(std::ostream& out, const Potion& potion)
{
    switch(potion.getType())
    {
    case Potion::health: return (out << "Health");
    case Potion::strength: return (out << "Strength");
    case Potion::poison: return (out << "Poison");
    case Potion::max_type: return (out << "Uknown potion");
    }
    return (out << "Unknown potion");
}

void takePotion(Player& player)
{
    Potion potion{Potion::getRandomPotion()};

    switch(potion.getType())
    {
    case Potion::health:
        player.increaseHealth((potion.getSize() == Potion::large) ? 5 : 2);
        break;
    case Potion::strength:
        player.inscreaseDamage(1);
        break;
    case Potion::poison:
        player.reduceHealth(1);
        break;
    case Potion::max_type:
        break;
    }

    std::cout << "You drank a " << potion.getName() << ".\n";
}

void fightMonster(Player& player)
{
    Monster monster{Monster::getRandomMonster()};

    std::cout << "You have encountered " << monster.getName() << " (" << monster.getSymbol() << ")\n";

    while (true)
    {
        char command{getCommand()};

        if (command == 'r' || command == 'R')
        {
            if (static_cast<bool>(Random::get(0, 1)))
            {
                std::cout << "You failed to flee.\n";
                attackPlayer(player, monster);
            }
            else
            {
                std::cout << "You successfully fled.\n";
                return;
            }
        }

        else if (command == 'f' || command == 'F')
        {
            attackMonster(monster, player);

            if (monster.isDead())
            {
                upgradePlayer(player, monster);
                break;
            }

            attackPlayer(player, monster);
        }

        if (player.isDead() || player.hasWon())
            return;
    }

    if (!Random::get(0, 2))
    {
        char command{};
        while (command != 'y' && command != 'n')
        {
            std::cout << "You found a mythical potion! Do you want to drink it? [y/n]: ";
            std::cin >> command;
            ignoreLine();
        }
        if (command == 'y')
            takePotion(player);
        else
            std::cout << "Never mind...\n";
    }
}

void showLoseScreen(const Player& player)
{
    std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
    std::cout << "Too bad you can't take it with you!\n";
}

void showWinScreen(const Player& player)
{
    std::cout << "Congratulations! You reached " << player.getLevel() << " level and found " << player.getGold() << " gold.\n";
    std::cout << "You deserve every coin!\n";
}

void gameLoop()
{
    Player player{setupPlayer()};
    std::cout << "Welcome, " << player.getName() << '\n';

    while (true)
    {
        fightMonster(player);

        if (player.isDead())
        {
            showLoseScreen(player);
            break;
        }

        if (player.hasWon())
        {
            showWinScreen(player);
            break;
        }

        std::cout << '\n';
    }

    system("pause");
}
