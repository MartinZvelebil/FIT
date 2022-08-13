#ifndef BOMB_H
#define BOMB_H
#include <memory>
#include <vector>

#include "Position.h"

/**
 * @class CBomb
 * @brief Class is responsible for bomb atributes, bomb handling and creating explosion (not handling the map changes)
 */
class CBomb
{
public:
    /**
     * @brief Contructor for new bomb object
     * @param position Bomb position on the map
     * @param radius How many blocks wide the explosion should be
     * @param owner Who placed the bomb
     */
    CBomb(const CPosition & position, int radius, char owner);

    /**
    * Default CBomb destructor
    */
    ~CBomb() = default;

    /**
     * @brief Creating unique pointer on bomb, so I can keep pointers in vector in MapContainer
     * @return unique pointer on object
     */
    std::unique_ptr<CBomb> CreatePtr() const;

    /**
     * @return Position of bomb
     */
    CPosition CheckPosition() const;

    /**
     * @return Char of bomb (B allways at the moment)
     */
    char CheckChar() const;

    /**
     * @return Owner character (E/P)
     */
    char CheckOwner() const;

    /**
     * @return True, if bomb should explode else only sub one tick
     */
    bool MakeTick();

    //Pure map data only for checking the flame validity (on the borders
    /**
     * @brief Generating the flame effect of bomb (handling walls etc.)
     * @param m_pureMapData MapData for validating walls
     * @return Vector of positions, where destruction should happen
     */
    std::vector<CPosition> FlameEffect(std::vector<std::vector<char>> & m_pureMapData);
private:
    CPosition m_position;
    int m_radius;
    char m_owner;
    char m_representingChar; //Is allways B
    int m_ticks; //Is allways starting on 8, slowly ticking out
};
#endif //BOMB_H
