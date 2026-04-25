#include "reg-gauss-markov-mobility-model.h"
#include "position-allocator.h"
#include <cmath>
#include "ns3/double.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include <cmath>

namespace ns3
{
NS_LOG_COMPONENT_DEFINE("RegGaussMarkovMobilityModel");
NS_OBJECT_ENSURE_REGISTERED(RegGaussMarkovMobilityModel);

TypeId
RegGaussMarkovMobilityModel::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::RegGaussMarkovMobilityModel")
            .SetParent<MobilityModel>()
            .SetGroupName("Mobility")
            .AddConstructor<RegGaussMarkovMobilityModel>()
            .AddAttribute("Bounds",
                          "Bounds of the area to cruise.",
                          BoxValue(Box(-100.0, 100.0, -100.0, 100.0, 0.0, 100.0)),
                          MakeBoxAccessor(&RegGaussMarkovMobilityModel::m_bounds),
                          MakeBoxChecker())
            .AddAttribute("TimeStep",
                          "Change current direction and speed after moving for this time.",
                          TimeValue(Seconds(1.0)),
                          MakeTimeAccessor(&RegGaussMarkovMobilityModel::m_timeStep),
                          MakeTimeChecker())
            .AddAttribute(
		        "Alpha",
		        "A constant representing the tunable parameter in the Gauss-Markov model.",
		        DoubleValue(1.0),
		        MakeDoubleAccessor(&RegGaussMarkovMobilityModel::m_alpha),
		        MakeDoubleChecker<double>())
            .AddAttribute("MeanVelocity",
                          "A random variable used to assign the average velocity.",
                          StringValue("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"),
                          MakePointerAccessor(&RegGaussMarkovMobilityModel::m_rndMeanVelocity),
                          MakePointerChecker<RandomVariableStream>())
            .AddAttribute("MeanDirection",
                          "A random variable used to assign the average direction.",
                          StringValue("ns3::UniformRandomVariable[Min=0.0|Max=6.283185307]"),
                          MakePointerAccessor(&RegGaussMarkovMobilityModel::m_rndMeanDirection),
                          MakePointerChecker<RandomVariableStream>())
            .AddAttribute("MeanPitch",
                          "A random variable used to assign the average pitch.",
                          StringValue("ns3::ConstantRandomVariable[Constant=0.0]"),
                          MakePointerAccessor(&RegGaussMarkovMobilityModel::m_rndMeanPitch),
                          MakePointerChecker<RandomVariableStream>())
            .AddAttribute("NormalVelocity",
                          "A gaussian random variable used to calculate the next velocity value.",
                          StringValue("ns3::NormalRandomVariable[Mean=0.0|Variance=1.0|Bound=10."
                                      "0]"), // Defaults to zero mean, and std dev = 1, and bound to
                                             // +-10 of the mean
                          MakePointerAccessor(&RegGaussMarkovMobilityModel::m_normalVelocity),
                          MakePointerChecker<NormalRandomVariable>())
            .AddAttribute(
		        "NormalDirection",
		        "A gaussian random variable used to calculate the next direction value.",
		        StringValue("ns3::NormalRandomVariable[Mean=0.0|Variance=1.0|Bound=10.0]"),
		        MakePointerAccessor(&RegGaussMarkovMobilityModel::m_normalDirection),
		        MakePointerChecker<NormalRandomVariable>())
            .AddAttribute(
		        "NormalPitch",
		        "A gaussian random variable used to calculate the next pitch value.",
		        StringValue("ns3::NormalRandomVariable[Mean=0.0|Variance=1.0|Bound=10.0]"),
		        MakePointerAccessor(&RegGaussMarkovMobilityModel::m_normalPitch),
		        MakePointerChecker<NormalRandomVariable>())
		        
           // The below lines has been added by Ala Altaweel to set the attributes for the Regulated Gauss-Markov mobility model
	   .AddAttribute ("NumberOfLevels",
                        "Number of levels within the bounds of the area to cruise.",
                         DoubleValue (1.0),  // Default value
                         MakeDoubleAccessor (&RegGaussMarkovMobilityModel::m_altitudeLevels),
                         MakeDoubleChecker<double> ())
                         
           .AddAttribute ("SafetyBuffer",
                        "safety space between each level.",
                         DoubleValue (0.35),  // Default value
                         MakeDoubleAccessor (&RegGaussMarkovMobilityModel::m_safetyBuffer),
                         MakeDoubleChecker<double> ());
           // End of added code by Ala Altaweel
    return tid;
}

RegGaussMarkovMobilityModel::RegGaussMarkovMobilityModel()
{
    m_meanVelocity = 0.0;
    m_meanDirection = 0.0;
    m_meanPitch = 0.0;
    m_event = Simulator::ScheduleNow(&RegGaussMarkovMobilityModel::Start, this);
    m_helper.Unpause();
}

void
RegGaussMarkovMobilityModel::Start()
{
    if (m_meanVelocity == 0.0)
    {
        // Initialize the mean velocity, direction, and pitch variables
        std::cout << "NumberOfLevels: " << m_altitudeLevels<< std::endl<< std::endl;
        std::cout << "SafetyBuffer: " << m_safetyBuffer<< std::endl<< std::endl;
        m_meanVelocity = m_rndMeanVelocity->GetValue();
        m_meanDirection = m_rndMeanDirection->GetValue();
        m_meanPitch = m_rndMeanPitch->GetValue();
        double cosD = std::cos(m_meanDirection);
        double cosP = std::cos(m_meanPitch);
        double sinD = std::sin(m_meanDirection);
        double sinP = std::sin(m_meanPitch);
        // Initialize the starting velocity, direction, and pitch to be identical to the mean ones
        m_Velocity = m_meanVelocity;
        m_Direction = m_meanDirection;
        m_Pitch = m_meanPitch;
        // Set the velocity vector to give to the constant velocity helper
        m_helper.SetVelocity(
            Vector(m_Velocity * cosD * cosP, m_Velocity * sinD * cosP, m_Velocity * sinP));
    }
    m_helper.Update();

    // Get the next values from the gaussian distributions for velocity, direction, and pitch
    double rv = m_normalVelocity->GetValue();
    double rd = m_normalDirection->GetValue();
    double rp = m_normalPitch->GetValue();

    // Calculate the NEW velocity, direction, and pitch values using the Gauss-Markov formula:
    // newVal = alpha*oldVal + (1-alpha)*meanVal + sqrt(1-alpha^2)*rv
    // where rv is a random number from a normal (gaussian) distribution
    double one_minus_alpha = 1 - m_alpha;
    double sqrt_alpha = std::sqrt(1 - m_alpha * m_alpha);
    m_Velocity = m_alpha * m_Velocity + one_minus_alpha * m_meanVelocity + sqrt_alpha * rv;
    m_Direction = m_alpha * m_Direction + one_minus_alpha * m_meanDirection + sqrt_alpha * rd;
    m_Pitch = m_alpha * m_Pitch + one_minus_alpha * m_meanPitch + sqrt_alpha * rp;

    // Calculate the linear velocity vector to give to the constant velocity helper
    double cosDir = std::cos(m_Direction);
    double cosPit = std::cos(m_Pitch);
    double sinDir = std::sin(m_Direction);
    double sinPit = std::sin(m_Pitch);
    double vx = m_Velocity * cosDir * cosPit;
    double vy = m_Velocity * sinDir * cosPit;
    double vz = m_Velocity * sinPit;
    m_helper.SetVelocity(Vector(vx, vy, vz));

    m_helper.Unpause();

    DoWalk(m_timeStep);
}


void
RegGaussMarkovMobilityModel::DoWalk(Time delayLeft)
{

    // The below lines has been added by Ala Altaweel to Update the position of the node in respect of bounds (m_bounds).
    // The below lines are used instead of using the built in UpdateWithBounds() function.
    Vector position = m_helper.GetCurrentPosition();
    position.x = std::min(m_bounds.xMax, position.x);
    position.x = std::max(m_bounds.xMin, position.x);
    position.y = std::min(m_bounds.yMax, position.y);
    position.y = std::max(m_bounds.yMin, position.y);
    // End of added code by Ala Altaweel
    
    //m_helper.UpdateWithBounds(m_bounds);
    Vector speed = m_helper.GetVelocity();
    Vector nextPosition = position;
    nextPosition.x += speed.x * delayLeft.GetSeconds();
    nextPosition.y += speed.y * delayLeft.GetSeconds();
    nextPosition.z += speed.z * delayLeft.GetSeconds();
    if (delayLeft.GetSeconds() < 0.0)
    {
        delayLeft = Seconds(1.0);
    }
    
    // The below lines has been added by Ala Altaweel to confirm that the position at next time step is still within the same level of the current position.
    // If the position is out of level, alter the velocity vector and average direction to keep the position within the level
    
    // Determine the level based on the Position.z
    double levelHeight = (m_bounds.zMax - m_bounds.zMin - (m_safetyBuffer * (m_altitudeLevels - 1))) / m_altitudeLevels;  // Height of each level
    int level = floor((position.z - m_bounds.zMin) / (levelHeight + m_safetyBuffer));
    
    // Check if nextPosition.z is within the level's z-range
    double lowerBound = m_bounds.zMin + level * (levelHeight + m_safetyBuffer);
    double upperBound = lowerBound + levelHeight;

    if (nextPosition.x >= m_bounds.xMin && nextPosition.x < m_bounds.xMax && nextPosition.y >= m_bounds.xMin && nextPosition.y < m_bounds.xMax && nextPosition.z >= lowerBound && nextPosition.z < upperBound)
    {
        m_event = Simulator::Schedule(delayLeft, &RegGaussMarkovMobilityModel::Start, this);
        //std::cout<< "Current position for z is:"<< position.z << "at time:" << Simulator::Now().GetSeconds() << "s\n"<< std::endl;
    }
    else
    {
        if (nextPosition.x > m_bounds.xMax || nextPosition.x < m_bounds.xMin)
        {
            speed.x = -speed.x;
            m_meanDirection = M_PI - m_meanDirection;
        }

        if (nextPosition.y > m_bounds.yMax || nextPosition.y < m_bounds.yMin)
        {
            speed.y = -speed.y;
            m_meanDirection = -m_meanDirection;
        }

        if (nextPosition.z > upperBound || nextPosition.z < lowerBound)
        {
            //std::cout<< "Next position for z is:"<< nextPosition.z << "at time:" << Simulator::Now().GetSeconds() << "s\n"<< std::endl;
            speed.z = -speed.z;
            m_meanPitch = -m_meanPitch;
        }
        m_Direction = m_meanDirection;
        m_Pitch = m_meanPitch;
        m_helper.SetVelocity(speed);
        m_helper.Unpause();
        m_event = Simulator::Schedule(delayLeft, &RegGaussMarkovMobilityModel::Start, this);
    }   
    // End of added code by Ala Altaweel
    NotifyCourseChange();
}

void
RegGaussMarkovMobilityModel::DoDispose()
{
    // chain up
    MobilityModel::DoDispose();
}

Vector
RegGaussMarkovMobilityModel::DoGetPosition() const
{
    m_helper.Update();
    return m_helper.GetCurrentPosition();
}

void
RegGaussMarkovMobilityModel::DoSetPosition(const Vector& position)
{
    m_helper.SetPosition(position);
    m_event.Cancel();
    m_event = Simulator::ScheduleNow(&RegGaussMarkovMobilityModel::Start, this);
}

Vector
RegGaussMarkovMobilityModel::DoGetVelocity() const
{
    return m_helper.GetVelocity();
}

int64_t
RegGaussMarkovMobilityModel::DoAssignStreams(int64_t stream)
{
    m_rndMeanVelocity->SetStream(stream);
    m_normalVelocity->SetStream(stream + 1);
    m_rndMeanDirection->SetStream(stream + 2);
    m_normalDirection->SetStream(stream + 3);
    m_rndMeanPitch->SetStream(stream + 4);
    m_normalPitch->SetStream(stream + 5);
    return 6;
}

} // namespace ns3
