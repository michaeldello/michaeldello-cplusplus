//-----------------------------------------------------------------------------
//
// Robot Adapter Design Pattern Implementation
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <random>
#include "adapterpattern.h"

namespace RobotAdapter
{
    //---------------------------------------------------------------------------
    // Static Variables
    //---------------------------------------------------------------------------

    // Use high quality (Mersenne Twister) random number generator to 
    // simulate/seed
    // a functioning robot arm in a random state
    static std::random_device rd;  // Non-deterministic random seed
    static std::mt19937 gen(rd());

    // Create distributions per Robot Arm Implementation
    static std::uniform_int_distribution<int> 
        ExistingRobotArmDist(0, ExistingRobotArm::MAX_MM);

    static std::uniform_int_distribution<unsigned int> 
        NewRobotArmDistXY(0, NewRobotArm::MAX_XY_MM);

    static std::uniform_int_distribution<unsigned int> 
        NewRobotArmDistZ(0, NewRobotArm::MAX_Z_MM);

    //---------------------------------------------------------------------------
    // ExistingRobotArm Implementation
    //---------------------------------------------------------------------------
    ExistingRobotArm::ExistingRobotArm()
    : _currentX{ExistingRobotArmDist(gen)}, 
      _currentY{ExistingRobotArmDist(gen)}, 
      _currentZ{ExistingRobotArmDist(gen)}
    {
        std::cout << typeid(*this).name()
                << " - creating object with initial position: { "
                << _currentX << ", "
                << _currentY << ", "
                << _currentX << " }"
                << std::endl;
    }

    //---------------------------------------------------------------------------
    void ExistingRobotArm::_reportMove(int mm, const std::string &axis) const
    {
        std::cout << typeid(*this).name() 
                  << " - moving "
                  << axis
                  << " position by "
                  << mm
                  << " millimeters"
                  << std::endl;
    }

    //---------------------------------------------------------------------------
    void ExistingRobotArm::_reportOutOfRange() const
    {
        std::cout << typeid(*this).name() 
                  << " - Request to move out of range"
                  << std::endl;
    }

    //---------------------------------------------------------------------------
    int ExistingRobotArm::getCurrentX() const
    {
        std::cout << typeid(*this).name() 
                  << " - getting current X position" 
                  << std::endl;
        
        return _currentX;
    }

    //---------------------------------------------------------------------------
    int ExistingRobotArm::getCurrentY() const
    {
        std::cout << typeid(*this).name() 
                  << " - getting current Y position" 
                  << std::endl;
        
        return _currentY;
    }

    //---------------------------------------------------------------------------
    int ExistingRobotArm::getCurrentZ() const
    {
        std::cout << typeid(*this).name() 
                  << " - getting current Z position" 
                  << std::endl;
        
        return _currentZ;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::up(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "Z (up)");
        if ((_currentZ + mm) > MAX_MM)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentZ += mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::down(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "Z (down)");
        if ((_currentZ - mm) < 0)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentZ -= mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::back(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "Y (back)");
        if ((_currentY + mm) > MAX_MM)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentY += mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::forth(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "Y (forth)");
        if ((_currentY - mm) < -MAX_MM)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentY -= mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::right(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "X (right)");
        if ((_currentX + mm) > MAX_MM)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentX += mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::left(int mm)
    {
        bool returncode = true;
        _reportMove(mm, "X (left)");
        if ((_currentX - mm) < -MAX_MM)
        {
            _reportOutOfRange();
            returncode = false;
        } 
        else
        {
            _currentX -= mm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    bool ExistingRobotArm::zero()
    {
        bool returncode = true;
        if (_currentX < 0) 
            returncode = right(_currentX); 
        else 
            returncode = left(_currentX);
        if (_currentY < 0) 
            returncode = returncode && back(_currentY);
        else 
            returncode = returncode && forth(_currentY);
        returncode = returncode && down(_currentZ);
        return returncode;
    } 


    //---------------------------------------------------------------------------
    // NewRobotArm Implementation
    //---------------------------------------------------------------------------
    NewRobotArm::NewRobotArm(tPosition xyz)
    : _currentXYZ{
        NewRobotArmDistXY(gen), 
        NewRobotArmDistXY(gen), 
        NewRobotArmDistZ(gen)}
    {
        std::cout << typeid(*this).name()
                  << " - creating object with initial position: {";
        for (size_t i = 0; i < _currentXYZ.size(); ++i)
        {
            std::cout << _currentXYZ.at(i);
            if (i < _currentXYZ.size() - 1) std::cout << ", ";
        }
        std::cout << " }" << std::endl;
    }

    //---------------------------------------------------------------------------
    bool NewRobotArm::moveXYZ(tPosition XYZmm)
    {
        bool returncode = true;
        if ((XYZmm.at(AI_X) > MAX_XY_MM) || 
            (XYZmm.at(AI_Y) > MAX_XY_MM) || 
            (XYZmm.at(AI_Z) > MAX_Z_MM))
        {
            std::cout << typeid(*this).name() 
                        << " - Requested move out of range" 
                        << std::endl;
            returncode = false;
        }
        else
        {
            std::cout << typeid(*this).name() 
                    << " - Moving to position: {"
                    << XYZmm.at(AI_X)
                    << ", "
                    << XYZmm.at(AI_Y)
                    << ", "
                    << XYZmm.at(AI_Z)
                    << "}"
                    << std::endl;
            _currentXYZ = XYZmm;
        }
        return returncode;
    }

    //---------------------------------------------------------------------------
    NewRobotArm::tPosition NewRobotArm::getCurrentXYZ() const
    {
        return _currentXYZ;
    }


    //---------------------------------------------------------------------------
    // NewRobotArmAdapter Implementation
    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::up(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_Z) += mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::down(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_Z) -= mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::back(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_Y) += mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::forth(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_Y) -= mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::right(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_X) += mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::left(int mm)
    {
        auto newPosition{_newRobotArm.getCurrentXYZ()};
        newPosition.at(_newRobotArm.AI_X) -= mm;
        return _newRobotArm.moveXYZ(newPosition);
    }

    //---------------------------------------------------------------------------
    bool NewRobotArmAdapter::zero()
    {
        return _newRobotArm.moveXYZ({0,0,0});
    }

} // namespace RobotAdapter