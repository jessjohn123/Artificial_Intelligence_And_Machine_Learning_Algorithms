package fs_student;

import robocode.*;

public class YourRobotName extends AdvancedRobot
{
    double m_prevEnergy = 100;
    int m_moveDirection = 1;
    int m_gunDirection = 1;
    public void run()
    {
        setTurnGunRight(99999);
    }
    public void onScannedRobot(ScannedRobotEvent e)
    {
        // Stay at right angle to the opponent
        setTurnRight(e.getBearing() + 90 - (45 * m_moveDirection));

        // If the bot has small drop in energy, its gonna fire
        double changeInEnergy = m_prevEnergy-e.getEnergy();
        if (changeInEnergy > 0 && changeInEnergy <= 3)
        {
            // Dodge!
            m_moveDirection = -m_moveDirection;
            setAhead((e.getDistance()/4+25) * m_moveDirection);
        }
        // When a bot is spotted, move around the gun and radar
        m_gunDirection = -m_gunDirection;
        setTurnGunRight(99999 * m_gunDirection + 2);

        // Fire at target
        fire ( 5 ) ;

        // Keep track of the energy level
        m_prevEnergy = e.getEnergy();
    }
}
