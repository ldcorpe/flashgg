#ifndef FLASHgg_WeightedObject_h
#define FLASHgg_WeightedObject_h

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace flashgg {

    class WeightedObject
    {


    public:
        WeightedObject();
        WeightedObject( const WeightedObject & );
        virtual ~WeightedObject();

        float weight( string key ) const;
        float centralWeight() const
        {
            std::cout << "DEBUG  my address is " << this << " size of labels " << _labels.size() << ", size of _weights " << _weights.size() << std::endl;

            //   for (auto label : _labels){
            //    std::cout << "DEBUG  labels " << label <<  std::endl;
            //    }
            //    for (auto w : _weights){
            //   std::cout << "DEBUG  weights " << w <<  std::endl;
            //   }
            return weight( central_key );
            //return 99.99;

        }
        float louieTest() const
        {
            return 99.99;

        }
        void setWeight( string key, float val );
        void setCentralWeight( float val ) { setWeight( central_key, val ); }
        bool hasWeight( string key ) const;
        void includeWeights( const WeightedObject &other );
        vector<string>::const_iterator weightListBegin() const { return _labels.begin(); }
        vector<string>::const_iterator weightListEnd() const { return _labels.end(); }

    private:
        vector<string> _labels;
        vector<float> _weights;
        static constexpr const char *central_key = "Central";
    };
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
