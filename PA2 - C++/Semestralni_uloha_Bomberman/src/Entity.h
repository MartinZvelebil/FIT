#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <memory>

#include "Position.h"
#include "Bomb.h"

/**
 * @class CEntity
 * @brief Polymorphic class representing players/ghosts
 */
class CEntity
{
public:
    /**
     * @param pos - Position of my new entity
     * @param hearts - Number of lives of new entity
     * @param charval - Char, that will be displayed on map
     */
    CEntity(CPosition pos, int hearts, char charval);

    /**
     * @brief Default virtual destructor
     */
    virtual ~CEntity() = default;

    /**
     * @brief Virtual method returning representing value of Entity
     * @return Char, which is representing the Entity
     */
    virtual char ToString() const = 0;

    /**
     * @brief When user / AI changes position, this method manage it
     * @param m_pureMapData Map without entities and bombs for checking move validity
     * @param m_bombs Changing CMapContainers m_bombs, if Player decided to place bomb
     * @param playerPosition Position of Player 1, mainly used by AIPlayer
     */
    virtual void Move(std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs, CPosition playerPosition) = 0;

    /**
     * @brief When entity is hit by bomb/enemy, sub heart
     */
    void DeleteHeart();

    /**
     * @return Unique pointer representing entity (Ghost/Player)
     */
    virtual std::unique_ptr<CEntity> CreatePtr() const = 0;


    /**
     * @return Position (X,Y Coordinates) of entity on the map
     */
    CPosition CheckPosition();

    /**
     * @return Number of entity hearts. Typically called when validating if entity is alive or not
     */
    int CheckHearts() const;
protected:
    CPosition m_position;
    int m_hearts;
    char m_char;
};

#endif //ENTITY_H
