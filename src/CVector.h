#ifndef CVECTOR_H
#define CVECTOR_H

/**
*  subclass CVector
*   holds information used for moving objects
*	such as current position and vector of movement
*/

class CVector
{
    public:
        CVector(double m_x, double m_y);
        CVector & Add(const CVector & a);
        void SetX(double x);
        void SetY(double y);
        double GetX() const;
        double GetY() const;
    private:
        double m_x;
        double m_y;
};

#endif // CVECTOR_H
