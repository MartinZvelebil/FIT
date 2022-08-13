#ifndef GHOST_H
#define GHOST_H
#include <vector>
#include <string>

#include "Position.h"
#include "Entity.h"
//#include <MANY OTHER INCLUDES>

/**
 * @class CGhost
 * @brief Represents ghosts
 */
class CGhost : public CEntity
{
public:

    /**
     * @brief Constructor for new Ghost
     * @param pos Spawning position of ghost
     * @param hearts Number of hearts for ghost (Can be more than one)
     * @param charval Character, which will represent the ghosts
     * @param rangeOfMotion Path for ghost to walk on
     * @param RangeOfMotionIndex Where is ghost currently located in the rangeOfMotion vector
     */
    CGhost(CPosition pos, int hearts, char charval, std::vector<CPosition> rangeOfMotion, int RangeOfMotionIndex);

    /**
     * Default CGhost destructor
     */
    virtual ~CGhost() = default;

    /**
     * @brief Overriding polymorphic method
     * @brief Moving ghost every tick on his prepared path
     * @param m_pureMapData If checking of map validity would be neccessarry
     * @param m_bombs
     */
    void Move(std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs, CPosition playerPosition) override;

    /**
     * @brief Overriding polymorphic method
     * @return Unique pointer for CGhosts
     */
    std::unique_ptr<CEntity> CreatePtr() const override;

    /**
     * @brief Overriding polymorphic method
     * @return Char representing ghost
     */
    char ToString() const override;

private:
    /**
     * @brief Best chosen path for ghost to walk on
     */
    std::vector<CPosition> m_rangeOfMotion;

    /**
     * @brief On what position in pre-programmed path I'm currently at
     */
    int m_rangeOfMotionIndex;

    /**
     * @brief Is true if ghost is going RIGHT
     * @brief Is false if ghost is goind LEFT
     */
    bool m_directionR;
};

#endif //GHOST_H
