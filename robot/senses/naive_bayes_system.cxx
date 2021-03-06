#include "stdheader.h"
#include "naive_bayes_system.h"
#include "utils.h"
#include "utils-inl.h"

/*//return logsumexp(dest)
template<int R, int C, typename Units> float logsumexp_vector_values(math::Matrix<R,C, Units>* dest){
    float sum = 0; for(int i = 0; i < dest->size(); i++){ sum += (*dest)[i]; } return sum; }*/

void NaiveBayesSystem::init(){
    alpha = 0;
}
double NaiveBayesSystem::get_probability(){
    return alpha;
}
void NaiveBayesSystem::operate() {
    //cout << "nbs operate" << endl;
    float sqrt2pi = sqrt(2*M_PI);
    //declarations
        //mu = mean
        //sig = std
        //sig2 = std
        //y = actual  <-- holds p(y)
#define X(aa, bb, cc, dd, ee)\
    const bb& mean_##cc = mean_input_##cc.getValue(); \
    const bb& std_##cc = std_input_##cc.getValue(); \
    const bb& actual_##cc = actual_input_##cc.getValue(); \
    bb y_##cc; copy_matrix(&y_##cc,&actual_##cc); \
    bb mu_##cc; copy_matrix(&mu_##cc,&mean_##cc); \
    bb sig_##cc; copy_matrix(&sig_##cc,&std_##cc); \
    bb sig2_##cc; copy_matrix(&sig2_##cc,&std_##cc); 
    #include "input_type_table.h"
    #include "tool_type_table.h"
#undef X
    double emission_probability = 0; //in log domain
    double transition_probability = 0; //in log domain
    //calculations  <-- find sum(log(P(y)) given Gaussian(mu,sig)
        //sub(y,mu)
        //sqr(y)
        //sqr(sig2)
        //mult(sig2,2)
        //div(y,sig2)
        //exp(y)
        //mult(y,-1)
        //mult(sig,sqrt2pi)
        //div(y,sig)
        //log(y)
#define X(aa, bb, cc, dd, ee) \
    sub_vector_values(&y_##cc,&mu_##cc); \
    div_vector_values(&y_##cc,&sig_##cc); \
    //square_vector_values(&y_##cc); \
    square_vector_values(&sig2_##cc); \
    mult_vector_values(&sig2_##cc,2,0); \
    div_vector_values(&y_##cc,&sig2_##cc); \
    mult_vector_values(&y_##cc,-1,0); \
    exp_vector_values(&y_##cc); \
    /*normalize*/\
    /*mult_vector_values(&sig_##cc,sqrt2pi,0); \*/\
    /*div_vector_values(&y_##cc,&sig_##cc); \*/\
    /*convert to the log domain*/\
    log_vector_values(&y_##cc); \
    /*sum to obtain emission probability across all sensors*/\
    emission_probability += sum_vector_values(&y_##cc); 
    #include "input_type_table.h"
    #include "tool_type_table.h"
#undef X
#define X(aa, bb, cc, dd, ee) \
    /*obtain joint probability between parameters and sensors*/\
    //alpha = y_ft[0];\
    //alpha = emission_probability + transition_probability + alpha;\
    //alpha = logsumexp(emission_probability + transition_probability + alpha);
    //alpha += sum_vector_values(&y_##cc);
    //alpha += sum_vector_values(&sig_##cc);
    #include "input_type_table.h"
    #include "tool_type_table.h"
#undef X
    //cout << "probability: " << alpha << endl;*/
    output_value_probability->setData(&alpha);
}
