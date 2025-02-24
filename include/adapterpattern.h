#ifndef ADAPTERPATTERN_H
#define ADAPTERPATTERN_H
//-----------------------------------------------------------------------------
//
// This header provides the types and class declarations to implement the 
// Adapter Design Pattern.
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
//-----------------------------------------------------------------------------

#include <array>
#include <string>

namespace RobotAdapter
{

    //-------------------------------------------------------------------------
    // Client's Robot Arm Required Interface
    class RobotArmInterface
    {
    public:
        // Use pure virtual methods as this is an interface for specific robot
        // implementations
        virtual bool up(int mm) = 0;
        virtual bool down(int mm) = 0;
        virtual bool back(int mm) = 0;
        virtual bool forth(int mm) = 0;
        virtual bool left(int mm) = 0;
        virtual bool right(int mm) = 0;
        virtual bool zero() = 0;
        virtual ~RobotArmInterface()=default;
    };

    //-------------------------------------------------------------------------
    // Existing Robot Arm
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

    //-------------------------------------------------------------------------
    // New Robot Arm
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
        NewRobotArm(tPosition xyz = HOME );

        // Move the arm according to the XYZ position given in millimeters
        bool moveXYZ(tPosition XYZmm);

        // Return a copy of the position since it is small, and will not grow in 
        // the future
        tPosition getCurrentXYZ() const;

    private:
        tPosition _currentXYZ;
    };

    //-------------------------------------------------------------------------
    // New Robot Arm Adapter
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

}

#endif // ADAPTERPATTERN_H