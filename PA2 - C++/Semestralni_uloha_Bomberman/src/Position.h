#ifndef POSITION_H
#define POSITION_H
#include <string>

/**
 * @class CPosition
 * @brief Class representing positions of every character (players, enemies..)
 */
class CPosition
{
public:
    /**
     * @brief New position declaration
     * @param x X axis coordinate
     * @param y Y axis coordinate
     */
    CPosition(int x, int y);

    /**
     * Constructor for tmp positions with non-valid X,Y
     */
    CPosition(void);

    /**
    * Default CPosition destructor
    */
    ~CPosition() = default;

    /**
     * @return X axis coordinate
     */
    int GetXPosition() const;

    /**
     * @return Y axis coordinate
     */
    int GetYPosition() const;

    /**
     * @brief Compares if two positions are equal
     * @param Rpos Position on the right of =
     * @return True if x and y axis are equal
     * @return False if x and y axis aren't equal
     */
    bool operator == (CPosition Rpos) const;

    /**
     * @param x How many to add/subtract from the x coordinates
     * @param y How many to add/subtract from the y coordinates
     */
    void ChangePosition( int x, int y);
private:
    int m_x;
    int m_y;
};
#endif //POSITION_H
