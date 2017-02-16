using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FullSailAFI.SteeringBehaviors.Core;

namespace FullSailAFI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region Constructors
        public Flock()
        {
        }
        #endregion

        #region TODO Suggested helper methods

        private void CalculateAverages()
        {
            Vector3 m_AvgPos = new Vector3(0, 0, 0);
            Vector3 m_AvgForward = new Vector3(0, 0, 0);

            for(int i = 0; i < Boids.Count; i++)
            {
                m_AvgPos += Boids[i].Position;
                m_AvgForward += Boids[i].Velocity;
            }

            m_AvgPos /= Boids.Count;
            m_AvgForward /= Boids.Count;

            AveragePosition = m_AvgPos;
            AverageForward = m_AvgForward;
            return;
        }

        private Vector3 CalculateAlignmentAcceleration(MovingObject boid)
        {
            Vector3 outcome = new Vector3(0, 0, 0);
            outcome = AverageForward / boid.MaxSpeed;
            if(outcome.Length > 1)
            {
                outcome.Normalize();
            }
            return outcome * AlignmentStrength;
        }

        private Vector3 CalculateCohesionAcceleration(MovingObject boid)
        {
            Vector3 outcome = new Vector3(0, 0, 0);
            outcome = AveragePosition - boid.Position;

            float dist = outcome.Length;
            outcome.Normalize();

            if (dist < FlockRadius)
                outcome *= dist / FlockRadius;

            return outcome * CohesionStrength;
        }

        private Vector3 CalculateSeparationAcceleration(MovingObject boid)
        {
            Vector3 sum = new Vector3(0, 0, 0);
            for(int i = 0; i < Boids.Count; i++)
            {
                if (Boids[i] == boid)
                    continue;

                Vector3 vec = boid.Position - Boids[i].Position;
                float dist = vec.Length;
                float safeDistance = boid.SafeRadius + Boids[i].SafeRadius;

                if(dist < safeDistance)
                {
                    vec.Normalize();
                    vec *= (safeDistance - dist) / safeDistance;
                    sum += vec;
                }
            }
            if(sum.Length > 1.0f)
            {
                sum.Normalize();
            }
            return sum * SeparationStrength;
        }

        #endregion

        #region TODO

        public virtual void Update(float deltaTime)
        {
            CalculateAverages();
            for (int i = 0; i < Boids.Count; i++)
            {
                Vector3 acceleration = CalculateAlignmentAcceleration(Boids[i]);
                acceleration += CalculateCohesionAcceleration(Boids[i]);
                acceleration += CalculateSeparationAcceleration(Boids[i]);
                float accelerationMultiplier = Boids[i].MaxSpeed;

                acceleration *= accelerationMultiplier * deltaTime;
                Boids[i].Velocity += acceleration;

                if (Boids[i].Velocity.Length > Boids[i].MaxSpeed)
                {
                    Boids[i].Velocity.Normalize();
                    Boids[i].Velocity *= Boids[i].MaxSpeed;
                }
                Boids[i].Update(deltaTime);
            }
            return;
        }
        #endregion
    }
}
