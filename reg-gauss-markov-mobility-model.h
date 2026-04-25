#ifndef REG_GAUSS_MARKOV_MOBILITY_MODEL_H
#define REG_GAUSS_MARKOV_MOBILITY_MODEL_H

#include "box.h"
#include "constant-velocity-helper.h"
#include "mobility-model.h"
#include "position-allocator.h"
#include "ns3/log.h"
#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/random-variable-stream.h"

namespace ns3
{

/**
 * \brief Regulated Gauss-Markov mobility model
 *
 * This is a 3D version of the Regulated Gauss-Markov mobility model, which is based on the 
 * original Gauss-Markov mobility model. 
 * Check the published paper for the mathmatical details of this mobility,
 *
 * Here is an example of how to implement the model and set the initial node positions for Regulated Gauss-Markov mobility model:
 *\code
   
     MobilityHelper mobility;
	mobility.SetMobilityModel ("ns3::RegGaussMarkovMobilityModel", //ns3::RegGaussMarkovMobilityModel
	"Bounds", BoxValue (Box (0, 247, 0, 247, 0, 247)),
	"TimeStep", TimeValue (Seconds (0.1)),
	"Alpha", DoubleValue (0.85),
	"MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=20|Max=60]"),
	"MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=6.283185307]"), // 2 pi = 6.283185307 rad = 360 degree 
	"MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
	"NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
	"NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
	"NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"),
	"NumberOfLevels", DoubleValue(11.0), // set the value for NumberOfLevels (i.e., for 11 UAVs)
	"SafetyBuffer", DoubleValue(0.5));  // set the value for SafetyBuffer (i.e., 0.5 meter)
     
     mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
     	"X", StringValue ("ns3::UniformRandomVariable[Min=0|Max=247]"),
      	"Y", StringValue ("ns3::UniformRandomVariable[Min=0|Max=247]"),
      	"Z", StringValue ("ns3::UniformRandomVariable[Min=0|Max=22]"));  
      // For this UAV, the Z-level is set between 0 and 22. This range should be adjusted for other UAVs to ensure a unique level per UAV.
      // According to Equation (13) in the paper, the Level Height (L_H) is computed as:
      // L_H = [(Z_max - Z_min) - (B_z * (NoL - 1))] / NoL = [(247 - 0) - (0.5 * 10)] / 11 = 22 meter

    mobility.Install (wifiStaNodes);
    
*\endcode
 */
class RegGaussMarkovMobilityModel : public MobilityModel
{
  public:	
    /**
     * Register this type with the TypeId system.
     * \return the object TypeId
     */
    static TypeId GetTypeId();
    RegGaussMarkovMobilityModel();

  private:
    /**
     * Initialize the model and calculate new velocity, direction, and pitch
     */
    void Start();
    /**
     * Perform a walk operation
     * param timeLeft time until Start method is called again
     */
    void DoWalk(Time timeLeft);
    void DoDispose() override;
    Vector DoGetPosition() const override;
    void DoSetPosition(const Vector& position) override;
    Vector DoGetVelocity() const override;
    int64_t DoAssignStreams(int64_t) override;
    ConstantVelocityHelper m_helper; //!< constant velocity helper
    Time m_timeStep;                 //!< duraiton after which direction and speed should change
    double m_alpha;                  //!< tunable constant in the model
    double m_meanVelocity;           //!< current mean velocity
    double m_meanDirection;          //!< current mean direction
    double m_meanPitch;              //!< current mean pitch
    double m_Velocity;               //!< current velocity
    double m_Direction;              //!< current direction
    double m_Pitch;                  //!< current pitch
    // Below line has been added by Ala Altaweel to declare the attributes for the Regulated Gauss-Markov mobility model
    double m_altitudeLevels;         //!< number of levels
    double m_safetyBuffer;         //!< safety buffer between levels
    // End of added code by Ala Altaweel
    Ptr<RandomVariableStream> m_rndMeanVelocity;  //!< rv used to assign avg velocity
    Ptr<NormalRandomVariable> m_normalVelocity;   //!< Gaussian rv used to for next velocity
    Ptr<RandomVariableStream> m_rndMeanDirection; //!< rv used to assign avg direction
    Ptr<NormalRandomVariable> m_normalDirection;  //!< Gaussian rv for next direction value
    Ptr<RandomVariableStream> m_rndMeanPitch;     //!< rv used to assign avg. pitch
    Ptr<NormalRandomVariable> m_normalPitch;      //!< Gaussian rv for next pitch
    EventId m_event;                              //!< event id of scheduled start
    Box m_bounds;                                 //!< bounding box

};

} // namespace ns3

#endif /* REG_GAUSS_MARKOV_MOBILITY_MODEL_H */
