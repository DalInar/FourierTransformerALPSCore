#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <alps/params.hpp>
#include "alps/gf/gf.hpp"
#include "alps/gf/fourier.hpp"
#include "alps/gf/tail.hpp"
#include "boost/program_options.hpp"
#include "cluster_transform.h"
//#include "selfconsistency.h"

void define_dca_parameters(alps::params &p);

void define_parameters(alps::params &p){
    define_dca_parameters(p);
    p.define<double>("MU", "Chemical Potential");
    p.define<double>("t", "Hopping");
    p.define<double>("tprime", 0., "nearest-neighbor hopping ");
    p.define<int>("N", "Number of imaginary time discretization points");
    p.define<int>("NMATSUBARA", "Number of matsubara frequency discretization points");
    p.define<int>("FLAVORS", 2, "Number of spins or (diagonal) orbitals");
    p.define<double>("BETA", "Inverse temperature");
    p.define<double>("H", "Staggered (antiferromagnetic) field");

    p.define<double>("sc.SELFCONSISTENCY_CONVERGENCE", 1.e-10, "Difference in Green's function at which the iteration is converged");
    p.define<int>("sc.MAX_IT", 1000, "Maximum number of DMFT iterations");
    p.define<bool>("sc.ADJUST_MU", false, "set to true to find target density, rather than run for fixed mu");
    p.define<double>("sc.TARGET_DENSITY", "desired target density, see also sc.ADJUST_MU");
    p.define<double>("sc.RELAX_RATE", 1., "Convergence acceleration (or deceleration) parameter. Choose 1 if unsure.");
    p.define<bool>("sc.AFM_SELFCONSISTENCY", false, "Use the AFM self-consistency condition.");
    p.define<bool>("sc.SPIN_SYMMETRIZATION", true, "symmetrize 'up' and 'down' spins after each solver call.");
    p.define<bool>("sc.K_SPACE_SYMMETRIZATION", true, "apply point group symmetrries to G(k) during DCA self consistency");
    p.define<std::string>("G0_OMEGA_OUTPUT", "Output filename for G0");
}

alps::params read_parameters_from_sim_file(std::string sim_filename){
    alps::params parms(sim_filename);
    define_parameters(parms);

    if (parms.help_requested(std::cout)) {
        exit(0);
    }
    return parms;
}

int main(int argc, char** argv){
	std::cout<<"I WILL TRANSFORM YOU!!"<<std::endl;
	std::cout<<"Right now we only transform from Matsubara k-space to Matsubara real space for Charge Order case"<<std::endl;
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
            ("help", "produce help message")
            ("input", po::value< std::vector<std::string> > (), "input file")
            ("transform", po::value< std::vector<std::string> >(), "what type of transformation?")
            ("paramfile", po::value< std::vector<std::string> > (), "parameter file");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help")){
	    std::cout << desc << "\n";
	    return 1;
	}

    std::string parm_filename = vm["paramfile"].as< std::vector<std::string> >()[0];
    std::string h5gf_filename = vm["input"].as< std::vector<std::string> >()[0];


    std::cout<<"Parameter file: "<<parm_filename<<std::endl;
    std::cout<<"G file: "<<h5gf_filename<<std::endl;
    alps::params params=read_parameters_from_sim_file(parm_filename);
    alps::hdf5::archive iar(h5gf_filename);

    std::cout<<params<<std::endl;

    ClusterTransformer * CT_ptr_;
    CT_ptr_ = ((params["dca.AFM_SELFCONSISTENCY"].as<bool>() || params["dca.CO_SELFCONSISTENCY"].as<bool>()) ?
            (ClusterTransformer *) (new DoubleCellClusterTransformer(params)): (ClusterTransformer *) (new NormalStateClusterTransformer(params)));


    double beta = params["BETA"];
    int n_matsubara = params["NMATSUBARA"];
    int n_tau = params["N"];
    int n_tau_plus_one = 1 + n_tau;
    std::cout<< "N_mats = "<<n_matsubara<<std::endl;
    double_cell_matsubara_kspace_gf green0_matsubara_kspace_doublecell(
            alps::gf::omega_k_sigma1_sigma2_gf(alps::gf::matsubara_positive_mesh(beta, n_matsubara),
                    CT_ptr_->momentum_mesh(), alps::gf::index_mesh(2),
                    alps::gf::index_mesh(2)));

    std::cout<<"Loading G0"<<std::endl;
    std::cout<<green0_matsubara_kspace_doublecell.mesh1().extent()<<std::endl;
    std::cout<<green0_matsubara_kspace_doublecell.mesh2().extent()<<std::endl;
    std::cout<<green0_matsubara_kspace_doublecell.mesh3().extent()<<std::endl;
    std::cout<<green0_matsubara_kspace_doublecell.mesh4().extent()<<std::endl;
    std::cout<<iar.get_filename()<<std::endl;
    std::vector< std::string > attrs = iar.list_children("/G0");
    std::cout << attrs.size() << std::endl;
    for(int i = 0; i<attrs.size(); i++){
        std::cout<<attrs[i]<<std::endl;
    }

    boost::multi_array<std::complex<double>,4> data;
    data.resize(boost::extents[512][8][2][2]);
    iar["/G0/data"] >> data;
    std::cout<< data.shape()[0] << std::endl;
    std::cout<< data.shape()[1] << std::endl;
    std::cout<< data.shape()[2] << std::endl;
    std::cout<< data.shape()[3] << std::endl;
    std::cout<< data[0][0][0][0] <<std::endl;
    //std::cout<<<<std::endl;


    green0_matsubara_kspace_doublecell.load(iar,"/G0");

    std::cout<<"Creating boxes"<<std::endl;
    double_cell_itime_kspace_gf green0_itime_kspace(alps::gf::itime_k_sigma1_sigma2_gf(alps::gf::itime_mesh(beta, 1 + n_tau), CT_ptr_->momentum_mesh(), alps::gf::index_mesh(2), alps::gf::index_mesh(2)));
    cluster_itime_realspace_gf green0_itime_sc_rs(alps::gf::itime_r1_r2_sigma_gf(alps::gf::itime_mesh(beta, n_tau_plus_one), CT_ptr_->real_space_mesh(), CT_ptr_->real_space_mesh(),alps::gf::index_mesh(2)));

    alps::gf::fourier_frequency_to_time(green0_matsubara_kspace_doublecell, green0_itime_kspace);

    green0_itime_sc_rs = CT_ptr_->transform_into_real_space(green0_itime_kspace);
    alps::hdf5::archive g_tau_savefile("G0_tau_rs_output", "w");
    green0_itime_sc_rs.save(g_tau_savefile, "/G0");
//    CT_ptr_((params["dca.AFM_SELFCONSISTENCY"].as<bool>() || params["dca.CO_SELFCONSISTENCY"].as<bool>()) ?
//            (ClusterTransformer *) (new DoubleCellClusterTransformer(params))
//                                                                                                          : (ClusterTransformer *) (new NormalStateClusterTransformer(
//                    params))),


}
