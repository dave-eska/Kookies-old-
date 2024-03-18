#include"tile.h"

namespace Tile{
    class TransitionArea: public Base{
        private:
            std::string filename;
        public:
            void attachLevel(std::string levelName);
            void changeLevel(std::vector<std::unique_ptr<Base>>& tiles);

    };
};
