#include <Eigen/Dense>
#include <vector>
#include <string>
#include <memory>

typedef Eigen::Matrix4d Mat4;

class CJoint {
public:
    double getQ() const { return q_; }
    void setQ(double q) { if ((q > qMax_) || (q < qMin_)) { throw std::out_of_range("q is out of range"); } else { q_ = q; }}

    double getQMax() const { return qMax_; }
    double getQMin() const { return qMin_; }
    

private:
    
    double q_ ;
    double qMax_;
    double qMin_;

    virtual ~CJoint() = default;

    virtual Mat4 getTransform() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual std::unique_ptr<CJoint> clone() const = 0;
   

};