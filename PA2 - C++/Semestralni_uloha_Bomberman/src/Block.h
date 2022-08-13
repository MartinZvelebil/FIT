#ifndef BLOCK_H
#define BLOCK_H


class CBlock
{
    public:
    enum TypeOfBlock {
        SPACE,
        WALL,
        DESTRUCTABLE_WALL,
        BOMB,
        GHOST,
        SCORE_BOOST,
        PLAYER,
        ENEMY_PLAYER,
        BONUS_BOMB,
        BONUS_BOMB_RANGE,
        BOMB_EFFECT
    };
    /**
     * @brief Converts type of block to printable symbol
     * @param type Type of block to convert
     * @return Char representing that type of block
     */
    static char GetBlockChar(TypeOfBlock type);
private:
    //SPACE, WALL, DESTRUCTABLE_WALL, SCORE_BOOST, BONUS_BOMB and BONUS_BOMB_RANGE are strict in map format (see map). Others can be changed here.
    constexpr static const char m_characters[11] = {' ', '#', ':', 'B', '@', '$', 'P','E', 'a', 'O', '*'};
};


#endif //BLOCK_H
