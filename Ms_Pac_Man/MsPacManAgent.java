package fs_student;

import game.controllers.PacManController;
import game.core.G;
import game.core.Game;
import java.util.ArrayList;

public class MsPacManAgent implements PacManController//, Constants
{
	private static final int MINIMUM_DISTANCE = 10;
	public int getAction(Game game, long time)
	{
		int current = game.getCurPacManLoc();

		//if any non-edible ghost is too close (less than min distance), run away
		for(int i = 0; i < Game.NUM_GHOSTS; i++)
			if(game.getEdibleTime(i) == 0 && game.getLairTime(i) == 0)
				if(game.getPathDistance(current, game.getCurGhostLoc(i)) < MINIMUM_DISTANCE)
					return game.getNextPacManDir(game.getCurGhostLoc(i),false,Game.DM.PATH);

		//find the nearest edible ghost and go after them
		int m_minDistance = Integer.MAX_VALUE;
		int m_minGhost = -1;

		for(int i = 0;i < Game.NUM_GHOSTS; i++)
			if(game.getEdibleTime(i) > 0)
			{
				int distance = game.getPathDistance(current,game.getCurGhostLoc(i));

				if(distance < m_minDistance)
				{
					m_minDistance = distance;
					m_minGhost = i;
				}
			}

		if(m_minGhost != -1)
			return game.getNextPacManDir(game.getCurGhostLoc(m_minGhost),true,Game.DM.PATH);

		//bait the Ghosts
		if(game.getNumberPowerPills() >= 1)
		{
			int m_theClosestPath = game.getPathDistance(game.getCurGhostLoc(0), current);
			int m_theClosestGhost = 0;
			for (int i = 1; i < Game.NUM_GHOSTS; i++)
			{
				if(m_theClosestPath > game.getPathDistance(game.getCurGhostLoc(i), game.getCurPacManLoc()))
				{
					m_theClosestGhost = game.getCurGhostLoc(i);
					m_theClosestPath = game.getPathDistance(game.getCurGhostLoc(i), game.getCurPacManLoc());
				}
			}

			int[] m_powerPillsLocation = game.getPowerPillIndices();
			int closetPillsPath = 60000000;
			int closestPowerPills = 0;
			for (int i = 0; i < m_powerPillsLocation.length; i++)
			{
				if(game.checkPowerPill(i) &&
						closetPillsPath > game.getPathDistance(game.getCurPacManLoc(), m_powerPillsLocation[i]))
				{
					closetPillsPath = game.getPathDistance(game.getCurPacManLoc(), m_powerPillsLocation[i]);
					closestPowerPills = m_powerPillsLocation[i];
				}
			}

			if(game.getPathDistance(game.getCurPacManLoc(), closestPowerPills) < 2 &&
					game.getPathDistance(game.getCurPacManLoc(), m_theClosestGhost) > 3 &&
					game.getPathDistance(m_theClosestGhost, closestPowerPills) -
							game.getPathDistance(game.getCurPacManLoc(), closestPowerPills) > 1)
				return game.getNextPacManDir(game.getReverse(game.getCurPacManDir()),true,Game.DM.PATH);
		}

		//go after the pills and power pills
		int[] m_pills = game.getPillIndices();
		int[] m_powerPills = game.getPowerPillIndices();

		ArrayList<Integer> target=new ArrayList<Integer>();

		for(int i = 0;i < m_pills.length; i++)
			if(game.checkPill(i))
				target.add(m_pills[i]);

		for(int i = 0; i < m_powerPills.length; i++)
			if(game.checkPowerPill(i))
				target.add(m_powerPills[i]);

		int[] targetsArr = new int[target.size()];

		for(int i = 0;i < targetsArr.length;i++)
			targetsArr[i] = target.get(i);


		return game.getNextPacManDir(game.getTarget(current, targetsArr, true, Game.DM.PATH),true, Game.DM.PATH);

	}
}