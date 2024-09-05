#include "CVector.h"

CVector::CVector(double x, double y)
{
    m_x = x;
    m_y = y;
}

CVector & CVector::Add(const CVector & a)
{
    m_x += a.m_x;
    m_y += a.m_y;
    return *this;
}

void CVector::SetX(double x)
{
    m_x = x;
}
void CVector::SetY(double y)
{
    m_y = y;
}

double CVector::GetX() const
{
    return m_x;
}
double CVector::GetY() const
{
    return m_y;
}
