#ifndef Tank_H  // Header guard to prevent multiple inclusions
#define Tank_H

#include <vector>

using namespace std;

Class Tank{
    private:
        int id;
        vector<int> position;

    public:
        Tank();
        Tank(int id, vector<int> position);
        #setter

        #getter
        

}

#endif