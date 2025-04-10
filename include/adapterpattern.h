// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_ADAPTERPATTERN_H_
#define INCLUDE_ADAPTERPATTERN_H_
//------------------------------------------------------------------------------
//
// This header provides the types and class declarations to implement an
// Adapter Design Pattern example.
//
// In this example, the client uses the RobotArmInterface to control robot arms.
// An existing robot arm class, ExistingRobotArm, implements this interface.
// However, a new robot arm's control software is developed separately in the
// NewRobotArm class, and does not conform to the
// RobotArmInterface the client uses for controlling robot arms, instead
// implementing a different set of methods.
//
// An adapter class called NewRobotArmAdapter adapts the RobotArmInterface to
// the new robot's control interface using composition, allowing the client to
// use the new robot arm in the same way it controls existing robot arms.
//
// This design pattern implementation allows existing implementations to remain
// untouched, reducing the risk of breakage and improving maintainability.
//
//------------------------------------------------------------------------------

#include <array>
#include <string>

namespace RobotAdapter
{
    //--------------------------------------------------------------------------
    // Class: RobotArmInterface
    //
    // Description:
    //    This is the common interface class used to control any Robot Arm 
    //    technology. It
    //    is meant to be inherited and overridden by concrete classes, hence it
    //    specifies pure virtual methods.
    //
    class RobotArmInterface
    {
    public:
        virtual bool up(int mm) = 0;
        virtual bool down(int mm) = 0;
        virtual bool back(int mm) = 0;
        virtual bool forth(int mm) = 0;
        virtual bool left(int mm) = 0;
        virtual bool right(int mm) = 0;
        virtual bool zero() = 0;
        virtual ~RobotArmInterface() = default;
    };

    //--------------------------------------------------------------------------
    // Class: ExistingRobotArm
    //
    // Description:
    //    This is the class used to control an existing Robot Arm technology. It
    //    inherits and overrides the common RobotArmInterface class interface.
    //
    class ExistingRobotArm: public RobotArmInterface
    {
        int _currentX;
        int _currentY;
        int _currentZ;
        void _reportMove(int mm, const std::string &axis) const;
        void _reportOutOfRange() const;
    public:
        static constexpr int MAX_MM{1000};

        // Constructor initializes random startup position
        ExistingRobotArm();

        int getCurrentX() const;
        int getCurrentY() const;
        int getCurrentZ() const;

        // Inherited Interface
        bool up(int mm) override;
        bool down(int mm) override;
        bool back(int mm) override;
        bool forth(int mm) override;
        bool left(int mm) override;
        bool right(int mm) override;
        bool zero() override;
    };

    //--------------------------------------------------------------------------
    // Class: NewRobotArm
    //
    // Description:
    //    This implements the interface to a new Robot Arm technology.
    //
    //    This class allows the client to specify 3 dimensional movement using a
    //    single method.
    //
    class NewRobotArm
    {
    public:
        // Using traditional enum here to index into Position
        enum eAxisIndices { AI_X = 0, AI_Y, AI_Z, AI_SIZE };
        using tPosition = std::array<unsigned int, AI_SIZE>;
        static constexpr unsigned int MAX_XY_MM{2000};
        static constexpr unsigned int MAX_Z_MM{1000};
        static constexpr tPosition HOME{0,0,0};
        static constexpr tPosition MAX_XYZ{MAX_XY_MM, MAX_XY_MM, MAX_Z_MM};

        // Constructor to initialize startup position
        explicit NewRobotArm(tPosition xyz = HOME);

        // Move the arm according to the XYZ position given in millimeters
        bool moveXYZ(tPosition XYZmm);

        // Return a copy of the position since it is small, and will not grow in
        // the future
        tPosition getCurrentXYZ() const;

    private:
        tPosition _currentXYZ;
    };

    //--------------------------------------------------------------------------
    // Class: NewRobotArmAdapter
    //
    // Description:
    //    This is an Adapter for the NewRobotArm class. This adapter allows 
    //    client code to use a NewRobotArm class object as ExistingRobotArm 
    //    class object. The client code does this by using the common 
    //    RobotArmInterface class, with the main program instantiating the
    //    required class as needed.
    //
    //    This class's interface inherits and overrides the RobotArmInterface 
    //    interface. It encapsulates an instance of the NewRobotArm class.
    //
    class NewRobotArmAdapter: public RobotArmInterface
    {
        // Adaptee, decouple using composition
        NewRobotArm _newRobotArm;
    public:
        bool up(int mm) override;
        bool down(int mm) override;
        bool back(int mm) override;
        bool forth(int mm) override;
        bool left(int mm) override;
        bool right(int mm) override;
        bool zero() override;
    };

} // namespace RobotAdapter

#endif // INCLUDE_ADAPTERPATTERN_H_