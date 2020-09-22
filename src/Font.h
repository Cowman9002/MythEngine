#pragma once

#include <map>

namespace myth
{
   struct FontChar
    {
        unsigned texture_id = 0;
        float width = 0, height = 0;
        float bearing_x = 0, bearing_y = 0;
        int advance = 0;
    };

    class Font
    {
    private:
        std::map<char, FontChar> m_characters;
    public:
        inline void setChar(const char& character, const FontChar& fontChar) { m_characters[character] = fontChar; };
        inline FontChar getChar(const char& character)
        {
            auto a = m_characters.find(character);
            if(a != m_characters.end())
            {
                return a->second;
            }
            return FontChar{};
        };
    };

}
